/*! \file compare.h
    \author Mamontov

    Declares a simple comparator
 */
#ifndef COMPARE_H
#define COMPARE_H

#include <QtGlobal>
#include <math.h>
#define PRECISION 0.0001
/*! We don't use qFuzzyCompare, due to it's must check strictness and have
    some issues with comparing zero values
 */
inline bool fuzzyCompare(qreal a,qreal b)
{
    return fabs(a-b)<PRECISION;
}

#endif // COMPARE_H
