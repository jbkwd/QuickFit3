#include "qffcsfitevaluation.h"
#include "qffcsfitevaluationeditor.h"
#include "../fcs/qfrdrfcsdatainterface.h"



QFFCSFitEvaluation::QFFCSFitEvaluation(QFProject* parent):
    QFEvaluationItem(parent, true, false)
{
    //ctor
}

QFFCSFitEvaluation::~QFFCSFitEvaluation()
{
    //dtor
}


void QFFCSFitEvaluation::intWriteXML(QXmlStreamWriter& w) {
}

void QFFCSFitEvaluation::intReadData(QDomElement* e) {
}


QFEvaluationEditor* QFFCSFitEvaluation::createEditor(int i, QWidget* parent) {
    return new QFFCSFitEvaluationEditor(parent);
};

bool QFFCSFitEvaluation::isApplicable(QFRawDataRecord* record) {
    return record->inherits("QFRDRFCSDataInterface");
}

