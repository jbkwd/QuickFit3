#include "qfprdrfcs.h"
#include <QtGui>
#include "qfrdrfcsdata.h"
#include "dlgcsvparameters.h"

QFPRDRFCS::QFPRDRFCS(QObject* parent):
    QObject(parent)
{
    //ctor
}

QFPRDRFCS::~QFPRDRFCS()
{
    //dtor
}

QFRawDataRecord* QFPRDRFCS::createRecord(QFProject* parent) {
    return new QFRDRFCSData(parent);
}


void QFPRDRFCS::registerToMenu(QMenu* menu) {
    QAction* actFCS=new QAction(QIcon(":/fcs_open.png"), tr("Open FCS Data from File"), parentWidget);
    actFCS->setStatusTip(tr("Insert a new FCS data item from a specified file (CSV or ALV-5000 format)"));
    connect(actFCS, SIGNAL(triggered()), this, SLOT(insertFCS()));
    menu->addAction(actFCS);
}


void QFPRDRFCS::insertALV5000File(const QString& filename, const QMap<QString, QVariant>& paramValues, const QStringList& paramReadonly) {
    unsigned int cc=1;
    QString mode="";
    unsigned int runCount=0;
    bool crossCorrelation=false;
    try {
        ALV_analyze(filename, mode, cc, runCount, crossCorrelation);
    } catch (std::exception& E) {
        cc=0;
        QMessageBox::critical(parentWidget, tr("QuickFit 3.0"), tr("Error while importing ALV5000 file '%1':\n%2").arg(filename).arg(E.what()));
        reporter->log_error(tr("Error while importing ALV5000 file '%1':\n    %2\n").arg(filename).arg(E.what()));

    }
    for (unsigned int i=0; i<cc; i++) {
        QMap<QString, QVariant> p=paramValues;
        p["CHANNEL"]=i;
        QFRawDataRecord* e=project->addRawData(getID(), tr("%1 - CH%2").arg(QFileInfo(filename).fileName()).arg(i), QStringList(filename), p, paramReadonly);
        if (e->error()) {
            project->deleteRawData(e->getID());
            QMessageBox::critical(parentWidget, tr("QuickFit 3.0"), tr("Error while importing ALV5000 file '%1' (channel %3/%4):\n%2").arg(filename).arg(e->errorDescription()).arg(i+1).arg(cc));
            reporter->log_error(tr("Error while importing ALV5000 file '%1':\n    %2\n").arg(filename).arg(e->errorDescription()));
        }
    }
}

void QFPRDRFCS::insertCSVFile(const QString& filename, const QMap<QString, QVariant>& paramValues, const QStringList& paramReadonly) {
    QFRawDataRecord* e=project->addRawData(getID(), QFileInfo(filename).fileName(), QStringList(filename), paramValues, paramReadonly);
    if (e->error()) {
        project->deleteRawData(e->getID());
        QMessageBox::critical(parentWidget, tr("QuickFit 3.0"), tr("Error while importing '%1':\n%2").arg(filename).arg(e->errorDescription()));
        reporter->log_error(tr("Error while importing '%1':\n    %2\n").arg(filename).arg(e->errorDescription()));
    }
}

void QFPRDRFCS::insertFCS() {
    if (project) {
        QString alvf=tr("ALV-5000 file (*.asc)");
        QString asciif=tr("ASCII Data Files (*.txt *.dat *.csv)");
        QString currentFCSFileFormatFilter=settings->getQSettings()->value("fcs/current_fcs_format_filter", alvf).toString();
        QStringList files = QFileDialog::getOpenFileNames(parentWidget,
                              tr("Select FCS Data File(s) to Import ..."),
                              settings->getCurrentRawDataDir(),
                              alvf+";;"+asciif, &currentFCSFileFormatFilter);
        //std::cout<<"filter: "<<currentFCSFileFormatFilter.toStdString()<<std::endl;
        settings->getQSettings()->setValue("fcs/current_fcs_format_filter", currentFCSFileFormatFilter);
        QMap<QString, QVariant> p;
        if (currentFCSFileFormatFilter==alvf) {
            p["FILETYPE"]="ALV5000";
            p["CHANNEL"]=0;
        } else if (currentFCSFileFormatFilter==asciif) {
            p["FILETYPE"]="CSV_CORR";
            p["CSV_SEPARATOR"]=QString(",");
            p["CSV_COMMENT"]=QString("#");
            // TODO: insert dialog to select CSV properties!
        }
        QStringList paramsReadonly;
        paramsReadonly<<"FILETYPE"<<"CHANNEL"<<"CSV_SEPARATOR"<<"CSV_COMMENT";
        QStringList list = files;
        QStringList::Iterator it = list.begin();
        reporter->setProgressRange(0, list.size());
        reporter->setProgress(0);
        int i=0;
        while(it != list.end()) {
            i++;
            if (QFile::exists(*it)) {
                //std::cout<<"loading "<<(*it).toStdString()<<std::endl;
                reporter->log_text(tr("loading [%2] '%1' ...\n").arg(*it).arg(currentFCSFileFormatFilter));
                if (currentFCSFileFormatFilter==alvf) {
                    insertALV5000File(*it, p, paramsReadonly);
                } else {
                    insertCSVFile(*it, p, paramsReadonly);
                }
                //std::cout<<"loading "<<(*it).toStdString()<<" ... done!\n";
                settings->setCurrentRawDataDir(QFileInfo(*it).dir().absolutePath());
                //std::cout<<"loading "<<(*it).toStdString()<<" ... done ... done!\n";
                ++it;
            }
            reporter->setProgress(i);
            QApplication::processEvents();
        }
        reporter->setProgress(0);
        //std::cout<<"loading done ...\n";
        //tvMain->expandToDepth(2);
    }
}

Q_EXPORT_PLUGIN2(qfrdrfcs, QFPRDRFCS)
