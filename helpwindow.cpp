#include "helpwindow.h"
#include "ui_helpwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    //Setup ui
    ui->setupUi(this);
    //Extract path to help parts
    QString app_path=QApplication::applicationDirPath();
    QString delimiter="/";
    if (app_path.contains('\\'))
        delimiter="\\";
    //Open help for reading
    QFile fl(app_path+delimiter+"help.htm");
    if (fl.open(QIODevice::ReadOnly | QIODevice::Text))
    {
     QTextStream s(&fl);
     //Read data and set html
     ui->helpView->setHtml(s.readAll());
     fl.close();
    }
    //Display message in case of fail
    else QMessageBox::warning(NULL,"Error!",app_path+delimiter+"help.htm: can't be opened!");
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
