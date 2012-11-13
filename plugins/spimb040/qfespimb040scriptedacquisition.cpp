#include "qfespimb040scriptedacquisition.h"
#include "ui_qfespimb040scriptedacquisition.h"

#include "qfpluginservices.h"
#include "qfstagecombobox.h"
#include "qfextensionmanager.h"
#include "../interfaces/qfextensionlinearstage.h"
#include "qfextension.h"
#include <QtGui>
#include <QtCore>
#include "qfcompleterfromfile.h"
#include "qfespimb040mainwindow2.h"
#include "qfespimb040scriptedacquisitiontools.h"





QFESPIMB040ScriptedAcquisitionDocSearchThread::QFESPIMB040ScriptedAcquisitionDocSearchThread(QStringList files, QObject *parent):
    QThread(parent)
{
    this->files=files;
    stopped=false;
}

void QFESPIMB040ScriptedAcquisitionDocSearchThread::stopThread()
{
    stopped=true;
}

void QFESPIMB040ScriptedAcquisitionDocSearchThread::run()
{
    for (int i=0; i<files.size(); i++) {
        QFile f(files[i]);
        if (f.open(QIODevice::ReadOnly|QIODevice::Text)) {
            QString contents=f.readAll();
            f.close();
            int pos=0;
            QRegExp rx("<!--\\s*func:([\\w\\.\\_]*)\\s*-->(.*)<!--\\s*/func:\\1\\s*-->");
            while ((pos = rx.indexIn(contents, pos)) != -1) {
                QString name=rx.cap(1).trimmed();
                QString help=rx.cap(2).trimmed();
                QRegExp rx2("<!--\\s*template\\s*-->(.*)<!--\\s*/template\\s*-->");
                QString templ="";
                if (rx2.indexIn(help,0)!=-1) {
                    templ=rx2.cap(1);
                }
                emit foundFunction(name, templ, help);
                pos += rx.matchedLength();
                if (stopped) break;
             }
        }
        if (stopped) break;
    }
}















QFESPIMB040ScriptedAcquisition::QFESPIMB040ScriptedAcquisition(QFESPIMB040MainWindow2* mainWindow, QFESPIMB040AcquisitionTools* acqTools, QFPluginLogService* log, QWidget* parent, QFPluginServices* pluginServices, QFESPIMB040OpticsSetup* opticsSetup, QFESPIMB040AcquisitionDescription* acqDescription, QFESPIMB040ExperimentDescription* expDescription, QString configDirectory) :
    QWidget(parent),
    ui(new Ui::QFESPIMB040ScriptedAcquisition)
{

    this->m_pluginServices=pluginServices;
    this->opticsSetup=opticsSetup;
    this->acqDescription=acqDescription;
    this->expDescription=expDescription;
    this->log=log;
    this->acqTools=acqTools;
    this->mainWindow=mainWindow;

    acquisitionTools=new QFESPIMB040ScriptedAcquisitionTools(this, mainWindow, acqTools, log, this, pluginServices, opticsSetup, acqDescription, expDescription);
    instrumentControl=new QFESPIMB040ScriptedAcquisitionInstrumentControl(this, mainWindow, acqTools, log, this, pluginServices, opticsSetup, acqDescription, expDescription);
    acquisitionControl=new QFESPIMB040ScriptedAcquisitionAcquisitionControl(this, mainWindow, acqTools, log, this, pluginServices, opticsSetup, acqDescription, expDescription);


    engine=new QScriptEngine();


    ui->setupUi(this);
    recentMaskFiles=new QRecentFilesMenu(this);
    recentMaskFiles->setUseSystemFileIcons(false);
    recentMaskFiles->setAlwaysEnabled(true);
    connect(recentMaskFiles, SIGNAL(openRecentFile(QString)), this, SLOT(openScriptNoAsk(QString)));
    ui->btnOpen->setMenu(recentMaskFiles);


    updateReplaces();
    //bindLineEdit(ui->edtPrefix1);
    ui->btnCancel->setVisible(false);
    ui->widProgress->setVisible(false);
    ui->labStatus->setVisible(false);
    setScriptFilename(tr("new_acquisition_script.js"));


    QDir d(QFPluginServices::getInstance()->getPluginHelpDirectory("ext_spimb040")+"acquisition_script/");
    QStringList filter;
    filter<<"*.html"<<"*.htm"<<"*.txt";
    QStringList files=d.entryList(filter, QDir::Files);
    threadsFinished=0;
    maxThreads=2;
    QList<QStringList> absFiles;

    for (int i=0; i<maxThreads; i++) {
        QStringList sl;
        absFiles.push_back(sl);
    }
    for (int i=0; i<files.size(); i++) {
        QString file=d.absoluteFilePath(files[i]);
        absFiles[i%maxThreads].append(file);
    }
    for (int i=0; i<maxThreads; i++) {
        threads.append(new QFESPIMB040ScriptedAcquisitionDocSearchThread(absFiles[i], this));
        connect(threads[i], SIGNAL(finished()), this, SLOT(threadFinished()));
        connect(threads[i], SIGNAL(foundFunction(QString,QString,QString)), this, SLOT(addFunction(QString,QString,QString)));
    }
    QTimer::singleShot(10, this, SLOT(delayedStartSearchThreads()));
}

QFESPIMB040ScriptedAcquisition::~QFESPIMB040ScriptedAcquisition()
{
    for (int i=0; i<threads.size(); i++) {
        if (threads[i]->isRunning()) {
            threads[i]->stopThread();
            threads[i]->wait();
        }
    }
    delete ui;
    delete engine;
}

QString QFESPIMB040ScriptedAcquisition::getScript() const
{
    return ui->edtScript->toPlainText();
}

void QFESPIMB040ScriptedAcquisition::loadSettings(QSettings &settings, QString prefix)
{
    lastScript=settings.value(prefix+"script", tr("tools.logText(\"Hello World!\\n\");")).toString();
    ui->edtScript->setPlainText(lastScript);
    recentMaskFiles->readSettings(settings, prefix+"recentScripts/");
}

void QFESPIMB040ScriptedAcquisition::storeSettings(QSettings &settings, QString prefix) const
{
    settings.setValue(prefix+"script", ui->edtScript->toPlainText());
    recentMaskFiles->storeSettings(settings, prefix+"recentScripts/");
}


void QFESPIMB040ScriptedAcquisition::on_btnExecute_clicked()
{
    emit doAcquisition();
    QApplication::processEvents();
    performAcquisition();
}

void QFESPIMB040ScriptedAcquisition::on_btnCancel_clicked()
{
    engine->abortEvaluation();;
}

void QFESPIMB040ScriptedAcquisition::updateReplaces()
{
    setGlobalReplaces(opticsSetup, expDescription, acqDescription);
    //setReplaceValue("acquisition1_name",  cleanStringForFilename(ui->cmbAcquisitionSettings1->currentConfigName()));
    //setReplaceValue("acquisition2_name",  cleanStringForFilename(ui->cmbAcquisitionSettings2->currentConfigName()));
}

void QFESPIMB040ScriptedAcquisition::on_btnNew_clicked()
{
    if (maybeSave()) {
        ui->edtScript->setPlainText("");
        setScriptFilename(tr("new_acquisition_script.js"));
        lastScript=ui->edtScript->toPlainText();
    }
}

void QFESPIMB040ScriptedAcquisition::on_btnOpen_clicked()
{
    openScript("last", true);
}

void QFESPIMB040ScriptedAcquisition::on_btnSave_clicked()
{
    QDir().mkpath(ProgramOptions::getInstance()->getConfigFileDirectory()+"/plugins/ext_spimb040/acquisitionScripts/");
    QString dir=ProgramOptions::getInstance()->getQSettings()->value("QFESPIMB040ScriptedAcquisition/lastScriptDir", ProgramOptions::getInstance()->getConfigFileDirectory()+"/plugins/ext_spimb040/acquisitionScripts/").toString();
    QDir d(dir);
    QString filename=qfGetSaveFileName(this, tr("save acquisition script ..."), d.absoluteFilePath(currentScript), tr("acquisition script (*.js)"));
    if (!filename.isEmpty()) {
        bool ok=true;
        /*if (QFile::exists(filename)) {
            ok=false;
            if (QMessageBox::question(this, tr("save acquisition script ..."), tr("The file\n  '%1'\nalready exists. Overwrite?").arg(filename), QMessageBox::Yes|QMessageBox::No, QMessageBox::No)==QMessageBox::Yes) {
                ok=true;
            }
        }*/
        if (ok) {
            QFile f(filename);
            if (f.open(QIODevice::WriteOnly|QIODevice::Text)) {
                QTextStream s(&f);
                s<<ui->edtScript->toPlainText().toUtf8();
                lastScript=ui->edtScript->toPlainText();
                f.close();
                setScriptFilename(filename);
            }

        }
    }
    ProgramOptions::getInstance()->getQSettings()->setValue("QFESPIMB040ExperimentDescription/lasttemplatedir", dir);
}


#define ADD_SCRIPT_FUNCTION(engine, function, name) { \
    QScriptValue fun = engine->newFunction(function); \
    engine->globalObject().setProperty(name, fun); \
  }
#define ADD_SCRIPT_QOBJECT(engine, someObject, name) { \
     QScriptValue objectValue = engine->newQObject(someObject);\
     engine->globalObject().setProperty(name, objectValue);\
   }
#define ADD_SCRIPT_QFOBJECT(engine, someObject, name) { \
    someObject->setEngine(engine); \
     QScriptValue objectValue = engine->newQObject(someObject);\
     engine->globalObject().setProperty(name, objectValue);\
   }

void QFESPIMB040ScriptedAcquisition::performAcquisition()
{
    ui->btnCancel->setVisible(true);
    ui->widProgress->setVisible(true);
    ui->btnExecute->setEnabled(false);
    ui->widProgress->setSpin(true);
    ui->edtScript->setEnabled(false);
    ui->btnNew->setEnabled(false);
    ui->btnOpen->setEnabled(false);
    ui->btnOpenExample->setEnabled(false);
    ui->btnOpenTemplate->setEnabled(false);
    ui->labStatus->setVisible(true);
    ui->widProgress->setMode(QModernProgressWidget::GradientRing);
    QString script=ui->edtScript->toPlainText();
    log->log_text(tr("\n\n====================================================================================\n"));
    log->log_text(tr("== SCRIPTED ACQUISITION                                                           ==\n"));
    log->log_text(tr("====================================================================================\n"));
    log->log_text(tr("Script:\n%1\n").arg(script));
    log->log_text(tr("====================================================================================\n"));
    QScriptSyntaxCheckResult checkResult=QScriptEngine::checkSyntax(script);
    if (checkResult.state()==QScriptSyntaxCheckResult::Valid) {
        log->log_text(tr("  - script syntax check OK\n"));
        ui->labStatus->setText(tr("script syntax check OK ..."));
        log->log_text(tr("  - initializing script engine\n"));
        ui->labStatus->setText(tr("initializing script engine ..."));
        delete engine;
        engine=new QScriptEngine();
        engine->setProcessEventsInterval(50);
        engine->clearExceptions();
        engine->collectGarbage();
        log->log_text(tr("  - registering script objects\n"));
        ui->labStatus->setText(tr("registering script objects ..."));

        ADD_SCRIPT_QFOBJECT(engine, acquisitionTools, "tools");
        ADD_SCRIPT_QFOBJECT(engine, instrumentControl, "instrument");
        ADD_SCRIPT_QFOBJECT(engine, acquisitionControl, "acquisition");

        ui->labStatus->setText(tr("running script ..."));
        log->log_text(tr("  - running script\n"));
        QScriptValue result = engine->evaluate(script);
        if (engine->hasUncaughtException()) {
            int line = engine->uncaughtExceptionLineNumber();
            log->log_error(tr("\n\n  - SCRIPT ERROR: uncaught exception at line %1: %2").arg(line).arg(result.toString()));
        } else {
            log->log_text(tr("\n\n  - SCRIPT FINISHED SUCCESSFULLY: result %1\n").arg(result.toString()));
        }
    } else {
        if (checkResult.state()==QScriptSyntaxCheckResult::Error) log->log_error(tr("Error in script (l. %1, col. %2): %3").arg(checkResult.errorLineNumber()).arg(checkResult.errorColumnNumber()).arg(checkResult.errorMessage()));
        else  log->log_error(tr("Incomplete script (l. %1, col. %2): %3").arg(checkResult.errorLineNumber()).arg(checkResult.errorColumnNumber()).arg(checkResult.errorMessage()));
    }
    log->log_text(tr("====================================================================================\n"));
    log->log_text(tr("== SCRIPTED ACQUISITION ... DONE!!!                                               ==\n"));
    log->log_text(tr("====================================================================================\n\n\n"));
    ui->btnCancel->setVisible(false);
    ui->widProgress->setVisible(false);
    ui->btnExecute->setEnabled(true);
    ui->edtScript->setEnabled(true);
    ui->btnNew->setEnabled(true);
    ui->btnOpen->setEnabled(true);
    ui->btnOpenExample->setEnabled(true);
    ui->btnOpenTemplate->setEnabled(true);
    ui->labStatus->setVisible(false);
}

void QFESPIMB040ScriptedAcquisition::setStatus(const QString &text)
{
    ui->labStatus->setText(text);
}

void QFESPIMB040ScriptedAcquisition::setStatusProgressRange(double minimum, double maximum)
{
    ui->progress->setRange(minimum, maximum);
}

void QFESPIMB040ScriptedAcquisition::setStatusProgress(double value)
{
    ui->progress->setPercentageMode(QModernProgressWidget::Percent);
    ui->progress->setDisplayPercent(true);
    ui->progress->setValue(value);
}

void QFESPIMB040ScriptedAcquisition::incStatusProgress()
{
    ui->progress->setPercentageMode(QModernProgressWidget::Percent);
    ui->progress->setDisplayPercent(true);
    ui->progress->setValue(ui->progress->value()+1);
}


void QFESPIMB040ScriptedAcquisition::on_btnSyntaxCheck_clicked()
{
    QString script=ui->edtScript->toPlainText();
    QScriptSyntaxCheckResult checkResult=QScriptEngine::checkSyntax(script);
    if (checkResult.state()==QScriptSyntaxCheckResult::Valid) {
        log->log_text(tr("\n\nScript syntax check OK\n"));
    } else {
        if (checkResult.state()==QScriptSyntaxCheckResult::Error) log->log_error(tr("Error in script (l. %1, col. %2): %3").arg(checkResult.errorLineNumber()).arg(checkResult.errorColumnNumber()).arg(checkResult.errorMessage()));
        else  log->log_error(tr("Incomplete script (l. %1, col. %2): %3").arg(checkResult.errorLineNumber()).arg(checkResult.errorColumnNumber()).arg(checkResult.errorMessage()));
    }
}

void QFESPIMB040ScriptedAcquisition::on_btnOpenExample_clicked()
{
    openScript(ProgramOptions::getInstance()->getAssetsDirectory()+"/plugins/spimb040/acquisitionScriptExamples/", false);
}

void QFESPIMB040ScriptedAcquisition::on_btnOpenTemplate_clicked()
{
    openScript(ProgramOptions::getInstance()->getAssetsDirectory()+"/plugins/spimb040/acquisitionScriptTemplates/", false);
}

void QFESPIMB040ScriptedAcquisition::on_edtScript_cursorPositionChanged()
{
    QTextCursor tc = ui->edtScript->textCursor();
    /*
    tc.select(QTextCursor::WordUnderCursor);
    QString text=tc.selectedText();

    QString word=text.toLower();*/


    QString text=ui->edtScript->toPlainText();
    QString word;
    int newPos=tc.position();
    if (newPos>=0 && newPos<text.size()) {
        word+=text[newPos];
        int p=newPos-1;
        while (p>=0 && (text[p].isLetterOrNumber()||text[p]=='_'||text[p]=='.')) {
            word=text[p]+word;
            p--;
        }
        p=newPos+1;
        while (p<text.size() && (text[p].isLetterOrNumber()||text[p]=='_'||text[p]=='.')) {
            word=word+text[p];
            p++;
        }
        word=word.toLower();
    }



    if (functionhelp.contains(word)) {
        ui->labHelp->setText(getFunctionHelp(word));
        //ui->labTemplate->setText(tr("<tt>%1</tt>").arg(getFunctionTemplate(word)));
    } else {
        ui->labHelp->setText(tr("no help for '%1'' available ...").arg(word));
    }
}

void QFESPIMB040ScriptedAcquisition::on_btnHelp_clicked()
{
    m_pluginServices->displayHelpWindow(m_pluginServices->getPluginHelpDirectory("ext_spimb040")+"/acquisition_script.html");
}

bool QFESPIMB040ScriptedAcquisition::maybeSave() {
    if (ui->edtScript->toPlainText().isEmpty()) return true;
    if (ui->edtScript->toPlainText()==lastScript) return true;
    int r=QMessageBox::question(this, tr("save acquisition script ..."), tr("The current script has not been saved.\n  Delete?\n    Yes: Any changes will be lost.\n    No: You will be asked for a filename for the script.\n    Cancel: return to editing the script."), QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::No);
    if (r==QMessageBox::Yes) {
        return true;
    } else if (r==QMessageBox::No) {
        on_btnSave_clicked();
        return true;
    }

    return false;
}

void QFESPIMB040ScriptedAcquisition::setScriptFilename(QString filename)
{
    currentScript=filename;
    recentMaskFiles->addRecentFile(filename);
    ui->labScriptFilename->setText(tr("current script: <tt><i>%1</i></tt>").arg(QFileInfo(filename).fileName()));
}

QString QFESPIMB040ScriptedAcquisition::getFunctionTemplate(QString name)
{
    return functionhelp.value(name.toLower(), qMakePair(name+"()", QString())).first;
}

QString QFESPIMB040ScriptedAcquisition::getFunctionHelp(QString name)
{
    return functionhelp.value(name.toLower(), qMakePair(QString(""), tr("<b><tt>%1(...)</tt></b> - <i>function </i>:<br>no help available").arg(name))).second;
}


void QFESPIMB040ScriptedAcquisition::openScript(QString dir, bool saveDir) {
    if (maybeSave()) {
        if (dir=="last") {
            QDir().mkpath(ProgramOptions::getInstance()->getConfigFileDirectory()+"/plugins/ext_spimb040/acquisitionScripts/");
            dir=ProgramOptions::getInstance()->getQSettings()->value("QFESPIMB040ScriptedAcquisition/lastScriptDir", ProgramOptions::getInstance()->getConfigFileDirectory()+"/plugins/ext_spimb040/acquisitionScripts/").toString();
        }
        QString filename=qfGetOpenFileName(this, tr("open script ..."), dir, tr("acquisition script (*.js)"))    ;
        if (QFile::exists(filename)) {
            QFile f(filename);
            if (f.open(QIODevice::ReadOnly|QIODevice::Text)) {
                ui->edtScript->setPlainText(QString::fromUtf8(f.readAll()));
                setScriptFilename(filename);
                f.close();
                lastScript=ui->edtScript->toPlainText();
            }
        }
        if (saveDir) ProgramOptions::getInstance()->getQSettings()->setValue("QFESPIMB040ScriptedAcquisition/lastScriptDir", dir);
    }

}

void QFESPIMB040ScriptedAcquisition::openScriptNoAsk(QString filename)
{
    if (maybeSave()) {
        if (QFile::exists(filename)) {
            QFile f(filename);
            if (f.open(QIODevice::ReadOnly|QIODevice::Text)) {
                ui->edtScript->setPlainText(QString::fromUtf8(f.readAll()));
                setScriptFilename(filename);
                f.close();
                lastScript=ui->edtScript->toPlainText();
            }
        }
    }
}

void QFESPIMB040ScriptedAcquisition::threadFinished()
{
    threadsFinished++;
    if (threadsFinished>=maxThreads) {
        ui->progress->setSpin(false);
        ui->progress->setVisible(false);
        ui->labProgress->setVisible(false);
    }
}

void QFESPIMB040ScriptedAcquisition::delayedStartSearchThreads()
{
    helpModel.setStringList(defaultWords);
    ui->progress->setVisible(true);
    ui->progress->setSpin(true);
    ui->labProgress->setVisible(true);
    threadsFinished=0;
    for (int i=0; i<threads.size(); i++) {
        threads[i]->start();
    }
}

void QFESPIMB040ScriptedAcquisition::addFunction(QString name, QString templ, QString help)
{
    functionhelp[name.toLower()]=qMakePair(templ, help);
    QStringList sl;
    /*sl=compExpression->stringlistModel()->stringList();
    if (!sl.contains(name)) sl.append(name);
    if (!sl.contains(templ)) sl.append(templ);
    sl.sort();
    compExpression->stringlistModel()->setStringList(sl);*/
    sl=helpModel.stringList();
    sl.removeAll(name);
    sl.removeAll(templ);
    sl.append(templ);
    sl.sort();
    helpModel.setStringList(sl);
}

