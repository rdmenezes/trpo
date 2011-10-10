#include "ubox.h"
#include "mainwindow.h"


UBox::UBox(QWidget *parent, QPoint where) :
    QWidget(parent) {

    this->superView = (MainWindow *)parent;

    QPoint newPos = QPoint(where.x() - 90, where.y() - 50);
    this->setGeometry(newPos.x() - newPos.x() % superView->cellLength(),
                      newPos.y() - newPos.y() % superView->cellLength(),
                      180, 100);
    
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

    painter.setBrush(QColor(255, 255, 255, 222));
    painter.drawRect(frame);

    QWidget::paintEvent(event);
}


void UBox::mousePressEvent(QMouseEvent *event) {
    isPressed = true;
    dragStart = event->pos();
    this->raise();
}

void UBox::mouseMoveEvent(QMouseEvent *event) {

    if (isPressed) {
        QPoint newPos = this->geometry().topLeft() + event->pos() - dragStart;
        
        this->move(newPos.x() - newPos.x() % superView->cellLength(),
                   newPos.y() - newPos.y() % superView->cellLength());
    }
}

void UBox::mouseReleaseEvent(QMouseEvent *event) {

    event = 0x0;
    isPressed = false;

    this->superView->elementWasClicked(this);
}

void UBox::mouseDoubleClickEvent(QMouseEvent *event) {

    event = 0x0;
    editText();
}

void UBox::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Escape) {
        commitChanges();
    }
}

void UBox::removeBox() {
    this->destroy(true, true);
}

void UBox::editText() {
    this->label->setHidden(true);

    text = new QTextEdit(label->text(), this);
    text->setGeometry(label->geometry());
    text->setHidden(false);
    text->selectAll();
    text->setFocus();
}

void UBox::commitChanges() {
    label->setText(text->toPlainText());
    text->setHidden(true);
    delete text;

    label->setHidden(false);
}
