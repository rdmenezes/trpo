/*! \file   diagramscene.h
    \author Mamontov

    Declares a diagram scene, that provides working with scene
*/
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include "diagram.h"
#include "box.h"
#include "diagramobject.h"

#include "tool.h"

//Defines a default block space
#define BLOCK_SPACE_Y 3
//Defines a block space x
#define BLOCK_SPACE_X 3

enum TextEditState
{
   TES_NONE,
   TES_EDIT
};

enum DragState
{
    DS_NONE,
    DS_BLOCK_RESIZE,
    DS_BLOCK_MOVE,
    DS_ALABEL_MOVE,
    DS_ARROW_MOVE,
    DS_ALINE_RESIZE
};

enum BlockCorner
{
    BC_UPPERLEFT,
    BC_UPPERRIGHT,
    BC_LOWERLEFT,
    BC_LOWERRIGHT
};

/*! Arrow Editing state
 */
enum ArrowEditState
{
    AES_NONE,
    AES_EDIT
};

enum AnnotationLineDrawingState
{
    ALDS_SPECIFIEDNONE,
    ALDS_SPECIFIEDFIRSTPOINT
};

//enum ArrowDirection;
/*! A class of panel for tool selection
 */
class ToolPanel;
/*! \class DiagramScene
    Declares a scene, containing the diagram data
*/
class DiagramScene : public QGraphicsScene
{
Q_OBJECT
private:
    Tool                 *   m_tool;               //!< Current tool
    Diagram              *   m_diag;               //!< Diagram data
    QGraphicsProxyWidget *   m_editor;             //!< A current text editor


    QGraphicsView *          m_view;               //!< View widget pointer for data
    QRectF                   m_default_block_size; //!< Default block size tool
    QRectF                   m_default_number_size; //!< Default number size for block
    TextEditState            m_edit_state;         //!< Text edit state
    QGraphicsProxyWidget *   m_label_editor_in_scene; //!< A label editor in scene
    DragState                m_dragstate;          //!< Dragging state work
    Box                  *   m_draggingblock;      //!< Dragged block
    BlockCorner              m_resizingblockcorner; //!< Resizing corner of block
    qreal                    m_blockmovingparams[2]; //!< Defines an x and y relevance to moving
    QRectF                   m_alabel_block_size;   //!< Size of annotation label
    FreeComment           *   m_moving_label;        //!< Moving annotation label
    ArrowEditState           m_arrow_state;         //!< Arrow editing state
    AnnotationLineDrawingState  m_alds;            //!< Annotation line drawing state
    QPointF                  m_aline_firstpoint;   //!< Annotation line first point
    //Arguments for resizing annotation line
    CommentLine           *    m_resizing_aline;     //!< Resizing annotation line

    void annnotationLabelMoveLeave(const QPointF & pos);
    /*! Enters into annotation line resizing state
        \param[in] pos position
        \param[in] item item, that is being analyzed
     */
    void leaveAnnotationLineResize(const QPointF & pos);
    /*! Enters into annotation line resizing state
        \param[in] pos position
        \param[in] item item, that is being analyzed
     */
    void enterAnnotationLineResize(const QPointF & pos, CommentLine * item);
    /*! Determines dragging box action
        \param[in] item item data
        \param[in] pos  position
     */
    void determineDraggingBoxAction(Box * item,const QPointF & pos);
    /*! Leaves arrow moving action
        \param[in] pos position
     */
    void arrowMoveLeave(const QPointF & pos);;
protected:
    /*! Handles a beginning of dragging item
        \param[in] event event data
     */
    void  blockResizeMoveEnter ( QGraphicsSceneMouseEvent * event );
    /*! Handles an ending for dragging item
        \param[in] event event data
     */
    void  blockResizeMoveLeave ( QGraphicsSceneMouseEvent * event );
public:
    /*! Declares diagram scene
        \param[in] d      diagram
        \param[in] parent not used
     */
    explicit DiagramScene(Diagram * d,QObject *parent = 0);
    /*! Returns a default binded view
        \return view
     */
    QGraphicsView * view() const { return views()[0]; }
    /*! Sets a diagram
        \param[in] d  diagram
     */
    inline void setDiagram(Diagram * d) { m_diag=d; }
    /*! Enables editor mode (adds ObjectTextEditor at specified rectangle)
        \param[in] on whether it's on
        \param[in] r  rectangle
        \param[in] e  event, that is provided to editor
        \param[in] d  diagram object
     */
    void toggleEditMode(bool on, const QRectF & r=QRectF(),
                                 QKeyEvent * e=NULL,
                                 DiagramObject * d=NULL);
    /*! Event, that occures when mouse is being pressed
        \param[in] ev event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /*! Event, that occures when mouse is being pressed
        \param[in] ev event
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    /*! Event, that occures when mouse is being released
        \param[in] ev event
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /*! Event, that occures when key is being pressed
        \param[in] ev event
    */
    void keyPressEvent(QKeyEvent * event);
    /*! Returns a diagram
     */
    inline Diagram * diagram() { return m_diag; }
    /*! Tries to save a diagram to file
        \param[in] filename name of file
        \return true on success
     */
    bool save(const QString & filename);
    /*! Tries to save a diagram to file
        \param[in] filename name of file
        \return true on success
     */
    bool load(const QString & filename);
    /*! Hides UI (Panel and text edit, if needed)
     */
    void hideUI();
    /*! Clears a scene, removing all objects, and setting state variables to beginning
     */
    void clear();

    /*! Sets a new tool for data
     */
    inline void setTool(Tool * tool)
    {
      if (m_tool)
            m_tool->clearState();
      m_tool=tool;
      m_tool->setDiagramData(this,m_diag);
      if (m_tool)
            m_tool->initState();
    }
signals:

public slots:

};

#endif // DIAGRAMSCENE_H
