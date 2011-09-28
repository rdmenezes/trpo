#ifndef UBOX_H
#define UBOX_H

#include <QWidget>
#include <QPainter>
#include <QLabel>

class MainWindow;
class QTextEdit;

class UBox : public QWidget {
    Q_OBJECT



public:
    explicit UBox(QWidget *parent, QPoint where);

    void removeBox();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    QLabel *label;
    MainWindow *superView;

    QTextEdit *text;

signals:

public slots:

};

#endif // UBOX_H
