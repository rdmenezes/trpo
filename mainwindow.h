/*! \file   MainWindow
    \author Mamontov
    Describes a main window of project
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "diagramset.h"


//User interface
namespace Ui {
    class MainWindow;
}

class ToolSceneData;

/*! Class of main window of project
 */
class MainWindow : public QMainWindow {
    Q_OBJECT


private:
      /*! A tool items in table
       */
      QVector<ToolSceneData *>  m_tool_table_items;
public:
    /*! Creates a new windoe
     */
    explicit MainWindow(QWidget *parent = 0);
    /*! Selects a tool by data
        \param[in] tool data
     */
    void selectTool(ToolSceneData * toolData);
    /*! Destructor
     */
    ~MainWindow();

protected:
    /*! Retranslate event
        \param[in] e event params
     */
    void changeEvent(QEvent *e);
    /*! Event, that occurs, when user presses the key
        \param[in] event paremeters
     */
    void keyPressEvent(QKeyEvent *event);
    
    QString *path;
    
protected slots:
    void open();
    void save();
    void saveAs();
    void exportDiagram();
    /*! Shows a help dialog
     */
    void showHelp();
private:
    DiagramSet *   m_set; //!< Set of diagrams
    Ui::MainWindow *ui;  //!< UI
};

#endif // MAINWINDOW_H
