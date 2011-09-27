#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>

#include <QVector>

#include <QGraphicsOpacityEffect>

#include "utoolbarview.h"
#include "ubox.h"
#include "uarrow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

    void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;

    UToolbarView *toolbar;

    QVector<UBox *> *boxes;
    QVector<UArrow *> *arrows;

    void showHelp();
    void showStartupHint();
    void showToolbar();
};

#endif // MAINWINDOW_H
