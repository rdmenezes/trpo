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

    //Setup splitter
    QList<int> sizes;
    sizes<<750<<150;
    ui->sptItems->setSizes(sizes);

    //Setup graphic interface
    DiagramScene * scene=new DiagramScene(m_set->get(0));
    ui->view->setScene(scene);
    scene->setSceneRect(0,0,ui->view->viewport()->width(),
                            ui->view->viewport()->height());
    scene->setView(ui->view);


    //Set some signals
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionExport_to_PNF_Ctrl_E,SIGNAL(triggered()),this,SLOT(exportDiagram()));
    connect(ui->actionShow_Help_F1,SIGNAL(triggered()),this,SLOT(showHelp()));
    path = NULL;
}

MainWindow::~MainWindow() {
    /** TODO: It must be performed if diagram is not empty,
              not fucking every time!
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Question,
                                                QString(""),
                                                QString("Do you want to save the changes you made?"),
                                                QMessageBox::Save | QMessageBox::Discard, this);

    if (messageBox->exec() == QMessageBox::Save) {
        this->save();
    }
    */
    delete ui;
    delete m_set;
    delete path;
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
        handled=true;
    }
    else if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier) {
        this->save();
        handled=true;
    }
    else if (event->key() == Qt::Key_S && event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
        this->saveAs();
        handled=true;
    }
    else if (event->key() == Qt::Key_E && event->modifiers() == Qt::ControlModifier) {
        this->exportDiagram();
        handled=true;
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
        if (path) delete path;
        path = new QString(temp);
        
        QGraphicsView *view = ui->view;
        DiagramScene *scene = (DiagramScene *)view->scene();
        
        if (!(scene->load(*path))) {
            QMessageBox::warning(NULL, QString("Error"), QString("Can't open file."));
        }
    }
}

void MainWindow::save() {
    
    if (path == 0) {
        this->saveAs();
    }
    else {
        QGraphicsView *view = ui->view;
        DiagramScene *scene = (DiagramScene *)view->scene();
        bool success=scene->save(*path);
        if (!success) {
            QMessageBox::warning(NULL, QString("Error"), QString("Can't save file."));
        }
    }
}

void MainWindow::saveAs() {
    
    QString temp = QFileDialog::getSaveFileName();
    if (!temp.isEmpty()) {
        if (path) delete path;
        path = new QString(temp);
        this->save();
    }
}

void MainWindow::exportDiagram() {
    
    QString temp = QFileDialog::getSaveFileName();
    if (!temp.isEmpty()) {
        
        QGraphicsView *view =  ui->view;
        DiagramScene *scene = (DiagramScene *)view->scene();
        
        if (!(scene->exportTo(temp))) {
            QMessageBox::warning(NULL, QString("Error"), QString("Can't export diagram."));
        }
    }
}
