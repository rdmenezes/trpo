#ifndef UBOX_H
#define UBOX_H

#include <QWidget>
#include <QPainter>
#include <QLabel>

class UBox : public QWidget {
    Q_OBJECT



public:
    explicit UBox(QWidget *parent, QPoint where);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent (QMouseEvent *event);

    QLabel *label;
    QWidget *superView;

signals:

public slots:

};

#endif // UBOX_H
