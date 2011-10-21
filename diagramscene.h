/*! \file   diagramscene.h
    \author Mamontov

    Declares a diagram scene, that provides working with scene
*/
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneDragDropEvent>
#include "tooltype.h"
#include "diagram.h"
#include "boxitem.h"
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
    DS_ARROW_MOVE
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

enum ArrowDirection;
/*! A class of panel for tool selection
 */
class ToolPanel;
/*! Label editor, used for editing labels of blocks and annotation labels
 */
class LabelEdit;
/*! \class DiagramScene
    Declares a scene, containing the diagram data
*/
class DiagramScene : public QGraphicsScene
{
Q_OBJECT
private:
    QGraphicsView *          m_view;               //!< View widget pointer for data
    ToolType                 m_tooltype;           //!< Selected type of tool
    ToolPanel            *   m_panel;              //!< A pointer to panel
    QGraphicsProxyWidget *   m_panel_in_scene;     //!< A pointer to panel
    Diagram              *   m_diag;               //!< Diagram data
    QRectF                   m_default_block_size; //!< Default block size tool
    QRectF                   m_default_number_size; //!< Default number size for block
    TextEditState            m_edit_state;         //!< Text edit state
    LabelEdit            *   m_label_editor;       //!< A current label editor
    QGraphicsProxyWidget *   m_label_editor_in_scene; //!< A label editor in scene
    DragState                m_dragstate;          //!< Dragging state work
    BoxItem              *   m_draggingblock;      //!< Dragged block
    BlockCorner              m_resizingblockcorner; //!< Resizing corner of block
    qreal                    m_blockmovingparams[2]; //!< Defines an x and y relevance to moving
    QRectF                   m_alabel_block_size;   //!< Size of annotation label
    ALabelItem           *   m_moving_label;        //!< Moving annotation label
    ArrowEditState           m_arrow_state;         //!< Arrow editing state
    ArrowPoint           *   m_last_arrow_point;          //!< Last point added to scene
    ArrowSegment         *   m_moving_segment;      //!< Segment that can be moved
    /*! Process tool selection by keys
        \param[in] event event
        \return true, if handled
    */
    bool processKeyToolSelect(QKeyEvent * event);
    /*! Processes changing block id
        \param[in] block block, whose id is changing
        \param[in] previd previous block id
        \param[in] newid  new block id
    */
    void processChangeBlockID(BoxItem * block, char previd, char newid);
    /*! Processes removing of items
        \param[in] items clicked items
     */
    void processRemoving(const QList<QGraphicsItem *> & items);
    /*! Determines dragging box action
        \param[in] item item data
        \param[in] pos  position
     */
    void determineDraggingBoxAction(BoxItem * item,const QPointF & pos);
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
    void arrowMoveLeave(const QPointF & pos);
    /*! Processes arrow joining
     */
    void processArrowJoin(const QPointF & pos,ArrowSegment * seg);
    /*! Proecessed arrow merge
        \param[in] pos  last arrow position, that will move to
        \param[in] seg  segment
        \param[in] mydir direction of adding segment
        \param[in] segdir  direction of segment
     */
    void processArrowMerge(const QPointF & pos, ArrowSegment * seg, ArrowDirection * mydir, ArrowDirection  * segdir);
    /*! Processes arrow joining
        \param[in] p position
        \param[in] seg segment
     */
    void processArrowJoinSegments(const QPointF & pos, ArrowSegment * seg);
    /*! Adds arrow, pointing to block
        \param[in] p   new point
        \param[om] box block
     */
    void addArrowPointingToBlock(ArrowPoint * p,BoxItem * box);
    /*! Prolongs arrow to block edge
        \param[in] p    point
        \param[in] box  block
     */
    void prolongArrowToBlockEdge(ArrowPoint * p,BoxItem * box);
    /*! Processes arrow addition, pointing to block
        \param[in] p position
        \param[in] box  box
     */
    void processArrowPointingToBlock(ArrowPoint * p, BoxItem * box);
    /*! Process common arrow building
        \param[in] pos  pointf
     */
    void processArrowCommonBuilding(const QPointF & scenePos);
    /*! Process arrow operation while clicking on blank space
        \param[in] event event
     */
    void processArrowClickOnBlankSpace(QGraphicsSceneMouseEvent * event);
    /*! Process arrow operation while clicking on block space
        \param[in] event event
     */
    void processArrowClickOnBox(QGraphicsSceneMouseEvent * event,BoxItem * box);
    /*! Process arrow operation while clicking on block space
        \param[in] event event
     */
    void processArrowClickOnLine(QGraphicsSceneMouseEvent * event,ArrowSegment * seg);
    /*! Process escape pressing on blank space
        \param[in] pos position
     */
    void processArrowEscapePress(const QPointF & pos);
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
    /*! Event, that occures when mouse is being pressed
        \param[in] ev event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /*! Event, that occures when mouse is being pressed
        \param[in] ev event
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /*! Event, that occures when key is being pressed
        \param[in] ev event
    */
    void keyPressEvent(QKeyEvent * event);
    /*! Sets parent widget for that view
        \param[in] v parent widget
    */
    inline void setView(QGraphicsView * v) { m_view=v; }
    /*! Sets a tool for working with diagram.
        \note Makes panel disappear, so must be called at end of living of panel
        \param[in] t tool type
    */
    void setTool(ToolType t);
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
    /*! Returns a diagram
     */
    Diagram * diagram() { return m_diag; }
    /*! Adds a block at specified place
        \param[in] event event data
     */
    void addBlock(QGraphicsSceneMouseEvent *event);
    /*! Adds an annotation at specified place
        \param[in] event event data
     */
    void addAnnotationLabel(QGraphicsSceneMouseEvent *event);
    /*! Decrements block id
        \param[in] block block
     */
    void decrementBlockID(BoxItem * block);
    /*! Increments block id
        \param[in] block block
     */
    void incrementBlockID(BoxItem * block);
    /*! Gets a new edit state
        \param[in] edit state
     */
    inline TextEditState editState() const { return m_edit_state; }
    /*! Toggles editing state on
        \param[in] edit  editor
        \param[in] sed   proxy widget
     */
    inline void toggleEditStateOn(LabelEdit * edit, QGraphicsProxyWidget * sed)
    {
       this->m_edit_state=TES_EDIT;
       this->m_label_editor=edit;
       this->m_label_editor_in_scene=sed;
    }
    /*! Toggles editing state off
     */
    void toggleEditStateOff();
signals:

public slots:

};

#endif // DIAGRAMSCENE_H
