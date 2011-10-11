#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog( OsProxy *aOsProxy, DomainModel *aDomainModel, QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::PreferencesDialog ),
    osProxy( aOsProxy ),
    domainModel( aDomainModel ),
    domainDelegate( new DomainDelegate( this ) )
{
    ui->setupUi( this );
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

    ui->domainListTableView->setModel( domainModel );
    ui->domainListTableView->setItemDelegate( domainDelegate );
    ui->domainListTableView->horizontalHeader()->setStretchLastSection( true );

    ui->autostartCheckBox->setCheckState( osProxy->autostartEnabled() ? Qt::Checked : Qt::Unchecked );
    connect( ui->autostartCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( updateAutostartCheckBox() ) );
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
    delete domainDelegate;
}

void PreferencesDialog::show()
{
    QDialog::show();
    raise();
    activateWindow();
}

void PreferencesDialog::addDomain()
{
    domainModel->addDomain( "", "" );
}

void PreferencesDialog::raiseDomain()
{
    int row = getSelectedRow();
    if ( row >= 0 )
        domainModel->raiseDomain( row );
}

void PreferencesDialog::lowerDomain()
{
    int row = getSelectedRow();
    if ( row >= 0 )
        domainModel->lowerDomain( row );
}

void PreferencesDialog::deleteDomain()
{
    int row = getSelectedRow();
    if ( row >= 0 )
        domainModel->removeDomain( row );
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

void PreferencesDialog::updateAutostartCheckBox()
{
    if ( ui->autostartCheckBox->checkState() == Qt::Checked ) {
        qDebug( "enabling autostart" );
        osProxy->enableAutostart();
    } else {
        qDebug( "disabling autostart" );
        osProxy->disableAutostart();
    }
}
