#include "qfselectionlistdialog.h"
#include "ui_qfselectionlistdialog.h"
#include <QInputDialog>
#include "qfhtmldelegate.h"

QFSelectionListDialog::QFSelectionListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QFSelectionListDialog)
{
    ui->setupUi(this);
}

QFSelectionListDialog::~QFSelectionListDialog()
{
    delete ui;
}

void QFSelectionListDialog::init(QStringList itemNames, QStringList itemData, QSettings& settings, const QString &prefix) {
    QList<QVariant> itData;
    for (int i=0; i<itemData.size(); i++) {
        itData<<itemData[i];
    }
    init(itemNames, itData, QList<QColor>(), settings, prefix);
}

void QFSelectionListDialog::init(QStringList itemNames, QStringList itemData, QList<QColor> colors, QSettings &settings, const QString &prefix)
{
    init(itemNames, itemData, settings, prefix);
    for (int i=0; i<itemNames.size(); i++) {
        QListWidgetItem* item=ui->listWidget->item(i);
        item->setTextColor(colors.value(i, item->textColor()));
    }
}

void QFSelectionListDialog::init(QStringList itemNames, QList<QVariant> itemData, QList<QColor> colors, QSettings &settings, const QString &prefix)
{
    for (int i=0; i<itemNames.size(); i++) {
        QListWidgetItem* item=new QListWidgetItem(itemNames[i], ui->listWidget);
        item->setCheckState(Qt::Checked);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        item->setData(Qt::UserRole, itemData.value(i, itemNames.value(i, "")));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setItemDelegate(new QFHTMLDelegate(ui->listWidget));

     settings.beginGroup(prefix+"selections");
     int count=settings.value("count", 0).toInt();
     getDataColumnsByUserSaved.clear();
     ui->comboBox->clear();
     for (int i=0; i<count; i++) {
         settings.beginGroup(QString("item%1").arg(i));
         QString n=settings.value("name", "").toString();
         QStringList item=settings.value("items", QStringList()).toStringList();
         if (!n.isEmpty()) {
             getDataColumnsByUserSaved[n]=item;
         }
         ui->comboBox->addItem(n);
         settings.endGroup();
     }
     ui->comboBox->setCurrentIndex(settings.value("lastActivated", -1).toInt());
     settings.endGroup();


}

void QFSelectionListDialog::setLabel(const QString &text) {
    ui->label->setText(text);
}

void QFSelectionListDialog::writeList(QSettings &settings, const QString &prefix) {
    settings.beginGroup(prefix+"selections");
    QStringList keys=getDataColumnsByUserSaved.keys();
    settings.setValue("count", keys.size());
    for (int i=0; i<keys.size(); i++) {
        settings.beginGroup(QString("item%1").arg(i));
        settings.setValue("name", keys[i]);
        settings.setValue("items", getDataColumnsByUserSaved[keys[i]]);
        settings.endGroup();
    }
    settings.setValue("lastActivated", ui->comboBox->currentIndex());
    settings.endGroup();
}

QStringList QFSelectionListDialog::getSelectedDataStrings() const {
    QStringList data;
    for (int i=0; i<ui->listWidget->count(); i++) {
        if (ui->listWidget->item(i)->checkState()==Qt::Checked) data.append(ui->listWidget->item(i)->data(Qt::UserRole).toString());
    }
    return data;
}

QList<QVariant> QFSelectionListDialog::getSelected() const
{
    QList<QVariant> data;
    for (int i=0; i<ui->listWidget->count(); i++) {
        if (ui->listWidget->item(i)->checkState()==Qt::Checked) data.append(ui->listWidget->item(i)->data(Qt::UserRole));
    }
    return data;
}

QList<int> QFSelectionListDialog::getSelectedIndexes() const {
    QList<int> data;
    for (int i=0; i<ui->listWidget->count(); i++) {
        if (ui->listWidget->item(i)->checkState()==Qt::Checked) data.append(i);
    }
    return data;
}

void QFSelectionListDialog::addWidget(const QString& label, QWidget *widget)
{
    ui->formLayout->addRow(label, widget);
}

void QFSelectionListDialog::selectItems(QList<bool> items)
{
    for (int i=0; i<ui->listWidget->count(); i++) {
        ui->listWidget->item(i)->setCheckState(items.value(i, ui->listWidget->item(i)->checkState()==Qt::Checked)?(Qt::Checked):(Qt::Unchecked));
    }
}

void QFSelectionListDialog::on_btnSave_clicked() {
    QString name=tr("my selection name");
    QStringList items=getDataColumnsByUserSaved.keys();
    items<<name;
    bool ok=false;
    name=QInputDialog::getItem(NULL, tr("save selection"), tr("name for new selection:"), items, items.size()-1, true, &ok);
    if (ok) {
        QStringList data;
        for (int i=0; i<ui->listWidget->count(); i++) {
            if (ui->listWidget->item(i)->checkState()==Qt::Checked) data.append(ui->listWidget->item(i)->data(Qt::UserRole).toString());
        }
        data.sort();
        getDataColumnsByUserSaved[name]=data;
        disconnect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getDataColumnsByUserComboBoxSelected(int)));
        ui->comboBox->clear();
        ui->comboBox->addItems(getDataColumnsByUserSaved.keys());
        ui->comboBox->setCurrentIndex(ui->comboBox->findText(name));
        connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getDataColumnsByUserComboBoxSelected(int)));
    }
}

void QFSelectionListDialog::on_btnSelectNone_clicked() {
    for (int i=0; i<ui->listWidget->count(); i++) {
        ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
    }
}

void QFSelectionListDialog::on_btnSelectAll_clicked() {
    for (int i=0; i<ui->listWidget->count(); i++) {
        ui->listWidget->item(i)->setCheckState(Qt::Checked);
    }
}

void QFSelectionListDialog::getDataColumnsByUserComboBoxSelected(int index) {
    QString name=ui->comboBox->itemText(index);
    QStringList newItems=getDataColumnsByUserSaved.value(name, QStringList());
    if (getDataColumnsByUserSaved.contains(name)) {
        for (int i=0; i<ui->listWidget->count(); i++) {
            if (newItems.contains(ui->listWidget->item(i)->data(Qt::UserRole).toString()) && (!ui->listWidget->item(i)->data(Qt::UserRole).toString().isEmpty())) {
                ui->listWidget->item(i)->setCheckState(Qt::Checked);
            } else {
                ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
            }
        }
    }
}


void QFSelectionListDialog::getDataColumnsByUserItemChanged(QListWidgetItem *widgetitem) {
    if (!ui->listWidget) return;
    QStringList data;
    for (int i=0; i<ui->listWidget->count(); i++) {
        if (ui->listWidget->item(i)->checkState()==Qt::Checked) data.append(ui->listWidget->item(i)->data(Qt::UserRole).toString());
    }
    data.sort();

    QMapIterator<QString, QStringList> it(getDataColumnsByUserSaved);
    QString item="";
    while (it.hasNext()) {
        it.next();
        QStringList ld=it.value();
        ld.sort();
        if (data==ld) {
            item=it.key();
        }
    }

    disconnect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getDataColumnsByUserComboBoxSelected(int)));
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(item));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getDataColumnsByUserComboBoxSelected(int)));
}
