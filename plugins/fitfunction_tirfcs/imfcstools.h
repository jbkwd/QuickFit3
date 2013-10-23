#ifndef IMFCSTOOLS_H
#define IMFCSTOOLS_H

#include "math.h"
#include "qfmathtools.h"

inline double TIRFCS_newAeff(double a, double wxy) {
    return qfSqr(a)/qfSqr(erf(a/wxy)+wxy/QF_SQRTPI/a*(exp(-qfSqr(a/wxy))-1.0));
}


inline double TIRFCS_newAeffError(double a, double ea, double wxy, double ewxy){
    return 0;
}


inline double QFFitFunctionsTIRFCCSFWDiff2ColorCCF_pixelcorrfactor(double a, double dx, double D, double t, double wg, double wr) {
    const double dt_sigma=sqrt(8.0*D*t+wg*wg+wr*wr);
    const double dt_sigma2=qfSqr(dt_sigma);
    return ( (a-dx)*erf(M_SQRT2*(a-dx)/dt_sigma)-2.0*dx*erf(M_SQRT2*dx/dt_sigma)+(a+dx)*erf(M_SQRT2*(a+dx)/dt_sigma)+
            dt_sigma/QF_SQRT2PI*(exp(-2.0*qfSqr(a-dx)/dt_sigma2)-2.0*exp(-2.0*dx*dx/dt_sigma2)+exp(-2.0*qfSqr(a+dx)/dt_sigma2)) )/
            (2.0*a*a);
}



inline double QFFitFunctionsTIRFCCSFWDiff2ColorCCF_corrfactor_2Dxy(double a, double dx, double dy, double D, double t, double wg, double wr) {
    return QFFitFunctionsTIRFCCSFWDiff2ColorCCF_pixelcorrfactor(a, dx, D, t, wg, wr)*
           QFFitFunctionsTIRFCCSFWDiff2ColorCCF_pixelcorrfactor(a, dy, D, t, wg, wr);
}


inline double QFFitFunctionsTIRFCCSFWADiff2ColorCCF_pixelcorrfactor(double a, double dx, double Gamma, double alpha, double t, double wg, double wr) {
    const double dt_sigma=sqrt(8.0*Gamma*pow(t, alpha)+wg*wg+wr*wr);
    const double dt_sigma2=qfSqr(dt_sigma);
    return ( (a-dx)*erf(M_SQRT2*(a-dx)/dt_sigma)-2.0*dx*erf(M_SQRT2*dx/dt_sigma)+(a+dx)*erf(M_SQRT2*(a+dx)/dt_sigma)+
            dt_sigma/QF_SQRT2PI*(exp(-2.0*qfSqr(a-dx)/dt_sigma2)-2.0*exp(-2.0*dx*dx/dt_sigma2)+exp(-2.0*qfSqr(a+dx)/dt_sigma2)) )/
            (2.0*a*a);
}


inline double QFFitFunctionsTIRFCCSFWADiff2ColorCCF_corrfactor_2Dxy(double a, double dx, double dy, double Gamma, double alpha, double t, double wg, double wr) {
    return QFFitFunctionsTIRFCCSFWADiff2ColorCCF_pixelcorrfactor(a, dx, Gamma, alpha, t, wg, wr)*
           QFFitFunctionsTIRFCCSFWADiff2ColorCCF_pixelcorrfactor(a, dy, Gamma, alpha, t, wg, wr);
}

#endif // IMFCSTOOLS_H