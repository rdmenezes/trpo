#ifndef UTOOLBARVIEW_H
#define UTOOLBARVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>


typedef enum U_TOOL_ENUM {
    UToolHand = 1,
    UToolSelect,
    UToolBox,
    UToolArrow,
    UToolEraser
} UTool;

class UToolbarView : public QWidget {
    Q_OBJECT

private:
    void setItems();

private slots:
    void selectHand();
    void selectSelect();
    void selectBox();
    void selectArrow();
    void selectEraser();

public:
    explicit UToolbarView(QWidget *parent = 0);

    UTool currentTool();
    void selectTool(UTool selectedTool);

protected:
    void paintEvent(QPaintEvent *event);

    QWidget *superView;

    UTool tool;

public:

signals:

};

#endif // UTOOLBARVIEW_H
