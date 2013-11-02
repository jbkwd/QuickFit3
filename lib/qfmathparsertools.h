#ifndef QFMATHPARSERTOOLS_H
#define QFMATHPARSERTOOLS_H

#include "lib_imexport.h"
#include "qftools.h"
#include <cmath>
#include <QTextIStream>
#include <QTextStream>

#include <QList>
#include <QMap>
#include <QList>
#include <QString>
#include <QPair>
#include <QStack>
#include "../extlibs/MersenneTwister.h"
#include <stdint.h>
#include <QDebug>

class QFMathParser; // forward



/**
 * \defgroup qfmpultil utilities for QFMathParser function parser class
 * \ingroup qf3lib_mathtools_parser
 */
/*@{*/

#define QFMATHPARSER_VERSION "0.3"


/** possible result types
 *    - \c qfmpDouble: a floating-point number with double precision. This is
 *                     also used to deal with integers
 *    - \c qfmpString: a string of characters
 *    - \c qfmpBool:   a boolean value true|false
 *  .
 */
enum qfmpResultType {qfmpDouble=0x01,  /*!< \brief a floating-point number with double precision. This is also used to deal with integers */
                     qfmpString=0x02,  /*!< \brief a string of characters */
                     qfmpBool=0x04,   /*!< \brief a boolean value true|false */
                     qfmpDoubleVector=0x08,  /*!< \brief a vector of floating point numbers */
                     qfmpStringVector=0x10, /*!< \brief a vector of strings */
                     qfmpBoolVector=0x20, /*!< \brief a vector of booleans */
                     qfmpVoid=0x40  /*!< \brief a vector of floating point numbers */
                     };

/** \brief result of any expression  */
struct QFLIB_EXPORT qfmpResult {
    public:
        qfmpResult();
        qfmpResult(double value);
        qfmpResult(int value);
        qfmpResult(QString value);
        qfmpResult(bool value);
        qfmpResult(const QVector<double> &value);
        qfmpResult(const QVector<bool> &value);
        qfmpResult(const QStringList &value);

        qfmpResult(const qfmpResult &value);
        //qfmpResult(qfmpResult &value);
        //explicit qfmpResult(qfmpResult value);
        qfmpResult& operator=(const qfmpResult &value);

        QFLIB_EXPORT void setInvalid();
        QFLIB_EXPORT void setVoid();
        /** \brief convert the value this struct representens into a QString */
        QFLIB_EXPORT QString toString(int precision=10) const;

        /** \brief convert the value this struct representens into a QString and adds the name of the datatype in \c [...] */
        QFLIB_EXPORT QString toTypeString(int precision=10) const;

        /** \brief convert the value this struct to an integer */
        QFLIB_EXPORT int32_t toInteger() const;
        /** \brief convert the value this struct to an integer */
        QFLIB_EXPORT uint32_t toUInt() const;
        /** \brief is this result convertible to integer? */
        QFLIB_EXPORT bool isInteger() const;
        /** \brief is this result convertible to unsigned integer? */
        QFLIB_EXPORT bool isUInt() const;
        /** \brief returns the size of the result (number of characters for string, numbers of entries in vectors, 0 for void and 1 else) */
        QFLIB_EXPORT int length() const;

        QFLIB_EXPORT void setDouble(double val);
        QFLIB_EXPORT void setBoolean(bool val);
        QFLIB_EXPORT void setString(const QString& val);
        QFLIB_EXPORT void setDoubleVec(const QVector<double>& val);
        QFLIB_EXPORT void setDoubleVec(int size=0, double defaultVal=0);
        QFLIB_EXPORT void setBoolVec(const QVector<bool>& val);
        QFLIB_EXPORT void setBoolVec(int size=0, bool defaultVal=false);
        QFLIB_EXPORT void setStringVec(const QStringList& val);
        QFLIB_EXPORT void setStringVec(int size=0, const QString& defaultVal=QString(""));
        /** \brief converst the result to a vector of number (numbers and number vectors are converted!) */
        QFLIB_EXPORT QVector<double> asVector() const;
        QFLIB_EXPORT QVariantList asVariantList() const;
        /** \brief converst the result to a vector of number (numbers and number vectors are converted!) */
        QFLIB_EXPORT QStringList asStrVector() const;
        /** \brief converst the result to a vector of number (numbers and number vectors are converted!) */
        QFLIB_EXPORT QVector<bool> asBoolVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of number */
        QFLIB_EXPORT bool  convertsToVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of number */
        QFLIB_EXPORT bool  convertsToBoolVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of number */
        QFLIB_EXPORT bool  convertsToStringVector() const;
        /** \brief converst the result to a vector of integers (numbers and number vectors are converted!) */
        QFLIB_EXPORT QVector<int> asIntVector() const;
        /** \brief returns \c true, if the result may be converted to a vector of integers */
        QFLIB_EXPORT bool  convertsToIntVector() const;
        /** \brief returns \c true if the result is valid and not void */
        QFLIB_EXPORT bool isUsableResult() const;
        /** \brief converst the result to a number (numbers are converted!) */
        QFLIB_EXPORT double asNumber() const;
        /** \brief converst the result to a number (numbers are converted and from a number vector the first element is returned!) */
        QFLIB_EXPORT double asNumberAlsoVector() const;
        /** \brief converst the result to a number (numbers are converted and from a number vector the first element is returned!) */
        QFLIB_EXPORT QString asStringAlsoVector() const;
        /** \brief converst the result to a number (numbers are converted and from a number vector the first element is returned!) */
        QFLIB_EXPORT bool asBooleanAlsoVector() const;
        /** \brief converst the result to a string (strings are converted!) */
        QFLIB_EXPORT QString asString() const;
        /** \brief converst the result to a boolean (numbers and booleans are converted!) */
        QFLIB_EXPORT bool asBool() const;
        /** \brief returns the type */
        QFLIB_EXPORT qfmpResultType getType() const;
        /** \brief returns a string, describing the type of this result */
        QFLIB_EXPORT QString typeName() const;

        /** \brief returns an invalid result */
        QFLIB_EXPORT static qfmpResult invalidResult();
        /** \brief returns an void result */
        QFLIB_EXPORT static qfmpResult voidResult();

        QFLIB_EXPORT static qfmpResult add(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult sub(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult mul(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult div(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult mod(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult power(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult bitwiseand(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult bitwiseor(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult logicand(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult logicor(const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult logicnot(const qfmpResult& l, QFMathParser *p);
        QFLIB_EXPORT static qfmpResult neg(const qfmpResult& l, QFMathParser* p);
        QFLIB_EXPORT static qfmpResult bitwisenot(const qfmpResult& l, QFMathParser* p);

        QFLIB_EXPORT static void add(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void sub(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void mul(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void div(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void mod(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void power(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void bitwiseand(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void bitwiseor(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void logicand(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void logicor(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void logicnot(qfmpResult& result, const qfmpResult& l, QFMathParser *p);
        QFLIB_EXPORT static void logicnand(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void logicnor(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void logicxor(qfmpResult& result, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void neg(qfmpResult& result, const qfmpResult& l, QFMathParser* p);
        QFLIB_EXPORT static void bitwisenot(qfmpResult& result, const qfmpResult& l, QFMathParser* p);

        QFLIB_EXPORT static void la_add(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        /*QFLIB_EXPORT static void la_sub(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_mul(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_div(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_mod(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_power(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_bitwiseand(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_bitwiseor(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_logicand(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_logicor(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_logicnot(qfmpResult& l, QFMathParser *p);
        QFLIB_EXPORT static void la_logicnand(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_logicnor(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_logicxor(qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void la_neg(qfmpResult& l, QFMathParser* p);
        QFLIB_EXPORT static void la_bitwisenot(qfmpResult& l, QFMathParser* p);*/

        QFLIB_EXPORT bool operator==(const qfmpResult& other) const;
        QFLIB_EXPORT bool operator!=(const qfmpResult& other) const;

        QFLIB_EXPORT static void compareequal(qfmpResult& res, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void comparenotequal(qfmpResult& res, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void comparegreater(qfmpResult& res, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void comparegreaterequal(qfmpResult& res, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void comparesmaller(qfmpResult& res, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);
        QFLIB_EXPORT static void comparesmallerequal(qfmpResult& res, const qfmpResult& l, const qfmpResult& r, QFMathParser* p);

        bool isValid;
        qfmpResultType type;   /*!< \brief type of the result */
        QString str;       /*!< \brief contains result if \c type==qfmpString */
        double num;            /*!< \brief contains result if \c type==qfmpDouble */
        bool boolean;          /*!< \brief contains result if \c type==qfmpBool */
        QVector<double> numVec; /*!< \brief contains result if \c type==qfmpDoubleVector */
        QStringList strVec;
        QVector<bool> boolVec;

};












/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is numeric->numeric, e.g. sin(x). The result is numeric->numeric or num_vector->num_vector

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return ;\
    }\
    if (params[0].type==qfmpDouble) {\
        r.setDouble(CFUNC(params[0].num));\
    } else if(params[0].type==qfmpDoubleVector) {\
        r.setDoubleVec(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i]);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
}

/** \brief same as QFMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(FName, CFUNC, CFUNC)



/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is numeric_vector->numeric_vector (QVector<double> -> QVector<double>)

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to any number parameter
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_NUMERICVEC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector) {\
        r.setDoubleVec(CFUNC(params[0].numVec));\
    } else {\
        p->qfmpError(QObject::tr("%1(x) argument has to be vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for QFMathParser from a C template-function that
           is vector->vector it has to accept QList and QVector as arguments! The result of the parser
           function will have the same type as the input!

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to any number parameter
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_VECTOR_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector) {\
        r.setDoubleVec(CFUNC(params[0].numVec));\
    } else if(params[0].type==qfmpStringVector) {\
        r.setStringVec(CFUNC(params[0].strVec));\
    } else if(params[0].type==qfmpBoolVector) {\
        r.setBoolVec(CFUNC(params[0].boolVec));\
    } else {\
        p->qfmpError(QObject::tr("%1(x) argument has to be a vector of numbers/booleans/strings").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is numeric_vector->numeric (QVector<double> -> double), e.g. qfstatisticsMedian()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(x) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].numVec));\
    } else {\
        p->qfmpError(QObject::tr("%1(x) argument has to be a vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric_vector, numeric)->number , e.g. qfstatisticsQuantile()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM1VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec, params[1].num));\
    } else {\
        p->qfmpError(QObject::tr("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric_vector, numeric)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM1VEC2POSVEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec, params[1].num));\
    } else if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDoubleVector) {\
        QVector<double> res=params[1].numVec;\
        for (int i=0; i<params[1].numvec.size(); i++) {\
            res[i]=CFUNC(params[0].numVec, res[i]);\
        }\
        r.setDoubleVec(res);\
    } else {\
        p->qfmpError(QObject::tr("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric_vector, numeric)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!
           The C-function expects (double* data, int length) and not a QVector<double> as parameter!)

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM1VEC2POSVEC_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec, params[1].num));\
    } else if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDoubleVector) {\
        QVector<double> res=params[1].numVec;\
        for (int i=0; i<params[1].numvec.size(); i++) {\
            res[i]=CFUNC(params[0].numVec.data(), params[0].numVec.size(), res[i]);\
        }\
    r.setDoubleVec(res);\
    } else {\
        p->qfmpError(QObject::tr("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric_vector, numeric)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM1NUM2VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDouble && params[1].type==qfmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].num, params[1].numVec));\
    } else if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDoubleVector) {\
        QVector<double> res=params[0].numVec;\
        for (int i=0; i<res.size(); i++) {\
            res[i]=CFUNC(res[i], params[1].numVec);\
        }\
        r.setDoubleVec(res);\
    } else {\
        p->qfmpError(QObject::tr("%1(x, p) argument x has to be a number or a vector of numbers and p a vector of number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric_vector)->number. If the second argument is also a vector, the function
           will be applied element-wise to this vector to build an output vector!
           The C-function expects (double, double* data, int length) and not a QVector<double> as parameter!)

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM1NUM2VEC_VECTONUM_CFUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(x, p) needs exacptly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDouble && params[1].type==qfmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].num, params[1].numVec.data(), params[1].numVec.size()));\
    } else if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDoubleVector) {\
        QVector<double> res=params[0].numVec;\
        for (int i=0; i<res.size(); i++) {\
            res[i]=CFUNC(res[i], params[1].numVec.data(), params[1].numVec.size());\
        }\
        r.setDoubleVec(res);\
    } else {\
        p->qfmpError(QObject::tr("%1(x, p) argument x has to be a number or a vector of numbers and p a vector of number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric_vector, numeric, numeric)->number

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_3PARAM1VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=3) {\
        p->qfmpError(QObject::tr("%1(x, p1, p2) needs exacptly 3 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDouble && params[2].type==qfmpDouble) {\
        r.setDouble(CFUNC(params[0].numVec, params[1].num, params[2].num));\
    } else {\
        p->qfmpError(QObject::tr("%1(x, p) argument x has to be a vector of numbers and p a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric_vector, numeric_vector)->numeric, e.g. qfstatisticsCorrelationCoefficient()

    The resulting function will:
      - check the number of arguments and their type
      - apply the C-function to the argument
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM2VEC_VECTONUM_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(x, y) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDoubleVector) {\
        r.setDouble(CFUNC(params[0].asVector(), params[1].asVector()));\
    } else {\
        p->qfmpError(QObject::tr("%1(x, y) arguments x and y have to be a vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}


/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is string->string.  The result is string->string or str_vector->str_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_STRING_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.type=qfmpString;\
    r.isValid=true;\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpString) {\
        r.str=CFUNC(params[0].str);\
    } else if (params[0].type==qfmpStringVector) {\
        r.setStringVec(params[0].strVec.size());\
        for (int i=0; i<params[0].strVec.size(); i++) {\
            r.strVec[i]=CFUNC(params[0].strVec[i]);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) argument has to be a string or string-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as QFMATHPARSER_DEFINE_1PARAM_STRING_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_1PARAM_STRING_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_1PARAM_STRING_FUNC(FName, CFUNC, CFUNC)


/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is numeric->bool.  The result is numeric->bool or num_vector->bool_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.type=qfmpBool;\
    r.isValid=false;\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.setBoolean(CFUNC(params[0].num));\
    } else if (params[0].type==qfmpDoubleVector) {\
        r.setBoolVec(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.boolVec[i]=CFUNC(params[0].numVec[i]);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) argument has to be a number or number-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as QFMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, CFUNC, CFUNC)



/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is numeric->string.  The result is numeric->string or num_vector->str_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any string parameter
      - apply the C-function item-wise to any string vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.type=qfmpString;\
    r.isValid=true;\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.str=CFUNC(params[0].num);\
    } else if (params[0].type==qfmpDoubleVector) {\
        r.setStringVec(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.strVec[i]=CFUNC(params[0].numVec[i]);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) argument has to be a number or number-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as QFMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, CFUNC, CFUNC)


/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is bool->string.  The result is bool->string or bool_vector->str_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any bool parameter
      - apply the C-function item-wise to any bool vector parameter item
      - result is also string
    .

    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_1PARAM_BOOL2STRING_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.type=qfmpString;\
    r.isValid=true;\
    if (n!=1) {\
        p->qfmpError(QObject::tr("%1(...) needs exacptly 1 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpBool) {\
        r.str=CFUNC(params[0].boolean);\
    } else if (params[0].type==qfmpBoolVector) {\
        r.setStringVec(params[0].boolVec.size());\
        for (int i=0; i<params[0].boolVec.size(); i++) {\
            r.strVec[i]=CFUNC(params[0].boolVec[i]);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) argument has to be a boolean or boolean-vector").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as QFMATHPARSER_DEFINE_1PARAM_BOOL2STRING_FUNC_SIMPLE(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_1PARAM_BOOL2STRING_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(FName, CFUNC, CFUNC)



/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric_vector)->numeric.  The result is (numeric, numeric)->numeric or (numeric, num_vector)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.type=qfmpDouble;\
    r.isValid=true;\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    if (params[0].type==qfmpDouble) {\
        pa=params[0].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[1].type==qfmpDouble) {\
        r.num=CFUNC(pa, params[1].num);\
    } else if(params[1].type==qfmpDoubleVector) {\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[1].numVec.size());\
        for (int i=0; i<params[1].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(pa, params[1].numVec[i]);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) second argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric)->numeric, e.g. yn(x, p), p is a number, x may be a vector
             The result is (numeric, numeric)->numeric or (num_vector, numeric)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.isValid=true;\
    r.type=qfmpDouble;\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    if (params[1].type==qfmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.num=CFUNC(params[0].num, pa);\
    } else if(params[0].type==qfmpDoubleVector) {\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}








/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_3PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.isValid=true;\
    r.type=qfmpDouble;\
    if (n!=3) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 3 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    if (params[1].type==qfmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==qfmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb);\
    } else if(params[0].type==qfmpDoubleVector) {\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}




/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric, numeric)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_4PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.isValid=true;\
    r.type=qfmpDouble;\
    if (n!=4) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 4 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    double pc=0; \
    if (params[1].type==qfmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==qfmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[3].type==qfmpDouble) {\
        pc=params[3].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) fourth argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb, pc);\
    } else if(params[0].type==qfmpDoubleVector) {\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb, pc);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric)->numeric, e.g. yn(p, x), p is a number, x may be a vector
             The result is (numeric, numeric)->numeric or (num_vector, numeric)->num_vector.
           The c-function takes the vector-argument as LAST argument!

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAMLVEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.isValid=true;\
    r.type=qfmpDouble;\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    if (params[1].type==qfmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.num=CFUNC(params[0].num, pa);\
    } else if(params[0].type==qfmpDoubleVector) {\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric)->num_vector.
             The c-function takes the vector-argument as LAST argument!

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_3PARAMLVEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.isValid=true;\
    r.type=qfmpDouble;\
    if (n!=3) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 3 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    if (params[1].type==qfmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==qfmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb);\
    } else if(params[0].type==qfmpDoubleVector) {\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}




/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric, numeric, numeric, numeric)->numeric
             The result is (numeric, numeric, numeric, numeric)->numeric or (num_vector, numeric, numeric, numeric)->num_vector.
             The c-function takes the vector-argument as LAST argument!

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_4PARAMLVEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.isValid=true;\
    r.type=qfmpDouble;\
    if (n!=4) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 4 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    double pa=0; \
    double pb=0; \
    double pc=0; \
    if (params[1].type==qfmpDouble) {\
        pa=params[1].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) second argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[2].type==qfmpDouble) {\
        pb=params[2].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) third argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[3].type==qfmpDouble) {\
        pc=params[3].num;\
    } else {\
        p->qfmpError(QObject::tr("%1(...) fourth argument has to be a number").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble) {\
        r.num=CFUNC(params[0].num, pa, pb, pc);\
    } else if(params[0].type==qfmpDoubleVector) {\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], pa, pb, pc);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}



/*! \brief This macro allows to easily define functions for QFMathParser from a C-function that
           is (numeric,numeric)->numeric, e.g. fmod(x, y).  The result is (numeric, numeric)->numeric or (num_vector, num_vector)->num_vector.

    The resulting function will:
      - check the number of arguments
      - apply the C-function to any number parameter
      - apply the C-function item-wise to any number vector parameter, i.e. [ fmod(x1,y1), fmod(x2,y2), ... ]
      - result is also a number or number vector
    .
    \param FName name of the function to declare
    \param NAME_IN_PARSER name the function should have in the parser (used for error messages only)
    \param CFUNC name of the C function to call
*/
#define QFMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC) \
static inline void FName(qfmpResult& r, const qfmpResult* params, unsigned int  n, QFMathParser* p){\
    r.isValid=true;\
    r.type=qfmpDouble;\
    if (n!=2) {\
        p->qfmpError(QObject::tr("%1(...) needs exactly 2 argument").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
        return; \
    }\
    if (params[0].type==qfmpDouble && params[1].type==qfmpDouble) {\
    r.num=CFUNC(params[0].num, params[1].num);\
    } else if(params[0].type==qfmpDoubleVector && params[1].type==qfmpDoubleVector) {\
        if (params[0].numVec.size()!=params[1].numVec.size()) {\
            p->qfmpError(QObject::tr("%1(x,y) both arguments have to have same length").arg(#NAME_IN_PARSER));\
            r.setInvalid();\
            return; \
        }\
        r.type=qfmpDoubleVector;\
        r.numVec.resize(params[0].numVec.size());\
        for (int i=0; i<params[0].numVec.size(); i++) {\
            r.numVec[i]=CFUNC(params[0].numVec[i], params[1].numVec[i]);\
        }\
    } else {\
        p->qfmpError(QObject::tr("%1(...) first argument has to be a number or vector of numbers").arg(#NAME_IN_PARSER));\
        r.setInvalid();\
    }\
    return; \
}

/** \brief same as QFMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(FName, CFUNC, CFUNC)

/** \brief same as QFMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC(FName, CFUNC, CFUNC)

/** \brief same as QFMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC(FName, NAME_IN_PARSER, CFUNC), but NAME_IN_PARSER==CFUNC */
#define QFMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC_SIMPLE(FName, CFUNC) QFMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC(FName, CFUNC, CFUNC)


/*@}*/

#endif // QFMATHPARSERTOOLS_H
