#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class HelpWindow : public QDialog {
    Q_OBJECT
public:
    HelpWindow(QWidget *parent = 0);
    ~HelpWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Dialog *ui;
};

#endif // HELPWINDOW_H
