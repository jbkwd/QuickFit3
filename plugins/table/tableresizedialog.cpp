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


#include "tableresizedialog.h"
#include "ui_tableresizedialog.h"

TableResizeDialog::TableResizeDialog(QWidget *parent) :
    QFDialog(parent),
    ui(new Ui::TableResizeDialog)
{
    ui->setupUi(this);
}

TableResizeDialog::~TableResizeDialog()
{
    delete ui;
}

void TableResizeDialog::init(int width, int height)
{
    ui->labWidth->setText(QString::number(width));
    ui->spinWidth->setValue(width);
    ui->labHeight->setText(QString::number(height));
    ui->spinHeight->setValue(height);
}

int TableResizeDialog::newWidth() const
{
    return ui->spinWidth->value();
}

int TableResizeDialog::newHeight() const
{
    return ui->spinHeight->value();
}
