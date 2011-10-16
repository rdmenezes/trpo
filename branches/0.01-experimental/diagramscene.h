/*! \file   diagramscene.h
    \author Mamontov

    Declares a diagram scene, that provides working with scene
*/
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "tooltype.h"
#include "diagram.h"
#include "boxitem.h"
//Defines a default block space
#define BLOCK_SPACE_Y 3
//Defines a block space x
#define BLOCK_SPACE_X 3
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
    QGraphicsView *          m_view;               //!< View widget pointer for data
    ToolType                 m_tooltype;           //!< Selected type of tool
    ToolPanel            *   m_panel;              //!< A pointer to panel
    QGraphicsProxyWidget *   m_panel_in_scene;     //!< A pointer to panel
    Diagram              *   m_diag;               //!< Diagram data
    QRectF                   m_default_block_size; //!< Default block size tool
    QRectF                   m_default_number_size; //!< Default number size for block
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
    /*! Returns a defulat block numer size
     */
    const QRectF & getDefaultBlockNumberSize() const;
    /*! Returns a diagram
     */
    Diagram * diagram() { return m_diag; }
    /*! Adds a block at specified place
        \param[in] event event data
     */
    void addBlock(QGraphicsSceneMouseEvent *event);
    /*! Decrements block id
        \param[in] block block
     */
    void decrementBlockID(BoxItem * block);
    /*! Increments block id
        \param[in] block block
     */
    void incrementBlockID(BoxItem * block);
signals:

public slots:

};

#endif // DIAGRAMSCENE_H
