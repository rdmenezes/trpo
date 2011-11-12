/*! \file    keytest.h
    \author  Mamontov

    Contains a key testing function
 */
#include <QKeyEvent>
#ifndef KEYTEST_H
#define KEYTEST_H

/*! Determines whether text edit button is pressed
    \param[in] event event parameters
 */
bool isTextEditKey(QKeyEvent * event);

#endif // KEYTEST_H
