#include "ubox.h"

UBox::UBox(QWidget *parent, QPoint where) :
    QWidget(parent) {

    this->superView = parent;

    this->setGeometry(where.x(), where.y(), 170, 100);

    this->label = new QLabel(QString("label"), this);

    this->label->setAlignment(Qt::AlignHCenter);
    this->label->setGeometry(0, 4, this->size().width(), this->size().height() - 4);



    this->setHidden(false);
}


void UBox::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    QRect frame = QRect(0, 0, this->frameSize().width(), this->frameSize().height());

    painter.setPen(QColor("#000000"));
    painter.setBrush(QColor("#af9f4f"));
    painter.drawRect(frame);

    QWidget::paintEvent(event);
}

void UBox::mouseDoubleClickEvent (QMouseEvent *event) {
    //
    event = 0x0;
    this->destroy(true, true);
}
