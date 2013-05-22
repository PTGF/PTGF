/*!
   \file SettingManager.cpp
   \author Dane Gardner <dane.gardner@gmail.com>

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2013 Argo Navis Technologies, LLC

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
   for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "SettingManagerPrivate.h"

#include <QApplication>
#include <QMenuBar>

#ifndef NO_XML_MODULE
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#endif

#ifndef NO_SQL_MODULE
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#endif

#include <QDebug>

#include <ActionManager/ActionManager.h>
#include <CoreWindow/CoreWindow.h>
#include <PluginManager/PluginManager.h>

#include "SettingDialog.h"
#include "ISettingPageFactory.h"


namespace Core {
namespace SettingManager {

/*!
   \class SettingManager
   \brief Manages persisted settings throughout the framework.
   singleton class

   \note Right now, this is just a wrapper for the QSettings object that comes
         with Qt4. In the future, I'd like to see something that is a bit more
         robust backing the settings in this project.
 */

/*!
   \fn SettingManager::instance()
   \brief Access to the singleton instance of this class
   \returns A pointer to the singleton instance of this class
 */
SettingManager &SettingManager::instance()
{
    static SettingManager m_Instance;
    return m_Instance;
}

/*!
   \fn SettingManager::SettingManager()
   \brief Constructor
   \internal
 */
SettingManager::SettingManager(QObject *parent) :
    QObject(parent),
    d(new SettingManagerPrivate)
{
    d->q = this;
}

/*!
   \fn SettingManager::~SettingManager()
   \brief Destructor
   \internal
 */
SettingManager::~SettingManager()
{
    d->m_Settings->sync();
}

bool SettingManager::initialize()
{
    if(d->m_Initialized) {
        return false;
    }

    try {

        ActionManager::ActionManager &actionManager = ActionManager::ActionManager::instance();
        ActionManager::MenuPath path("Tools");
        QAction *settingDialog = actionManager.createAction(NULL, path);
        settingDialog->setText(tr("Settings"));
        settingDialog->setToolTip(tr("Change application and plugin settings"));
        settingDialog->setShortcut(QKeySequence::Preferences);
        connect(settingDialog, SIGNAL(triggered()), this, SLOT(settingDialog()));


        /* Check the object pool for anything we should manage */
        PluginManager::PluginManager &pluginManager = PluginManager::PluginManager::instance();
        foreach(QObject *object, pluginManager.allObjects()) { d->pluginObjectRegistered(object); }
        connect(&pluginManager, SIGNAL(objectAdded(QObject*)), d.data(), SLOT(pluginObjectRegistered(QObject*)));
        connect(&pluginManager, SIGNAL(objectRemoving(QObject*)), d.data(), SLOT(pluginObjectDeregistered(QObject*)));

    } catch(...) {
        return false;
    }

    return d->m_Initialized = true;
}

void SettingManager::shutdown()
{
    if(!d->m_Initialized) {
        return;
    }

    // ...
}


SettingManager::FormatTypes SettingManager::formatType() const
{
    return d->m_FormatType;
}
void SettingManager::setFormatType(const FormatTypes &type)
{
    d->m_FormatType = type;

    switch(d->m_FormatType) {
    case FormatType_Native:
        d->m_Settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName(), this);
        break;
    case FormatType_Ini:
        d->m_Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName(), this);
        break;
#ifndef NO_XML_MODULE
    case FormatType_Xml:
        d->m_Settings = new QSettings(d->m_XmlFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName(), this);
        break;
#endif

#ifndef NO_SQLITE_MODULE
    case FormatType_Sqlite:
        d->m_Settings = new QSettings(d->m_SqliteFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName(), this);
        break;
#endif

    }

}


/*!
   \fn SettingManager::setValue()
   \brief Sets the value of setting key to value. If the key already exists, the previous value is overwritten.
   \param key
   \param value
 */
void SettingManager::setValue(const QString &key, const QVariant &value)
{
    d->m_Settings->setValue(key, value);
}

/*!
   \fn SettingManager::value()
   \brief Returns the value for setting key. If the setting doesn't exist, returns defaultValue.
   \param key
   \param defaultValue
 */
QVariant SettingManager::value(const QString &key, const QVariant &defaultValue) const
{
    return d->m_Settings->value(key, defaultValue);
}

/*!
   \fn SettingManager::remove()
   \brief Removes the setting key and any sub-settings of key.
 */
void SettingManager::remove(const QString &key)
{
    d->m_Settings->remove(key);
}

/*!
   \fn SettingManager::contains()
   \brief Returns true if there exists a setting called key; returns false otherwise.
 */
bool SettingManager::contains(const QString &key) const
{
    return d->m_Settings->contains(key);
}

/*!
   \fn SettingManager::beginGroup()
   \brief Appends prefix to the current group.
   \param prefix
   \sa endGroup
   \sa group
 */
void SettingManager::beginGroup(const QString &prefix)
{
    d->m_Settings->beginGroup(prefix);
}

/*!
   \fn SettingManager::endGroup()
   \brief Resets the group to what it was before the corresponding beginGroup() call.
   \sa beginGroup
   \sa group
 */
void SettingManager::endGroup()
{
    d->m_Settings->endGroup();
}

/*!
   \fn SettingManager::group()
   \brief Returns the current group.
   \returns The current group
   \sa beginGroup
   \sa endGroup
 */
QString SettingManager::group() const
{
    return d->m_Settings->group();
}


void SettingManager::settingDialog()
{
    SettingDialog dialog(d->m_Pages, &CoreWindow::CoreWindow::instance());
    dialog.exec();
}






/***** PRIVATE IMPLEMENTATION *****/

SettingManagerPrivate::SettingManagerPrivate() :
    QObject(NULL),
    q(NULL),
    m_Initialized(false),
    m_Settings(new QSettings())
{

#ifndef NO_XML_MODULE
    m_XmlFormat = QSettings::registerFormat(QString("xml"), readXmlFile, writeXmlFile, Qt::CaseSensitive);
#endif

#ifndef NO_SQLITE_MODULE
    m_SqliteFormat = QSettings::registerFormat("sqlite", readSqliteFile, writeSqliteFile, Qt::CaseSensitive);
#endif

}

void SettingManagerPrivate::registerPageFactory(ISettingPageFactory *page)
{
    m_Pages.append(page);
}

void SettingManagerPrivate::deregisterPageFactory(ISettingPageFactory *page)
{
    if(m_Pages.contains(page)) {
        m_Pages.removeAll(page);
    }
}

void SettingManagerPrivate::pluginObjectRegistered(QObject *object)
{
    ISettingPageFactory *settingPageFactory = qobject_cast<ISettingPageFactory *>(object);
    if(settingPageFactory) registerPageFactory(settingPageFactory);
}

void SettingManagerPrivate::pluginObjectDeregistered(QObject *object)
{
    ISettingPageFactory *settingPageFactory = qobject_cast<ISettingPageFactory *>(object);
    if(settingPageFactory) deregisterPageFactory(settingPageFactory);
}



static inline QString variantToString(const QVariant &v)
{
    QString result;

    switch (v.type()) {
        case QVariant::Invalid:
            result = QLatin1String("@Invalid()");
            break;

        case QVariant::ByteArray: {
            QByteArray a = v.toByteArray();
            result = QLatin1String("@ByteArray(");

            for(int i = 0; i < a.size(); ++i) {
                unsigned char value = (unsigned char)a.at(i);
                if(value < 16) { result += "0"; }
                result += QString::number(value, 16);
            }

            result += QLatin1Char(')');
            break;
        }

        case QVariant::String:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::KeySequence: {
            result = v.toString();
            if (result.startsWith(QLatin1Char('@')))
                result.prepend(QLatin1Char('@'));
            break;
        }

        case QVariant::Rect: {
            QRect r = qvariant_cast<QRect>(v);
            result += QLatin1String("@Rect(");
            result += QString::number(r.x());
            result += QLatin1Char(' ');
            result += QString::number(r.y());
            result += QLatin1Char(' ');
            result += QString::number(r.width());
            result += QLatin1Char(' ');
            result += QString::number(r.height());
            result += QLatin1Char(')');
            break;
        }

        case QVariant::Size: {
            QSize s = qvariant_cast<QSize>(v);
            result += QLatin1String("@Size(");
            result += QString::number(s.width());
            result += QLatin1Char(' ');
            result += QString::number(s.height());
            result += QLatin1Char(')');
            break;
        }

        case QVariant::Point: {
            QPoint p = qvariant_cast<QPoint>(v);
            result += QLatin1String("@Point(");
            result += QString::number(p.x());
            result += QLatin1Char(' ');
            result += QString::number(p.y());
            result += QLatin1Char(')');
            break;
        }

        default: {
            QByteArray a;
            {
                QDataStream s(&a, QIODevice::WriteOnly);
                s.setVersion(QDataStream::Qt_4_0);
                s << v;
            }

            result = QLatin1String("@Variant(");

            for(int i = 0; i < a.size(); ++i) {
                unsigned char value = (unsigned char)a.at(i);
                if(value < 16) { result += "0"; }
                result += QString::number(value, 16);
            }

            result += QLatin1Char(')');

            break;
        }

    }

    return result;
}

static inline QStringList splitArgs(const QString &s, int idx)
{
    int l = s.length();
    Q_ASSERT(l > 0);
    Q_ASSERT(s.at(idx) == QLatin1Char('('));
    Q_ASSERT(s.at(l - 1) == QLatin1Char(')'));

    QStringList result;
    QString item;

    for (++idx; idx < l; ++idx) {
        QChar c = s.at(idx);
        if (c == QLatin1Char(')')) {
            Q_ASSERT(idx == l - 1);
            result.append(item);
        } else if (c == QLatin1Char(' ')) {
            result.append(item);
            item.clear();
        } else {
            item.append(c);
        }
    }

    return result;
}

static inline QVariant stringToVariant(const QString &s)
{
    if (s.startsWith(QLatin1Char('@'))) {
        if (s.endsWith(QLatin1Char(')'))) {
            if (s.startsWith(QLatin1String("@ByteArray("))) {
                QByteArray result;

                QByteArray a(s.toLatin1().mid(11,s.size() -12));
                bool okay;
                for(int i = 0; i < a.count(); i += 2) {
                    result.append((unsigned char)a.mid(i,2).toInt(&okay, 16));
                }

                return QVariant(result);
            } else if (s.startsWith(QLatin1String("@Variant("))) {

                QByteArray str = s.toLatin1().mid(9,s.size() -10);

                QByteArray a;
                bool okay;
                for(int i = 0; i < str.count(); i += 2) {
                    a.append((unsigned char)str.mid(i,2).toInt(&okay, 16));
                }

                QDataStream stream(&a, QIODevice::ReadWrite);
                stream.setVersion(QDataStream::Qt_4_0);

                QVariant result;
                stream >> result;

                return result;
            } else if (s.startsWith(QLatin1String("@Rect("))) {
                QStringList args = splitArgs(s, 5);
                if (args.size() == 4)
                    return QVariant(QRect(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt()));
            } else if (s.startsWith(QLatin1String("@Size("))) {
                QStringList args = splitArgs(s, 5);
                if (args.size() == 2)
                    return QVariant(QSize(args[0].toInt(), args[1].toInt()));
            } else if (s.startsWith(QLatin1String("@Point("))) {
                QStringList args = splitArgs(s, 6);
                if (args.size() == 2)
                    return QVariant(QPoint(args[0].toInt(), args[1].toInt()));
            } else if (s == QLatin1String("@Invalid()")) {
                return QVariant();
            }
        }
        if (s.startsWith(QLatin1String("@@")))
            return QVariant(s.mid(1));
    }

    return QVariant(s);
}







#ifndef NO_XML_MODULE
bool SettingManagerPrivate::readXmlFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QXmlStreamReader reader(&device);
    QStringList elementStack;

    while(!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if(reader.isStartElement() && reader.name() != "Settings") {
            elementStack << reader.name().toString();        // Push
        } else if(reader.isEndElement()) {
            if(!elementStack.isEmpty()) {
                elementStack.removeLast();                   // Pop
            }
        } else if(reader.isCharacters() && !reader.isWhitespace()) {
            map[elementStack.join("/")] = stringToVariant(reader.text().toString());
        }
    }

    if(reader.hasError()) {
        qCritical() << tr("An error occured while reading settings from file: \"%1\"").arg(reader.errorString());
        return false;
    }

    return true;
}

struct KeyPath {
    KeyPath() {}
    KeyPath(QString name) : name(name) {}
    KeyPath(QString name, QVariant value) : name(name), value(value) {}

    QString name;
    QVariant value;
    QList<KeyPath *> children;
};

static inline void writeElement(KeyPath *root, QXmlStreamWriter *writer)
{
    if(root->children.isEmpty()) {
        writer->writeStartElement(root->name);
        writer->writeCharacters(variantToString(root->value));
        writer->writeEndElement();
        return;
    }

    writer->writeStartElement(root->name);
    foreach(KeyPath *child, root->children) {
        writeElement(child, writer);
    }
    writer->writeEndElement();
}

bool SettingManagerPrivate::writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map)
{

    KeyPath *root = new KeyPath();
    foreach(QString key, map.keys()) {
        KeyPath *current = root;
        QVariant value = map.value(key);

        QStringList keyPath = key.split('/');
        for(int i = 0; i < keyPath.count(); ++i) {
            QString name = keyPath.at(i);

            if(name == keyPath.last()) {
                current->children << new KeyPath(name, value);

            } else {
                bool found = false;

                foreach(KeyPath *child, current->children) {
                    if(child->name == name) {
                        current = child;
                        found = true;
                        break;
                    }
                }

                if(!found) {
                    KeyPath *child = new KeyPath(name);
                    current->children.append(child);
                    current = child;
                }

            }
        }
    }


    QXmlStreamWriter writer(&device);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("Settings");

    foreach(KeyPath *child, root->children) {
        writeElement(child, &writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    return true;
}
#endif


#ifndef NO_SQLITE_MODULE
bool SettingManagerPrivate::readSqliteFile(QIODevice &device, QSettings::SettingsMap &map)
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qCritical() << Q_FUNC_INFO << "SQLite driver is not available, please try another method of settings persistence";
        return false;
    }

    // Open the database
    device.close();    // Can't get there from here
    QString fileName = SettingManager::instance().d->m_Settings->fileName();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", fileName);
    if(!db.open()) {
        qCritical() << tr("Couldn't open SQLite database file: %1").arg(db.lastError().text());
        return false;
    }

    // Ensure that the Settings table exists
    if(!db.tables().contains("Settings")) {
        qCritical() << tr("Settings table doesn't exist");
        return false;
    }

    // Read all settings from table into memory
    QSqlQuery q("SELECT * FROM 'Settings'", db);
    if(!q.exec()) {
        qCritical() << tr("Couldn't get values from Settings table: %1").arg(db.lastError().text());
        return false;
    }

    // Transfer data into the settings map
    while(q.next()) {
        QString key = q.value(0).toString();
        QVariant value = stringToVariant(q.value(1).toString());

        if(!key.isEmpty() && value.isValid()) {
            map[key] = value;
        }
    }

    q.finish();

    // Close the datasbase, and return
    db.close();
    return true;
}

bool SettingManagerPrivate::writeSqliteFile(QIODevice &device, const QSettings::SettingsMap &map)
{

    if(!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qCritical() << Q_FUNC_INFO << "SQLite driver is not available, please try another method of settings persistence";
        return false;
    }

    // Prepare the data to be saved
    QStringList replaceKeys, replaceValues;
    foreach(QString key, map.keys()) {
        replaceKeys << key;
        replaceValues << variantToString(map.value(key));
    }


    // Open the database
    device.close();    // Can't get there from here
    QString fileName = SettingManager::instance().d->m_Settings->fileName();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", fileName);
    if(!db.open()) {
        qCritical() << tr("Couldn't open SQLite database file: %1").arg(db.lastError().text());
        return false;
    }

    // Ensure that the Settings table exists
    QSqlQuery createQuery("CREATE TABLE IF NOT EXISTS Settings ( key TEXT, value TEXT )", db);
    if(!createQuery.exec()) {
        qCritical() << tr("Couldn't create Settings table: %1").arg(db.lastError().text());
        db.close();
        return false;
    }
    createQuery.finish();

    // Do the batch query for data previously prepared
    QSqlQuery replaceQuery(db);
    replaceQuery.prepare("INSERT OR REPLACE Settings VALUES (?, ?)");
    replaceQuery.addBindValue(replaceKeys);
    replaceQuery.addBindValue(replaceValues);
    if(!replaceQuery.execBatch()) {
        qCritical() << tr("Couldn't update Settings table: %1").arg(db.lastError().text());
        db.close();
        return false;
    }
    replaceQuery.finish();

    // Close the database, and return
    db.close();
    return true;
}
#endif




} // namespace SettingManager
} // namespace Core
