#include "keytest.h"

bool isTextEditKey(QKeyEvent * event)
{
    bool isalphanumeric=event->text()[0].isLetterOrNumber();
    bool iscommon=event->key()==Qt::Key_Delete || event->key()==Qt::Key_Return
                                               || event->key()==Qt::Key_Space;
    return isalphanumeric || iscommon;
}
