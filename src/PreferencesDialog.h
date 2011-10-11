#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "DomainModel.h"
#include "DomainDelegate.h"
#include "OsProxy.h"

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog( OsProxy *osProxy, DomainModel *domainModel, QWidget *parent = 0 );
    ~PreferencesDialog();
    DomainModel *getDomainModel();

public slots:
    void show();
    void addDomain();
    void raiseDomain();
    void lowerDomain();
    void deleteDomain();
    void updateAutostartCheckBox();

private:
    int getSelectedRow();

    Ui::PreferencesDialog *ui;
    DomainModel    *domainModel;
    DomainDelegate *domainDelegate;
    OsProxy        *osProxy;
};

#endif // PREFERENCESDIALOG_H
