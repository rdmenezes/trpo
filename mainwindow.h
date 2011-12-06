/*! \file   MainWindow
    \author Mamontov
    Describes a main window of project
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "diagramset.h"
#include "diagram.h"
#include "tool.h"

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
      /*! Path for object
       */
      QString * m_path;
      /*! Diagram data
       */
      DiagramSet * m_set;
      /*! Whether we should delete an objects
       */
      bool      m_own;
      //Previous position of horizontalSlider
      int previousScaleValue;
public:
    /*! Creates a new window
        \param[in] parent object
     */
    explicit MainWindow(QWidget *parent = 0,
                        DiagramSet * set = 0,
                        const DiagramParent & p=DiagramParent());
    /*! Selects a tool by data
        \param[in] tool data
     */
    void selectTool(ToolSceneData * toolData);
    /*! Selects a tool
        \param[in] tool data
     */
    void selectTool(Tool * tool);
    /*! Sets an action text
        \param[in[ text text
     */
    void setActionText(const QString & text);
    /*! Changes a set of data
        \param[in] set of data
     */
    void changeSet(DiagramSet * set);
    /*! Destructor
     */
    ~MainWindow();

protected:
    /*! Retranslate event
        \param[in] e event params
     */
    void changeEvent(QEvent *e);
        
protected slots:
    void open();
    void save();
    void saveAs();
    void exportDiagram();
    void scale(int coef);
    /*! Shows a help dialog
     */
    void showHelp();

public:
    Ui::MainWindow *ui;  //!< UI

    /*! Event, that occurs, when user presses the key
        \param[in] event paremeters
     */
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
