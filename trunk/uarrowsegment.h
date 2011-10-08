#ifndef UARROWSEGMENT_H
#define UARROWSEGMENT_H

#include <QWidget>


class MainWindow;

class UArrowSegment : public QWidget {
    Q_OBJECT
public:
    explicit UArrowSegment(QWidget *parent = 0);

signals:

public slots:

protected:
    MainWindow *superView;

};

#endif // UARROWSEGMENT_H
