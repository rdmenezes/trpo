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
#include "arrowsegment.h"
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

enum ArrowDirection;
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
    ArrowPoint           *   m_last_arrow_point;          //!< Last point added to scene
    ArrowSegment         *   m_moving_segment;      //!< Segment that can be moved
    //Arguments for adding annotation line
    ArrowSegment         *   m_aline_segment;      //!< Segment, where is binded annotation
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
    /*! Removes annotation line
        \param[in] line annotation line to be removed
     */
    void removeAnnotationLine(CommentLine * line);
    /*! Processes pressing annotation line to segment
        \param[in] pos position
        \param[in] seg segment
     */
    void processAnnotationLineToSegment(const QPointF & pos, ArrowSegment * seg);
    /*! Processes pressing annotation line to box
        \param[in] pos position
        \param[in] box  box item
     */
    void processAnnotationLineToBox(const QPointF & pos,Box * box);
    /*! Processes pressing annotation line on blank space (setting second point)
        \param[in] pos position
     */
    void processAnnotationLineSecondPointOnBlank(const QPointF & pos);
    /*! Processes pressing annotation line on blank space (setting first point)
        \param[in] pos position
     */
    void processAnnotationLinePointOnBlank(const QPointF & pos);
    /*! Processes pressing annotation line, when escape is pressed
        \param[in] pos position
     */
    void processAnnotationLineEscapePress(const QPointF & pos);
    /*! Processes changing block id
        \param[in] block block, whose id is changing
        \param[in] previd previous block id
        \param[in] newid  new block id
    */
    void processChangeBlockID(Box * block, char previd, char newid);
    /*! Determines dragging box action
        \param[in] item item data
        \param[in] pos  position
     */
    void determineDraggingBoxAction(Box * item,const QPointF & pos);
    /*! Removes arrow segment
        \param[in] seg segment
     */
    void removeArrowSegment(ArrowSegment * seg);
    /*! Enables arrow moving action
        \param[in] pos position
        \param[in] seg segment
     */
    void enterArrowMove(const QPointF & pos,ArrowSegment * seg);
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



    /*! Computes a default block sizes;
     */
    QRectF getDefaultBlockSize(const QPointF & pos);
    /*! Returns a default block numer size
     */
    const QRectF & getDefaultBlockNumberSize() const;
    /*! Returns a default annotation label size
        \param[in] pos  center position of label
     */
    QRectF getDefaultAnnotationLabelSize(const QPointF & pos);
    /*! Adds a block at specified place
        \param[in] event event data
     */
    void addBlock(QGraphicsSceneMouseEvent *event);
    /*! Adds an annotation at specified place
        \param[in] event event data
     */
    void addAnnotationLabel(QGraphicsSceneMouseEvent *event);
    /*! Gets a new edit state
        \param[in] edit state
     */
    inline TextEditState editState() const { return m_edit_state; }
    /*! Toggles editing state off
     */
    void toggleEditStateOff();

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
    /*! Exports a diagram to file
        \param[in] filename name of file
        \return true on success
     */
    bool exportTo(const QString & filename);
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
