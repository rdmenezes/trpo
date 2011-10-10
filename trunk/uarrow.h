#ifndef UARROW_H
#define UARROW_H

#include <QObject>
#include <QVector>

class UArrowSegment;
class MainWindow;

class UArrow : public QObject {
    Q_OBJECT
public:
//    explicit UArrow(QObject *parent = 0);
    explicit UArrow(QWidget *parent, QWidget *slot);
    
    MainWindow *superView;

signals:

public slots:

protected:
    QVector<UArrowSegment *> *segments;

};

#endif // UARROW_H
