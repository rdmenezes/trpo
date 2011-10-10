#include "utoolbarview.h"

UToolbarView::UToolbarView(QWidget *parent) :
    QWidget(parent) {

    this->superView = parent;

    this->setHidden(true);

    int itemsCount = 5;
    int height = 128;
    int width = itemsCount * height;

    QSize superFrame = superView->frameSize();
    QRect frame = QRect((superFrame.width() - width) / 2,
                        (superFrame.height() - height) / 2,
                        width, height);

    this->setGeometry(frame);

    this->setItems();

    this->tool = UToolSelect;
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

    QPushButton *hand = new QPushButton(QString("Hand"), this);
    hand->setGeometry(4 + 128 * 0, 4, 120, 120);

    QPushButton *select = new QPushButton(QString("Select"), this);
    select->setGeometry(4 + 128 * 1, 4, 120, 120);

    QPushButton *box = new QPushButton(QString("Box"), this);
    box->setGeometry(4 + 128 * 2, 4, 120, 120);

    QPushButton *arrow = new QPushButton(QString("Arrow"), this);
    arrow->setGeometry(4 + 128 * 3, 4, 120, 120);

    QPushButton *eraser = new QPushButton(QString("Eraser"), this);
    eraser->setGeometry(4 + 128 * 4, 4, 120, 120);

    connect(hand, SIGNAL(clicked()), this, SLOT(selectHand()));
    connect(select, SIGNAL(clicked()), this, SLOT(selectSelect()));
    connect(box, SIGNAL(clicked()), this, SLOT(selectBox()));
    connect(arrow, SIGNAL(clicked()), this, SLOT(selectArrow()));
    connect(eraser, SIGNAL(clicked()), this, SLOT(selectEraser()));
}

UTool UToolbarView::currentTool() {
    return this->tool;
}

void UToolbarView::selectTool(UTool selectedTool) {
    this->tool = selectedTool;
    this->setHidden(true);
}

void UToolbarView::selectHand() {
    this->selectTool(UToolHand);
}

void UToolbarView::selectSelect() {
    this->selectTool(UToolSelect);
}

void UToolbarView::selectBox() {
    this->selectTool(UToolBox);
}

void UToolbarView::selectArrow() {
    this->selectTool(UToolArrow);
}

void UToolbarView::selectEraser() {
    this->selectTool(UToolEraser);
}
