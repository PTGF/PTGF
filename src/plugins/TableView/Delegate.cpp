/*!
   \file Delegate.cpp
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

#include "Delegate.h"

#include <QApplication>
#include <QHelpEvent>
#include <QDebug>

namespace Plugins {
namespace TableView {

/*! \class Plugins::TableView::Delegate
    \brief A basic delegate to assist the tree and table views in the set of standard views available for the main
           OpenSpeeShop plugin.
    \sa QStyledItemDelegate QAbstractItemDelegate
 */

/*! \fn Delegate::Delegate()
    \brief Constructor.
 */
Delegate::Delegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

/*! \fn Delegate::paint()
    \brief Handles special case items (like CallStackEntry and percentage values).
    \reimp QStyledItemDelegate::paint()
    \sa sizeHint() selected() deselected()
 */
void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString headerTitle = index.model()->headerData(index.column(), Qt::Horizontal, Qt::DisplayRole).toString();
    int indexDataType = index.data().userType();
    if((indexDataType == QVariant::Double || indexDataType == QMetaType::Float) && headerTitle.contains("%")) {

        // Draw the background first
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);
        opt.text = QString();
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

        // Setup the progress bar
        double progress = index.data().toDouble();
        QStyleOptionProgressBarV2 progressBarOption;
        progressBarOption.state = opt.state;
        progressBarOption.palette = opt.palette;
        progressBarOption.palette.setBrush(QPalette::WindowText, Qt::red);
        progressBarOption.fontMetrics = opt.fontMetrics;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = (int)progress;
        progressBarOption.text = QString("%1%").arg(progress, 5, 'f', 2, QChar(0x2002));
        progressBarOption.textVisible = true;

        // Don't exceed a maximum height, or it looks silly
        QRect rect(option.rect);
        QPoint center = rect.center();
        rect.setHeight( ((rect.height() <= rect.width()/5)? rect.height(): rect.width()/5) );
        rect.moveCenter(center);
        progressBarOption.rect = rect;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        return;
    }

    QStyledItemDelegate::paint(painter, option, index);
}


/*! \fn Delegate::selected()
    \brief Keeps track of selected items, and notifies the view of any size changes.
    This is part of a work around involving the known bug detailed in Delegate::sizeHint().
    \sa deselected() sizeHint() print() TableView::selectionChanged()
 */
void Delegate::selected(const QModelIndex &index)
{
    if(!m_SelectedRows.contains(index)) {
        m_SelectedRows.insert(index);
        emit sizeHintChanged(index);
    }
}


/*! \fn Delegate::deselected()
    \brief Keeps track of selected items, and notifies the view of any size changes.
    This is part of a work around involving the known bug detailed in Delegate::sizeHint().
    \sa selected() sizeHint() print() TableView::selectionChanged()
 */
void Delegate::deselected(const QModelIndex &index)
{
    if(m_SelectedRows.contains(index)) {
        m_SelectedRows.remove(index);
        emit sizeHintChanged(index);
    }
}

QString Delegate::displayText(const QVariant &value, const QLocale& locale) const
{
    switch (value.userType()) {
    case QMetaType::Float:
    case QVariant::Double:
        return QString("%1").arg(value.toDouble(), 10, 'f', 6, QChar(0x2002));
    case QVariant::Int:
    case QVariant::LongLong:
        return QString("%1").arg(value.toLongLong(), 4, 10, QChar(0x2002));
    case QVariant::UInt:
    case QVariant::ULongLong:
        return QString("%1").arg(value.toULongLong(), 4, 10, QChar(0x2002));
    default:
        return QStyledItemDelegate::displayText(value, locale);
    }
}


} // namespace TableView
} // namespace Plugins
