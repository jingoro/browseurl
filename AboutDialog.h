#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

    void show();

public slots:
    void accept();
    void reject();

private:
    Ui::AboutDialog *ui;

    QRect origGeometry;
};

#endif // ABOUTDIALOG_H
