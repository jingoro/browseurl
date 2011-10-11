#ifndef DOMAINMODEL_H
#define DOMAINMODEL_H

#include <QAbstractTableModel>
#include <QString>

#include "Domain.h"

class DomainModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    explicit DomainModel( QObject *parent = 0 );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role );

    void addDomain( const QString &domain, const QString &localPath );
    void raiseDomain( int index );
    void lowerDomain( int index );
    void removeDomain( int index );

    QList< Domain > getDomains() const;

signals:

public slots:

private:
    QList< Domain > domains;
};

#endif // DOMAINMODEL_H
