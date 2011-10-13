#include "helpwindow.h"
#include "ui_helpwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString app_path=QApplication::applicationDirPath();
    QString delimiter="/";
    if (app_path.contains('\\'))
        delimiter="\\";
    QFile fl(app_path+delimiter+"help.htm");
    if (fl.open(QIODevice::ReadOnly | QIODevice::Text))
    {
     QTextStream s(&fl);
     ui->helpView->setHtml(s.readAll());
     fl.close();
    }
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
