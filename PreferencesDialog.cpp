#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    domainModel = new DomainModel( this );
    domainDelegate = new DomainDelegate( this );
    ui->setupUi(this);
    ui->domainListTableView->setModel( domainModel );
    ui->domainListTableView->setItemDelegate( domainDelegate );
    ui->domainListTableView->horizontalHeader()->setStretchLastSection( true );
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
    delete domainDelegate;
    delete domainModel;
}

void PreferencesDialog::addDomain()
{
    domainModel->addDomain( "", "" );
}

void PreferencesDialog::raiseDomain()
{
    int row = getSelectedRow();
    if ( row >= 0 ) {
        domainModel->raiseDomain( row );
    }
}

void PreferencesDialog::lowerDomain()
{
    int row = getSelectedRow();
    if ( row >= 0 ) {
        domainModel->lowerDomain( row );
    }
}

void PreferencesDialog::deleteDomain()
{
    int row = getSelectedRow();
    if ( row >= 0 ) {
        domainModel->removeDomain( row );
    }
}

int PreferencesDialog::getSelectedRow()
{
    QItemSelectionModel *selection = ui->domainListTableView->selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();
    if ( ! indexes.isEmpty() ) {
        return indexes.first().row();
    } else {
        return -1;
    }
}

DomainModel *PreferencesDialog::getDomainModel()
{
    return domainModel;
}
