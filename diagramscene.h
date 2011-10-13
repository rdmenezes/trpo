#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "tooltype.h"

class DiagramScene : public QGraphicsScene
{
Q_OBJECT
private:
    QGraphicsView *   m_view;      //!< View for data
    ToolType          m_tooltype;  //!< Selected type of tool
public:
    explicit DiagramScene(QObject *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);

    inline void setView(QGraphicsView * v) { m_view=v; }
    inline void setTool(ToolType t)  {m_tooltype=t; }
signals:

public slots:

};

#endif // DIAGRAMSCENE_H
