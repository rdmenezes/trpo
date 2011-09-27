#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPropertyAnimation>

#include "utoolbarview.h"

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
private:
    Ui::MainWindow *ui;

    UToolbarView *toolbar;

    void showHelp();
    void showStartupHint();
    void showToolbar();
};

#endif // MAINWINDOW_H
