#include <QGraphicsView>
#include <QMessageBox>
#include <QFileDialog>
#include <QSlider>
#include <math.h>
#include "mainwindow.h"
#include "helpwindow.h"
#include "ui_mainwindow.h"
#include "diagramscene.h"
#include "tooldelegate.h"
#include "diagramtesttool.h"
#include "diagram.h"
#include "selecttool.h"
#include "boxtool.h"
#include "arrowtool.h"
#include "commentlinetool.h"
#include "freecommenttool.h"
#include "erasertool.h"
#include "graphicunittests.h"
#include <QFile>
#include <QTextStream>
#define VIEW_WIDTH_X 102
#define VIEW_WIDTH_Y 102

MainWindow::MainWindow(QWidget *parent,
                       DiagramSet * set,
                       const DiagramParent & p) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    previousScaleValue=4;

    //Creates a diagram set
    if (!set)
    {
     m_set=new DiagramSet();
     m_own=true;
    }
    else
    {
     m_set=set;
     m_own=false;
    }
    //Setup UI
    ui->setupUi(this);
    //Set window Title
    this->setWindowTitle("IDEF0 Diagram Editor");

    //Setup splitter
    QList<int> sizes;
    sizes<<696<<204;
    ui->sptItems->setSizes(sizes);

    //Setup graphic interface
    Diagram * diag=m_set->create(p);
    DiagramScene * scene=new DiagramScene(diag);
    diag->bind(scene,m_set);
    ui->view->setScene(scene);
    scene->setSceneRect(0,0,ui->view->width()-2,
                            ui->view->height()-2);
    //Set mouse tracking for  data.
    ui->view->setMouseTracking(true);

    //Setup delegates table

    QVector<Tool *> tool_ptrs;
    tool_ptrs<<new SelectTool();      // 0
    tool_ptrs<<new EraserTool();      // 1
    tool_ptrs<<new BoxTool();         // 2
    tool_ptrs<<new ArrowTool();       // 3
    tool_ptrs<<new CommentLineTool(); // 4
    tool_ptrs<<new FreeCommentTool(); // 5
    QVector<QGraphicsView *> views;
    for (int row=0;row<3;row++)
    {
        for (int col=0;col<2;col++)
        {
            //Create name of node
            QString name(":/");
            name+=QString::number(row*2+col+1);
            name+=".png";
            //Create a view
            QGraphicsView * editor=new QGraphicsView(this);
            editor->setFixedSize(VIEW_WIDTH_X,VIEW_WIDTH_Y);
            ToolScene  * scene=new ToolScene();
            ToolSceneData * data=new ToolSceneData(new QImage(name),
                                                   this,
                                                   tool_ptrs[row*2+col]
                                                  );
            m_tool_table_items<<data;
            ui->toolBox->addWidget(editor,row,col);
            editor->setScene(scene);
            views<<editor;
        }
    }
    for (int i=0;i<views.size();i++)
    {
        views[i]->scene()->setSceneRect(0,0,
                                        views[i]->width()-2,
                                        views[i]->height()-2);
        static_cast<ToolScene*>(views[i]->scene())->setData(m_tool_table_items[i]);
    }
    static_cast<ToolScene*>(views[2]->scene())->select();
    diag->commit();
    //Set some signals
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionExport_to_PNF_Ctrl_E,SIGNAL(triggered()),this,SLOT(exportDiagram()));
    connect(ui->actionShow_Help_F1,SIGNAL(triggered()),this,SLOT(showHelp()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),this,SLOT(scale(int)));
    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(redo()));

    m_path = NULL;

    ui->view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //Perform test of box
    //GraphicUnitTests gut(diag);
    //gut.performDefaultObjectTest();


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
    static_cast<DiagramScene *>(ui->view->scene())->diagram()->clear();
    delete ui;
    if (m_own)
         delete m_set;
    delete m_path;
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
    else if (event->key() == Qt::Key_Z && event->modifiers() == Qt::ControlModifier) {
        this->undo();
        handled=true;
    }
    else if (event->key() == Qt::Key_R && event->modifiers() == Qt::ControlModifier) {
        this->redo();
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
    else if (event->key() == Qt::Key_F12) {
        this->selectTool(new DiagramTestTool());
        handled=true;
    }
    else if (event->modifiers()==Qt::ControlModifier)
    {
        Qt::Key arr[6]={Qt::Key_1,Qt::Key_2,Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6};
        for (int i=0;i<6 && !handled;i++)
            if (event->key()==arr[i])
            {
               m_tool_table_items[i]->scene()->select();
               handled=true;
            }
    }
    if (!handled)
        this->QMainWindow::keyPressEvent(event);
}


void MainWindow::showHelp() {
   HelpWindow d(this);
   d.exec();
}


void MainWindow::scale(int coef){
     QGraphicsView *view = ui->view;
     QRectF area;
     area=view->sceneRect();

     //depending on new and previous positions of horizontalSlider
     if (coef==1)
     {
         if (previousScaleValue==0)
             view->scale((pow(2.0, coef+1-previousScaleValue))/3, (pow(2.0, coef+1-previousScaleValue))/3);
         else
             view->scale((pow(2.0, coef+2-previousScaleValue))/3, (pow(2.0, coef+2-previousScaleValue))/3);
     }
     else
     {
         if (previousScaleValue==1)
         {
             if (coef==0)
                 view->scale((pow(2.0, coef-1-previousScaleValue))*3, (pow(2.0, coef-1-previousScaleValue))*3);
             else
                 view->scale((pow(2.0, coef-2-previousScaleValue))*3, (pow(2.0, coef-2-previousScaleValue))*3);
         }
         else
             view->scale(pow(2.0f, coef-previousScaleValue), pow(2.0f, coef-previousScaleValue));
     }
     view->setSceneRect(area);
     previousScaleValue=coef;
}


void MainWindow::open() {
    
    QString temp = QFileDialog::getOpenFileName();
    if (!temp.isEmpty()) {
        if (m_path) delete m_path;
        m_path = new QString(temp);
        

        if (!load(*m_path)) {
            QMessageBox::warning(NULL, QString("Error"), QString("Can't open file."));
        }
    }
}

void MainWindow::save() {
    
    if (m_path == 0) {
        this->saveAs();
    }
    else {
        bool success=saveTo(*m_path);
        if (!success) {
            QMessageBox::warning(NULL, QString("Error"), QString("Can't save file."));
        }
    }
}

bool  MainWindow::saveTo(const QString & str)
{
    QDomDocument  doc("IDEFML");
    QDomElement  root=doc.createElement("root");
    doc.appendChild(root);
    m_set->save(&doc,&root);
    QFile file(str);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QTextStream stream(&file);
    QString string=doc.toString();
    stream<<string;
    file.close();
    return true;
}

bool MainWindow::load(const QString & str)
{
    QDomDocument  doc("IDEFML");
    QFile file(str);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    QDomElement root=doc.documentElement();
    if (root.tagName()!="root")
        return false;

    DiagramSet * newset=new DiagramSet();
    QDomElement el=root.firstChildElement("diagram_set");
    QMap<void *,Serializable*> addrmap;
    newset->load(&el,addrmap);
    DiagramScene * scene=static_cast<DiagramScene*>(ui->view->scene());
    scene->clear();
    if (m_own)
    {
        delete m_set;
    }
    m_own=true;
    m_set=newset;
    Diagram * diagram=m_set->get((int)0);
    scene->setDiagram(diagram);
    diagram->bind(scene,m_set);
    diagram->fillScene(scene);
    for (int i=0;i<m_tool_table_items.size();i++)
        m_tool_table_items[i]->tool()->setDiagramData(scene,diagram);
    scene->tool()->initState();
    diagram->commit();
    file.close();
    return true;
}

void MainWindow::saveAs() {
    
    QString temp = QFileDialog::getSaveFileName();
    if (!temp.isEmpty()) {
        if (m_path) delete m_path;
        m_path = new QString(temp);
        this->save();
    }
}

void MainWindow::exportDiagram() {
    
    QString temp = QFileDialog::getSaveFileName();
    if (!temp.isEmpty()) {
        
        QGraphicsView *view =  ui->view;
        DiagramScene *scene = (DiagramScene *)view->scene();
        QImage img;
        scene->diagram()->exportTo(img);
        QString format="png";
        QStringList lst=temp.split('.');
        //If supplied filename
        if (lst.size()>1)
        {
            QString lastformat=lst[lst.size()-1].toUpper();
            if (lastformat=="JPG"
                || lastformat=="JPEG"
                || lastformat=="GIF"
                || lastformat=="BMP")
                format=lastformat.toLower();
        }
        //Save image
        bool success=img.save(temp,format.toStdString().c_str());
        if (!success)
        {
            QMessageBox::warning(NULL, QString("Error"), QString("Can't export diagram."));
        }
    }
}

void MainWindow::selectTool(Tool * tool)
{
 for (int i=0;i<m_tool_table_items.size();i++)
     m_tool_table_items[i]->deselect();
 static_cast<DiagramScene *>(ui->view->scene())->setTool(tool);
 ui->view->setFocus();
}

void MainWindow::selectTool(ToolSceneData * toolData)
{
  selectTool(toolData->tool());
}

void MainWindow::setActionText(const QString & text)
{
    ui->currentAction->setText(text);
}

void MainWindow::undo()
{
   DiagramScene * scene=static_cast<DiagramScene*>(ui->view->scene());
   scene->diagram()->rollback();
}
void MainWindow::redo()
{
    DiagramScene * scene=static_cast<DiagramScene*>(ui->view->scene());
    scene->diagram()->redo();
}
