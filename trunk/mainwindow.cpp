#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->mainToolBar->setHidden(true);
    this->setWindowTitle("IDEF0 Diagram Builder");

    toolbar = new UToolbarView(this);


    boxes = new QVector<UBox *>();
    arrows = new QVector<UArrow *>();


    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0.3);
    ui->centralWidget->setGraphicsEffect(effect);

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

    if (event->key() == Qt::Key_F1) {
         showHelp();
    }
    else if (event->key() == Qt::Key_Shift) {
        showToolbar();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        
        if (toolbar->currentTool() == UToolBox) {

            QPoint point = event->pos();
            UBox *box = new UBox(this, point);

            boxes->append(box);
        }
        else if (toolbar->currentTool() == UToolEraser) {

        }
        else if (toolbar->currentTool() == UToolArrow) {
            
        }
        else if (toolbar->currentTool() == UToolSelect) {
            
        }
    }
}

void MainWindow::showStartupHint() {

}

void MainWindow::showHelp() {
   HelpWindow a;
   a.exec();
}

void MainWindow::showToolbar() {

    toolbar->raise();
    toolbar->setHidden(!toolbar->isHidden());
}


void MainWindow::elementWasClicked(QWidget *element) {
    
    if (toolbar->currentTool() == UToolEraser) {
        UBox *box = (UBox *)element;
        box->removeBox();
    }
    else if (toolbar->currentTool() == UToolArrow) {
        QWidget *slot = element;
        UArrow *arrow = new UArrow(this, slot);
        arrows->append(arrow);
    }
}

void MainWindow::slotWasClicked(QWidget *slot) {
    
    if (toolbar->currentTool() == UToolArrow) {
        UArrow *arrow = new UArrow(this, slot);
        arrows->append(arrow);
    }
}

int MainWindow::cellLength() {
    return 20;
}
