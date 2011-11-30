/*! \file    labeledit.h
    \author  Mamontov

    Describes a specific editor for labels
 */
#ifndef LABELEDIT_H
#define LABELEDIT_H

#include "diagramobject.h"
#include "diagramscene.h"
#include <QPlainTextEdit>
#include <time.h>

/*! Ctrl pressed time
 */
#define LE_CTRL_PRESSED 0.5
/*! \class ObjectTextEditor
    Time for label editing
 */
class ObjectTextEditor : public QPlainTextEdit
{
Q_OBJECT

private:
        /*! Diagram scene
         */
        DiagramScene * m_scene;
        /*! Box item pointer
         */
        DiagramObject * m_object;
public:
        /*! Constructs a label edit
            \param[in] scene  scene, where it's placed
            \param[in] object object data
         */
        ObjectTextEditor(DiagramScene * scene, DiagramObject * object);
        /*! Describes a key pressing event, that registers a control pressing
            \param[in] e event
         */
        void keyPressEvent(QKeyEvent *e);
signals:

public slots:

};

#endif // LABELEDIT_H
