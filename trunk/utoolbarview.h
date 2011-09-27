#ifndef UTOOLBARVIEW_H
#define UTOOLBARVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>


typedef enum U_TOOL_ENUM {
    UToolHand = 1,
    UToolSelect,
    UToolBox,
    UToolArrow
} UTool;

class UToolbarView : public QWidget {
    Q_OBJECT

private:
    void setItems();

public:
    explicit UToolbarView(QWidget *parent = 0);

    UTool currentTool();

protected:
    void paintEvent(QPaintEvent *event);

    QWidget *superView;

    UTool tool;

public:

signals:

public slots:
    void selectTool();
};

#endif // UTOOLBARVIEW_H
