/*! \file    toolpanel.h
    \author  Mamontov

    Describes a panel with tools, that responses for tools changing
*/
#ifndef TOOLPANEL_H
#define TOOLPANEL_H

#include <QWidget>
#include "tooltype.h"

class DiagramScene; //!< Class of referenced scene
//Width of panel
#define PANEL_WIDTH 335
//Height of panel
#define PANEL_HEIGHT 60
//Border between size of panel and it's real rectangle
#define PANEL_BORDER 2
//Panel round radius
#define PANEL_ROUND_RADIUS 3
//Panel icon offset
#define PANEL_Y_OFFSET 4
//First panel offset
#define PANEL_FIRST_X_OFFSET 4
//Panel offset created by each icon
#define PANEL_X_OFFSET 55
//Panel icon size
#define PANEL_ICON_SIZE 50



/*! \class ToolPanel
    Describes a tool panel, that responses for tools changing
*/
class ToolPanel : public QWidget
{
Q_OBJECT
private:
    DiagramScene *     m_scene;  //!< Tool reference, that will be changed
    /*! Draws an icon by index
        \param[in] p painter
        \param[in] i index of picture
    */
    void drawIconByIndex(QPainter * p,int i);
public:
    /*! Creates a tool panel with specified geometry. Also needs a pointer
        to changing variable
        \param[in] parent parent widget (QGraphicsView)
     */
    explicit ToolPanel(QWidget *parent = 0);
protected:
    /*! Event that provides a repainting of  panel
        \param[in] event   parameters
     */
    void paintEvent(QPaintEvent * event);
    /*! Event, that occures when mouse is being pressed
        \param[in] ev event
     */
    void mousePressEvent(QMouseEvent *event);
signals:

public slots:

};

#endif // TOOLPANEL_H
