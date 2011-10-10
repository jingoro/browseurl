#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "DomainModel.h"
#include "DomainDelegate.h"

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();
    DomainModel *getDomainModel();

public slots:
    void addDomain();
    void raiseDomain();
    void lowerDomain();
    void deleteDomain();

private:
    int getSelectedRow();

    Ui::PreferencesDialog *ui;
    DomainModel    *domainModel;
    DomainDelegate *domainDelegate;
};

#endif // PREFERENCESDIALOG_H
