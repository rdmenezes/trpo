/*! \file    helpwindow.h
    \author  Mamontov

    Describes a window with help.
 */
#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
//UI for help window
namespace Ui {
    class Dialog;
}
/*! \class HelpWindow
    Describes a help window dialog
 */
class HelpWindow : public QDialog {
    Q_OBJECT
public:
    /*! Creates a new help window
     */
    HelpWindow(QWidget *parent = 0);
    /*! Destructor
     */
    ~HelpWindow();

protected:
    /*! Retranslating event
        \param[in] e event params
     */
    void changeEvent(QEvent *e);

private:
    Ui::Dialog *ui;  //!< UI
};

#endif // HELPWINDOW_H
