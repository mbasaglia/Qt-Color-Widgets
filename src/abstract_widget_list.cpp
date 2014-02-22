/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#include "abstract_widget_list.hpp"
#include <QToolButton>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>

Abstract_Widget_List::Abstract_Widget_List(QWidget *parent) :
    QWidget(parent)
{
    connect(&mapper_up,SIGNAL(mapped(QWidget*)),SLOT(up_clicked(QWidget*)));
    connect(&mapper_down,SIGNAL(mapped(QWidget*)),SLOT(down_clicked(QWidget*)));
    connect(&mapper_remove,SIGNAL(mapped(QWidget*)),SLOT(remove_clicked(QWidget*)));


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
    table->setShowGrid(false);

    QPushButton* add_button = new QPushButton(QIcon::fromTheme("list-add"),
                                              tr("Add New"));

    verticalLayout->addWidget(add_button);
    connect(add_button,SIGNAL(clicked()),SLOT(append()));

}

void Abstract_Widget_List::setRowHeight(int row, int height)
{
    table->setRowHeight(row,height);
}

void Abstract_Widget_List::clear()
{
    widgets.clear();
    while(table->rowCount() > 0)
        table->removeRow(0);
}


void Abstract_Widget_List::remove(int i)
{
    if ( isValidRow(i) )
    {
        widgets.removeAt(i);
        table->removeRow(i);
        if ( i == 0 && !widgets.isEmpty() )
            table->cellWidget(0,1)->setEnabled(false);
        else if ( i != 0 && i == count() )
            table->cellWidget(count()-1,2)->setEnabled(false);

        emit removed(i);
    }
}


void Abstract_Widget_List::appendWidget(QWidget *w)
{
    int row = count();
    table->insertRow(row);

    QWidget* b_up = create_button(w,&mapper_up,"go-up",tr("Move Up"));
    QWidget* b_down = create_button(w,&mapper_down,"go-down",tr("Move Down"));
    QWidget* b_remove = create_button(w,&mapper_remove,"list-remove",tr("Remove"));
    if ( row == 0 )
        b_up->setEnabled(false);
    else
        table->cellWidget(row-1,2)->setEnabled(true);
    b_down->setEnabled(false);

    table->setCellWidget(row,0,w);
    table->setCellWidget(row,1,b_up);
    table->setCellWidget(row,2,b_down);
    table->setCellWidget(row,3,b_remove);

    widgets.push_back(w);
}

QWidget *Abstract_Widget_List::widget(int i)
{
    if ( isValidRow(i) )
        return widgets[i];
    return 0;
}


QWidget *Abstract_Widget_List::create_button(QWidget *data, QSignalMapper *mapper,
                                             QString icon_name,
                                             QString text, QString tooltip) const
{

    QToolButton* btn = new QToolButton;
    btn->setIcon(QIcon::fromTheme(icon_name));
    btn->setText(text);
    btn->setToolTip(tooltip.isNull() ? btn->text() : tooltip );
    connect(btn,SIGNAL(clicked()),mapper,SLOT(map()));
    mapper->setMapping(btn,data);
    return btn;
}

void Abstract_Widget_List::remove_clicked(QWidget *w)
{
    int row = widgets.indexOf(w);
    remove(row);
}

void Abstract_Widget_List::up_clicked(QWidget *w)
{

    int row = widgets.indexOf(w);
    if ( row > 0 )
        swap(row,row-1);
}

void Abstract_Widget_List::down_clicked(QWidget *w)
{
    int row = widgets.indexOf(w);
    if ( row+1 < count() )
        swap(row,row+1);
}
