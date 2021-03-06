/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>),
    German Cancer Research Center/University Heidelberg



    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "qfrdrtablecurvefitdialog.h"
#include "ui_qfrdrtablecurvefitdialog.h"
#include "dlgqfprogressdialog.h"
#include "qffitalgorithmthreaddedfit.h"
#include "programoptions.h"
#include "qftools.h"
#include "qffitfunctionmanager.h"
#include "qffitalgorithmmanager.h"

QFRDRTableCurveFitDialog::QFRDRTableCurveFitDialog(QFRDRTable *table, int colX, int colY, int colW, QFRDRTable::GraphDataSelection sel, QWidget *parent, bool logX, bool logY, int resultColumn, int addGraph, int sourceGraph) :
    QFDialog(parent),
    ui(new Ui::QFRDRTableCurveFitDialog)
{
    ui->setupUi(this);
    selection=sel;
    overwriteGraph=-1;
    overwritePlot=-1;
    this->sourceGraph=sourceGraph;
    parameterTable=new QFFitFunctionValueInputTable(this);
    intInit(table,  colX,  colY,  colW, parent,  logX,  logY,  resultColumn,  addGraph);
    setWindowFlags(windowFlags()|Qt::WindowMinMaxButtonsHint);
}

QFRDRTableCurveFitDialog::QFRDRTableCurveFitDialog(QFRDRTable *table, int plotid, int graphid, QWidget *parent):
    QFDialog(parent),
    ui(new Ui::QFRDRTableCurveFitDialog)
{
    ui->setupUi(this);
    int resultColumn=-1,  addGraph=-1;
    this->table=table;
    parameterTable=new QFFitFunctionValueInputTable(this);
    readFitProperties(plotid, graphid, &resultColumn, &addGraph);
    overwriteGraph=graphid;
    overwritePlot=plotid;
    this->sourceGraph=-1;
    //qDebug()<<"QFRDRTableCurveFitDialog("<<plotid<<graphid<<"): "<<overwriteGraph;
    intInit(table,  colX,  colY,  colW, parent,  ui->chkLogX->isChecked(),  ui->chkLogY->isChecked(),  resultColumn,  addGraph, true);
    setWindowFlags(windowFlags()|Qt::WindowMinMaxButtonsHint);
}

void QFRDRTableCurveFitDialog::intInit(QFRDRTable *table, int colX, int colY, int colW, QWidget *parent, bool logX, bool logY, int resultColumn, int addGraph, bool propsAlreadySet)
{
    setParent(parent);
    ui->btnHelpAlgorithm->setDefaultAction(ui->cmbFitAlgorithm->getHelpAction());
    ui->btnHelpModel->setDefaultAction(ui->cmbFitFunction->getHelpAction());
    ui->btnModelSelect->setDefaultAction(ui->cmbFitFunction->getSelectAction());
    parameterTable->setEditRanges(true);
    parameterTable->setEditErrors(true);
    this->colX=colX;
    this->colY=colY;
    this->colW=colW;
    ui->chkWeighted->setChecked(colW!=-1);
    ui->chkWeighted->setEnabled(true);//colW!=-1);
    ui->cmbWeightOptions->setEnabled(/*(colW!=-1) &&*/ ui->chkWeighted->isChecked());
    ui->chkBootstrap->setEnabled(true);
    ui->widBootStrap->setVisible(ui->chkBootstrap->isChecked());
    //ui->chkBootstrap->setChecked(false);
    this->table=table;
    ui->chkLogX->setChecked(logX);
    ui->chkLogY->setChecked(logY);


    ui->cmbAddGraph->clear();
    ui->cmbAddGraph->addItem(tr("--- no ---"));
    ui->cmbAddGraph->addItem(tr("--- add new graph ---"));
    for (int i=0; i<table->colgraphGetPlotCount(); i++) {
        ui->cmbAddGraph->addItem(tr("add to: %1").arg(table->colgraphGetPlotTitle(i)));
    }
    ui->cmbAddGraph->addItem(tr("--- overwrite old graph ---"));
    ui->cmbSaveColumn->clear();
    ui->cmbSaveColumn->addItem(tr("--- no ---"));
    ui->cmbSaveColumn->addItem(tr("--- add new column ---"));
    for ( int i=0; i<table->tableGetColumnCount(); i++) {
        ui->cmbSaveColumn->addItem(tr("save in column %1 [%2]").arg(i+1).arg(table->tableGetColumnTitle(i)));
    }

    ui->pltDistribution->getXAxis()->set_axisLabel(tr("x-data [%1: %2]").arg(colX+1).arg(table->tableGetColumnTitle(colX)));
    ui->pltDistribution->getYAxis()->set_axisLabel(tr("y-data [%1: %2]").arg(colY+1).arg(table->tableGetColumnTitle(colY)));


    if (addGraph<0) {
        ui->cmbAddGraph->setCurrentIndex(0);
    } else {
        ui->cmbAddGraph->setCurrentIndex(addGraph+2);
    }
    if (overwriteGraph>=0) {
        ui->cmbAddGraph->setCurrentIndex(ui->cmbAddGraph->count()-1);
        //qDebug()<<"cmbAddGraph: "<<ui->cmbAddGraph->currentIndex();
    }
    if (resultColumn<0) {
        ui->cmbSaveColumn->setCurrentIndex(0);
    } else {
        ui->cmbSaveColumn->setCurrentIndex(resultColumn+2);
    }
    ui->datacut->setLogScale(logX, 20);


    ui->tabParams->setItemDelegate(new QFFitFunctionValueInputDelegate(ui->tabParams));

    readDataFromTable();

    ui->tabParams->setModel(parameterTable);



    if (!propsAlreadySet) {
        ui->cmbFitAlgorithm->setCurrentAlgorithm("fit_lmfit");

        ui->cmbFitFunction->setCurrentFitFunction("gen_polynom");
    }








    ui->pltResiduals->set_displayToolbar(false);
    ui->pltResiduals->getXAxis()->set_axisLabel(tr("X"));
    //ui->pltResiduals->getXAxis()->set_logAxis(true);
    ui->pltResiduals->getYAxis()->set_axisLabel(tr("residuals"));
    ui->pltDistribution->getXAxis()->set_axisLabel("");
    ui->pltDistribution->getYAxis()->set_axisLabel(tr("Y"));
    ui->pltDistribution->getXAxis()->set_drawMode1(JKQTPCADMticks);
    ui->pltResiduals->getXAxis()->set_drawMode1(JKQTPCADMcomplete);
    ui->pltResiduals->get_plotter()->setBorder(1,1,1,1);
    ui->pltDistribution->get_plotter()->setBorder(1,1,1,1);
    ui->pltResiduals->synchronizeToMaster(ui->pltDistribution, true, false);
    ui->pltDistribution->get_plotter()->set_useAntiAliasingForSystem(true);
    ui->pltDistribution->get_plotter()->set_useAntiAliasingForGraphs(true);
    ui->pltResiduals->get_plotter()->set_useAntiAliasingForSystem(true);
    ui->pltResiduals->get_plotter()->set_useAntiAliasingForGraphs(true);
    ui->pltResiduals->set_displayMousePosition(false);
    ui->pltDistribution->set_displayMousePosition(false);
    ui->pltDistribution->get_plotter()->set_keyFontSize(9);
    ui->pltResiduals->get_plotter()->set_keyFontSize(9);
    ui->pltResiduals->useExternalDatastore(ui->pltDistribution->getDatastore());
    ui->pltResiduals->setMinimumHeight(75);
    ui->pltResiduals->get_plotter()->set_showKey(false);
    ui->pltDistribution->setMinimumHeight(75);
    ui->pltDistribution->get_plotter()->setBorder(1,1,1,1);
    ui->pltDistribution->get_plotter()->set_useAntiAliasingForSystem(true);
    ui->pltDistribution->get_plotter()->set_useAntiAliasingForGraphs(true);
    ui->pltDistribution->get_plotter()->set_keyFontSize(9);
    ui->pltDistribution->get_plotter()->set_keyPosition(JKQTPkeyInsideTopLeft);
    QColor cb("white");
    cb.setAlphaF(0.5);
    ui->pltDistribution->get_plotter()->set_keyBackgroundColor(cb);
    ui->pltDistribution->get_plotter()->set_keyPosition(JKQTPkeyInsideTopLeft);

    connect(ui->pltDistribution->get_plotter()->get_actZoomAll(), SIGNAL(triggered()), ui->pltResiduals, SLOT(zoomToFit()));
    connect(ui->pltResiduals, SIGNAL(zoomChangedLocally(double,double,double,double,JKQtPlotter*)), ui->pltDistribution, SLOT(synchronizeXAxis(double,double,double,double,JKQtPlotter*)));//SLOT(pzoomChangedLocally(double,double,double,double,JKQtBasePlotter*)));
    connect(ui->pltDistribution, SIGNAL(zoomChangedLocally(double,double,double,double,JKQtPlotter*)), ui->pltResiduals, SLOT(synchronizeXAxis(double,double,double,double,JKQtPlotter*)));


    QSettings* set=ProgramOptions::getInstance()->getQSettings();
    if (set) {
        loadWidgetGeometry(*set, this, pos(), size(), "QFRDRTableCurveFitDialog/windowsize");
        loadSplitter(*set, ui->splitter, "QFRDRTableCurveFitDialog/splitter/");
        if (!propsAlreadySet) {
            ui->chkPlotErrors->setChecked(set->value("QFRDRTableCurveFitDialog/ploterrors", ui->chkPlotErrors->isChecked()).toBool());
            ui->chkWeightedResiduals->setChecked(set->value("QFRDRTableCurveFitDialog/weightedresiduals", ui->chkWeightedResiduals->isChecked()).toBool());
            ui->cmbWeightOptions->setCurrentIndex(set->value("QFRDRTableCurveFitDialog/cmbWeightOptions", ui->cmbWeightOptions->currentIndex()).toInt());
            ui->chkBootstrap->setChecked(set->value("QFRDRTableCurveFitDialog/chkBootstrap", ui->chkBootstrap->isChecked()).toBool());
            ui->spinBSDistortion->setValue(set->value("QFRDRTableCurveFitDialog/spinBSDistortion", ui->spinBSDistortion->value()).toDouble());
            ui->spinBSFraction->setValue(set->value("QFRDRTableCurveFitDialog/spinBSFraction", ui->spinBSFraction->value()).toDouble());
            ui->spinBSRepeats->setValue(set->value("QFRDRTableCurveFitDialog/spinBSRepeats", ui->spinBSRepeats->value()).toInt());
            ui->cmbFitAlgorithm->setCurrentAlgorithm(set->value("QFRDRTableCurveFitDialog/algorithm", ui->cmbFitAlgorithm->currentFitAlgorithmID()).toString());
            ui->cmbFitFunction->setCurrentFitFunction(set->value("QFRDRTableCurveFitDialog/model", ui->cmbFitFunction->currentFitFunctionID()).toString());
        }
    }

    methodChanged(0);//ui->cmbFitFunction->currentIndex());

    updateFitStatistics();

    //QApplication::restoreOverrideCursor();
    parameterTable->rebuildModel();
    replotGraph();
    connectSignals(true);

    //on_btnGuess_clicked();
    //on_btnFit_clicked();
}

QFRDRTableCurveFitDialog::~QFRDRTableCurveFitDialog()
{
    QSettings* set=ProgramOptions::getInstance()->getQSettings();
    if (set) {
        saveWidgetGeometry(*set, this, "QFRDRTableCurveFitDialog/windowsize");
        set->setValue("QFRDRTableCurveFitDialog/ploterrors", ui->chkPlotErrors->isChecked());
        set->setValue("QFRDRTableCurveFitDialog/weightedresiduals", ui->chkWeightedResiduals->isChecked());
        set->setValue("QFRDRTableCurveFitDialog/cmbWeightOptions", ui->cmbWeightOptions->currentIndex());

        set->setValue("QFRDRTableCurveFitDialog/chkBootstrap", ui->chkBootstrap->isChecked());
        set->setValue("QFRDRTableCurveFitDialog/spinBSDistortion", ui->spinBSDistortion->value());
        set->setValue("QFRDRTableCurveFitDialog/spinBSFraction", ui->spinBSFraction->value());
        set->setValue("QFRDRTableCurveFitDialog/spinBSRepeats", ui->spinBSRepeats->value());

        set->setValue("QFRDRTableCurveFitDialog/algorithm", ui->cmbFitAlgorithm->currentFitAlgorithmID());
        set->setValue("QFRDRTableCurveFitDialog/model", ui->cmbFitFunction->currentFitFunctionID());
        saveSplitter(*set, ui->splitter, "QFRDRTableCurveFitDialog/splitter/");
    }

    delete ui;
}

void QFRDRTableCurveFitDialog::saveResults()
{

    QFFitFunction* model=ui->cmbFitFunction->createCurrentInstance();
    int saveCol=ui->cmbSaveColumn->currentIndex();
    int saveGraph=ui->cmbAddGraph->currentIndex();
    //qDebug()<<"QFRDRTableCurveFitDialog::saveResults(): "<<saveCol<<saveGraph;
    if (model && lastResults.size()>0) {
        bool emits=table->colgraphGetDoEmitSignals();
        table->colgraphSetDoEmitSignals(false);
        int savedTo=-1;
        if (saveCol==1) {
            savedTo=table->tableGetColumnCount();
            table->tableSetColumnData(savedTo, lastResults);
            table->tableSetColumnTitle(savedTo, tr("fit results, %1").arg(model->id()));
            table->tableSetColumnData(savedTo+1, lastResultsErrors);
            table->tableSetColumnTitle(savedTo+1, tr("fit errors, %1").arg(model->id()));
            for (int i=0; i<lastResults.size(); i++) {
                table->tableSetComment(i, savedTo, parameterNames.value(i, "")+"/"+parameterLabels.value(i, ""));
                table->tableSetComment(i, savedTo+1, parameterNames.value(i, "")+"/"+parameterLabels.value(i, ""));
            }

        } else if (saveCol>=2){
            savedTo=saveCol-2;
            table->tableSetColumnData(savedTo, lastResults);
            table->tableSetColumnTitle(savedTo, tr("fit results, %1").arg(model->id()));
            table->tableSetColumnData(savedTo+1, lastResultsErrors);
            table->tableSetColumnTitle(savedTo+1, tr("fit errors, %1").arg(model->id()));
            for (int i=0; i<lastResults.size(); i++) {
                table->tableSetComment(i, savedTo, parameterNames.value(i, "")+"/"+parameterLabels.value(i, ""));
                table->tableSetComment(i, savedTo+1, parameterNames.value(i, "")+"/"+parameterLabels.value(i, ""));
            }
        }
        if (saveGraph==1) {
            table->colgraphAddPlot(tr("curve fit results"), ui->pltDistribution->getXAxis()->get_axisLabel(), ui->pltDistribution->getYAxis()->get_axisLabel(), ui->pltDistribution->getXAxis()->get_logAxis(), ui->pltDistribution->getYAxis()->get_logAxis());
            int plotID=table->colgraphGetPlotCount()-1;
            table->colgraphAddGraph(plotID, colX, colY, QFRDRColumnGraphsInterface::cgtPoints, tr("data"));
            if (savedTo>=0) {
                table->colgraphAddFunctionGraph(plotID, model->id(), QFRDRColumnGraphsInterface::cgtQFFitFunction, fitresult, savedTo);
            } else {
                table->colgraphAddFunctionGraph(plotID, model->id(), QFRDRColumnGraphsInterface::cgtQFFitFunction, fitresult, lastResultD);
            }
            int gid=table->colgraphGetGraphCount(plotID)-1;
            table->colgraphSetGraphTitle(plotID, gid, resultComment+", "+resultPars+", "+resultStat);
            if (sourceGraph>=0 && sourceGraph<table->colgraphGetGraphCount(plotID)) {
                table->colgraphSetGraphColor(plotID, gid, table->getPlot(plotID).graphs[sourceGraph].color.darker());
                table->colgraphSetGraphLineWidth(plotID, gid, 1.0);
            }
            writeFitProperties(plotID, table->colgraphGetGraphCount(plotID)-1, savedTo);
        } else if (saveGraph>=2 && saveGraph<ui->cmbAddGraph->count()-1){
            if (savedTo>=0) {
                table->colgraphAddFunctionGraph(saveGraph-2, model->id(), QFRDRColumnGraphsInterface::cgtQFFitFunction, fitresult, savedTo);
            } else {
                table->colgraphAddFunctionGraph(saveGraph-2, model->id(), QFRDRColumnGraphsInterface::cgtQFFitFunction, fitresult, lastResultD);
            }
            int gid=table->colgraphGetGraphCount(saveGraph-2)-1;
            table->colgraphSetGraphTitle(saveGraph-2, gid, resultComment+", "+resultPars+", "+resultStat);
            if (sourceGraph>=0 && sourceGraph<table->colgraphGetGraphCount(saveGraph-2)) {
                table->colgraphSetGraphColor(saveGraph-2, gid, table->getPlot(saveGraph-2).graphs[sourceGraph].color.darker());
                table->colgraphSetGraphLineWidth(saveGraph-2, gid, 1.0);
            }
            writeFitProperties(saveGraph-2, table->colgraphGetGraphCount(saveGraph-2)-1, savedTo);
        } else if (saveGraph==ui->cmbAddGraph->count()-1) {
            if (savedTo>=0) {
                table->colgraphSetFunctionGraph(overwritePlot, overwriteGraph, model->id(), QFRDRColumnGraphsInterface::cgtQFFitFunction, fitresult, savedTo);
            } else {
                table->colgraphSetFunctionGraph(overwritePlot, overwriteGraph, model->id(), QFRDRColumnGraphsInterface::cgtQFFitFunction, fitresult, lastResultD);
            }
            table->colgraphSetGraphTitle(overwritePlot, overwriteGraph, resultComment+", "+resultPars+", "+resultStat);

            writeFitProperties(overwritePlot, overwriteGraph, savedTo);
        }
        delete model;
        table->colgraphSetDoEmitSignals(emits);
    }
    accept();
}



void QFRDRTableCurveFitDialog::on_btnFit_clicked()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    connectSignals(false);

    QFFitFunction* model=ui->cmbFitFunction->createCurrentInstance();
    QFFitAlgorithm* algorithm=ui->cmbFitAlgorithm->createCurrentInstance();
    table->restoreQFFitAlgorithmParameters(algorithm);

    int rmin=getRangeMin();
    int rmax=getRangeMax();

    QVector<double> datX;
    QVector<double> datY;
    QVector<double> datW;

    /*for (int i=rmin; i<=rmax; i++) {
        datX<<dataX[i];
        datY<<dataY[i];
        datW<<dataW[i];
    }*/
    for (int i=0; i<datapoints; i++) {
        if (dataX[i]>=ui->datacut->get_userMin() && dataX[i]<=ui->datacut->get_userMax()) {
            datX<<dataX[i];
            datY<<dataY[i];
            datW<<getWeight(i);// dataW[i];
        }
    }
    double* dx=datX.data();
    double* dy=datY.data();
    double* dw=datW.data();
    if (!ui->chkWeighted->isChecked()) dw=NULL;
    int items=datX.size();

    //qDebug()<<"datapoints="<<datapoints<<"   items="<<items;


    bool ok=false;
    QString error="";

    if (model && algorithm && items>1) {
        QVector<double> fitParamsIn, fitParamsOut, fitParamsErrOut, fitParamsErrIn, fitParamsMin, fitParamsMax;
        QVector<bool> fitFix;
        QStringList ids=model->getParameterIDs();
        for (int i=0; i<ids.size(); i++) {
            QFFitFunction::ParameterDescription d=model->getDescription(ids[i]);
            fitParamsIn<<getParamValue(ids[i], d.initialValue);
            fitParamsMin<<getParamMin(ids[i], d.minValue);
            fitParamsMax<<getParamMax(ids[i], d.maxValue);
            fitFix<<getParamFix(ids[i], d.initialFix);
            fitParamsErrOut<<getParamError(ids[i], 0.0);
            //qDebug()<<"in:  "<<ids[i]<<" = "<<fitParamsIn.last()<<fitFix.last();
        }
        fitParamsOut=fitParamsIn;
        fitParamsErrIn=fitParamsErrOut;

        ok=false;




        dlgQFProgressDialog* progress=new dlgQFProgressDialog(this);
        dlgQFProgressDialogReporter* progressReporter=new dlgQFProgressDialogReporter(progress);
        progress->setWindowTitle(tr("Curve fit progress"));
        progress->reportTask(tr("fitting curve ..."));
        progress->reportStatus(tr("setting up curve fit ..."));
        progress->setProgressMax(100);
        progress->setProgress(0);
        progress->show();
        algorithm->setReporter(progressReporter);

        algorithm->setErrorEstimateModeFit(QFFitAlgorithm::fpeAlgorithm);
        if (ui->chkBootstrap->isChecked()) {
            algorithm->setErrorEstimateModeFit(QFFitAlgorithm::fpeBootstrapping);
            algorithm->setBootstrapRepeats(ui->spinBSRepeats->value());
            algorithm->setBootstrapDistortion(ui->spinBSDistortion->value()/100.0);
            algorithm->setBootstrapFraction(ui->spinBSFraction->value()/100.0);
        }

        QFFitAlgorithm::FitResult res;
        try {


            model->calcParameter(fitParamsIn.data(), fitParamsErrOut.data());
            model->calcParameter(fitParamsIn.data(), fitParamsErrOut.data());


            if (!progress->isCanceled()) {

                progress->reportStatus(tr("fitting ..."));
                progress->setProgressMax(100);
                progress->setProgress(0);
                QFFitAlgorithmThreadedFit* doFitThread=new QFFitAlgorithmThreadedFit(this);
                doFitThread->init(algorithm, fitParamsOut.data(), fitParamsErrOut.data(), dx, dy, dw, items, model, fitParamsIn.data(), fitFix.data(), fitParamsMin.data(), fitParamsMax.data());
                doFitThread->start(QThread::HighPriority);
                QTime t;
                t.start();
                while (!doFitThread->isFinished()) {
                    if (t.elapsed()>10) {
                        QApplication::processEvents(QEventLoop::AllEvents, 20);
                        if (progress->isCanceled()) {
                          doFitThread->terminate();
                          break;
                        }
                        t.start();
                    }
                }
                progress->setProgressFull();
                progress->reportStatus(tr("calculating parameters and errors ..."));
                ok=!progress->isCanceled();
                if (progress->isCanceled()) {
                    error=tr("fit canceled by user");
                }
                res=doFitThread->getResult();
                delete doFitThread;
            }



            for (int i=0; i<model->paramCount(); i++) {
                if (!(model->isParameterVisible(i, fitParamsOut.data()) && (!fitFix[i]) && model->getDescription(i).fit)) {
                    fitParamsErrOut[i]=fitParamsErrIn[i];
                }
            }

            model->calcParameter(fitParamsOut.data(), fitParamsErrOut.data());
            model->sortParameter(fitParamsOut.data(), fitParamsErrOut.data(), fitFix.data());
            model->calcParameter(fitParamsOut.data(), fitParamsErrOut.data());



        } catch(std::exception& E) {
            QFPluginLogTools::log_error(tr("error during curve fitting, error message: %1\n").arg(E.what()));
            error=E.what();
            ok=false;
        }




        lastResults.clear();
        lastResultsErrors.clear();
        parameterNames.clear();
        parameterLabels.clear();

        lastResultD.clear();
        resultComment="";
        resultCommentLong="";
        resultPars="";
        if (ok && res.fitOK) {
            for (int i=0; i<ids.size(); i++) {
                paramMap[ids[i]].value=fitParamsOut[i];
                paramMap[ids[i]].error=fitParamsErrOut[i];
                paramMap[ids[i]].fix=fitFix[i];
                lastResults<<fitParamsOut[i];
                lastResultsErrors<<fitParamsErrOut[i];
                lastResultD<<fitParamsOut[i];
                if (model->isParameterVisible(i, fitParamsOut.data())) {
                    if (!resultPars.isEmpty()) resultPars+=tr(", ");
                    resultPars+=QString("%1=(%2{\\pm}%3)").arg(ids[i]).arg(doubleToLatexQString(roundWithError(fitParamsOut[i], fitParamsErrOut[i]))).arg(doubleToLatexQString(roundError(fitParamsErrOut[i])));
                }
                parameterNames<<ids[i];
                parameterLabels<<model->getDescription(i).name;

            }
            resultComment=tr("model: %1").arg(model->shortName());
            resultCommentLong=tr("<b>model:</b> %1").arg(model->shortName());
            resultCommentLong+=tr("<br><b>algorithm:</b> %1").arg(algorithm->shortName());
            resultCommentLong+=tr("<br><b>message:</b> %1").arg(res.messageSimple);
            ok=true;
        } else {
            error=tr("Fit failed.\n  Reason: %1.").arg(res.messageSimple);
            ok=false;
        }


        if (progress) {
            algorithm->setReporter(NULL);
            delete progressReporter;
            progress->close();
            progress->deleteLater();
        }





    }

    updateFitStatistics();

    QApplication::restoreOverrideCursor();
    parameterTable->rebuildModel();
    replotGraph();
    if (!ok) {
        QMessageBox::critical(this, tr("Fit Error"), tr("an error occured during the model fit.\nerror message:\n    %1").arg(error));
    }
    connectSignals(true);
    if (model) delete model;
    if (algorithm) delete algorithm;
}

void QFRDRTableCurveFitDialog::on_btnGuess_clicked()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    connectSignals(false);

    QFFitFunction* model=ui->cmbFitFunction->createCurrentInstance();

    int rmin=getRangeMin();
    int rmax=getRangeMax();

    QVector<double> datX;
    QVector<double> datY;
    QVector<double> datW;

    for (int i=0; i<datapoints; i++) {
        if (dataX[i]>=ui->datacut->get_userMin() && dataX[i]<=ui->datacut->get_userMax()) {
            datX<<dataX[i];
            datY<<dataY[i];
            datW<<getWeight(i);//dataW[i];
        }
    }
    double* dx=datX.data();
    double* dy=datY.data();
    double* dw=datW.data();
    int items=datX.size();


    bool ok=false;
    QString error="";

    if (model && items>1) {
        QVector<double> fitParams, fitParamsErrs, fitParamsMin, fitParamsMax;
        QVector<bool> fitFix;
        QStringList ids=model->getParameterIDs();
        bool hasFixed=false;
        for (int i=0; i<ids.size(); i++) {
            QFFitFunction::ParameterDescription d=model->getDescription(ids[i]);
            fitParams<<getParamValue(ids[i], d.initialValue);
            fitParamsMin<<getParamMin(ids[i], d.minValue);
            fitParamsMax<<getParamMax(ids[i], d.maxValue);
            fitFix<<getParamFix(ids[i], d.initialFix);
            hasFixed=hasFixed||fitFix.last();
            fitParamsErrs<<0;
        }
        QVector<double> fitParamsInit=fitParams;
        ok=model->estimateInitial(fitParams.data(), dx, dy, items, fitFix.data());

        model->calcParameter(fitParams.data(), fitParamsErrs.data());
        model->sortParameter(fitParams.data(), fitParamsErrs.data(), fitFix.data());
        model->calcParameter(fitParams.data(), fitParamsErrs.data());








        lastResults.clear();
        lastResultsErrors.clear();
        parameterNames.clear();
        parameterLabels.clear();

        lastResultD.clear();
        resultComment="";
        resultCommentLong="";

        bool replaceFixed=true;
        if (hasFixed) {
            bool fixedChanged=false;
            for (int i=0; i<ids.size(); i++) {
                if (fitFix[i] && fitParamsInit[i]!=fitParams[i]) {
                    fixedChanged=true;
                    break;
                }
            }
            if (fixedChanged) {
                replaceFixed=(QMessageBox::question(this, tr("Parameter guess"), tr("The fit function returned guessed values for fixed parameters.\nDo you want to overwrite this fixed parameter values?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::Yes);
            }

        }

        if (ok) {
            for (int i=0; i<ids.size(); i++) {
                if (!fitFix[i] || (fitFix[i] && replaceFixed)) {
                    paramMap[ids[i]].value=fitParams[i];
                    paramMap[ids[i]].error=0;
                    paramMap[ids[i]].fix=fitFix[i];
                    lastResults<<fitParams[i];
                    lastResultsErrors<<0;
                    lastResultD<<fitParams[i];
                    parameterNames<<ids[i];
                    parameterLabels<<model->getDescription(i).name;
                } else {
                    paramMap[ids[i]].value=fitParamsInit[i];
                    paramMap[ids[i]].error=0;
                    paramMap[ids[i]].fix=fitFix[i];
                    lastResults<<fitParamsInit[i];
                    lastResultsErrors<<0;
                    lastResultD<<fitParamsInit[i];
                    parameterNames<<ids[i];
                    parameterLabels<<model->getDescription(i).name;
                }
                resultPars+=QString("%1=%2").arg(ids[i]).arg(doubleToLatexQString(fitParamsInit[i]));
            }
            resultComment=tr("guess: %1").arg(model->shortName());
            resultCommentLong=tr("<b>guess:</b> %1").arg(model->shortName());
            ok=true;
        } else {
            error=tr("Guess failed.");
            ok=false;
        }





    }

    updateFitStatistics();

    QApplication::restoreOverrideCursor();
    parameterTable->rebuildModel();
    replotGraph();
    if (!ok) {
        QMessageBox::critical(this, tr("Guess Error"), tr("an error occured during the model parameter guess.\nerror message:\n    %1").arg(error));
    }

    connectSignals(true);
    if (model) delete model;
}

void QFRDRTableCurveFitDialog::on_btnConfigAlgorithm_clicked()
{
    QFFitAlgorithm* algorithm=ui->cmbFitAlgorithm->createCurrentInstance();
    if (algorithm) {
        table->restoreQFFitAlgorithmParameters(algorithm);
        table->configFitAlgorithm(algorithm);
    }
}


void QFRDRTableCurveFitDialog::replotGraph()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QFFitFunction* model=ui->cmbFitFunction->createCurrentInstance();
    updateFitStatistics();

    ui->pltDistribution->set_doDrawing(false);
    ui->pltDistribution->set_emitSignals(false);
    ui->pltResiduals->set_doDrawing(false);
    ui->pltResiduals->set_emitSignals(false);
    ui->pltResiduals->getXAxis()->set_logAxis(ui->chkLogX->isChecked());
    ui->pltDistribution->getXAxis()->set_logAxis(ui->chkLogX->isChecked());
    ui->pltDistribution->getYAxis()->set_logAxis(ui->chkLogY->isChecked());
    ui->pltDistribution->get_plotter()->clearGraphs(true);
    ui->pltDistribution->get_plotter()->clearOverlayElement(true);
    ui->pltResiduals->get_plotter()->clearGraphs(true);
    ui->pltResiduals->get_plotter()->clearOverlayElement(true);

    JKQTPdatastore* ds=ui->pltDistribution->get_plotter()->getDatastore();
    ds->clear();
    size_t c_X=ds->addCopiedColumn(dataX.data(), dataX.size(), tr("x-data"));
    size_t c_Y=ds->addCopiedColumn(dataY.data(), dataY.size(), tr("y-data"));
    QVector<double> datW, resW;
    for (int i=0; i<dataY.size(); i++) {
        datW<<getWeight(i);
        resW<<(residualsY.value(i, 0.0)/datW.last());
    }
    size_t c_W=ds->addCopiedColumn(datW.data(), datW.size(), tr("weight"));
    size_t c_ResY=ds->addCopiedColumn(residualsY.data(), residualsY.size(), tr("residuals"));
    size_t c_ResYW=ds->addCopiedColumn(resW.data(), resW.size(), tr("weighted residuals"));

    size_t c_Xlim=ds->addCopiedColumn(dataX.data()+getRangeMin(), qMin(dataX.size()-getRangeMin(), getRangeMax()-getRangeMin()), tr("x-data, limited"));
    size_t c_Ylim=ds->addCopiedColumn(dataY.data(), qMin(dataY.size()-getRangeMin(), getRangeMax()-getRangeMin()), tr("y-data, limitd"));
    size_t c_Wlim=ds->addCopiedColumn(datW.data()+getRangeMin(), qMin(datW.size()-getRangeMin(), getRangeMax()-getRangeMin()), tr("weight, limited"));
    size_t c_ResYlim=ds->addCopiedColumn(residualsY.data()+getRangeMin(), qMin(residualsY.size()-getRangeMin(), getRangeMax()-getRangeMin()), tr("residuals, limited"));
    size_t c_ResYWlim=ds->addCopiedColumn(resW.data()+getRangeMin(), qMin(resW.size()-getRangeMin(), getRangeMax()-getRangeMin()), tr("weighted residuals, limited"));

    JKQTPxyLineErrorGraph* g_data=new JKQTPxyLineErrorGraph(ui->pltDistribution->get_plotter());
    g_data->set_drawLine(true);
    g_data->set_title(tr("data"));
    g_data->set_xColumn(c_X);
    g_data->set_yColumn(c_Y);
    g_data->set_yErrorColumn(c_W);
    g_data->set_xErrorStyle(JKQTPnoError);
    if (ui->chkPlotErrors->isChecked()) g_data->set_yErrorStyle(JKQTPerrorBars);
    else g_data->set_yErrorStyle(JKQTPnoError);
    g_data->set_color(QColor("blue"));
    g_data->set_errorColor(g_data->get_color().lighter());
    g_data->set_drawLine(false);
    g_data->set_symbol(JKQTPcross);
    g_data->set_symbolSize(7);
    ui->pltDistribution->addGraph(g_data);

    JKQTPoverlayVerticalRange* ovlRange=new JKQTPoverlayVerticalRange(ui->datacut->get_userMin(), ui->datacut->get_userMax(), ui->pltDistribution->get_plotter());
    ovlRange->set_inverted(true);
    QColor fillRange=QColor("grey");
    fillRange.setAlphaF(0.75);
    ovlRange->set_fillColor(fillRange);
    ui->pltDistribution->get_plotter()->addOverlayElement(ovlRange);

    if (model) {
        JKQTPxQFFitFunctionLineGraph* g_fit=new JKQTPxQFFitFunctionLineGraph(ui->pltDistribution->get_plotter());
        g_fit->set_drawLine(true);
        g_fit->set_title(fitresult);
        g_fit->set_color(QColor("red"));
        g_fit->set_fitFunction(model, true);
        QStringList ids=model->getParameterIDs();
        QVector<double> vecP;
        for (int i=0; i<ids.size(); i++) {
            QFFitFunction::ParameterDescription d=model->getDescription(ids[i]);
            vecP<<getParamValue(ids[i], d.initialValue);
        }
        g_fit->set_params(vecP);
        ui->pltDistribution->addGraph(g_fit);
    }


    JKQTPxyLineGraph* g_res=new JKQTPxyLineGraph(ui->pltResiduals->get_plotter());
    g_res->set_drawLine(true);
    g_res->set_xColumn(c_Xlim);
    g_res->set_sortData(JKQTPxyGraph::SortedX);
    if (ui->chkWeightedResiduals->isChecked()) {
        //qDebug()<<"weighted";
        g_res->set_title(tr("weighted residuals"));
        g_res->set_yColumn(c_ResYWlim);
    } else {
        //qDebug()<<"non-weighted";
        g_res->set_title(tr("residuals"));
        g_res->set_yColumn(c_ResYlim);
    }
    g_res->set_color(g_data->get_color());
    g_res->set_symbol(JKQTPcross);
    g_res->set_symbolSize(7);
    ui->pltResiduals->addGraph(g_res);


    //qDebug()<<"----"<<ui->pltResiduals->getYMin()<<ui->pltResiduals->getYMax();
    ui->pltResiduals->zoomToFit(false,true);
    //qDebug()<<ui->pltResiduals->getYMin()<<ui->pltResiduals->getYMax();
    ui->pltResiduals->set_doDrawing(true);
    ui->pltResiduals->set_emitSignals(true);
    ui->pltResiduals->update_plot();
    //qDebug()<<ui->pltResiduals->getYMin()<<ui->pltResiduals->getYMax();
    ui->pltDistribution->set_doDrawing(true);
    ui->pltDistribution->set_emitSignals(true);
    //qDebug()<<ui->pltResiduals->getYMin()<<ui->pltResiduals->getYMax();
    ui->pltDistribution->zoomToFit();
    //qDebug()<<ui->pltResiduals->getYMin()<<ui->pltResiduals->getYMax();
    ui->pltDistribution->update_plot();
    //qDebug()<<ui->pltResiduals->getYMin()<<ui->pltResiduals->getYMax()<<"----";
    QApplication::restoreOverrideCursor();
}
void QFRDRTableCurveFitDialog::showHelp()
{
    QFPluginServices::getInstance()->displayHelpWindow(QFPluginServices::getInstance()->getPluginHelpDirectory("table")+"fit.html");
}


void QFRDRTableCurveFitDialog::methodChanged(int /*method*/)
{
    parameterTable->setDoRebuildModel(false);
    parameterTable->setEditErrors(true);
    parameterTable->setEditFix(true);
    parameterTable->setEditRanges(true);
    parameterNames.clear();
    parameterLabels.clear();
    //paramMap.clear();
    QFFitFunction* model=ui->cmbFitFunction->createCurrentInstance();
    QStringList ids=model->getParameterIDs();
    for (int i=0; i<ids.size(); i++) {
        QFFitFunction::ParameterDescription d=model->getDescription(ids[i]);
        if (!paramMap.contains(ids[i])) {
            paramMap[ids[i]].fix=d.initialFix;
            paramMap[ids[i]].error=0;
            paramMap[ids[i]].value=d.initialValue;
            paramMap[ids[i]].max=d.maxValue;
            paramMap[ids[i]].min=d.minValue;
        }
        paramMap[ids[i]].editable=d.userEditable;
    }

    ui->btnGuess->setEnabled(model->estimateInitial(NULL, NULL, NULL, 0));

    parameterTable->setWriteTo(&paramMap, model, true);
    parameterTable->setDoRebuildModel(true);
    ui->tabParams->setUpdatesEnabled(false);
    for (int i=0; i<ui->tabParams->model()->rowCount(); i++) {
        ui->tabParams->setRowHeight(i, QFontMetricsF(ui->tabParams->font()).height()*1.6);
    }
    ui->tabParams->verticalHeader()->setVisible(false);
    //ui->tabParams->horizontalHeader()
    for (int i=0; i<ui->tabParams->model()->columnCount(); i++) {
        if (i==0) ui->tabParams->setColumnWidth(i, 100);
        else if ((i-1)%parameterTable->getColsPerRDR()==0) ui->tabParams->setColumnWidth(i, 75);
        else if ((i-1)%parameterTable->getColsPerRDR()==1) ui->tabParams->setColumnWidth(i, 50);
        else if ((i-1)%parameterTable->getColsPerRDR()==2) ui->tabParams->setColumnWidth(i, 20);
        else if ((i-1)%parameterTable->getColsPerRDR()==3) ui->tabParams->setColumnWidth(i, 75);
        else if ((i-1)%parameterTable->getColsPerRDR()==4) ui->tabParams->setColumnWidth(i, 75);
    }
    ui->tabParams->setUpdatesEnabled(true);
    //on_btnFit_clicked();
}

void QFRDRTableCurveFitDialog::updateFitStatistics()
{
    QFFitFunction* model=ui->cmbFitFunction->createCurrentInstance();
    if (model) {
        QStringList ids=model->getParameterIDs();
        QVector<double> vecP;
        int params=0;
        for (int i=0; i<ids.size(); i++) {
            QFFitFunction::ParameterDescription d=model->getDescription(ids[i]);
            vecP<<getParamValue(ids[i], d.initialValue);
            if (d.fit &&  !getParamFix(ids[i], d.initialFix)){
                params++;
            }
        }
        residualsY.clear();
        residualsYW.clear();

        for (int i=0; i<dataX.size(); i++) {
            double v=model->evaluate(dataX[i], vecP.data());
            if (i<dataY.size()) {
                residualsY<<dataY[i]-v;
            } else {
                break;
            }
            if (i<dataY.size()) {
                residualsYW<<(dataY[i]-v)/getWeight(i);
            }
        }


        int items=residualsY.size();

        // estimate goodness-of-fit
        resultStat="";
        resultStatLong="";
        if (residualsY.size()>0) {
            double chi2=0;
            double chi2w=0;
            double TSS=0;
            double obsMean=0;
            for (int i=0; i<items; i++) {
                obsMean+=dataY[i];
            }
            obsMean=obsMean/double(items);
            for (int i=0; i<items; i++) {
                chi2+=qfSqr(residualsY[i]);
                chi2w+=qfSqr(residualsYW[i]);
                TSS+=qfSqr(dataY[i]-obsMean);
            }
            double R2=1.0-chi2/TSS;
            paramMap["chi2"].value=chi2;
            paramMap["chi2"].error=0;
            paramMap["chi2"].fix=false;
            paramMap["chi2"].editable=false;
            resultStat+=tr("\\chi^2=%1").arg(chi2);
            resultStatLong+=tr("&chi;<sup>2</sup>=%1").arg(chi2);
            paramMap["R2"].value=R2;
            paramMap["R2"].error=0;
            paramMap["R2"].fix=false;
            paramMap["R2"].editable=false;
            resultStat+=tr(", R^2=%1").arg(R2);
            resultStatLong+=tr(", R<sup>2</sup>=%1").arg(R2);
            resultStatLong+=tr("<br>parameters: %1").arg(params);
            resultStatLong+=tr("<br>data points: %1").arg(dataX.size());

        }

        ui->labFitResult->setHtml(resultCommentLong+"<br><b>statistics:</b><blockquote>"+resultStatLong+"</blockquote>");


        delete model;
    }

}

double QFRDRTableCurveFitDialog::getWeight(int i) const
{
    if (qMin(dataX.size(), dataY.size())<=0) return 1.0;
    const double defaultVal=1.0;//double(qMin(dataX.size(), dataY.size()));
    switch(ui->cmbWeightOptions->currentIndex()) {
        case 0: return fabs(dataW.value(i, defaultVal));
        case 1: return sqrt(fabs(dataY.value(i, defaultVal)));
        case 2: return fabs(dataY.value(i, defaultVal));
        default: return defaultVal;
    }
    return defaultVal;
}



void QFRDRTableCurveFitDialog::connectSignals(bool connectS)
{
    if (connectS) {
        connect(parameterTable, SIGNAL(fitParamChanged()), this, SLOT(replotGraph()));
        connect(ui->cmbFitFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(methodChanged(int)));
        connect(ui->chkLogX, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        connect(ui->chkLogY, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        connect(ui->chkPlotErrors, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        connect(ui->datacut, SIGNAL(slidersChanged(double,double,double,double)), this, SLOT(replotGraph()));
        connect(ui->chkWeightedResiduals, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        connect(ui->cmbWeightOptions, SIGNAL(currentIndexChanged(int)), this, SLOT(replotGraph()));
        connect(ui->chkWeighted, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
    } else {
        disconnect(parameterTable, SIGNAL(fitParamChanged()), this, SLOT(replotGraph()));
        disconnect(ui->cmbFitFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(methodChanged(int)));
        disconnect(ui->chkLogX, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        disconnect(ui->chkLogY, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        disconnect(ui->chkPlotErrors, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        disconnect(ui->datacut, SIGNAL(slidersChanged(double,double,double,double)), this, SLOT(replotGraph()));
        disconnect(ui->chkWeightedResiduals, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
        disconnect(ui->cmbWeightOptions, SIGNAL(currentIndexChanged(int)), this, SLOT(replotGraph()));
        disconnect(ui->chkWeighted, SIGNAL(toggled(bool)), this, SLOT(replotGraph()));
    }
}

void QFRDRTableCurveFitDialog::readDataFromTable()
{
    selection.getDataWithStride(&dataX, NULL, colX, table, NULL, NULL);
    selection.getDataWithStride(&dataY, NULL, colY, table, NULL, NULL);
    //dataX=table->tableGetColumnDataAsDouble(colX);
    //dataY=table->tableGetColumnDataAsDouble(colY);
    //qDebug()<<"dataX.size="<<dataX.size();
    //qDebug()<<"dataY.size="<<dataX.size();
    datapoints=qMin(dataX.size(), dataY.size());
    if (colW!=-1) {
        //dataW=table->tableGetColumnDataAsDouble(colW);
        selection.getDataWithStride(&dataW, NULL, colW, table, NULL, NULL);
        //qDebug()<<"dataW.size="<<dataW.size();
        if (dataW.size()>0 && dataW.size()<datapoints) datapoints=dataW.size();
    } else {
        dataW=QVector<double>(datapoints, 1);
    }
    if (dataX.size()>datapoints) dataX.remove(dataX.size()-(dataX.size()-datapoints), (dataX.size()-datapoints));
    if (dataY.size()>datapoints) dataY.remove(dataY.size()-(dataY.size()-datapoints), (dataY.size()-datapoints));
    if (dataW.size()>datapoints) dataW.remove(dataW.size()-(dataW.size()-datapoints), (dataW.size()-datapoints));

    //qDebug()<<"datapoints="<<datapoints;
    for (int i=datapoints-1; i>=0; i--) {
        if (!QFFloatIsOK(dataX[i]) || !QFFloatIsOK(dataY[i]) || (dataW.size()==dataX.size() && !QFFloatIsOK(dataW[i]))) {
            dataX.remove(i, 1);
            dataY.remove(i, 1);
            dataW.remove(i, 1);
        }
    }
    datapoints=dataX.size();
    //weights.clear();
    //for (int i=0; i<dataW.size(); i++) weights<<1.0/qfSqr(dataW[i]);
    //qDebug()<<"datapoints_after_clean="<<datapoints;

    if (dataX.size()>0) {
        double mi=qfstatisticsMin(dataX);
        double mif=0.1*fabs(mi);
        if (fabs(mif)<1e-15) mif=1;
        ui->datacut->set_min(mi-mif);
        ui->datacut->set_userMin(mi-mif);
        double ma=qfstatisticsMax(dataX);
        double maf=0.1*fabs(ma);
        if (fabs(maf)<1e-15) maf=1;
        ui->datacut->set_max(ma+maf);
        ui->datacut->set_userMax(ma+maf);
    }

}

int QFRDRTableCurveFitDialog::getRangeMin()
{
    int rm=0;
    for (int i=0; i<datapoints; i++) {
        if (dataX[i]>=ui->datacut->get_userMin()) {
            rm=i;
            break;
        }
    }
    return qBound(rm, 0, dataX.size()-1);
}

int QFRDRTableCurveFitDialog::getRangeMax()
{
/*    int rm=datapoints-1;
    for (int i=datapoints-1; i>=0; i--) {
        if (dataX[i]>ui->datacut->get_userMax()) {
            rm=i;
        } else {
            break;
        }
    }

    return qBound(0, rm, dataX.size()-1);*/
    int rm=getRangeMin();
    for (int i=rm; i<dataX.size(); i++) {
        if (dataX[i]<=ui->datacut->get_userMax()) {
            rm=i;
        } else {
            break;
        }
    }

    return qBound(0, rm, dataX.size()-1);

}



double QFRDRTableCurveFitDialog::getParamValue(const QString &param, double defaultVal) const
{
    return paramMap.value(param, QFFitFunctionValueInputTableFitParamData(defaultVal,0)).value;
}

double QFRDRTableCurveFitDialog::getParamError(const QString &param, double defaultVal) const
{
    return paramMap.value(param, QFFitFunctionValueInputTableFitParamData(0,defaultVal)).error;
}

bool QFRDRTableCurveFitDialog::getParamFix(const QString &param, bool defaultVal) const
{
    return paramMap.value(param, QFFitFunctionValueInputTableFitParamData(0,0,defaultVal)).fix;
}

double QFRDRTableCurveFitDialog::getParamMin(const QString &param, double defaultVal) const
{
    return paramMap.value(param, QFFitFunctionValueInputTableFitParamData(0,0,false,defaultVal)).min;
}

double QFRDRTableCurveFitDialog::getParamMax(const QString &param, double defaultVal) const
{
    return paramMap.value(param, QFFitFunctionValueInputTableFitParamData(0,0,false,defaultVal)).max;
}

void QFRDRTableCurveFitDialog::writeFitProperties(int pid, int gid, int saveToColumn)
{
    bool emits=table->colgraphGetDoEmitSignals();
    table->colgraphSetDoEmitSignals(false);
    table->colgraphSetGraphProperty(pid, gid, "FIT_TYPE", "LEAST_SQUARES");
    table->colgraphSetGraphProperty(pid, gid, "FIT_COLX", colX);
    table->colgraphSetGraphProperty(pid, gid, "FIT_COLY", colY);
    table->colgraphSetGraphProperty(pid, gid, "FIT_COLW", colW);
    table->colgraphSetGraphProperty(pid, gid, "FIT_ALGORITHM", ui->cmbFitAlgorithm->currentFitAlgorithmID());
    table->colgraphSetGraphProperty(pid, gid, "FIT_MODEL", ui->cmbFitFunction->currentFitFunctionID());
    table->colgraphSetGraphProperty(pid, gid, "FIT_CUTLOW", ui->datacut->get_userMin());
    table->colgraphSetGraphProperty(pid, gid, "FIT_CUTHIGH", ui->datacut->get_userMax());
    table->colgraphSetGraphProperty(pid, gid, "FIT_WEIGHTED", ui->chkWeighted->isChecked());
    table->colgraphSetGraphProperty(pid, gid, "FIT_WEIGHTED_MODE", ui->cmbWeightOptions->currentIndex());
    table->colgraphSetGraphProperty(pid, gid, "FIT_BOOTSTRAPPING", ui->chkBootstrap->isChecked());
    table->colgraphSetGraphProperty(pid, gid, "FIT_BS_DISTORTION", ui->spinBSDistortion->value());
    table->colgraphSetGraphProperty(pid, gid, "FIT_BS_FRACTION", ui->spinBSFraction->value());
    table->colgraphSetGraphProperty(pid, gid, "FIT_BS_REPEATS", ui->spinBSRepeats->value());
    table->colgraphSetGraphProperty(pid, gid, "FIT_FITLOGY", ui->chkFitLogY->isChecked());
    table->colgraphSetGraphProperty(pid, gid, "FIT_LOGX", ui->chkLogX->isChecked());
    table->colgraphSetGraphProperty(pid, gid, "FIT_LOGY", ui->chkLogY->isChecked());
    table->colgraphSetGraphProperty(pid, gid, "FIT_RESULTCOLUMN", ui->cmbSaveColumn->currentIndex());
    table->colgraphSetGraphProperty(pid, gid, "FIT_PLOT", pid);
    table->colgraphSetGraphProperty(pid, gid, "FIT_GRAPH", gid);
    table->colgraphSetGraphProperty(pid, gid, "FIT_SAVEDTOCOLUMN", saveToColumn);
    QFFitFunction* ff=ui->cmbFitFunction->createCurrentInstance();
    if (ff) {
        QStringList fpids=ff->getParameterIDs();
        QList<QVariant> fixparams, errparams;
        for (int i=0; i<fpids.size(); i++) {
            fixparams<<paramMap[fpids[i]].fix;
            errparams<<paramMap[fpids[i]].error;
        }
        delete ff;
        table->colgraphSetGraphProperty(pid, gid, "FIT_FIXEDPARAMS", fixparams);
        table->colgraphSetGraphProperty(pid, gid, "FIT_ERRORPARAMS", errparams);
    }


    table->colgraphSetGraphProperty(pid, gid, "FIT_isStrided", selection.isStrided);
    table->colgraphSetGraphProperty(pid, gid, "FIT_stride", selection.stride);
    table->colgraphSetGraphProperty(pid, gid, "FIT_strideStart", selection.strideStart);

    table->colgraphSetGraphProperty(pid, gid, "FIT_isDataSelect", selection.isDataSelect);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect1Column", selection.dataSelect1Column);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect1Operation", selection.dataSelect1Operation);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect1CompareValue", selection.dataSelect1CompareValue);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect1CompareValue2", selection.dataSelect1CompareValue2);

    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelectLogic12", selection.dataSelectLogic12);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect2Column", selection.dataSelect2Column);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect2Operation", selection.dataSelect2Operation);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect2CompareValue", selection.dataSelect2CompareValue);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect2CompareValue2", selection.dataSelect2CompareValue2);

    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelectLogic23", selection.dataSelectLogic23);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect3Column", selection.dataSelect3Column);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect3Operation", selection.dataSelect3Operation);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect3CompareValue", selection.dataSelect3CompareValue);
    table->colgraphSetGraphProperty(pid, gid, "FIT_dataSelect3CompareValue2", selection.dataSelect3CompareValue2);

    table->colgraphSetDoEmitSignals(emits);
}

void QFRDRTableCurveFitDialog::readFitProperties(int pid, int gid, int* resultColumn, int*addGraph)
{
    colX=table->colgraphGetGraphProperty(pid, gid, "FIT_COLX", colX).toInt();
    colY=table->colgraphGetGraphProperty(pid, gid, "FIT_COLY", colY).toInt();
    colW=table->colgraphGetGraphProperty(pid, gid, "FIT_COLW", colW).toInt();


    selection.isStrided=table->colgraphGetGraphProperty(pid, gid, "FIT_isStrided", selection.isStrided).toBool();
    selection.stride=table->colgraphGetGraphProperty(pid, gid, "FIT_stride", selection.stride).toInt();
    selection.strideStart=table->colgraphGetGraphProperty(pid, gid, "FIT_strideStart", selection.strideStart).toInt();

    selection.isDataSelect=table->colgraphGetGraphProperty(pid, gid, "FIT_isDataSelect", selection.isDataSelect).toInt();
    selection.dataSelect1Column=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect1Column", selection.dataSelect1Column).toInt();
    selection.dataSelect1Operation=(QFRDRTable::DataSelectOperation)table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect1Operation", selection.dataSelect1Operation).toInt();
    selection.dataSelect1CompareValue=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect1CompareValue", selection.dataSelect1CompareValue).toDouble();
    selection.dataSelect1CompareValue2=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect1CompareValue2", selection.dataSelect1CompareValue2).toDouble();

    selection.dataSelectLogic12=(QFRDRTable::DataSelectLogicOperation)table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelectLogic12", selection.dataSelectLogic12).toInt();
    selection.dataSelect2Column=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect2Column", selection.dataSelect2Column).toInt();
    selection.dataSelect2Operation=(QFRDRTable::DataSelectOperation)table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect2Operation", selection.dataSelect2Operation).toInt();
    selection.dataSelect2CompareValue=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect2CompareValue", selection.dataSelect2CompareValue).toDouble();
    selection.dataSelect2CompareValue2=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect2CompareValue2", selection.dataSelect2CompareValue2).toDouble();

    selection.dataSelectLogic23=(QFRDRTable::DataSelectLogicOperation)table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelectLogic23", selection.dataSelectLogic23).toInt();
    selection.dataSelect3Column=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect3Column", selection.dataSelect3Column).toInt();
    selection.dataSelect3Operation=(QFRDRTable::DataSelectOperation)table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect3Operation", selection.dataSelect3Operation).toInt();
    selection.dataSelect3CompareValue=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect3CompareValue", selection.dataSelect3CompareValue).toDouble();
    selection.dataSelect3CompareValue2=table->colgraphGetGraphProperty(pid, gid, "FIT_dataSelect3CompareValue2", selection.dataSelect3CompareValue2).toDouble();


    ui->cmbFitAlgorithm->setCurrentAlgorithm(table->colgraphGetGraphProperty(pid, gid, "FIT_ALGORITHM", ui->cmbFitAlgorithm->currentFitAlgorithmID()).toString());
    ui->cmbFitFunction->setCurrentFitFunction(table->colgraphGetGraphProperty(pid, gid, "FIT_MODEL", ui->cmbFitFunction->currentFitFunctionID()).toString());
    ui->datacut->set_userMin(table->colgraphGetGraphProperty(pid, gid, "FIT_CUTLOW", ui->datacut->get_userMin()).toDouble());
    ui->datacut->set_userMax(table->colgraphGetGraphProperty(pid, gid, "FIT_CUTHIGH", ui->datacut->get_userMax()).toDouble());
    bool logX=false;
    ui->chkWeighted->setChecked(table->colgraphGetGraphProperty(pid, gid, "FIT_WEIGHTED", ui->chkWeighted->isChecked()).toBool());
    ui->cmbWeightOptions->setCurrentIndex(table->colgraphGetGraphProperty(pid, gid, "FIT_WEIGHTED_MODE", ui->cmbWeightOptions->currentIndex()).toInt());
    //qDebug()<<pid<<gid<<"FIT_BOOTSTRAPPING="<<table->colgraphGetGraphProperty(pid, gid, "FIT_BOOTSTRAPPING");
    ui->chkBootstrap->setChecked(table->colgraphGetGraphProperty(pid, gid, "FIT_BOOTSTRAPPING", ui->chkBootstrap->isChecked()).toBool());

    ui->spinBSDistortion->setValue(table->colgraphGetGraphProperty(pid, gid, "FIT_BS_DISTORTION", ui->spinBSDistortion->value()).toDouble());
    ui->spinBSFraction->setValue(table->colgraphGetGraphProperty(pid, gid, "FIT_BS_FRACTION", ui->spinBSFraction->value()).toDouble());
    ui->spinBSRepeats->setValue(table->colgraphGetGraphProperty(pid, gid, "FIT_BS_REPEATS", ui->spinBSRepeats->value()).toInt());



    ui->chkFitLogY->setChecked(table->colgraphGetGraphProperty(pid, gid, "FIT_FITLOGY", ui->chkFitLogY->isChecked()).toBool());
    ui->chkLogX->setChecked(logX=table->colgraphGetGraphProperty(pid, gid, "FIT_LOGX", ui->chkLogX->isChecked()).toBool());
    ui->chkLogY->setChecked(table->colgraphGetGraphProperty(pid, gid, "FIT_LOGY", ui->chkLogY->isChecked()).toBool());
    ui->datacut->setLogScale(logX, 20);

    int savedToColumn=table->colgraphGetGraphProperty(pid, gid, "FIT_SAVEDTOCOLUMN", -1).toInt();
    QList<QVariant> fixparams=table->colgraphGetGraphProperty(pid, gid, "FIT_FIXEDPARAMS").toList();
    QList<QVariant> errorparams=table->colgraphGetGraphProperty(pid, gid, "FIT_ERRORPARAMS").toList();

    QFFitFunction* ff=ui->cmbFitFunction->createCurrentInstance();
    //qDebug()<<"readFitProperties :"<<ff<<savedToColumn;
    if (ff) {
        QStringList fpids=ff->getParameterIDs();
        //qDebug()<<"readFitProperties :"<<fpids;
        if (savedToColumn>=0)  {
            QList<QVariant> oldparams=table->tableGetColumnData(savedToColumn);
            QList<QVariant> oldparams1;
            if (savedToColumn+1<table->tableGetColumnCount()) oldparams1=table->tableGetColumnData(savedToColumn+1);
            for (int i=0; i<fpids.size(); i++) {
                paramMap[fpids[i]].value=oldparams.value(i, ff->getDescription(i).initialValue).toDouble();
                paramMap[fpids[i]].error=oldparams1.value(i, 0).toDouble();
                paramMap[fpids[i]].min=ff->getDescription(i).minValue;
                paramMap[fpids[i]].max=ff->getDescription(i).maxValue;
                paramMap[fpids[i]].fix=fixparams.value(i, ff->getDescription(i).initialFix).toBool();
            }
        } else {
            if (pid>=0 && pid<table->getPlotCount()) {
                const QFRDRTable::PlotInfo pi=table->getPlot(pid);
                if (gid>=0 && gid<pi.graphs.size()) {
                    for (int i=0; i<fpids.size(); i++) {
                        paramMap[fpids[i]].value=pi.graphs[gid].functionParameters.value(i, ff->getDescription(i).initialValue);
                        paramMap[fpids[i]].error=errorparams.value(i, 0).toDouble();
                        paramMap[fpids[i]].min=ff->getDescription(i).minValue;
                        paramMap[fpids[i]].max=ff->getDescription(i).maxValue;
                        paramMap[fpids[i]].fix=fixparams.value(i, ff->getDescription(i).initialFix).toBool();
                    }

                }
            }
        }
        delete ff;
    }

    if (addGraph) *addGraph=gid;
    if (resultColumn) *resultColumn=savedToColumn;

    readDataFromTable();
}

