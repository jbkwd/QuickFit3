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

#include "qffitfunctionslightsheet.h"
#include "qffitfunctionlightsheetgaussian.h"
#include "qffitfunctionlightsheetgaussianvar.h"
#include "qffitfunctionlightsheetslit.h"

QStringList QFFitFunctionsLightsheet::getIDs() const {
    QStringList res;
    res<<"lightsheet_gaussian";
    res<<"lightsheet_gaussian_sqrte";
    res<<"lightsheet_slit";
    return res;
}

QFFitFunction* QFFitFunctionsLightsheet::get(QString id, QObject* parent) const  {
    if (id=="lightsheet_gaussian") {
        return new QFFitFunctionLightsheetGaussian();
    } else if (id=="lightsheet_gaussian_sqrte") {
        return new QFFitFunctionLightsheetGaussianVar();
    } else if (id=="lightsheet_slit") {
        return new QFFitFunctionLightsheetSlit();
    }
    return NULL;
}

Q_EXPORT_PLUGIN2(fitfunctions_lightsheet, QFFitFunctionsLightsheet)
