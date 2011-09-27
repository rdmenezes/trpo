#ifndef UTOOLBARVIEW_H
#define UTOOLBARVIEW_H

#include <QWidget>
#include <QPainter>


class UToolbarView : public QWidget {
    Q_OBJECT

private:
    void setItems();

public:
    explicit UToolbarView(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

public:
    QWidget *superView;

signals:

public slots:

};

#endif // UTOOLBARVIEW_H
