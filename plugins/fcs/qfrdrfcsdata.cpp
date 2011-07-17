#include "qfrdrfcsdata.h"
#include <errno.h>
#include <QtXml>

QFRDRFCSData::QFRDRFCSData(QFProject* parent):
    QFRawDataRecord(parent)
{
    correlationRuns=0;
    correlationN=0;
    correlationT=NULL;
    correlation=NULL;
    correlationErrors=NULL;
    correlationMean=NULL;
    correlationStdDev=NULL;

    rateRuns=0;
    rateN=0;
    rateT=NULL;
    rate=NULL;

    autoCalcRateN=200;
    binnedRateN=0;
    binnedRateT=NULL;
    binnedRate=NULL;

    leaveout.clear();
}

QFRDRFCSData::~QFRDRFCSData()
{
    if (correlationT) free(correlationT);
    if (correlation) free(correlation);
    if (correlationErrors) free(correlationErrors);
    if (correlationMean) free(correlationMean);
    if (correlationStdDev) free(correlationStdDev);
    if (rateT) free(rateT);
    if (rate) free(rate);
    if (binnedRateT) free(binnedRateT);
    if (binnedRate) free(binnedRate);
    correlationRuns=0;
    correlationN=0;
    correlationT=NULL;
    correlation=NULL;
    correlationMean=NULL;
    correlationStdDev=NULL;
    correlationErrors=NULL;

    autoCalcRateN=-1;
    binnedRateN=0;
    binnedRateT=NULL;
    binnedRate=NULL;

    rateRuns=0;
    rateN=0;
    rateT=NULL;
    rate=NULL;
}

void QFRDRFCSData::resizeCorrelations(long long N, int runs) {
    if (correlationT) free(correlationT);
    if (correlation) free(correlation);
    if (correlationMean) free(correlationMean);
    if (correlationStdDev) free(correlationStdDev);
    correlationRuns=0;
    correlationN=0;
    correlationT=NULL;
    correlation=NULL;
    correlationMean=NULL;
    correlationStdDev=NULL;
    correlationErrors=NULL;
    runsVisibleList.clear();
    for (int i=0; i<=N; i++) runsVisibleList.append(true);
    correlationN=N;
    correlationRuns=runs;
    correlationT=(double*)calloc(correlationN, sizeof(double));
    correlationMean=(double*)calloc(correlationN, sizeof(double));
    correlationStdDev=(double*)calloc(correlationN, sizeof(double));
    correlation=(double*)calloc(correlationN*correlationRuns, sizeof(double));
    correlationErrors=(double*)calloc(correlationN*correlationRuns, sizeof(double));
    if (!correlationT || !correlation || !correlationMean || !correlationStdDev || !correlationErrors)
        setError(tr("Error while allocating memory for correlation function data!"));
    emit rawDataChanged();
}

void QFRDRFCSData::resizeRates(long long N, int runs) {
    if (rateT) free(rateT);
    if (rate) free(rate);
    rateRuns=0;
    rateN=0;
    rateT=NULL;
    rate=NULL;
    rateN=N;
    rateRuns=runs;
    rateT=(double*)calloc(rateN, sizeof(double));
    rate=(double*)calloc(rateN*rateRuns, sizeof(double));
    if (!rateT || !rate)
        setError(tr("Error while allocating memory for count rate data!"));
    emit rawDataChanged();
    rateMean.clear();
    rateStdDev.clear();
}

void QFRDRFCSData::resizeBinnedRates(long long N) {
    if (binnedRateT) free(binnedRateT);
    if (binnedRate) free(binnedRate);
    binnedRateN=0;
    binnedRateT=NULL;
    binnedRate=NULL;
    binnedRateN=N;
    if (N>0) {
        binnedRateT=(double*)calloc(binnedRateN, sizeof(double));
        binnedRate=(double*)calloc(binnedRateN*rateRuns, sizeof(double));
        if (!binnedRateT || !binnedRate)
            setError(tr("Error while allocating memory for binned count rate data!"));
        setIntProperty("BINNED_RATE_N", binnedRateN, false, false);
    }
    emit rawDataChanged();
}

void QFRDRFCSData::calcBinnedRate() {
    if (autoCalcRateN<=0) return;
    long long d=0;
    long N=0;
    /*d=ceil((double)rateN/(double)binnedRateN);
    N=ceil((double)rateN/(double)d);
    if (binnedRateN<=0 && autoCalcRateN>0) {*/
        d=(long long)ceil((double)rateN/(double)autoCalcRateN);
        N=(long long)ceil((double)rateN/(double)d);
        if ((double)rateN/(double)autoCalcRateN<=1) N=0;
        resizeBinnedRates(N);
    //}
    if (binnedRateN>0) {
        if (N!=binnedRateN) resizeBinnedRates(N);
        for (long run=0; run<rateRuns; run++) {
            for (long long i=0; i<binnedRateN; i++) {
                long long jmin=i*d;
                long long jmax=mmin(rateN, (i+1)*d)-1;
                double sum=0;
                for (long long j=jmin; j<=jmax; j++) {
                    sum=sum+rate[run*rateN+j];
                }
                binnedRate[run*binnedRateN+i]=sum/(double)(jmax-jmin+1);
                binnedRateT[i]=rateT[jmin];
            }
        }
    }
   emit rawDataChanged();
}

void QFRDRFCSData::recalculateCorrelations() {
    if (correlationN<=0 || correlationRuns<=0 || !correlation || !correlationMean || !correlationStdDev) {
       // std::cout<<"error at start of recalculateCorrelations()\n";
        return;
    }
    for (long long i=0; i<correlationN; i++) {
        double m=0.0;
        double s=0.0;
        double cnt=0;
        for (int run=0; run<correlationRuns; run++) {
            if (!leaveoutRun(run)) {
                double c=correlation[run*correlationN+i];
                m=m+c;
                s=s+c*c;
                cnt=cnt+1.0;
            }
        }
        if (cnt==0) correlationMean[i]=0;
        else correlationMean[i]=m/cnt;
        if (cnt<=1) correlationStdDev[i]=0;
        else correlationStdDev[i]=sqrt(s/cnt-m*m/cnt/cnt);
    }
    emit rawDataChanged();
}

double QFRDRFCSData::calcRateMean(int run) {
    if (rateN<=0 || !rate) {
       // std::cout<<"error at start of recalculateCorrelations()\n";
        return 0.0;
    }
    double m=0.0;
    if (run<rateRuns) {
        for (long long i=0; i<rateN; i++) {
            m=m+rate[run*rateN+i];
        }
    }
    double me= m/(double)rateN;
    rateMean[run]=me;
    return me;
}



double QFRDRFCSData::getRateMean(int run) {
    if (rateN<=0 || !rate) {
        return 0.0;
    }
    if (run<0) {
        double cnt=0;
        double sum=0;
        for (int i=0; i<rateN; i++) {
            double r=getRateMean(i);
            if (r>0) {
                sum=sum+r;
                cnt++;
            }
        }
        return sum/cnt;
    }
    if (!rateMean.contains(run)) {
        rateMean[run]=calcRateMean(run);
    }
    return rateMean[run];
}

double QFRDRFCSData::getRateStdDev(int run) {
   if (rateN<0 || !rate) {
        return 0.0;
    }
    if (run<0) {
        double cnt=0;
        double sum=0;
        for (int i=0; i<rateN; i++) {
            double r=getRateStdDev(i);
            if (r>0) {
                sum=sum+r;
                cnt++;
            }
        }
        return sum/cnt;
    }
    if (!rateStdDev.contains(run)) {
        rateStdDev[run]=calcRateStdDev(run);
    }
    return rateStdDev[run];
}

double QFRDRFCSData::calcRateStdDev(int run) {
    if (rateN<=0 || !rate) {
       // std::cout<<"error at start of recalculateCorrelations()\n";
        return 0.0;
    }
    double m=0.0;
    double s=0.0;
    if (run<rateRuns) {
        for (long long i=0; i<rateN; i++) {
            double v=rate[run*rateN+i];
            m=m+v;
            s=s+v*v         ;
        }
    }
    double sd=sqrt(s/(double)rateN-m*m/(double)rateN/(double)rateN);
    rateStdDev[run]=sd;
    return sd;
}

void QFRDRFCSData::calcRateMinMax(int run, double& min, double& max) {
    if (rateN<=0 || !rate) {
       // std::cout<<"error at start of recalculateCorrelations()\n";
        return;
    }
    min=max=0;
    if (run<rateRuns) {
        min=rate[run*rateN];
        max=min;
        for (long long i=0; i<rateN; i++) {
            double r=rate[run*rateN+i];
            if (r>max) max=r;
            if (r<min) min=r;
        }
    }
}


void QFRDRFCSData::exportData(const QString& format, const QString& filename)const  {
/*    if (!datamodel) return;
    QString f=format.toUpper();
    if (f=="CSV") {
        datamodel->saveCSV(filename);
    } else if (f=="SLK" || f=="SYLK") {
        datamodel->saveSYLK(filename);
    }*/
}


void QFRDRFCSData::intWriteData(QXmlStreamWriter& w) {
    if (leaveout.size()>0) {
        QString l="";
        for (int i=0; i<leaveout.size(); i++) {
            if (!l.isEmpty()) l=l+",";
            l=l+QString::number(leaveout[i]);
        }
        w.writeStartElement("leaveout");
        w.writeAttribute("list", l);
        w.writeEndElement();
    }
}

void QFRDRFCSData::intReadData(QDomElement* e) {
    leaveout.clear();
    if (e) {
        QDomElement te=e->firstChildElement("leaveout");
        QString l=te.attribute("list");
        QStringList li=l.split(",");
        for (int i=0; i<li.size(); i++) {
            bool ok=false;
            int lo=li[i].toUInt(&ok);
            if (ok) leaveout.append(lo);
        }
    }

    autoCalcRateN=getProperty("AUTO_BINNED_RATE_N", autoCalcRateN).toInt();

    QString filetype=getProperty("FILETYPE", "unknown").toString();
    //std::cout<<"reading data "<<filetype.toStdString()<<" from 1/"<<files.size()<<" '"<<files.join(", ").toStdString()<<"'\n";

    rateMean.clear();
    rateStdDev.clear();

    if (filetype.toUpper()=="ALV5000") {
        if (files.size()<=0) {
            setError(tr("there are no files in the FCS record!"));
            return;
        }
        loadFromALV5000(files[0]);
    } else if (filetype.toUpper()=="ISS_ALBA") {
        if (files.size()<=0) {
            setError(tr("there are no files in the FCS record!"));
            return;
        }
        loadCorrelationCurvesFromALBA(files[0]);
    } else if (filetype.toUpper()=="CSV_CORR") {
        if (files.size()<=0) {
            setError(tr("there are no files in the FCS record!"));
            return;
        }
        loadCorrelationCurvesFromCSV(files[0]);
    } else if (filetype.toUpper()=="CSV_RATE") {
        if (files.size()<=0) {
            setError(tr("there are no files in the FCS record!"));
            return;
        }
        loadCountRatesFromCSV(files[0]);
    } else if (filetype.toUpper()=="CSV_CORR_RATE") {
        if (files.size()<=1) {
            setError(tr("there are too few files in the FCS record!"));
            return;
        }
        loadCorrelationCurvesFromCSV(files[0]);
        loadCountRatesFromCSV(files[1]);
    }
    //std::cout<<"intReadData ended\n";
}

bool QFRDRFCSData::loadCountRatesFromCSV(QString filename) {
    char separatorchar=',';
    char commentchar='#';
    std::string d=getProperty("CSV_SEPARATOR", ",").toString().toStdString();
    std::string startswith=getProperty("CSV_STARTSWITH", "").toString().toStdString();
    double timefactor=getProperty("CSV_TIMEFACTOR", 1.0).toDouble();
    if (d.size()>0) separatorchar=d[0];
    d=getProperty("CSV_COMMENT", ",").toString().toStdString();
    if (d.size()>0) commentchar=d[0];
    try {
        datatable2 tab;                   // instanciate
        //std::cout<<"opening CSV: "<<filename.toStdString()<<std::endl;
        tab.load_csv(filename.toStdString(), separatorchar, commentchar, startswith);        // load some csv file
        long long lines=tab.get_line_count();
        long long columns=tab.get_column_count();
        resizeRates(lines, columns-1);
        //std::cout<<"resized correlation to: N="<<lines<<", runs="<<columns-1<<std::endl;
        for (long long l=0; l<lines; l++) {
            rateT[l]=tab.get(0, l)*timefactor;
            //std::cout<<correlationT[l]<<", ";
            for (int c=1; c<columns; c++) {
                rate[(c-1)*rateN+l]=tab.get(c, l);
                //std::cout<<correlation[(c-1)*correlationN+l]<<", ";
                //std::cout<<"c="<<c<<std::endl;
            }
            //std::cout<<std::endl;
        }
        //std::cout<<"recalculating correlations ... ";
        //std::cout<<"DONE!\n";
    } catch(datatable2_exception& e) {   // error handling with exceptions
        setError(tr("Error while reading correlation functions from CSV file '%1': %2").arg(filename).arg(QString(e.get_message().c_str())));
    }
    //std::cout<<"opening CSV: "<<filename.toStdString()<<" ... DONE!\n";
    emit rawDataChanged();
    calcBinnedRate();
    return true;
}

bool QFRDRFCSData::loadCorrelationCurvesFromCSV(QString filename) {
    char separatorchar=',';
    char commentchar='#';
    double timefactor=getProperty("CSV_TIMEFACTOR", 1.0).toDouble();
    std::string d=getProperty("CSV_SEPARATOR", ",").toString().toStdString();
    std::string startswith=getProperty("CSV_STARTSWITH", "").toString().toStdString();
    if (d.size()>0) separatorchar=d[0];
    d=getProperty("CSV_COMMENT", ",").toString().toStdString();
    if (d.size()>0) commentchar=d[0];
    try {
        datatable2 tab;                   // instanciate
        //std::cout<<"opening CSV: "<<filename.toStdString()<<std::endl;
        //std::cout<<"startswith = "<<startswith<<std::endl;
        tab.load_csv(filename.toStdString(), separatorchar, commentchar, startswith);        // load some csv file
        long long lines=tab.get_line_count();
        int columns=tab.get_column_count();
        resizeCorrelations(lines, columns-1);
        //std::cout<<"resized correlation to: N="<<lines<<", runs="<<columns-1<<std::endl;
        for (long long l=0; l<lines; l++) {
            correlationT[l]=tab.get(0, l)*timefactor;
            //std::cout<<correlationT[l]<<", ";
            for (int c=1; c<columns; c++) {
                correlation[(c-1)*correlationN+l]=tab.get(c, l);
                //std::cout<<correlation[(c-1)*correlationN+l]<<", ";
                //std::cout<<"c="<<c<<std::endl;
            }
            //std::cout<<std::endl;
        }
        //std::cout<<"recalculating correlations ... ";
        recalculateCorrelations();
        //std::cout<<"DONE!\n";
    } catch(datatable2_exception& e) {   // error handling with exceptions
        setError(tr("Error while reading correlation functions from CSV file '%1': %2").arg(filename).arg(QString(e.get_message().c_str())));
    }
    //std::cout<<"opening CSV: "<<filename.toStdString()<<" ... DONE!\n";
    emit rawDataChanged();
    return true;
}

bool QFRDRFCSData::loadCorrelationCurvesFromALBA(QString filename) {
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream stream(&f);
        QString line;
        // search for [Data] in file
        do {
            line=stream.readLine().toLower();
        } while ((!line.isNull()) && (!line.contains("[data]")));
        if (line.contains("[data]")) {
            QList<double> tdata, cdata;
            do {
                line=stream.readLine();
                QStringList sl=line.split(",");
                if (sl.size()>=2) {
                    tdata.append(sl[0].toDouble());
                    cdata.append(sl[1].toDouble());
                    //std::cout<<tdata[tdata.size()-1]<<", "<<cdata[cdata.size()-1]<<std::endl;
                }
            } while (!line.isNull());
            resizeCorrelations(qMin(tdata.size(), cdata.size()), 1);
            for (int i=0; i<qMin(tdata.size(), cdata.size()); i++) {
                correlationT[i]=tdata[i];
                correlation[i]=cdata[i];
            }
        } else {
            setError(tr("ISS ALBA file '%1' does not contain a [data] section").arg(filename));
            return false;
        }
    } else {
        setError(tr("could not open file '%1'").arg(filename));
        return false;
    }
    recalculateCorrelations();
    emit rawDataChanged();
    return true;
}

bool QFRDRFCSData::loadFromALV5000(QString filename) {
    //std::cout<<"loadFromALV5000('"<<filename.toStdString()<<"')\n";
    FILE* alv_file=fopen(filename.toAscii().data(), "r");
    //std::cout<<"    alv_file = "<<alv_file<<"\n";
    if (!alv_file) {
        setError(tr("could not open file '%1'").arg(filename));
        return false;
    }
    if (ferror(alv_file)) {
        setError(tr("%2").arg(filename).arg(strerror(errno)));
        return false;
    }

    int runs=0;
    bool isDual=false;

    bool readingHeader=true;
    //bool isCorrelation=false;
    //bool isCounRate=false;
    bool findIdentifier=true;
    ALV_TOKEN token=ALV_getToken(alv_file, readingHeader);
    // first we parse the header (until we find the first quoted string)
    // the data from the header is interpeted and the resulting quickfitDataStoreItem are
    // beeing created.
    while (token.type!=ALV_EOF && readingHeader) {
        if (findIdentifier) {
            if (token.type==ALV_NAME) {
                if (token.value.contains("ALV-5000", Qt::CaseInsensitive)) {
                    setQFProperty("ALV_TYPE", token.value, false, true);
                    findIdentifier=false;
                } else if (token.type!=ALV_LINEBREAK && token.type!=ALV_EOF) {
                    setError(tr("did not find file header").arg(filename));
                    return false;
                }
            }
        } else {
            if (token.type==ALV_NAME) { // here we parse "<name> : <value>"
                QString name=token.value; // store <name>
                QString value;
                // get next token which has to be a colon':'
                token=ALV_getToken(alv_file, readingHeader);
                if (token.type!=ALV_COLON) {
                    if (name.toLower().startsWith("alv") && name.toLower().endsWith("data")) {
                        int pnum=1;
                        while (propertyExists(QString("ALV_TYPE%1").arg(pnum))) pnum++;
                        setQFProperty(QString("ALV_TYPE%1").arg(pnum), token.value, false, true);
                    } else {
                        setError(tr("colon ':' expected, but found '%2'").arg(filename).arg(token.value));
                        return false;
                    }
                } else {
                    // get next token which has to be a value or a quoted string or a linebreak
                    token=ALV_getToken(alv_file, readingHeader);
                    if (token.type==ALV_QUOTED) {
                        value=token.value;
                    } else if (token.type==ALV_VALUE) {
                        value=token.value;
                    } else if (token.type!=ALV_LINEBREAK) {
                        setError(tr("linebreak, number or quoted string expected").arg(filename));
                        return false;
                    }
                    // here we check whether this is an interpreted data field (i.e. it is stored in a separate field of item1
                    if (name.compare("Date",  Qt::CaseInsensitive)==0) {
                        QDate date=ALV_toDate(value);
                        if (date.year()<1950) date.setDate(date.year()+100, date.month(), date.day());
                        setQFProperty("DATE", date, false, true);
                    } else if (name.compare("Time",  Qt::CaseInsensitive)==0) {
                        QTime date=ALV_toTime(value);
                        setQFProperty("TIME", date, false, true);
                    } else if (name.contains("Duration",  Qt::CaseInsensitive)) {
                        setQFProperty("DURATION [s]", token.doubleValue, false, true);
                    } else if (name.compare("Runs",  Qt::CaseInsensitive)==0) {
                        runs=(int)round(token.doubleValue);
                    } else if (name.contains("Temperature",  Qt::CaseInsensitive)) {
                        setQFProperty("TEMPERATURE [K]", token.doubleValue, false, true);
                    } else if (name.contains("Viscosity",  Qt::CaseInsensitive)) {
                        setQFProperty("VISCOSITY [cp]", token.doubleValue, false, true);
                    } else if (name.contains("Refractive Index",  Qt::CaseInsensitive)) {
                        setQFProperty("REFRACTIVE_INDEX", token.doubleValue, false, true);
                    } else if (name.contains("Wavelength",  Qt::CaseInsensitive)) {
                        setQFProperty("WAVELENGTH [nm]", token.doubleValue, false, true);
                    } else if (name.contains("Angle",  Qt::CaseInsensitive)) {
                        setQFProperty("ANGLE [�]", token.doubleValue, false, true);
                    } else if (name.contains("MeanCR",  Qt::CaseInsensitive)) {
                        // ignore this property, as it is calculated by this class
                    } else if (name.contains("SampMemo",  Qt::CaseInsensitive)) {
                        QString text=getProperty("SAMPLE_MEMO", "").toString();
                        if (!value.isEmpty()) text=text+"\n"+value;
                        setQFProperty("SAMPLE_MEMO", text, false, true);
                    } else if (name.compare("Mode",  Qt::CaseInsensitive)==0) {
                        setQFProperty("MODE", value, false, true);
                        setQFProperty("CROSS_CORRELATION", (bool)value.contains("CROSS", Qt::CaseInsensitive), false, true);
                        isDual=value.contains("DUAL", Qt::CaseInsensitive);
                        setQFProperty("DUAL_CHANNEL", isDual, false, true);
                    } else {
                        setQFProperty(name, value, false, true);
                    }
                }

            } else if (token.type==ALV_QUOTED) {
                // we stop reading the header when we meet the first quoted string token
                // this is possible as every line of the header begins with an unquoted name
                // token followed by a colon and number/quoted or alineabreak (identifier line!)
                readingHeader=false;
            }
        }
        if (readingHeader) token=ALV_getToken(alv_file, readingHeader);
    }

    int channel=0;
    if (isDual) {
        if (propertyExists("CHANNEL")) channel=getProperty("CHANNEL", 0).toInt();
    }

    //std::cout<<"parsing data section ... runs="<<runs<<"  channel="<<channel<<std::endl;
    while (token.type!=ALV_EOF) {
        bool getNew=true;
        if (token.type==ALV_QUOTED && token.value.contains("correlation", Qt::CaseInsensitive)) {
            //std::cout<<"  parsing correlation section ..."<<std::endl;
            getNew=false;
            QVector<QVector<double> > dat;
            token=ALV_readNumberMatrix(alv_file, &dat);
            resizeCorrelations(dat.size(), runs);
            for (long long  i=0; i<dat.size(); i++) {
                QVector<double>& d=dat[i];
                if (d.size()<=0) {
                    setError(tr("too few columns in line %1 of correlation block.").arg(i));
                    return false;
                }
                correlationT[i]=d[0]/1000.0;
                if (runs==1) {
                    // one run => there is no average in the file!
                    if (d.size()<=1+channel)  {
                        setError(tr("too few columns in line %1 of correlation block.").arg(i));
                        return false;
                    }
                    correlation[i]=d[1+channel];
                } else if (runs>1) {
                    // multiple runs -> average is in file -> ignore average column
                    if (d.size()<=1+channel*(1+runs)+runs) {
                        setError(tr("too few columns in line %1 of correlation block.").arg(i));
                        return false;
                    }
                    for (long j=0; j<runs; j++) {
                        correlation[j*correlationN+i]=d[1+channel*(1+runs)+(1+j)];
                    }
                }
            }
        }
        if (token.type==ALV_QUOTED && token.value.contains("count", Qt::CaseInsensitive)) {
            //std::cout<<"  parsing rate section ..."<<std::endl;
            getNew=false;
            QVector<QVector<double> > dat;
            token=ALV_readNumberMatrix(alv_file, &dat);
            resizeRates(dat.size(), runs);
            //std::cout<<"  reading in rate section ... lines="<<dat.size()<<" runs="<<runs<<" channel="<<channel<<std::endl;
            for (long long i=0; i<dat.size(); i++) {
                QVector<double>& d=dat[i];
                if (d.size()<1) {
                    setError(tr("too few columns in line %1 of rate block.").arg(i));
                    return false;
                }
                rateT[i]=d[0];
                if (runs==1) {
                    // one run => there is no average in the file!
                    if (d.size()<=1+channel)  {
                        setError(tr("too few columns in line %1 of rate block.").arg(i));
                        return false;
                    }
                    rate[i]=d[1+channel];
                } else if (runs>1) {
                    // multiple runs -> average is in file -> ignore average column
                    if (d.size()<=1+channel) {
                        setError(tr("too few columns in line %1 of rate block.").arg(i));
                        return false;
                    }
                    rate[(runs-1)*rateN+i]=d[1+channel];
                }
            }
        }
        if (getNew) token=ALV_getToken(alv_file, false);
    }
    fclose(alv_file);

   // std::cout<<"recalc correlations ..."<<std::endl;
    recalculateCorrelations();

   // std::cout<<"calc binned rates ..."<<std::endl;
    calcBinnedRate();
    //std::cout<<"DONE !!!"<<std::endl;
    emit rawDataChanged();
    return true;
}

QString QFRDRFCSData::getCorrelationRunName(int run) {
    //if (run<0) return tr("average");
    if (run<correlationRuns) return tr("run %1").arg(run);
    return QString("");
}





