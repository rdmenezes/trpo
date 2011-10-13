#include "mainwindow.h"
#include "helpwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->setWindowTitle("IDEF0 Diagram Builder");



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
    bool handled=false;
    if (event->key() == Qt::Key_F1) {
        showHelp();
        handled=true;
    }
    if (!handled)
        this->QMainWindow::keyPressEvent(event);
}



void MainWindow::showHelp() {
   HelpWindow d(this);
   d.exec();
}


