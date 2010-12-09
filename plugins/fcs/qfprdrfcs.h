#ifndef QFPRDRFCS_H
#define QFPRDRFCS_H

#include "qfpluginrawdata.h"
/*!
    \defgroup qf3rdrdp_fcs FCS Raw Data Record Plugin
    \ingroup qf3rawdataplugins
*/

/*! \brief FCS raw data record plugin class
    \ingroup qf3rdrdp_fcs
    */

class QFPRDRFCS : public QObject, public QFPluginRawDataRecordBase {
        Q_OBJECT
        Q_INTERFACES(QFPluginRawDataRecord)
    public:
        /** Default constructor */
        QFPRDRFCS(QObject* parent=NULL);
        /** Default destructor */
        virtual ~QFPRDRFCS();

        /** \brief Create a new QFRawDataRecord in the given project \a parent. Also adds the record to the project. */
        virtual QFRawDataRecord* createRecord(QFProject* parent);

        /*! \brief create menu items in \a menu that start insert of record

            The menu items should be connected to slots in the plugin.
        */
        virtual void registerToMenu(QMenu* menu);

        /** \brief short ID for the plugin */
        virtual QString getID() { return tr("fcs"); };

        /** \brief name for the plugin */
        virtual QString getName() { return tr("FCS Correlation Curve Plugin"); };

        /** \brief short description for the plugin */
        virtual QString getDescription() { return tr("manages Fluorescence Correlations Spectroscopy (FCS) curve data."); };

        /** \brief author the plugin */
        virtual QString getAuthor() { return tr("Jan Krieger"); };

        /** \brief copyright information the plugin */
        virtual QString getCopyright() { return tr("(c) 2010 by Jan Krieger"); };

        /** \brief weblink for the plugin */
        virtual QString getWeblink() { return tr(""); };

        /** \brief icon for the plugin */
        virtual QString getIconFilename() { return QString(":/fcs_logo.png"); };
    protected slots:
        /** \brief insert FCS data from file*/
        void insertFCS();
    protected:
        /*! \brief add a ALV5000 file to the current project

            \param filename filename of the ALV5000 file
            \param paramValues values for parameters in paramNames
            \param paramReadonly names of initial parameters that should be declared readonly
        */
        void insertALV5000File(const QString& filename, const QMap<QString, QVariant>& paramValues, const QStringList& paramReadonly);

        /*! \brief add a CSV file to the current project

            \param filename filename of the CSV file
            \param paramValues values for parameters in paramNames
            \param paramReadonly names of initial parameters that should be declared readonly
        */
        void insertCSVFile(const QString& filename, const QMap<QString, QVariant>& paramValues, const QStringList& paramReadonly);
    private:
};

#endif // QFPRDRFCS_H
