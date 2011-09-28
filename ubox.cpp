#include "ubox.h"
#include "mainwindow.h"

#include <QTextEdit>


UBox::UBox(QWidget *parent, QPoint where) :
    QWidget(parent) {

    this->superView = (MainWindow *)parent;

    this->setGeometry(where.x(), where.y(), 170, 100);

    this->label = new QLabel(QString("label"), this);

    this->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->label->setGeometry(0, 0, this->size().width(), this->size().height());

    this->text = 0x0;

    this->setHidden(false);
}

void UBox::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    QRect frame = QRect(0, 0, this->frameSize().width(), this->frameSize().height());

    QPen pen(QColor("#000000"));
    pen.setWidth(2);
    painter.setPen(pen);

    painter.setBrush(QColor("#ffffff"));
    painter.drawRect(frame);

    QWidget::paintEvent(event);
}

void UBox::mouseReleaseEvent(QMouseEvent *event) {

    event = 0x0;
    this->superView->elementDidClicked(this);
}

void UBox::mouseDoubleClickEvent (QMouseEvent *event) {

    event = 0x0;

    this->label->setHidden(true);

    text = new QTextEdit(label->text(), this);
    text->setGeometry(label->geometry());
    text->setHidden(false);
}

void UBox::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Escape) {
        label->setText(text->toPlainText());
        text->setHidden(true);
        delete text;
        //text = 0x0;

        label->setHidden(false);
    }
}

void UBox::removeBox() {
    this->destroy(true, true);
}
