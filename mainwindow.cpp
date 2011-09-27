#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->mainToolBar->setHidden(true);
    this->setWindowTitle("IDEF0 Diagram Builder");

    toolbar = new UToolbarView(this);

    showStartupHint();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    printf("%d\n", event->key());
    if (event->key() == Qt::Key_F1) {
        showHelp();
    }
    else if (event->key() == 16777248) {
        // showToolbar();
    }
}

void MainWindow::showStartupHint() {

}

#include "helpwindow.h"
void MainWindow::showHelp() {
   HelpWindow * a=new HelpWindow();
   a->exec();
}

void MainWindow::showToolbar() {

}
