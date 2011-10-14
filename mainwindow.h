/*! \file   MainWindow
    \author Mamontov
    Describes a main window of project
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

//User interface
namespace Ui {
    class MainWindow;
}
/*! Class of main window of project
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /*! Creates a new windoe
     */
    explicit MainWindow(QWidget *parent = 0);
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
private:
    Ui::MainWindow *ui;  //!< UI
    /*! Shows a help dialog
     */
    void showHelp();
};

#endif // MAINWINDOW_H
