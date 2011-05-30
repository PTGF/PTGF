#ifndef MODELDESCRIPTORLISTWIDGET_H
#define MODELDESCRIPTORLISTWIDGET_H

#include <QWidget>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QDomDocument>
#include <QUuid>
#include <QMap>
#include <QList>
#include <QFile>
#include <QTreeWidgetItem>
#include <QSortFilterProxyModel>

namespace Plugins {
namespace OpenSpeedShop {

class ModelDescriptor;

class ModelDescriptorListWidget : public QTreeView
{
    Q_OBJECT

public:
    explicit ModelDescriptorListWidget(QAbstractItemModel *descriptorsModel, QWidget *parent = 0);
    ~ModelDescriptorListWidget();

    QString filter() const;
    void setFilter(const QString &regex);

    QString experimentType() const;
    void setExperimentType(const QString &experimentType);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

signals:
    void currentDescriptorChanged(const QUuid &descriptorUid);
    void descriptorSingleClicked(const QUuid &descriptorUid);
    void descriptorDoubleClicked(const QUuid &descriptorUid);

public slots:
    void selectRow(const QUuid &uid);

protected:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    QModelIndex findIndex(const QUuid &uid, const QModelIndex &parent = QModelIndex()) const;
    QString m_ExperimentType;

protected slots:
    void itemSingleClicked(QModelIndex index);
    void itemDoubleClicked(QModelIndex index);

};


} // namespace OpenSpeedShop
} // namespace Plugins
#endif // MODELDESCRIPTORLISTWIDGET_H