/*! \file   diagramscene.h
    \author Mamontov

    Declares a diagram scene, that provides working with scene
*/
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "tooltype.h"
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
    QGraphicsView *   m_view;      //!< View widget pointer for data
    ToolType          m_tooltype;  //!< Selected type of tool
    ToolPanel     *   m_panel;     //!< A pointer to panel
public:
    /*! Declares diagram scene
        \param[in] parent not used
     */
    explicit DiagramScene(QObject *parent = 0);
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
    /*! Sets a tool for working with diagram
        \param[in] t tool type
    */
    inline void setTool(ToolType t)  {m_tooltype=t; }
signals:

public slots:

};

#endif // DIAGRAMSCENE_H
