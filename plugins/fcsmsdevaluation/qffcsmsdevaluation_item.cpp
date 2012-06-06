#include "qffcsmsdevaluation_item.h"
#include "qffcsmsdevaluation_editor.h"
#include "../interfaces/qfrdrfcsdatainterface.h"
#include "qfmathtools.h"



#define sqr(x) ((x)*(x))

QFFCSMSDEvaluationItem::QFFCSMSDEvaluationItem(QFProject* parent):
    QFUsesResultsByIndexAndModelEvaluation(parent, true, false)
{
    currentIndex=-1;
    currentModel=0;
    currentWeights=0;
}

QFFCSMSDEvaluationItem::~QFFCSMSDEvaluationItem() {

}

QString QFFCSMSDEvaluationItem::getEvaluationResultID(int currentIndex, int model) const {
    if (currentIndex<0) return QString("%1_%2_m%3_runavg").arg(getType()).arg(getID()).arg(model);
    return QString("%1_%2_m%3_run%4").arg(getType()).arg(getID()).arg(model).arg(currentIndex);
}



void QFFCSMSDEvaluationItem::intWriteData(QXmlStreamWriter& w) {
    QFUsesResultsByIndexAndModelEvaluation::intWriteData(w);
    w.writeStartElement("msd_config");
    w.writeAttribute("current_weights", QString::number(currentWeights));
    w.writeEndElement();

}

void QFFCSMSDEvaluationItem::intReadData(QDomElement* e) {
    QFUsesResultsByIndexAndModelEvaluation::intReadData(e);

    QDomElement e1=e->firstChildElement("msd_config");
    currentWeights=e1.attribute("current_weights", "0").toInt();


}



QFEvaluationEditor* QFFCSMSDEvaluationItem::createEditor(QFPluginServices* services, QWidget* parent) {
    return new QFFCSMSDEvaluationEditor(services, parent);
}

bool QFFCSMSDEvaluationItem::isApplicable(QFRawDataRecord* record) {
    return record->inherits("QFRDRFCSDataInterface");
}


int QFFCSMSDEvaluationItem::getIndexMin(QFRawDataRecord *r) const {
    QFRDRFCSDataInterface* data=qobject_cast<QFRDRFCSDataInterface*>(r);
    if (data) {
        return -1;
    }
    return QFUsesResultsByIndexAndModelEvaluation::getIndexMin(r);
}

int QFFCSMSDEvaluationItem::getIndexMax(QFRawDataRecord *r) const {
    QFRDRFCSDataInterface* data=qobject_cast<QFRDRFCSDataInterface*>(r);
    if (data) {
        return data->getCorrelationRuns()-1;
    }
    return QFUsesResultsByIndexAndModelEvaluation::getIndexMin(r);
}



void QFFCSMSDEvaluationItem::setCurrentWeights(int index)
{
    currentWeights=index;
}





QString QFFCSMSDEvaluationItem::getCurrentModelName() const
{
    return getModelName(getCurrentModel());
}



int QFFCSMSDEvaluationItem::getCurrentWeights() const
{
    return currentWeights;
}






/////////////////////////////////////////////////////////////////////
// FITTING AND READING DATA FOR FIT, FIT STATISTICS
/////////////////////////////////////////////////////////////////////

double* QFFCSMSDEvaluationItem::allocWeights(bool* weightsOKK, QFRawDataRecord* record_in, int run_in, int data_start, int data_end) const {
    if (weightsOKK) *weightsOKK=false;
    QFRawDataRecord* record=record_in;
    if (!record_in) record=getHighlightedRecord();
    QFRDRFCSDataInterface* data=qobject_cast<QFRDRFCSDataInterface*>(record);
    //JKQTPdatastore* ds=pltData->getDatastore();
    //JKQTPdatastore* dsres=pltResiduals->getDatastore();
    //QFFitFunction* ffunc=getFitFunction();
    int run=run_in;
    if (run<=-100) run=getCurrentIndex();

    int N=data->getCorrelationN();


    double* weights=(double*)malloc(N*sizeof(double));
    bool weightsOK=false;
    if (currentWeights==1) {
        double* std=data->getCorrelationStdDev();
        weightsOK=true;
        for (int i=0; i<N; i++) {
            weights[i]=std[i];
            if ((data_start>=0) && (data_end>=0)) {
                if ((i>=data_start)&&(i<=data_end)) {
                    if ((fabs(weights[i])<10000*DBL_MIN)||(!QFFloatIsOK(weights[i]))) {
                        weightsOK=false;
                        break;
                    }
                };
            } else {
                if ((fabs(weights[i])<10000*DBL_MIN)||(!QFFloatIsOK(weights[i]))) {
                    weightsOK=false;
                    break;
                };
            }
        }
    }
    if (currentWeights==2) {
        double* std=NULL;
        if (run>=0) std=data->getCorrelationRunError(run);
        else std=data->getCorrelationStdDev();
        weightsOK=true;
        for (int i=0; i<N; i++) {
            weights[i]=std[i];
            if ((data_start>=0) && (data_end>=0)) {
                if ((i>=data_start)&&(i<=data_end)) {
                    if ((fabs(weights[i])<10000*DBL_MIN)||(!QFFloatIsOK(weights[i]))) {
                        weightsOK=false;
                        break;
                    }
                };
            } else {
                if ((fabs(weights[i])<10000*DBL_MIN)||(!QFFloatIsOK(weights[i]))) {
                    weightsOK=false;
                    break;
                };
            }
        }
    }
    if (!weightsOK) {
        for (int i=0; i<N; i++) weights[i]=1;
        if (currentWeights==0) weightsOK=true;
    }
    if (weightsOKK) *weightsOKK=weightsOK;
    return weights;
}


QVector<double> QFFCSMSDEvaluationItem::getMSD(QFRawDataRecord *record, int index, int model) const {
    QVector<double> res;
    QFRDRFCSDataInterface* data=qobject_cast<QFRDRFCSDataInterface*>(record);
    if (data) {
        res=getFitValueNumberArray(record, index, "msd");
    }
    return res;
}

QVector<double> QFFCSMSDEvaluationItem::getMSDTaus(QFRawDataRecord *record, int index, int model) const {
    QVector<double> res;
    QFRDRFCSDataInterface* data=qobject_cast<QFRDRFCSDataInterface*>(record);
    if (data) {
        res=getFitValueNumberArray(record, index, "msd_tau");
    }
    return res;
}




QFFitStatistics QFFCSMSDEvaluationItem::calcFitStatistics(QFRawDataRecord *record, int index, int model, double *taus, double* modelValsIn, uint32_t N, uint32_t MaxEntParams, int datacut_min, int datacut_max, int runAvgWidth, int residualHistogramBins) {
    QFRDRFCSDataInterface* data=qobject_cast<QFRDRFCSDataInterface*>(record);
    if (data) {

        double* corrdata=data->getCorrelationMean();
        if (index>=0) corrdata=data->getCorrelationRun(index);

        double* modelVals=modelValsIn;
        bool freeModelVals=false;
        if (!modelVals) {
            modelVals=(double*)malloc(N*sizeof(double));
            QVector<double> msd=getMSD(record, index, model);
            QVector<double> msdTau=getMSDTaus(record, index, model);
            evaluateModel(record, index, model, taus, modelVals, N, msdTau.data(), msd.data(), msd.size());
            freeModelVals=true;
        }
        //double* modelVals=(double*)malloc(N*sizeof(double));
        //
        //(record, index, model, taus, modelVals, N);
        double* weights=allocWeights(NULL, record, index);

        QFFitStatistics result= calculateFitStatistics(N, taus, modelVals, corrdata, weights, datacut_min, datacut_max, MaxEntParams, runAvgWidth, residualHistogramBins);

        if (record) {
            if (hasResults(record, index)) {
                QString param="";
                setFitResultValue(record, index, model, param="fitstat_chisquared", result.residSqrSum);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("chi squared"), QString("<font size=\"+2\">&chi;<sup>2</sup></font>"));

                setFitResultValue(record, index,model,  param="fitstat_chisquared_weighted", result.residWeightSqrSum);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("weighted chi squared"), QString("<font size=\"+2\">&chi;<sup>2</sup></font> (weighted)"));

                setFitResultValue(record, index, model, param="fitstat_residavg", result.residAverage);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("residual average"), QString("&lang;E&rang;"));

                setFitResultValue(record, index, model, param="fitstat_residavg_weighted", result.residWeightAverage);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("weighted residual average"), QString("&lang;E&rang; (weighted)"));

                setFitResultValue(record, index, model, param="fitstat_residstddev", result.residStdDev);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("residual stddev"), QString("&radic;&lang;E<sup><font size=\"+1\">2</font></sup>&rang; "));

                setFitResultValue(record, index, model, param="fitstat_residstddev_weighted", result.residWeightStdDev);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("weighted residual stddev"), QString("&radic;&lang;E<sup><font size=\"+1\">2</font></sup>&rang;  (weighted)"));

                setFitResultValue(record, index, model, param="fitstat_fitparams", result.fitparamN);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("fit params"));

                setFitResultValue(record, index, model, param="fitstat_datapoints", result.dataSize);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("datapoints"));

                setFitResultValue(record, index, model, param="fitstat_dof", result.degFreedom);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("degrees of freedom"));

                setFitResultValue(record, index, model, param="fitstat_r2", result.Rsquared);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("R squared"), tr("R<sup>2</sup>"));

                setFitResultValue(record, index, model, param="fitstat_tss", result.TSS);
                setFitResultGroup(record, index, model, param, tr("fit statistics"));
                setFitResultLabel(record, index, model, param, tr("total sum of squares"));

            }
        }

        if (freeModelVals) free(modelVals);

        return result;
    }
    return QFFitStatistics();

}

















////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENT THE FIT MODEL, THE FIT AND ALL THE PARAMETERS REQUIRED FOR THOSE
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void QFFCSMSDEvaluationItem::evaluateModel(QFRawDataRecord *record, int index, int model, double *taus, double *modelEval, uint32_t N, double* msdTaus, double* msd, uint32_t Nmsd) const {
    if (!taus || !modelEval) return;
    //qDebug()<<"evalModel(N="<<N<<"  distributionN="<<distributionN<<")";

    for (uint32_t i=0; i<N; i++) {
        modelEval[i]=0;
    }

    if (model==0) { // simple 2D model without triplet

        // first we read the stored fit parameters:
        double wxy=getFitValue(record, index, model, "focus_width");
        double N_particle=getFitValue(record,index,model,"n_particle");;

        // now we evaluate the model
        /*if (msdTaus && msd && Nmsd>0) {
            bool ok=(N==Nmsd);
            if (ok) {
                for (int i=0; i<Nmsd; i++) {
                    if (msdTaus[i]!=taus[i]) {
                        ok=false;
                        break;
                    }
                }
            }
            for (uint32_t i=0; i<N; i++) {
                taus[i]=NAN;
                modelEval[i]=NAN;
            }

            if (ok) {
                for (uint32_t i=0; i<qMin(N, Nmsd); i++) {
                    taus[i]=msdTaus[i];
                    modelEval[i]=0;//1.0/N_particle/(1.0+2.0/3.0*msd[i]/sqr(wxy));
                }
            } else {
            }
        }*/
    } else if (model==1) { // simple 3D model without triplet

        // first we read the stored fit parameters:
        double gamma=getFitValue(record, index, model, "focus_struct_fac");
        double wxy=getFitValue(record, index, model, "focus_width");
        double N_particle=getFitValue(record,index,model,"n_particle");;

        // now we evaluate the model
        /*if (msdTaus && msd && Nmsd>0) {
            bool ok=(N==Nmsd);
            if (ok) {
                for (int i=0; i<Nmsd; i++) {
                    if (msdTaus[i]!=taus[i]) {
                        ok=false;
                        break;
                    }
                }
            }
            for (uint32_t i=0; i<N; i++) {
                taus[i]=NAN;
                modelEval[i]=NAN;
            }
            if (ok) {
                for (uint32_t i=0; i<qMin(N, Nmsd); i++) {
                    taus[i]=msdTaus[i];
                    modelEval[i]=0;//1.0/N_particle/(1.0+2.0/3.0*msd[i]/sqr(wxy))/sqrt(1.0+2.0/3.0*msd[i]/sqr(wxy)/sqr(gamma));
                }
            } else {
            }
        }*/
    }
}





QString QFFCSMSDEvaluationItem::getModelName(int model) const {
    switch(model) {
        case 0: return tr("simple 2D model");
        case 1: return tr("simple 3D model");
    }

    return "";
}

bool QFFCSMSDEvaluationItem::getParameterDefault(QFRawDataRecord *r, const QString &resultID, const QString &parameterID, QFUsesResultsEvaluation::FitParameterDefault &defaultValue) const {
    switch (currentModel) {
        case 0:
            if (parameterID=="n_particle") {
                defaultValue.value=1;
                return true;
                }
            if (parameterID=="focus_width") {
                defaultValue.value=250;
                return true;
            }
            break;
        case 1:
            if (parameterID=="n_particle") {
                defaultValue.value=1;
                return true;
                }
            if (parameterID=="focus_width") {
                defaultValue.value=250;
                return true;
                }
            if (parameterID=="focus_struct_fac") {
                defaultValue.value=6;
                return true;
            }
            break;
     }


    return false;
}


void QFFCSMSDEvaluationItem::doFit(QFRawDataRecord* record, int index, int model, int defaultMinDatarange, int defaultMaxDatarange, int runAvgWidth, int residualHistogramBins) {
    bool doEmit=record->isEmitResultsChangedEnabled();
    bool thisDoEmitResults=get_doEmitResultsChanged();
    bool thisDoEmitProps=get_doEmitResultsChanged();
    set_doEmitResultsChanged(false);
    set_doEmitPropertiesChanged(false);
    record->disableEmitResultsChanged();

    qDebug() << "START DEBUGGING 0: We enter the do fit Method with MODEL = " << model;



    /* IMPLEMENT THIS

      Ergebnisse k�nnen einfach mit einer der setFitResult... Methoden gespeichert werden:

        //                                          PARAMETERNAME           WERT
        setFitResultValueBool(record, index, model, "evaluation_completed", true);
        //                                      PARAMETERNAME   WERT   FEHLER und EINHEIT (z.B. "ms") dazu
        setFitResultValue(record, index, model, "my_parameter", value, error, unit);

    */

    // HERE IS A DUMMY IMPLEMENTATION THAT OUTPUTS A SIMPLE GAUSSIAN
    QFRDRFCSDataInterface* data=qobject_cast<QFRDRFCSDataInterface*>(record);
    if (data) {
        getProject()->getServices()->log_text(tr("running MSD fit with model '%1' on raw data record '%2', run %3 ... \n").arg(getModelName(model)).arg(record->getName()).arg(index));
        // which datapoints should we actually use?
        int rangeMinDatarange=0;
        int rangeMaxDatarange=data->getCorrelationN();
        if (defaultMinDatarange>=0) rangeMinDatarange=defaultMinDatarange;
        if (defaultMaxDatarange>=0) rangeMaxDatarange=defaultMaxDatarange;
        getProject()->getServices()->log_text(tr("   - fit data range: %1...%2 (%3 datapoints)\n").arg(defaultMinDatarange).arg(defaultMaxDatarange).arg(defaultMaxDatarange-defaultMinDatarange));




        uint32_t N=data->getCorrelationN();
        double* taus=data->getCorrelationT();
        int Ndist=rangeMaxDatarange-rangeMinDatarange;
        double* distTaus=(double*)calloc(Ndist,sizeof(double));
        double* dist=(double*)calloc(Ndist,sizeof(double));
        double* modelEval=(double*)malloc(N*sizeof(double));
        bool weightsOK=false;
        double* corrdata=data->getCorrelationMean();
        if (index>=0) corrdata=data->getCorrelationRun(index);
        double* weights=allocWeights(&weightsOK, record, index, rangeMinDatarange, rangeMaxDatarange);
        if (!weightsOK) getProject()->getServices()->log_warning(tr("   - weights have invalid values => setting all weights to 1\n"));

        bool fitSuccess=false;
        QString fitMessage, fitMessageHTML;


        //////////Load Model Parameters//////////////////////////////////////////////////////
        double gamma=getFitValue(record, index, model, "focus_struct_fac");
        double wxy=getFitValue(record, index, model, "focus_width");
        double N_particle=getFitValue(record,index,model,"n_particle");;


        QElapsedTimer time;
        time.start();

        /////////////////////////////////////////////////////////
        /// MSD Implementation ///////////////////////////////
        /////////////////////////////////////////////////////////
        if (model==0) {
            for (int i=0; i<Ndist; i++) {
                distTaus[i]=taus[rangeMinDatarange+i];
                dist[i]=3.0*sqr(wxy)/2.0*(1.0/N_particle/corrdata[rangeMinDatarange+i]-1.0);
            }
        } else if (model==1) {
            for (int i=0; i<Ndist; i++) {
                distTaus[i]=taus[rangeMinDatarange+i];
                dist[i]=6.0*10.0*distTaus[i];
            }
        }

        fitSuccess=true;
        /////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////

        // duration measurement
        double duration=double(time.elapsed())/1000.0;

        getProject()->getServices()->log_text(tr("   - fit completed after %1 msecs with result %2\n").arg(duration).arg(fitSuccess?tr("success"):tr("error")));
        fitMessage=tr("MSD finished with %1").arg(fitSuccess?tr("success"):tr("an error"));
        fitMessageHTML=tr("<b>MSD finished with %1</b>").arg(fitSuccess?tr("success"):tr("an error"));

        // now store the results:
        QString param;
        setFitResultValueNumberArray(record, index, model, param="msd_tau", distTaus, Ndist, QString("seconds"));
        setFitResultGroup(record, index, model, param, tr("fit results"));
        setFitResultLabel(record, index, model, param, tr("MSD: lag times"), QString("MSD: lag times <i>&tau;</i>"));
        setFitResultSortPriority(record, index, model, param, true);

        setFitResultValueNumberArray(record, index, model, param="msd", dist, Ndist);
        setFitResultGroup(record, index, model, param, tr("fit results"));
        setFitResultLabel(record, index, model, param, tr("MSD"), QString("MSD: <i>&lang;r<sup>2</sup>(&tau;)&rang;</i>"));
        setFitResultSortPriority(record, index, model, param, true);



        // save all the default values for all fit parameters as results.
        for (int i=0; i<getParameterCount(model); i++) {
            param=getParameterID(model, i);
            double value=getFitValue(record, index, model, param);
            setFitResultValue(record, index, model, param, value, getParameterUnit(model, i, false));
            setFitResultGroup(record, index, model, param, tr("fit results"));
            setFitResultLabel(record, index, model, param, getParameterName(model, i, false), getParameterName(model, i, true));
            setFitResultSortPriority(record, index, model, param, true);
        }

        // you can overwrite certain of these parameters using code like this:
        /*setFitResultValue(record, index, model, param="focus_struct_fac", getFitValue(record, index, model, param));
        setFitResultGroup(record, index, model, param, tr("fit results"));
        setFitResultLabel(record, index, model, param, tr("focus structure factor"), QString("focus structure factor <i>&gamma;</i>"));
        setFitResultSortPriority(record, index, model, param, true);*/




        // store number of iterations ... you may also store more fit algorithm properties like this
        setFitResultValueString(record, index, model, param="used_model", getModelName(model));
        setFitResultGroup(record, index, model, param, tr("fit properties"));
        setFitResultLabel(record, index, model, param, tr("used model name"), tr("used model name"));

        setFitResultValueInt(record, index, model, param="msd_N",Ndist);
        setFitResultGroup(record, index, model, param, tr("fit properties"));
        setFitResultLabel(record, index, model, param, tr("MSD: number of lag times"), tr("MSD: number of lag times"));

        setFitResultValueInt(record, index, model, param="used_model_id", model);
        setFitResultGroup(record, index, model, param, tr("fit properties"));
        setFitResultLabel(record, index, model, param, tr("used model id"), tr("used model id"));

        setFitResultValueInt(record, index, model, param="used_run", index);
        setFitResultGroup(record, index, model, param, tr("fit properties"));
        setFitResultLabel(record, index, model, param, tr("used run"), tr("used run"));

        setFitResultValue(record, index, model, param="runtime", duration, tr("seconds"));
        setFitResultGroup(record, index, model, param, tr("fit properties"));
        setFitResultLabel(record, index, model, param, tr("fit runtime"), tr("fit runtime"));

        setFitResultValueString(record, index, model, param="fitalg_message", fitMessage);
        setFitResultGroup(record, index, model, param, tr("fit properties"));
        setFitResultLabel(record, index, model, param, tr("MSD message"), tr("MSD message"));

        setFitResultValueString(record, index, model, param="fitalg_messageHTML", fitMessageHTML);
        setFitResultGroup(record, index, model, param, tr("fit properties"));
        setFitResultLabel(record, index, model, param, tr("MSD message"), tr("MSD message"));

        // CALCULATE FIT STATISTICS
        //   now we evaluate the model for the given distribution
        /////////////////////////
        /////////////////////////
        // I changed the 7th argument in the evaluateModel call from &(taus[rangeMindatarange]) to &(distTaus[rangeMinDatarange])
        // this is what the 7th and 8th argument used to be like: &(distTaus[rangeMinDatarange]), &(dist[rangeMinDatarange])
        // this was the last argument:  rangeMaxDatarange-rangeMinDatarange, now changed to Ndist
        ////////////////////////

        evaluateModel(record, index, model, taus, modelEval, N, distTaus,dist,Ndist);
        //   then we can call calcFitStatistics()
        QFFitStatistics fit_stat=calcFitStatistics(record, index, model, taus, corrdata, N,Ndist, rangeMinDatarange, rangeMaxDatarange, runAvgWidth, residualHistogramBins);
        //   finally we have to free the memory allocated in the calcFitStatistics() result.
        fit_stat.free();

        free(dist);
        free(weights);
        free(modelEval);
        free(distTaus);


    }

    if (doEmit) record->enableEmitResultsChanged(true);
    set_doEmitResultsChanged(thisDoEmitResults);
    set_doEmitPropertiesChanged(thisDoEmitProps);
}


QString QFFCSMSDEvaluationItem::getParameterName(int model, int id, bool html) const {
    switch (model) {
        case 0: // simple 2D model
            if (id==0) return (html)?tr("particle number N"):tr("particle number");
            if (id==1) return (html)?tr("focus size w<sub>xy</sub> [nm]"):tr("focus size [nm]");
        case 1: // simple 3D model
            if (id==0) return (html)?tr("particle number N"):tr("particle number");
            if (id==1) return (html)?tr("focus size w<sub>xy</sub> [nm]"):tr("focus size [nm]");
            if (id==2) return (html)?tr("axial ratio &gamma;"):tr("axial ratio");
    }
        return QString();
}


QString QFFCSMSDEvaluationItem::getParameterUnit(int model, int id, bool html) const {
    switch (model) {
        case 0:
            if (id==0) return QString("");
            if (id==1) return tr("nm");
        case 1:
            if (id==0) return QString("");
            if (id==1) return tr("nm");
            if (id==2) return QString("");
    }
    return QString();
}
int QFFCSMSDEvaluationItem::getParameterCount(int model) const {
    switch (model) {
        case 0: return 2;
        case 1: return 3;
    }
    return 0;
}

QString QFFCSMSDEvaluationItem::getParameterID(int model, int id) const {
    switch (model) {
        case 0:
            if (id==0) return tr("n_particle");
            if (id==1) return tr("focus_width");
        case 1:
            if (id==0) return tr("n_particle");
            if (id==1) return tr("focus_width");
            if (id==2) return tr("focus_struct_fac");
    }
    return QString("m%1_p%2").arg(model).arg(id);
}

int QFFCSMSDEvaluationItem::getModelCount(QFRawDataRecord *r, int index) const {
    return 2;
}