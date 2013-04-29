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
#include <QVBoxLayout>


Color_List_Widget_Item::Color_List_Widget_Item(int row, QColor color,
                                               Color_List_Widget* parent)
    : parent(parent), row(row), selector(new Color_Selector),
      up(new QToolButton), down(new QToolButton), remove(new QToolButton)
{

    selector->setColor(color);
    up->setIcon(QIcon::fromTheme("go-up"));
    up->setText(tr("Move Up"));
    up->setToolTip(up->text());
    down->setIcon(QIcon::fromTheme("go-down"));
    down->setText(tr("Move Down"));
    down->setToolTip(down->text());
    remove->setIcon(QIcon::fromTheme("list-remove"));
    remove->setText(tr("Remove"));
    remove->setToolTip(tr("Remove Color"));

    connect(up,SIGNAL(clicked()),SLOT(up_clicked()));
    connect(down,SIGNAL(clicked()),SLOT(down_clicked()));
    connect(remove,SIGNAL(clicked()),SLOT(remove_clicked()));
    connect(selector,SIGNAL(colorChanged(QColor)),SLOT(color_changed(QColor)));
    connect(parent,SIGNAL(removed(int)),SLOT(row_removed(int)));
}

void Color_List_Widget_Item::create_row( int row, QColor color, Color_List_Widget* parent)
{
    parent->table->insertRow(row);

    Color_List_Widget_Item *item = new Color_List_Widget_Item(row, color,parent);

    parent->table->setRowHeight(row,24);

    parent->table->setCellWidget(row,0,item->selector);
    parent->table->setCellWidget(row,1,item->up);
    parent->table->setCellWidget(row,2,item->down);
    parent->table->setCellWidget(row,3,item->remove);

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
    if ( row > 0 )
    {
        QColor c = parent->color(row-1);
        parent->setColor(row-1,selector->color());
        parent->setColor(row,c);
    }
}

void Color_List_Widget_Item::down_clicked()
{
    if ( row+1 < parent->count() )
    {
        QColor c = parent->color(row+1);
        parent->setColor(row+1,selector->color());
        parent->setColor(row,c);
    }
}

void Color_List_Widget_Item::color_changed(QColor c)
{
    parent->m_colors[row] = c;
    emit parent->colorChanged(row,c);
}

void Color_List_Widget_Item::row_removed(int orow )
{
    if ( row > 0 && orow < row )
            row--;
}


Color_List_Widget::Color_List_Widget(QWidget *parent) :
    QWidget(parent), table ( new QTableWidget(this) )
{
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    table = new QTableWidget(this);
    verticalLayout->addWidget(table);


    table->insertColumn(0);
    table->insertColumn(1);
    table->insertColumn(2);
    table->insertColumn(3);

    table->setColumnWidth(0,128);
    table->setColumnWidth(1,24);
    table->setColumnWidth(2,24);
    table->setColumnWidth(3,24);

    table->horizontalHeader()->hide();
    table->verticalHeader()->hide();
}

void Color_List_Widget::addColor(QColor c)
{
    foreach(Color_List_Widget_Item* item,old)
        delete item;
    old.clear();
    Color_List_Widget_Item::create_row(count(),c,this);
    m_colors.push_back(c);
    emit colorsChanged(m_colors);
}

void Color_List_Widget::setColor(int row, QColor c)
{
    Color_Selector *selector = qobject_cast<Color_Selector*>(table->cellWidget(row,0));
    if ( selector )
    {
        selector->setColor(c);
        m_colors[row] = c;
        emit colorChanged(row,c);
        emit colorsChanged(m_colors);
    }
}

QColor Color_List_Widget::color(int row) const
{
    if ( row >= 0 && row < count() )
        return m_colors[row];
    return QColor();
}

int Color_List_Widget::count() const
{
    return m_colors.size();
}


void Color_List_Widget::remove(int row)
{
    m_colors.removeAt(row);
    table->removeRow(row);
    emit removed(row);
    emit colorsChanged(m_colors);
}

void Color_List_Widget::setColors(const QList<QColor> &c)
{
    while ( table->rowCount() > 0 )
        table->removeRow(0);

    for(int i = 0; i < c.size() ; i++ )
        Color_List_Widget_Item::create_row(i,c[i],this);

    m_colors = c;
    emit colorsChanged(m_colors);
}

