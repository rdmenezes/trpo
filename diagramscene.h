#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>

class DiagramScene : public QGraphicsScene
{
Q_OBJECT
private:
    QGraphicsView *   m_view; //!< View for data
public:
    explicit DiagramScene(QObject *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);

    inline void setView(QGraphicsView * v) { m_view=v; }
signals:

public slots:

};

#endif // DIAGRAMSCENE_H
