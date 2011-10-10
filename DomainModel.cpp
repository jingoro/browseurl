#include <QDir>

#include "DomainModel.h"

DomainModel::DomainModel( QObject *parent ) :
    QAbstractTableModel( parent )
{
}

int DomainModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return domains.size();
}

int DomainModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return 2;
}

QVariant DomainModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    int row = index.row();
    int column = index.column();

    if ( row >= domains.size() || row < 0 )
        return QVariant();

    if ( role == Qt::DisplayRole ) {
        Domain domain = domains.at( row );
        switch ( column ) {
        case 0:
            return domain.getDomain();
        case 1:
            return domain.getLocalPath();
        }
    }
    return QVariant();
}

QVariant DomainModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal ) {
        switch ( section ) {
        case 0:
            return tr( "Domain" );
        case 1:
            return tr( "Local Path" );
        }
    }
    return QVariant();
}

Qt::ItemFlags DomainModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags( index ) | Qt::ItemIsEditable;
}

bool DomainModel::setData( const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        Domain domain = domains.at( index.row() );
        switch ( index.column() ) {
        case 0:
            domain.setDomain( value.toString() );
            break;
        case 1:
            domain.setLocalPath( value.toString() );
            break;
        default:
            return false;
        }
        domains.replace( index.row(), domain );
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void DomainModel::addDomain( const QString &domain, const QString &localPath )
{
    int size = domains.size();
    beginInsertRows( QModelIndex(), size - 1, size - 1 );
    domains.append( Domain( domain, localPath ) );
    endInsertRows();
}

void DomainModel::raiseDomain( int index )
{
    if ( index <= 0 || index >= domains.size() )
        return;
    beginMoveRows( QModelIndex(), index, index, QModelIndex(), index - 1 );
    Domain domain = domains.takeAt( index );
    domains.insert( index - 1, domain );
    endMoveRows();
}

void DomainModel::lowerDomain( int index )
{
    if ( index < 0 || index >= ( domains.size() - 1 ) )
        return;
    beginMoveRows( QModelIndex(), index, index, QModelIndex(), index + 2 );
    Domain domain = domains.takeAt( index );
    domains.insert( index + 1, domain );
    endMoveRows();
}

void DomainModel::removeDomain( int index )
{
    if ( index < 0 || index >= ( domains.size() ) )
        return;
    beginRemoveRows( QModelIndex(), index, index );
    domains.removeAt( index );
    endRemoveRows();
}

QList < Domain > DomainModel::getDomains() const
{
    return domains;
}
