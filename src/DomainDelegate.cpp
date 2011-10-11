#include <QLineEdit>
#include <QSpinBox>
#include "DomainDelegate.h"

DomainDelegate::DomainDelegate( QObject *parent ) :
    QItemDelegate( parent )
{
}

QWidget *DomainDelegate::createEditor( QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index ) const
{
    Q_UNUSED( option );
    Q_UNUSED( index );
    QLineEdit *editor = new QLineEdit( parent );
    return editor;
}

void DomainDelegate::setEditorData( QWidget *editor,
                                    const QModelIndex &index ) const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit *>( editor );
    lineEdit->setText( value );
}
