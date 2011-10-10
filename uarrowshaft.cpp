#include "uarrowshaft.h"

#include "mainwindow.h"

UArrowShaft::UArrowShaft(QWidget *parent, QWidget *slot) : UArrowSegment(parent, slot) {
    
    orientation = 1;
    
    this->setGeometry(QRect(slot->geometry().center(), QSize(superView->cellLength(), superView->cellLength())));
    
    this->setHidden(false);
}


void UArrowShaft::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    //QRect frame = QRect(0, 0, this->frameSize().width(), this->frameSize().height());

    QPen pen(QColor(0, 0, 0, 0));
    pen.setWidth(3);
    painter.setPen(pen);

    painter.setBrush(QColor(255, 255, 255, 222));
    if (orientation) {
        painter.drawLine(this->geometry().left(),
                         this->geometry().center().y(),
                         this->geometry().right(),
                         this->geometry().center().y());
    }
    else {
        painter.drawLine(this->geometry().center().x(),
                         this->geometry().top(),
                         this->geometry().center().x(),
                         this->geometry().bottom());
    }

    QWidget::paintEvent(event);
}


void UArrowShaft::mousePressEvent(QMouseEvent *event) {
//    isPressed = true;
//    dragStart = event->pos();
//    this->raise();
}

void UArrowShaft::mouseMoveEvent(QMouseEvent *event) {

//    if (isPressed) {
//        QPoint newPos = this->geometry().topLeft() + event->pos() - dragStart;
        
//        this->move(newPos.x() - newPos.x() % superView->cellLength(),
//                   newPos.y() - newPos.y() % superView->cellLength());
//    }
}

void UArrowShaft::mouseReleaseEvent(QMouseEvent *event) {

//    event = 0x0;
//    isPressed = false;

//    this->superView->elementWasClicked(this);
}

void UArrowShaft::mouseDoubleClickEvent(QMouseEvent *event) {

//    event = 0x0;
//    editText();
}
