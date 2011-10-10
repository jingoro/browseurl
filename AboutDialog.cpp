#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::show()
{
//    if ( origGeometry.isNull() ) {
//        QDialog::show();
//        origGeometry = geometry();
//    } else {
//        setGeometry( origGeometry );
//        QDialog::show();
//    }
    QDialog::show();
    raise();
    activateWindow();
}

//void AboutDialog::accept()
//{
//    hide();
//}

//void AboutDialog::reject()
//{
//    hide();
//}
