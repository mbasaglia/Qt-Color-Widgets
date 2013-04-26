/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

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

#include "color_list_widget.hpp"
#include <QHeaderView>



Color_List_Widget_Item::Color_List_Widget_Item(int row, QColor color,
                                               Color_List_Widget* parent)
    : parent(parent), row(row), selector(new Color_Selector),
      up(new QToolButton), down(new QToolButton), remove(new QToolButton)
{

    selector->setColor(color);
    up->setIcon(QIcon::fromTheme("go-up"));
    up->setText(tr("Move Up"));
    down->setIcon(QIcon::fromTheme("go-down"));
    down->setText(tr("Move Down"));
    remove->setIcon(QIcon::fromTheme("list-remove"));
    remove->setText(tr("Remove"));

    connect(up,SIGNAL(clicked()),SLOT(up_clicked()));
    connect(down,SIGNAL(clicked()),SLOT(down_clicked()));
    connect(remove,SIGNAL(clicked()),SLOT(remove_clicked()));
    connect(selector,SIGNAL(colorChanged(QColor)),SLOT(color_changed(QColor)));
    connect(parent,SIGNAL(removed(int)),SLOT(row_removed(int)));
}

void Color_List_Widget_Item::create_row( QColor color, Color_List_Widget* parent)
{

    int row = parent->rowCount();
    parent->insertRow(row);

    Color_List_Widget_Item *item = new Color_List_Widget_Item(row, color,parent);

    parent->setRowHeight(row,24);

    parent->setCellWidget(row,0,item->selector);
    parent->setCellWidget(row,1,item->up);
    parent->setCellWidget(row,2,item->down);
    parent->setCellWidget(row,3,item->remove);

    QTableWidgetItem* twit = new QTableWidgetItem;
    twit->setData(Qt::UserRole,QVariant(QMetaType::QObjectStar, item));
}

void Color_List_Widget_Item::remove_clicked()
{
    parent->remove(row);
    row = -1;
    // avoid deleting this within slot
    parent->old.push_back(this);
}

void Color_List_Widget_Item::up_clicked()
{
    QTableWidgetItem* twit = parent->item(row-1,0);
    if ( twit )
    {
        Color_List_Widget_Item* item = qobject_cast<Color_List_Widget_Item*>(
                        qvariant_cast<QObject *>(twit->data(Qt::UserRole)));
        if ( item )
        {
            // swap colors
            QColor c = item->selector->color();
            item->selector->setColor(selector->color());
            selector->setColor(c);
        }
    }
}

void Color_List_Widget_Item::down_clicked()
{
    QTableWidgetItem* twit = parent->item(row+1,0);
    if ( twit )
    {
        Color_List_Widget_Item* item = qobject_cast<Color_List_Widget_Item*>(
                        qvariant_cast<QObject *>(twit->data(Qt::UserRole)));
        if ( item )
        {
            // swap colors
            QColor c = item->selector->color();
            item->selector->setColor(selector->color());
            selector->setColor(c);
        }
    }
}


void Color_List_Widget_Item::color_changed(QColor c)
{
    emit parent->colorChanged(row,c);
}

void Color_List_Widget_Item::row_removed(int orow )
{
    if ( row > 0 && orow < row )
            row--;
}



Color_List_Widget::Color_List_Widget(QWidget *parent) :
    QTableWidget(parent)
{
    insertColumn(0);
    insertColumn(1);
    insertColumn(2);
    insertColumn(3);

    setColumnWidth(0,128);
    setColumnWidth(1,24);
    setColumnWidth(2,24);
    setColumnWidth(3,24);

    horizontalHeader()->hide();
    verticalHeader()->hide();
}

void Color_List_Widget::append(QColor c)
{
    foreach(Color_List_Widget_Item* item,old)
        delete item;
    old.clear();

    Color_List_Widget_Item::create_row(c,this);
}

void Color_List_Widget::setColor(int row, QColor c)
{
    Color_Selector *selector = qobject_cast<Color_Selector*>(cellWidget(row,0));
    if ( selector )
    {
        selector->setColor(c);
        emit colorChanged(row,c);
    }
}

QColor Color_List_Widget::color(int row) const
{
    Color_Selector *selector = qobject_cast<Color_Selector*>(cellWidget(row,0));
    if ( selector )
        return selector->color();
    return QColor();
}


void Color_List_Widget::remove(int row)
{
    removeRow(row);
    emit removed(row);
    if ( rowCount() == 0 && default_color.isValid() )
        append(default_color);
}

