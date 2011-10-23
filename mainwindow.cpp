#include "mainwindow.h"
#include "helpwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include "diagramscene.h"
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Creates a diaram set
    m_set=new DiagramSet();
    //Setup UI
    ui->setupUi(this);
    //Set window Title
    this->setWindowTitle("IDEF0 Diagram Editor");

    //Setup graphic interface
    DiagramScene * scene=new DiagramScene(m_set->get(0));
    QGraphicsView * view=new QGraphicsView(scene,this);
    view->setGeometry(0,0,
                      this->contentsRect().width(),
                      this->contentsRect().height()
                      -ui->menuBar->height()
                      );
    this->setCentralWidget(view);
    scene->setSceneRect(0,0,view->contentsRect().width()
    ,view->contentsRect().height()-ui->menuBar->height()
    );
    scene->setView(view);
    
    path = 0;
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_set;
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
    else if (event->key() == Qt::Key_O && event->modifiers() == Qt::ControlModifier) {
        this->open();
    }
    else if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier) {
        this->save();
    }
    else if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier | Qt::ShiftModifier) {
        this->saveAs();
    }
    else if (event->key() == Qt::Key_E && event->modifiers() == Qt::ControlModifier) {
        this->exportDiagram();
    }
    if (!handled)
        this->QMainWindow::keyPressEvent(event);
}


void MainWindow::showHelp() {
   HelpWindow d(this);
   d.exec();
}

void MainWindow::open() {
    
    QString temp = QFileDialog::getOpenFileName();
    if (!temp.isEmpty()) {
        *path = temp;
        
        QGraphicsView *view = (QGraphicsView *)this->centralWidget();
        DiagramScene *scene = (DiagramScene *)view->scene();
        
        if (!scene->load(*path)) {
            QMessageBox(QMessageBox::Warning, QString("Error"), QString("Can't open file."));
        }
    }
}

void MainWindow::save() {
    
    if (path == 0) {
        this->saveAs();
    }
    else {
        QGraphicsView *view = (QGraphicsView *)this->centralWidget();
        DiagramScene *scene = (DiagramScene *)view->scene();
        
        if (!scene->save(*path)) {
            QMessageBox(QMessageBox::Warning, QString("Error"), QString("Can't save file."));
        }
    }
}

void MainWindow::saveAs() {
    
    QString temp = QFileDialog::getSaveFileName();
    if (!temp.isEmpty()) {
        *path = temp;
        this->save();
    }
}

void MainWindow::exportDiagram() {
    
    QString temp = QFileDialog::getSaveFileName();
    if (!temp.isEmpty()) {
        
        QGraphicsView *view = (QGraphicsView *)this->centralWidget();
        DiagramScene *scene = (DiagramScene *)view->scene();
        
        if (!scene->exportTo(temp)) {
            QMessageBox(QMessageBox::Warning, QString("Error"), QString("Can't export diagram."));
        }
    }
}
