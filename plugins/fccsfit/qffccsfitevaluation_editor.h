#ifndef QFFCCSFITEVALUATIONEDITOR_H
#define QFFCCSFITEVALUATIONEDITOR_H

#include "qfevaluationeditor.h"
#include "qfrawdatarecord.h"
#include <QProgressDialog>
#include <QTextDocument>
#include "qffitfunctionplottools.h"
#include "jkqtplotter.h"
#include "qffcsweightingtools.h"
#include "dlgqffitalgorithmprogressdialog.h"
#include "qffitfunctionconfigforglobalfitinterface.h"

namespace Ui {
    class QFFCCSFitEvaluationEditor;
}

class QFFCCSFitEvaluationItem; // forward

/*! \brief editor for QFFCCSFitEvaluationItem
    \ingroup qf3rdrdp_FCCSfit

    The main part of this editor is a QTableView which allows to edit the fit parameters of several files.
    A special item delegate is used to create special editing widgets for the values and to select fit functions and RDRs.
*/
class QFFCCSFitEvaluationEditor : public QFEvaluationEditor {
        Q_OBJECT
    public:
        /** Default constructor */
        QFFCCSFitEvaluationEditor(QFPluginServices* services,  QFEvaluationPropertyEditor *propEditor, QWidget* parent=NULL);
        /** Default destructor */
        virtual ~QFFCCSFitEvaluationEditor();


        /** \brief get the lower datacut for the current record, reimplement this by calling getUserMin(QFRawDataRecord*,int,int) with a viable choice for \a defaultMin */
        virtual int getUserMin(int index);
        /** \brief get the upper datacut for the current record, reimplement this by calling getUserMin(QFRawDataRecord*,int,int) with a viable choice for \a defaultMax */
        virtual int getUserMax(int index);
        /** \brief get the lower datacut for the current record, reimplement this by calling getUserMin(QFRawDataRecord*,int,int) with a viable choice for \a defaultMin */
        virtual int getUserMin(QFRawDataRecord* rec, int index);
        /** \brief get the upper datacut for the current record, reimplement this by calling getUserMin(QFRawDataRecord*,int,int) with a viable choice for \a defaultMax */
        virtual int getUserMax(QFRawDataRecord* rec, int index);
        virtual int getUserRangeMax(QFRawDataRecord *rec, int index);
        virtual int getUserRangeMin(QFRawDataRecord *rec, int index);
        /** \brief get the lower datacut for the current record, the \a defaultMin should be read from editors current data cut widget */
        virtual int getUserMin(QFRawDataRecord* rec, int index, int defaultMin);
        /** \brief get the upper datacut for the current record, the \a defaultMax should be read from editors current data cut widget */
        virtual int getUserMax(QFRawDataRecord* rec, int index, int defaultMax);
        /** \brief set the upper and lower datacut for the current record */
        virtual void setUserMinMax(int userMin, int userMax);

    protected slots:
        /** \brief executed when the sliders values change */
        void slidersChanged(int userMin, int userMax, int min, int max);

        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQtPlotter *sender);
        /** \brief connect widgets to current data record */
        virtual void connectWidgets(QFEvaluationItem* current, QFEvaluationItem* old);
        /** \brief connected to the rawDataChanged() signal of the current record */
        virtual void resultsChanged();
        /** \brief read the settings */
        virtual void readSettings();
        /** \brief write the settings */
        virtual void writeSettings();

        /** \brief display fit algorithm help */
        void showFitAlgorithmHelp();
        /** \brief configure the fitting algorithm */
        void configFitAlgorithm();
        /** \brief called when the fit algorithm changes */
        void fitAlgorithmChanged(int model);

        void displayOptionsChanged();

        void on_cmbWeight_currentWeightChanged(QFFCSWeightingTools::DataWeight weight);

        void setCurrentRun(int run);

        void configureForSPFCCS();
        void configureForASPFCCS();
        void configureFor2CSPIMFCCS();
        void onConfigureGlobalItemClicked();

        void filesSetActivated(const QModelIndex& idx);
        void on_chkSaveStrings_toggled(bool checked);
        void on_btnEditRanges_toggled(bool enabled);
        void setParameterVisibility();
        void on_btnAddFile_clicked();
        void on_btnRemoveFile_clicked();
    protected:
        /** \brief the user interface object (using QtDesigner) */
        Ui::QFFCCSFitEvaluationEditor *ui;
        
        QList<QFFitFunctionConfigForGlobalFitInterface::GlobalFitConfig> globalConfig;
        QList<QAction*> actsGlobalConfig;
        QList<QMenu*> menusGlobalConfig;

        void buildGlobalConfigs(QFFCCSFitEvaluationItem* current);

        /** \brief where did we save the last report */
        QString currentSaveDirectory;
        QAction* actConfigureForNormalFCCS;
        QAction* actConfigureFor2CFCCS;
        QAction* actConfigureForAnomalousFCCS;
        QAction* actFitCurrent;
        QAction* actFitAllRunsMT;
        QAction* actFitAllRuns;
        QAction* actPrintReport;
        QAction* actSaveReport;
        QAction* actGuess;


        QAction* actResetCurrent;
        QAction* actResetAllRuns;
        QAction* actCopyToInitial;

        QMenu* menuFCCSFit;
        QMenu* menuEvaluation;

        /** \brief fit progress dialog */
        dlgQFFitAlgorithmProgressDialog* dlgFitProgress;
        /** \brief reporter for dlgFitProgress */
        dlgQFFitAlgorithmProgressDialogReporter* dlgFitProgressReporter;

        /*! \brief a flag that indicates whether we are currently updating the display data 
        
            This may be used to distinguish between events emitted by the user, changing something (in a widget, ...) or
            automtically by this class reloading some display ...
        */
        bool updatingData;

        /*! \brief execute the evaulation

            \param record the record to do the evaluation for
        */
        void doEvaluation(QFRawDataRecord* record);
        
        /*! \brief create an evaluation report for the current record */
        void createReportDoc(QTextDocument* document);

    protected slots:

        /** �brief ensures that the parameter table model is displayed properly */
        void ensureCorrectParamaterModelDisplay();

        void fileChanged(int num, QFRawDataRecord* file);

        void plotMouseMoved(double x, double y);


    
        /** \brief evaluate current file */
        void fitCurrent();
        /** \brief all runs/pixels current file */
        void fitAllRunsThreaded();
        /** \brief all runs/pixels current file, multi-threaded version */
        void fitAllRuns();

        /** \brief display the data from the current raw data record */
        void displayData();
        /** \brief display the current evaluation results */
        void displayEvaluation();

        void resetCurrent();
        void resetAllPixels();
        void copyToInitial();

        void setParameterTableSpans();

        void guessFromCurrentFileSet();


};

#endif // QFFCCSFITEVALUATIONEDITOR_H
