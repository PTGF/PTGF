#ifndef MODELDESCRIPTOR_H
#define MODELDESCRIPTOR_H

#include <QObject>
#include <QDomDocument>
#include <QUuid>
#include <QStringList>
#include <QFile>

namespace Plugins {
namespace OpenSpeedShop {

class ModelDescriptor : public QObject
{
    Q_OBJECT
public:
    explicit ModelDescriptor(QObject *parent = 0);

    QUuid id() const;
    void setId(const QString &uuid);
    void setId(const QUuid &uuid);

    QString name() const;
    void setName(const QString &name);

    QString experimentType() const;
    void setExperimentType(const QString &experimentType);

    quint64 rowCount() const;
    void setRowCount(const quint64 &rowCount);

    QStringList modifiers() const;
    void removeModifier(const QString &modifier);
    void insertModifier(const QString &modifier);

    QStringList metrics() const;
    void removeMetric(const QString &metric);
    void insertMetric(const QString &metric);

    bool isEmpty();

    bool isDefault();
    void setDefault(bool value);

    static QList<ModelDescriptor *> fromXml(const QString &filePath, QObject *parent = NULL);
    static QList<ModelDescriptor *> fromXml(QDomDocument modelDescriptorDocument, QObject *parent = NULL);
    static ModelDescriptor *fromXml(const QDomElement &modelDescriptorElement, QObject *parent = NULL);
    static void toXml(const QString &filepath, QList<ModelDescriptor *> modelDescriptors);
    static QDomDocument toXml(QList<ModelDescriptor *> modelDescriptors);
    QDomElement toXml(QDomDocument document);

signals:
    void idChanged();
    void nameChanged();
    void experimentTypeChanged();
    void rowCountChanged();
    void modifiersChanged();
    void metricsChanged();
    void dataChanged();

protected:
    QUuid m_Id;
    QString m_Name;
    QString m_ExperimentType;
    quint64 m_RowCount;
    QStringList m_Modifiers;
    QStringList m_Metrics;
    bool m_Empty;
    bool m_Default;

};

} // namespace OpenSpeedShop
} // namespace Plugins

#endif // MODELDESCRIPTOR_H
