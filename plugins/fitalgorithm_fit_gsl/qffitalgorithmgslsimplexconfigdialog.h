/*
Copyright (c) 2008-2014 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center

    last modification: $LastChangedDate$  (revision $Rev$)

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

#ifndef QFFITALGORITHMGSLSIMPLEXCONFIGDIALOG_H
#define QFFITALGORITHMGSLSIMPLEXCONFIGDIALOG_H

#include <QDialog>
#include "qffitalgorithm.h"

namespace Ui {
    class QFFitAlgorithmGSLSimplexConfigDialog;
}

class QFFitAlgorithmGSLSimplexConfigDialog : public QDialog
{
        Q_OBJECT
        
    public:
        explicit QFFitAlgorithmGSLSimplexConfigDialog(QWidget *parent = 0);
        ~QFFitAlgorithmGSLSimplexConfigDialog();
        
        /** \brief initialize the dialog from the parameters in the given algorithm */
        void init(const QFFitAlgorithm* algorithm);
        /** \brief store the parameters to the given algorithm */
        void storeParams(QFFitAlgorithm* algorithm) const;
    private:
        Ui::QFFitAlgorithmGSLSimplexConfigDialog *ui;
};

#endif // QFFITALGORITHMGSLSIMPLEXCONFIGDIALOG_H
