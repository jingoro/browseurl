#ifndef DOMAINDELEGATE_H
#define DOMAINDELEGATE_H

#include <QItemDelegate>

class DomainDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DomainDelegate( QObject *parent = 0 );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;

signals:

public slots:

};

#endif // DOMAINDELEGATE_H
