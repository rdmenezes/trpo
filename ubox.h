#ifndef UBOX_H
#define UBOX_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QTextEdit>

class MainWindow;

class UBox : public QWidget {
    Q_OBJECT



public:
    explicit UBox(QWidget *parent, QPoint where);

    void removeBox();

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent (QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void editText();
    void commitChanges();

    QLabel *label;
    MainWindow *superView;

    QTextEdit *text;

    bool isPressed;
    QPoint dragStart;

signals:

public slots:

};

#endif // UBOX_H
