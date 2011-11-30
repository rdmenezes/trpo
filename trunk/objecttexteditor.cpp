#include "objecttexteditor.h"
#include "diagramscene.h"
#include "box.h"
#include "freecomment.h"

ObjectTextEditor::ObjectTextEditor(DiagramScene * scene, DiagramObject * object ) :
    QPlainTextEdit(NULL)
{
    setAttribute(Qt::WA_TranslucentBackground);
    m_scene=scene;
    m_object=object;
}


void ObjectTextEditor::keyPressEvent(QKeyEvent *e)
{
   if (e->key()==Qt::Key_Return &&
       (e->modifiers() & Qt::ControlModifier) !=0
      )
   {
     m_object->setText(this->toPlainText());
     m_scene->toggleEditMode(false);
   }
   else
   {
     this->QPlainTextEdit::keyPressEvent(e);
   }
}
