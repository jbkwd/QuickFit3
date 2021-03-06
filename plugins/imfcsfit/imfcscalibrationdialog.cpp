/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "imfcscalibrationdialog.h"
#include "qfpluginservices.h"
#include "programoptions.h"
#include "ui_imfcscalibrationdialog.h"
#include "qffitfunction.h"
#include "qffitfunctionmanager.h"

ImFCSCalibrationDialog::ImFCSCalibrationDialog(QWidget *parent) :
    QFDialog(parent),
    ui(new Ui::ImFCSCalibrationDialog)
{
    ui->setupUi(this);
    if (ProgramOptions::getInstance()) {
        ui->edtValues->setPlainText(ProgramOptions::getConfigValue("ImFCSCalibrationDialog/values", ui->edtValues->toPlainText()).toString());
        ui->spinFocusHeight->setValue(ProgramOptions::getConfigValue("ImFCSCalibrationDialog/focusheight", ui->spinFocusHeight->value()).toDouble());
        ui->spinFocusHeightError->setValue(ProgramOptions::getConfigValue("ImFCSCalibrationDialog/focusheight_error", ui->spinFocusHeightError->value()).toDouble());
        ui->chkFixOffset->setChecked(ProgramOptions::getConfigValue("ImFCSCalibrationDialog/fixoffset", ui->chkFixOffset->isChecked()).toBool());
        ui->cmbMicroscopy->setCurrentIndex(ProgramOptions::getConfigValue("ImFCSCalibrationDialog/microscopy", ui->cmbMicroscopy->currentIndex()).toInt());
        ui->radByPixelShift->setChecked(ProgramOptions::getConfigValue("ImFCSCalibrationDialog/bypixelshift", ui->radByPixelShift->isChecked()).toBool());
        ui->radByPixelSize->setChecked(ProgramOptions::getConfigValue("ImFCSCalibrationDialog/bypixelsize", ui->radByPixelSize->isChecked()).toBool());
    }
    setWindowFlags(windowFlags()|Qt::WindowMinMaxButtonsHint);
    on_cmbMicroscopy_currentIndexChanged();
}

ImFCSCalibrationDialog::~ImFCSCalibrationDialog()
{
    if (ProgramOptions::getInstance()) {
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/values", ui->edtValues->toPlainText());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/focusheight", ui->spinFocusHeight->value());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/focusheight_error", ui->spinFocusHeightError->value());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/fixoffset", ui->chkFixOffset->isChecked());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/microscopy", ui->cmbMicroscopy->currentIndex());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/bypixelshift", ui->radByPixelShift->isChecked());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/bypixelsize", ui->radByPixelSize->isChecked());
    }
    delete ui;
}

void ImFCSCalibrationDialog::closeEvent(QCloseEvent *event) {
    if (ProgramOptions::getInstance()) {
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/values", ui->edtValues->toPlainText());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/focusheight", ui->spinFocusHeight->value());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/focusheight_error", ui->spinFocusHeightError->value());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/fixoffset", ui->chkFixOffset->isChecked());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/microscopy", ui->cmbMicroscopy->currentIndex());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/bypixelshift", ui->radByPixelShift->isChecked());
        ProgramOptions::setConfigValue("ImFCSCalibrationDialog/bypixelsize", ui->radByPixelSize->isChecked());
    }
    QFDialog::closeEvent(event);
}

QList<double> ImFCSCalibrationDialog::getValues() const {
    QStringList v=ui->edtValues->toPlainText().split('\n');
    QList<double> vals;
    for (int i=0; i<v.size(); i++) {
        QString s=v[i].trimmed().toLower();
        if (!s.isEmpty()) {
            bool ok=false;
            double val=s.toDouble(&ok);
            if (ok && val>0) {
                vals.append(val);
            }
        }
    }
    return vals;
}

double ImFCSCalibrationDialog::getFocusHeight() const
{
    return ui->spinFocusHeight->value();
}

double ImFCSCalibrationDialog::getFocusHeightError() const
{
    return ui->spinFocusHeightError->value();
}

bool ImFCSCalibrationDialog::getFixOffset() const
{
    return ui->chkFixOffset->isChecked();
}

void ImFCSCalibrationDialog::setFitModels(const QStringList& models, const QString& defaultModel)
{
    ui->cmbFitModel->clear();
    if (!QFPluginServices::getInstance()) return;
    if (!QFPluginServices::getInstance()->getFitFunctionManager()) return;
    for (int i=0; i<models.size(); i++) {
        QFFitFunction* f=QFPluginServices::getInstance()->getFitFunctionManager()->createFunction(models[i]);
        if (f) ui->cmbFitModel->addItem(f->name(), models[i]);
    }
    ui->cmbFitModel->setCurrentIndex(ui->cmbFitModel->findData(defaultModel));
    on_cmbMicroscopy_currentIndexChanged();
}

QString ImFCSCalibrationDialog::getFitModel() const
{
    return ui->cmbFitModel->itemData(ui->cmbFitModel->currentIndex()).toString();
}

void ImFCSCalibrationDialog::on_btnHelp_clicked()
{
        QFPluginServices::getInstance()->displayPluginHelpWindow("imfcs_fit", "calibration.html");

}

void ImFCSCalibrationDialog::on_cmbMicroscopy_currentIndexChanged()
{
    switch (ui->cmbMicroscopy->currentIndex()) {
        case 0: // SPIM, camera, 3D
            if (ui->radByPixelSize->isChecked()) { ui->labError->clear(); ui->cmbFitModel->setCurrentIndex(ui->cmbFitModel->findData("fcs_spim_diffe2_newveff"));}
            if (ui->radByPixelShift->isChecked()) { ui->labError->clear(); ui->cmbFitModel->setCurrentIndex(ui->cmbFitModel->findData("fccs_spim_diffce2"));}
            break;
        case 1: // TIRF, camera, 2D
            if (ui->radByPixelSize->isChecked()) { ui->labError->clear(); ui->cmbFitModel->setCurrentIndex(ui->cmbFitModel->findData("fcs_tir_diffe2"));}
            if (ui->radByPixelShift->isChecked()) { ui->labError->clear(); ui->cmbFitModel->setCurrentIndex(ui->cmbFitModel->findData("fccs_tir_diffe2"));}
            break;
        case 2: // confocal, point, 3D
            if (ui->radByPixelSize->isChecked()) { ui->labError->clear(); ui->labError->setText(tr("<font color=\"red\"><b>!!! ERROR: This combination is impossible, as the pixel-size cannot be set on a point-detector !!!</b></font>"));}
            if (ui->radByPixelShift->isChecked()) { ui->labError->clear(); ui->cmbFitModel->setCurrentIndex(ui->cmbFitModel->findData("fccs_2f_diff3d_wz")); }
            break;

    }
    if (ui->radManual->isChecked()) {
        ui->labError->clear();
    }
}

void ImFCSCalibrationDialog::on_radManual_toggled()
{
    on_cmbMicroscopy_currentIndexChanged();
}

void ImFCSCalibrationDialog::on_radByPixelSize_toggled()
{
    on_cmbMicroscopy_currentIndexChanged();
}

void ImFCSCalibrationDialog::on_radByPixelShift_toggled()
{
    on_cmbMicroscopy_currentIndexChanged();
}
