/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright (C) 2012-2015 Mattia Basaglia
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "color_delegate.hpp"
#include "color_selector.hpp"
#include "color_dialog.hpp"

namespace color_widgets {

ColorDelegate::ColorDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
    {
        QColor col = qvariant_cast<QColor>(index.data());

        ColorPreview cp;
        cp.setColor(col);
        cp.paint(*painter,option.rect);

    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QWidget *ColorDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
    {
        ColorDialog *editor = new ColorDialog(parent);
        editor->setMinimumSize(editor->sizeHint());
        connect(editor, SIGNAL(colorChanged(QColor)), this, SLOT(color_changed()));
        connect(editor, SIGNAL(accepted()), this, SLOT(close_editor()));
        return editor;
    }
    else
        return QStyledItemDelegate::createEditor(parent, option, index);
}

void ColorDelegate::color_changed()
{
    ColorDialog *editor = qobject_cast<ColorDialog*>(sender());
    emit commitData(editor);
}
void ColorDelegate::close_editor()
{
    ColorDialog *editor = qobject_cast<ColorDialog*>(sender());
    emit closeEditor(editor);
}

void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    if (index.data().canConvert<QColor>())
    {
        ColorDialog *selector = qobject_cast<ColorDialog*>(editor);
        selector->setColor(qvariant_cast<QColor>(index.data()));
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void ColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
    {
        ColorDialog *selector = qobject_cast<ColorDialog *>(editor);
        model->setData(index, QVariant::fromValue(selector->color()));
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

QSize ColorDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<QColor>())
        return QSize(24,16);

    return QStyledItemDelegate::sizeHint(option, index);
}

} // namespace color_widgets
