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

#include "qfpfitfunctionstirfcs.h"
#include "qffitfunctionstirfcsdiffe2_relbright.h"
#include "qffitfunctionstirfcsdiffe2.h"
#include "qffitfunctionstirfcsadiffe2.h"
#include "qffitfunctionstirfcsadiff3de2.h"
#include "qffitfunctionstirfcsdiff3de2.h"
#include "qffitfunctionstirfcsdiff3de2_relbright.h"
#include "qffitfunctionstirfccsdiffflowe2.h"
#include "qffitfunctionstirfcsdiffflowe2.h"
#include "qffitfunctionstirffccsfw2csep2ddiffxy2colorccf.h"
#include "qffitfunctionstirffccsfw2csep2ddiffxy2coloracfg.h"
#include "qffitfunctionstirffccsfw2csep2ddiffxy2coloracfr.h"
#include "qffitfunctionstirffccsfw2ddiffxy2colorccf.h"
#include "qffitfunctionstirffccsfw2ddiffxy2coloracfg.h"
#include "qffitfunctionstirffccsfw2ddiffxy2coloracfr.h"
#include "qffitfunctionstirffccsfw2dadiffxy2colorccf.h"
#include "qffitfunctionstirffccsfw2dadiffxy2coloracfg.h"
#include "qffitfunctionstirffccsfw2dadiffxy2coloracfr.h"
#include "qftools.h"
#include "qffitfunctionstirfccsadiffe2.h"
#include "qffitfunctionstirfccsdiff.h"

QStringList QFPFitFunctionsTIRFCS::getIDs() const {
    QStringList res;
     res<<"fcs_tir_diffe2";
     res<<"fcs_tir_adiffe2";
     res<<"fcs_tir_diffe2_diffbright";
     res<<"fcs_tir_adiff3de2";
     res<<"fcs_tir_diff3de2";
     res<<"fcs_tir_diff3de2_diffbright";
     res<<"fccs_tir_diff_flowe2";
     res<<"fccs_tir_adiffe2";
     res<<"fccs_tir_diffe2";
     res<<"fcs_tir_diff_flowe2";
     res<<"fccs_tir_fw_2csep2ddiffxy2coloracfg";
     res<<"fccs_tir_fw_2csep2ddiffxy2coloracfr";
     res<<"fccs_tir_fw_2csep2ddiffxy2colorccf";
     res<<"fccs_tir_fw_2ddiffxy2coloracfg";
     res<<"fccs_tir_fw_2ddiffxy2coloracfr";
     res<<"fccs_tir_fw_2ddiffxy2colorccf";
     res<<"fccs_tir_fw_2dadiffxy2coloracfg";
     res<<"fccs_tir_fw_2dadiffxy2coloracfr";
     res<<"fccs_tir_fw_2dadiffxy2colorccf";
    return res;
}

QFFitFunction* QFPFitFunctionsTIRFCS::get(const QString &id) const  {
    if (id=="fcs_tir_diffe2") {
        return new QFFitFunctionsTIRFCSDiffE2();
    } else if (id=="fcs_tir_adiffe2") {
        return new QFFitFunctionsTIRFCSADiffE2();
    } else if (id=="fcs_tir_adiff3de2") {
        return new QFFitFunctionsTIRFCSADiff3DE2();
    } else if (id=="fccs_tir_diffe2") {
        return new QFFitFunctionsTIRFCCSDiffE2();
    } else if (id=="fcs_tir_diff3de2") {
        return new QFFitFunctionsTIRFCSDiff3DE2();
    } else if (id=="fcs_tir_diff3de2_diffbright") {
        return new QFFitFunctionsTIRFCSDiffE2Diff3Dbright();
    } else if (id=="fcs_tir_diffe2_diffbright") {
        return new QFFitFunctionsTIRFCSDiffE2Diffbright();
    } else if (id=="fccs_tir_diff_flowe2") {
        return new QFFitFunctionsTIRFCCSDiffFlowE2();
    } else if (id=="fccs_tir_adiffe2") {
        return new QFFitFunctionsTIRFCCSADiffE2();
    } else if (id=="fcs_tir_diff_flowe2") {
        return new QFFitFunctionsTIRFCSDiffFlowE2();
    } else if (id=="fccs_tir_fw_2csep2ddiffxy2coloracfg") {
        return new QFFitFunctionsTIRFFCCSFW2CSep2DDiffXY2ColorACFG();
    } else if (id=="fccs_tir_fw_2csep2ddiffxy2coloracfr") {
        return new QFFitFunctionsTIRFFCCSFW2CSep2DDiffXY2ColorACFR();
    } else if (id=="fccs_tir_fw_2csep2ddiffxy2colorccf") {
        return new QFFitFunctionsTIRFFCCSFW2CSep2DDiffXY2ColorCCF();

    } else if (id=="fccs_tir_fw_2ddiffxy2coloracfg") {
        return new QFFitFunctionsTIRFFCCSFW2DDiffXY2ColorACFG();
    } else if (id=="fccs_tir_fw_2ddiffxy2coloracfr") {
        return new QFFitFunctionsTIRFFCCSFW2DDiffXY2ColorACFR();
    } else if (id=="fccs_tir_fw_2ddiffxy2colorccf") {
        return new QFFitFunctionsTIRFFCCSFW2DDiffXY2ColorCCF();

    } else if (id=="fccs_tir_fw_2dadiffxy2coloracfg") {
        return new QFFitFunctionsTIRFFCCSFW2DADiffXY2ColorACFG();
    } else if (id=="fccs_tir_fw_2dadiffxy2coloracfr") {
        return new QFFitFunctionsTIRFFCCSFW2DADiffXY2ColorACFR();
    } else if (id=="fccs_tir_fw_2dadiffxy2colorccf") {
        return new QFFitFunctionsTIRFFCCSFW2DADiffXY2ColorCCF();

    }
    return NULL;
}

int QFPFitFunctionsTIRFCS::getGlobalFitConfigCount() const
{
    return 7;
}

QFFitFunctionConfigForGlobalFitInterface::GlobalFitConfig QFPFitFunctionsTIRFCS::getGlobalFitConfig(int i) const
{
	QFFitFunctionConfigForGlobalFitInterface::GlobalFitConfig res;
    int c=0;
    QString tirfccslabel=tr("configure for TIR-FCCS ...");
    
    if (i==c++) {
        res.shortLabel=QString("2-COLOR-TIR-FCCS/NORMAL_1DPERSPECIES");
        res.groupLabel=tirfccslabel;
        res.menuEntryLabel=tr("... Normal Diffusion, species A+B+AB, c/D per species");
        res.models<<"fccs_tir_fw_2ddiffxy2coloracfg"<<"fccs_tir_fw_2ddiffxy2coloracfr"<<"fccs_tir_fw_2ddiffxy2colorccf";
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);

    } else if (i==c++) {
        res.groupLabel=tirfccslabel;
        res.menuEntryLabel=tr("... Normal Diffusion, species A+B+AB, c/D per species, Da=Db=Dab");
        res.models<<"fccs_tir_fw_2ddiffxy2coloracfg"<<"fccs_tir_fw_2ddiffxy2coloracfr"<<"fccs_tir_fw_2ddiffxy2colorccf";
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(constructQStringListFromItems("diff_coeff_a", "diff_coeff_b", "diff_coeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);


    } else if (i==c++) {
        res.groupLabel=tirfccslabel;
        res.menuEntryLabel=tr("... Normal Diffusion, species A+B+AB, c/D per species, Da=Db, Dab");
        res.models<<"fccs_tir_fw_2ddiffxy2coloracfg"<<"fccs_tir_fw_2ddiffxy2coloracfr"<<"fccs_tir_fw_2ddiffxy2colorccf";
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(constructQStringListFromItems("diff_coeff_a", "diff_coeff_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
    } else  if (i==c++) {
        res.groupLabel=tirfccslabel;
        res.shortLabel=QString("2-COLOR-TIR-FCCS/ANOMALOUS_1DPERSPECIES");
        res.menuEntryLabel=tr("... Anomalous Diffusion, species A+B+AB, c/D per species");
        res.models<<"fccs_tir_fw_2dadiffxy2coloracfg"<<"fccs_tir_fw_2dadiffxy2coloracfr"<<"fccs_tir_fw_2dadiffxy2colorccf";
        res.roles<<"acf0"<<"acf1"<<"fccs";
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha_a"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha_b"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha_ab"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
        res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);


    } else if (i==c++) {

       res.groupLabel=tirfccslabel;
       res.shortLabel=QString("2-COLOR-TIR-FCCS/NORMAL_2DPERCHANNEL");
       res.menuEntryLabel=tr("... 2-comp. 2D Normal Diffusion (xy), species A+B+AB, c per species, D1/D2 per channel");
       res.models<<"fccs_tir_fw_2csep2ddiffxy2coloracfg"<<"fccs_tir_fw_2csep2ddiffxy2coloracfr"<<"fccs_tir_fw_2csep2ddiffxy2colorccf";
       res.roles<<"acf0"<<"acf1"<<"fccs";

       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_a"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_b"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("concentration_ab"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("crosstalk"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_x"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_y"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_distance_z"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("focus_width2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("pixel_width"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("count_rate2"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);
       res.globalParams << constructQListWithMultipleItems(QStringList("background1"), 3);


    } else if (i==c++) { // fccs_tir_diff_flowe2, fccs_tir_diff_flowe2, fccs_tir_diff_flowe2
        res.groupLabel=tirfccslabel;
        res.shortLabel=QString("2-PIXEL-TIR-FCCS/NORMAL+FLOW");
        res.menuEntryLabel=tr("... 2D Normal Diffusion+Flow, ACF + 4 neighbors");
        res.models<<"fccs_tir_diff_flowe2"<<"fccs_tir_diff_flowe2"<<"fccs_tir_diff_flowe2"<<"fccs_tir_diff_flowe2"<<"fccs_tir_diff_flowe2";
        res.roles<<"acf"<<"dccf"<<"dccf"<<"dccf"<<"dccf";
        res.globalParams << constructQListWithMultipleItems(QStringList("n_particle"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("n_components"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff1"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_coeff3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("vflowx"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("vflowy"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width"), 5);

    } else if (i==c++) { // fccs_tir_diff_flowe2, fccs_tir_diff_flowe2, fccs_tir_diff_flowe2
        res.groupLabel=tirfccslabel;
        res.shortLabel=QString("2-PIXEL-TIR-FCCS/ANOMALOUS");
        res.menuEntryLabel=tr("... 2D Anomalous Diffusion, ACF + 4 neighbors");
        res.models<<"fccs_tir_adiffe2"<<"fccs_tir_adiffe2"<<"fccs_tir_adiffe2"<<"fccs_tir_adiffe2"<<"fccs_tir_adiffe2";
        res.roles<<"acf"<<"dccf"<<"dccf"<<"dccf"<<"dccf";
        res.globalParams << constructQListWithMultipleItems(QStringList("n_particle"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("n_components"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff1"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha1"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha2"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_rho3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_acoeff3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("diff_alpha3"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_height"), 5);
        res.globalParams << constructQListWithMultipleItems(QStringList("focus_width"), 5);

    }
    return res;
}

Q_EXPORT_PLUGIN2(fitfunctions_tirfcs, QFPFitFunctionsTIRFCS)
