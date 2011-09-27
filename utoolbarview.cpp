#include "utoolbarview.h"

UToolbarView::UToolbarView(QWidget *parent) :
    QWidget(parent) {

    superView = parent;

    this->setHidden(true);

    int itemsCount = 3;
    int height = 128;
    int width = itemsCount * height;

    QSize superFrame = superView->frameSize();
    QRect frame = QRect((superFrame.width() - width) / 2,
                        (superFrame.height() - height) / 2,
                        width, height);

    this->setGeometry(frame);

    this->setItems();
}

void UToolbarView::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    QRect frame = QRect(0, 0, this->frameSize().width(), this->frameSize().height());

    painter.setPen(QColor("#d4d4d4"));
    painter.setBrush(QColor("#cfcfef"));
    painter.drawRect(frame);

    QWidget::paintEvent(event);
}

void UToolbarView::setItems() {

}
