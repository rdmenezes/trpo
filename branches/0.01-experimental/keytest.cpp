#include "keytest.h"

bool isTextEditKey(QKeyEvent * event)
{
    bool isup=event->text()[0]>='A' &&  event->text()[0]<='Z';
    bool islow=event->text()[0]>='a' &&  event->text()[0]<='z';
    bool isrusup=event->text()[0]>='À' && event->text()[0]<='ß';
    bool isruslow=event->text()[0]>='à' && event->text()[0]<='ÿ';
    bool isnumeric=event->text()[0]>='0' && event->text()[0]<='9';
    bool isbackspace=event->key()==Qt::Key_Backspace;
    bool isdeleteorreturn=event->key()==Qt::Key_Delete || event->key()==Qt::Key_Return
                                                       || event->key()==Qt::Key_Space;
    return isup || islow || isrusup || isruslow || isnumeric || isbackspace
                                                             || isdeleteorreturn;

}
