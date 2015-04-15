/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2015  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "color_delegate.hpp"
#include "color_selector.hpp"
#include "color_dialog.hpp"

Color_Delegate::Color_Delegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void Color_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
    {
        QColor col = qvariant_cast<QColor>(index.data());

        Color_Preview cp;
        cp.setColor(col);
        cp.paint(*painter,option.rect);

    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QWidget *Color_Delegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
    {
        Color_Dialog *editor = new Color_Dialog(parent);
        editor->setMinimumSize(editor->sizeHint());
        connect(editor, SIGNAL(colorChanged(QColor)), this, SLOT(color_changed()));
        connect(editor, SIGNAL(accepted()), this, SLOT(close_editor()));
        return editor;
    }
    else
        return QStyledItemDelegate::createEditor(parent, option, index);
}

void Color_Delegate::color_changed()
{
    Color_Dialog *editor = qobject_cast<Color_Dialog*>(sender());
    emit commitData(editor);
}
void Color_Delegate::close_editor()
{
    Color_Dialog *editor = qobject_cast<Color_Dialog*>(sender());
    emit closeEditor(editor);
}

void Color_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    if (index.data().canConvert<QColor>())
    {
        Color_Dialog *selector = qobject_cast<Color_Dialog*>(editor);
        selector->setColor(qvariant_cast<QColor>(index.data()));
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void Color_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
    {
        Color_Dialog *selector = qobject_cast<Color_Dialog *>(editor);
        model->setData(index, QVariant::fromValue(selector->color()));
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

QSize Color_Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
        return QSize(24,16);

    return QStyledItemDelegate::sizeHint(option, index);
}
