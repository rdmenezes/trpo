/*! \file    labeledit.h
    \author  Mamontov

    Describes a specific editor for labels
 */
#ifndef LABELEDIT_H
#define LABELEDIT_H

#include <QPlainTextEdit>
#include <time.h>
/*! A diagram scene aggregation
 */
class DiagramScene;
/*! An annotation label item
 */
class ALabelItem;
/*! Box item
 */
class Box;
/*! Ctrl pressed time
 */
#define LE_CTRL_PRESSED 0.5
/*! \class LabelEdit
    Time for label editing
 */
class LabelEdit : public QPlainTextEdit
{
Q_OBJECT

private:
        /*! Diagram scene
         */
        DiagramScene * m_scene;
        /*! Box item pointer
         */
        ::Box      * m_boxitem;
        /*! Annotation label pointer
         */
        ALabelItem   * m_alabelitem;
public:
        /*! Constructs a label edit
            \param[in] scene scene, where it's placed
            \param[in] item  item data
            \param[in] parent NULL
         */
        LabelEdit(DiagramScene * scene, ::Box * item,QWidget *parent = 0);
        /*! Constructs a label edit
            \param[in] scene scene, where it's placed
            \param[in] item  item data
            \param[in] parent NULL
         */
        LabelEdit(DiagramScene * scene, ALabelItem * item,QWidget *parent = 0);
        /*! Describes a key pressing event, that registers a control pressing
            \param[in] e event
         */
        void keyPressEvent(QKeyEvent *e);
signals:

public slots:

};

#endif // LABELEDIT_H
