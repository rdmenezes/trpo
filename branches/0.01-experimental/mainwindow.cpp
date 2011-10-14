#include "mainwindow.h"
#include "helpwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include "diagramscene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup UI
    ui->setupUi(this);
    //Set window Title
    this->setWindowTitle("IDEF0 Diagram Builder");

    //Setup graphic interface
    DiagramScene * scene=new DiagramScene();
    QGraphicsView * view=new QGraphicsView(scene,this);
    view->setGeometry(0,0,
                      this->contentsRect().width(),
                      this->contentsRect().height()
                      -ui->mainToolBar->height()
                      -ui->menuBar->height()
                      );
    this->setCentralWidget(view);
    scene->setSceneRect(0,0,view->contentsRect().width()
    ,view->contentsRect().height()
    );
    scene->setView(view);
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
    //If Pressed F1 show help
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


