#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QDomDocument>

#include <QStandardItemModel>
#include <QUuid>

namespace Plugins {
namespace OpenSpeedShop {

class ModelDescriptor;
class ModelDescriptorWidget;
class ModelDescriptorListWidget;
class DataModel;

class ModelManager : public QObject
{
    Q_OBJECT
public:
    static ModelManager *instance();

    void initialize();
    void shutdown();

    void fetchDescriptors();
    void importDescriptors(const QString &filepath = QString());
    void exportDescriptors(const QString &filepath = QString());
    QUuid createDescriptor();
    void removeDescriptor(const QUuid &descriptorUid);
    ModelDescriptorWidget *createDescriptorWidget(const QUuid &descriptorUid, QWidget *parent = NULL);
    ModelDescriptorListWidget *createDescriptorListWidget(QWidget *parent = NULL);
    QMimeData descriptorMimeData(const QUuid &descriptorUid);

    QUuid importModel(const QString &filepath = QString());
    void exportModel(const QUuid &modelUid, const QString &filepath = QString());
    QUuid fetchModel(const QUuid &descriptorUid, const QUuid &experimentUid);
    void unloadModel(const QUuid &modelUid);
    void unloadModel(const QUuid &descriptorUid, const QUuid &experimentUid);
    QMimeData modelMimeData(const QUuid &modelUid);
    QMimeData modelMimeData(const QUuid &descriptorUid, const QUuid &experimentUid);
    QAbstractItemModel *model(const QUuid &modelUid);
    QAbstractItemModel *model(const QUuid &descriptorUid, const QUuid &experimentUid);
    QUuid modelUid(const QUuid &descriptorUid, const QUuid &experimentUid) const;

protected:
    explicit ModelManager(QObject *parent = 0);

    void insertDescriptorIntoModel(QUuid descriptorUid);
    ModelDescriptor *descriptor(QUuid descriptorUid);
    QAbstractItemModel *descriptorModel();

    QHash<QUuid, ModelDescriptor *> m_DescriptorPool;
    QHash<QUuid, QStandardItem *> m_DescriptorToItem;
    QStandardItemModel m_DescriptorPoolModel;

    // DescriptorID -> ExperimentID -> ModelID
    QMultiHash<QUuid, QHash<QUuid, QUuid>*> m_ModelLookupTable;
    QHash<QUuid, DataModel*> m_ModelPool;

protected slots:
    void descriptorNameChanged(ModelDescriptor *descriptor = NULL);
    void descriptorTypeChanged(ModelDescriptor *descriptor = NULL);
};

} // namespace OpenSpeedShop
} // namespace Plugins

#endif // MODELMANAGER_H
