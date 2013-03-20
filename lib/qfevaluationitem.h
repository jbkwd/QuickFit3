#ifndef QEVALUATIONITEM_H
#define QEVALUATIONITEM_H

#include "lib_imexport.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QVariant>
#include <QtXml>
#include <QStringList>
#include <QMap>
#include <QPointer>
#include <QIcon>
#include <QAbstractTableModel>
#include "qfproject.h"
#include "qfevaluationeditor.h"
#include "qfrawdatarecord.h"
#include "qfproperties.h"
#include "qfpluginservices.h"
#include "qffitalgorithm.h"
#include "qffitfunction.h"
#include "qftools.h"


// forward declaration
class QFEvaluationPropertyEditor;


/*! \brief this class manages one evaluation record in the project
    \ingroup qf3lib_project

    An evaluation is a very basic node in the project tree. It does not come with a lot of features. Basically
    the notion of an evaluation is to take (some or all of the) data and produce some kind of result from these.

    The QFEvaluationItem may be used to represent a wide variety of different evaluations. In its basic form, no
    additional features are provided and the suer has to implement the selection of the records that apply to the
    evaluation and also the user interface. In addition to that some "special features" are implemented to allow the
    easy implementation of often occuring cases:
      * In many cases an evaluation is only applicable to a raw data set if certain conditions apply, e.g. if the data
        set has a certain type ... This may be represented by overwriting the isApplicable() function which returns
        \c true in its default implementation.
      * Many evaluations apply to a (selectable) set raw data records. To implement this the class allows to select
        and to highlight records. There may be multiple selected records, but only one highlighted record. The function
        getShowRDRList() returns \c true if a list of applicable records is to be displayed and getUseSelection() returns
        \c ture if the user shall be able to select multiple records from the list. The second one only applies if the
        first one is \c true of course. Also if getShowRDRList() returns \c true the user may highlight one record in
        any case, but the evaluation of course does not have to react to this. The selection/highlighting may be accessed
        by these functions:
          * getSelectedRecords()
          * selectRecord()
          * selectAllAplicableRecords()
          * deselectRecord()
          * deselectRecord()
          * getSelectedRecordCount()
          * getSelectedRecord()
          * clearSelectedRecords()
          * isSelected()
          * getHighlightedRecord()
          * setHighlightedRecord()
          * \b signal: highlightingChanged(QFRawDataRecord* formerRecord, QFRawDataRecord* currentRecord)
          * \b signal: selectionChanged(QList<QFRawDataRecord*> selectedRecords)
        .
    .

    Results created by evaluations are either stored in the according QFRawDataRecord objects (there are methods
    to save, read and display results) or in any other place possible (which then has to implemented). One could
    imagine to have an evaluation create a new, or update an existing raw data record.

    Evaluation items also contain properties, but they are not visible for the user and only used as a simple
    interface to load and store them.

    \section QFEvaluationRecord_Signals Signals & Slots
    This class has some signals that can be emitted when fit properties or fit results change. When setting a whole
    bunch of properties/results where each set() operation could emit a signal it may be wise to surround this portion
    of code by function calls that stop the emitting of these signals:
\code
    eval->set_doEmitPropertiesChanged(false); // disable emitting the signals
    eval->set_doEmitResultsChanged(false);

    // ... YOUR CODE HERE ...

    eval->set_doEmitPropertiesChanged(true); // enable emitting the signals again
    eval->set_doEmitResultsChanged(true);
    eval->emitPropertiesChanged(); // explicitly call the signals
    eval->emitResultsChanged();
\endcode
    Of course you also have to make sure to use the methods emitResultsChanged() and emitParametersChanged() functions,
    instead of emiting the signals directly.


    \section QFEvaluationRecord_Error Error Reporting
    Errors are reported by the methods error() which returns \c true if an error has occured and the method
    errorDescription() which returns a textual description of the error. The protected method setError() may be
    used to indicate that an error has occured to the class.


    \section QFEvaluationRecord_Help Online-Help
    The user may supply an Online-Help for the QFEvaluationRecord and QFEvaluationEditor. The Help has to be written
    as HTML page (or a set of these) and has to be stored as \verbatim plugins/help/<plugin_id>/<plugin_id>.html \endverbatim.
 */
class QFLIB_EXPORT QFEvaluationItem : public QObject, public QFProperties {
        Q_OBJECT
    public:


        /*! \brief class constructor, reads from QDomElement
            \param parent the project this object belongs to
            \param useSelection indicates whether the user may select a set of records or not.
         */
        QFEvaluationItem(QFProject* parent, bool showRDRList, bool useSelection);
        /** \brief Default destructor */
        virtual ~QFEvaluationItem();

        /** \brief initialize the object with the given data */
        void init(QString name=QString(""));

        /** \brief initialize from QDomElement */
        void init(QDomElement& e);

        /** \brief determines whether this evaluation is applicable to a given raw data record. This method is used to generate the
         *         list of raw data records presented to the user */
        bool isFilteredAndApplicable(QFRawDataRecord* record);


        /** \brief return the next sibling rawdata record in the project */
        inline QFEvaluationItem* getNext() { return project->getNextEvaluation(this); };
        /** \brief return the next sibling rawdata record in the project */
        inline QFEvaluationItem* getPrevious() { return project->getPreviousEvaluation(this); };
        /** \brief return the next sibling rawdata record in the project, which has the same type as this evaluation */
        inline QFEvaluationItem* getNextOfSameType() { return project->getNextEvaluationOfSameType(this); };
        /** \brief return the next sibling rawdata record in the project, which has the same type as this evaluation */
        inline QFEvaluationItem* getPreviousOfSameType() { return project->getPreviousEvaluationOfSameType(this); };

        /** \brief return the ID */
        inline int getID() const { return ID; };
        /** \brief return the name */
        inline QString getName() const { return name; }
        /** \brief return the description  */
        inline QString getDescription() const { return description; };
        /** \brief return a pointer to the project that contains this QRawDatarecord */
        inline QFProject* getProject() const { return project; }


        /** \brief returns \c true if an error occured */
        inline bool error() const { return errorOcc; }
        /** \brief returns the description of the last error */
        inline QString errorDescription() const { return errorDesc; }




        /** \brief returns whether the user may select a set of records (\c true ) or not (\c false ). */
        bool getUseSelection() const { return useSelection; };

        /** \brief returns whether a list of applicable records is displayed (\c true ) or not (\c false ). */
        bool getShowRDRList() const { return showRDRList; };

        /** \brief returns a list of currently selected items */
        QList<QPointer<QFRawDataRecord> > getSelectedRecords() const { return selectedRecords; };

        /** \brief add a record to the set of selected records */
        void selectRecord(QFRawDataRecord* record);

        /** \brief select all records from the project to which the evaluation is applicable */
        void selectAllAplicableRecords();

        /** \brief remove a record from the set of selected records */
        void deselectRecord(QFRawDataRecord* record);

        /** \brief remove the i-th record from the set of selected records */
        void deselectRecord(int i);

        /** \brief return the number of selected records */
        inline int getSelectedRecordCount() { return selectedRecords.size(); }

        /** \brief return the i-th selected record */
        QPointer<QFRawDataRecord> getSelectedRecord(int i);

        /** \brief empty list of selected records */
        void clearSelectedRecords();

        /** \brief returns \c true, if the given record is selected */
        bool isSelected(QFRawDataRecord* record) const { return selectedRecords.contains(record); };

        /** \brief returns the currently highlughted record */
        QFRawDataRecord* getHighlightedRecord() const { return highlightedRecord; };

        /** \brief this method may be called in order to select a new record or to select none (call with \c NULL as argument) */
        void setHighlightedRecord(QFRawDataRecord* record);

        /** \brief return type (short type string) */
        virtual QString getType() const { return tr("unknown"); };
        /** \brief return type (longer type string, user readable) */
        virtual QString getTypeName() const { return tr("unknown"); };
        /** \brief return a small icon (16x16) */
        virtual QIcon getSmallIcon() const { return QIcon(":/lib/projecttree_emptyeval.png"); };
        /** \brief return type description */
        virtual QString getTypeDescription() const { return tr("unknown"); };
        /** \brief return a large icon (32x32) */
        virtual QIcon getLargeIcon() const { return QIcon(":/lib/projecttree_emptyeval.png"); };
        /** \brief returns the name for the i-th editor pane */
        virtual QString getEditorName() { return QString(""); };
        /** \brief create an object for the i-th editor pane */
        virtual QFEvaluationEditor* createEditor(QFPluginServices* services,  QFEvaluationPropertyEditor *propEditor, QWidget* parent=NULL) { return NULL; };
        /** \brief write object contents into XML file */
        virtual void writeXML(QXmlStreamWriter& w);

        /** \brief list of the raw data records this evaluation is applicable to */
        QList<QPointer<QFRawDataRecord> > getApplicableRecords();

        /*! \brief the filter returned by this function is used to filter the evaluation reeults to display in the
                   "results" pane of the QFEvaluationPropertyEditor dialog.

            The default implementation returns: \code getType()+"_"+QString::number(getID())+"*"; \endcode

            If you want to refilter the results pane, you should make sure that this function returns the new filter
            and then \code emitResultsChanged() \endcode which will result in a redisplay of the evaluation results.
         */
        virtual QString getResultsDisplayFilter() const;

        /** \brief return whether resultsChanged() signals are enabled */
        inline bool get_doEmitResultsChanged() const {
            return doEmitResultsChanged;
        }
        /** \brief set whether resultsChanged() signals are enabled */
        inline void set_doEmitResultsChanged(bool enable) {
            doEmitResultsChanged=enable;
        }

        /** \brief return whether propertiesChanged() signals are enabled */
        inline bool get_doEmitPropertiesChanged() const {
            return doEmitPropertiesChanged;
        }
        /** \brief set whether propertiesChanged() signals are enabled */
        inline void set_doEmitPropertiesChanged(bool enable) {
            doEmitPropertiesChanged=enable;
        }

        QString getNameFilter() const;
        bool getNameFilterRegExp() const;
        QString getNameNotFilter() const;
        bool getNameNotFilterRegExp() const;

    public slots:
        /** \brief call this to tell the class that data has changed and the project has to be saved! */
        void setDataChanged();
        /** \brief set a wildcard/regexp filter string to filter records according to their name (each record containing the filter string will match)*/
        void setNameFilter(QString filter, bool regexp=false);
        /** \brief set a wildcard/regexp filter string to filter records according to their name (each NOT record containing the filter string will match)*/
        void setNameNotFilter(QString filter, bool regexp=false);
        /** \brief set a wildcard/regexp filter string to filter records according to their name (each NOT record containing the filter string will match)*/
        void setNameNameNotFilter(QString filter, QString filterNot, bool regexp=false, bool regexpNot=false);
        /** \brief set the name */
        void setName(const QString n);
        /** \brief set the description  */
        void setDescription(const QString& d);

        /** \brief emits resultsChanged(), only if doEmitResultsChanged is \c true */
        void emitResultsChanged(QFRawDataRecord* record=NULL, const QString& evaluationName=QString(""), const QString& resultName=QString(""));

        /** \brief emits propertiesChanged(), only if doEmitPropertiesChanged is \c true */
        void emitPropertiesChanged(const QString &property=QString(""), bool visible=true);
    signals:
        /** \brief emitted whenever at least one of the properties changes
         *
         *  \note \a property may be empty, signalling that no special properties, but possible multiple have changed
         */
        void propertiesChanged(const QString& property, bool visible);
        /** \brief emitted whenever the evaluation results change, if both arguments are empty, the changes are larger than just the given result */
        void resultsChanged(QFRawDataRecord* record, const QString& evaluationName, const QString& resultName);
        /** \brief signal emitted when the highlighted record changed */
        void highlightingChanged(QFRawDataRecord* formerRecord, QFRawDataRecord* currentRecord);
        /** \brief signal emitted when the selection changed */
        void selectionChanged(QList<QPointer<QFRawDataRecord> > selectedRecords);
        /** \brief basic properties changed (i.e. name, description and folder) */
        void basicPropertiesChanged();

    protected slots:
        /** \brief called when a record is going to be deleted in the project, so we can savely hide it in an editor or so! */
        void recordAboutToBeDeleted(QFRawDataRecord* r);

    protected:

        /** \brief determines whether this evaluation is applicable to a given raw data record. This method is used to generate the
         *         list of raw data records presented to the user */
        virtual bool isApplicable(QFRawDataRecord* record) { return true; };


        /** \brief ID of the raw data record */
        int ID;
        /** \brief name of the raw data record */
        QString name;
        /** \brief description of the raw data record */
        QString description;
        /** \brief indicates whether an error has occured */
        bool errorOcc;
        /** \brief contains the description of the last error */
        QString errorDesc;
        /** \brief pointer to the parent project object */
        QFProject* project;
        /** \brief en-/disable emiting of resultsChanged() signal */
        bool doEmitResultsChanged;
        /** \brief en-/disable emiting of propertiesChanged() signal */
        bool doEmitPropertiesChanged;

        QRegExp nameFilter;
        QRegExp nameNotFilter;

        /** \brief read object contents from QDomElement */
        void readXML(QDomElement& e);
        /** \brief write contents to QXmlStreamWriter (data tag) <b>IMPLEMENT IN CHILD CLASSES!</b>
         *
         * This method may be used to store additional data (like algorithm configuration ...) from the project file
         * \note You will have to explicitly call the intWriteData() metod of the parent class of your newly implemented
         * evaluation class in order to ensure that all data is correctly stored to the project file! Do so by calling
         * \code QFEvaluationItem::intWriteData(e); \endcode at the start of your method !!!
         */
        virtual void intWriteData(QXmlStreamWriter& w) {};
        /** \brief read in data stored in the project file <b>IMPLEMENT IN CHILD CLASSES!</b>
         *
         * This method may be used to read additional data (like algorithm configuration ...) from the project file
         * \note You will have to explicitly call the intReadData() metod of the parent class of your newly implemented
         * evaluation class in order to ensure that all data is correctly read from the project file!  Do so by calling
         * \code QFEvaluationItem::intReadData(e); \endcode at the start of your method !!!
         */
        virtual void intReadData(QDomElement* e) {};
        /** \brief set the internal error flag and description */
        void setError(QString description) { errorOcc=true; errorDesc=description; }

        /** \brief set the list of currently selected items */
        void setSelectedRecords(QList<QPointer<QFRawDataRecord> > records);

        /** \brief determines whether the user may select a set of records (\c true ) or whether the user may only
         *         highlight a single record  (\c false , default).
         * \note This only applied when showRDRList is \c true. The only way to set this property is by used of the
         *       class constructor, so the implementor of an evaluation is forced to think about this topic! */
        bool useSelection;

        /** \brief determines whether the user may select a set of records (\c true ) or whether the user may only
         *         highlight a single record  (\c false , default).
         * \note The only way to set this property is by used of the class constructor, so the implementor of an
         *       evaluation is forced to think about this topic! */
        bool showRDRList;

        /** \brief list of the currently selected records */
        QList<QPointer<QFRawDataRecord> > selectedRecords;

        /** \brief points to the currently highlighted record */
        QPointer<QFRawDataRecord> highlightedRecord;

};

#endif // QEVALUATIONITEM_H

