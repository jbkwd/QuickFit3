/*
Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate: 2015-03-05 23:57:54 +0100 (Do, 05 Mrz 2015) $  (revision $Rev: 3874 $)

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

#ifndef QFFitFunctionsSPIMFCSDiffFlowCENewVeff2_H
#define QFFitFunctionsSPIMFCSDiffFlowCENewVeff2_H
#include "qfpluginfitfunction.h"




/*! \brief QFFitFunction class for a SPIM-FCCS fit model with pure diffusion and flow and 1/sqrt(e) lateral width
    \ingroup qf3fitfunp_fitfunctions_spimfcs
*/
class QFFitFunctionsSPIMFCSDiffFlowCENewVeff2: public QFFitFunction {
    public:
        QFFitFunctionsSPIMFCSDiffFlowCENewVeff2();
        virtual ~QFFitFunctionsSPIMFCSDiffFlowCENewVeff2() {}
        /*! \copydoc QFFitFunction::name()   */
        virtual QString name() const { return QString("SPIM-FCS (camera): 3D Normal Diffusion+Flow (rect. pixel, 1/e^2 radii, new Veff)"); }

        /*! \copydoc QFFitFunction::id()   */
        virtual QString id() const { return QString("fcs_spim_diffflowce2"); }
        /*! \copydoc QFFitFunction::category()   */
        virtual QString category() const { return QObject::tr("SPIM-FCS (camera)"); }

        /*! \copydoc QFFitFunction::evaluate()   */
        virtual double evaluate(double t, const double* parameters) const;

        /*! \copydoc QFFitFunction::calcParameter()   */
        virtual void calcParameter(double* parameterValues, double* error=NULL) const;

        /*! \copydoc QFFitFunction::isParameterVisible()   */
        virtual bool isParameterVisible(int parameter, const double* parameterValues) const;
        /*! \copydoc QFFitFunction::getAdditionalPlotCount()   */
        virtual unsigned int getAdditionalPlotCount(const double* params) const;

        /*! \copydoc QFFitFunction::transformParametersForAdditionalPlot()   */
        virtual QString transformParametersForAdditionalPlot(int plot, double* params) const;

        /*! \copydoc QFFitFunction::get_implementsDerivatives()   */
        virtual bool get_implementsDerivatives() const { return false; }
};

#endif // QFFitFunctionsSPIMFCSDiffFlowCENewVeff2_H