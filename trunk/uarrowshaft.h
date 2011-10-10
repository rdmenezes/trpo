#ifndef UARROWSHAFT_H
#define UARROWSHAFT_H

//class UArrowSegment;
#include "uarrowsegment.h"

class UArrowShaft : public UArrowSegment {
public:
    UArrowShaft(QWidget *parent, QWidget *slot);

    bool orientation;

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent (QMouseEvent *event);
};

#endif // UARROWSHAFT_H
