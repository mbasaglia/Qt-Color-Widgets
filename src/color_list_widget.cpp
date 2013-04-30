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
#include "bound_color_selector.hpp"

Color_List_Widget::Color_List_Widget(QWidget *parent)
    : Abstract_Widget_List(parent)
{
    connect(this,SIGNAL(removed(int)),SLOT(emit_changed()));
}

void Color_List_Widget::setColors(const QList<QColor> &cols)
{
    clear();
    m_colors = cols;
    for( int i = 0; i < m_colors.size(); i++ )
        append_widget(&m_colors[i]);
    emit colorsChanged(m_colors);
}

void Color_List_Widget::swap(int a, int b)
{
    Bound_Color_Selector* sa = widget_cast<Bound_Color_Selector>(a);
    Bound_Color_Selector* sb = widget_cast<Bound_Color_Selector>(b);
    if ( sa && sb )
    {
        QColor ca = sa->color();
        sa->setColor(sb->color());
        sb->setColor(ca);
        emit colorsChanged(m_colors);
    }
}

void Color_List_Widget::append()
{
    m_colors.push_back(Qt::black);
    append_widget(&m_colors.back());
    emit colorsChanged(m_colors);
}

void Color_List_Widget::emit_changed()
{
    emit colorsChanged(m_colors);
}

void Color_List_Widget::append_widget(QColor *col)
{
    Bound_Color_Selector* cbs = new Bound_Color_Selector(col);
    connect(cbs,SIGNAL(colorChanged(QColor)),SLOT(emit_changed()));
    appendWidget(cbs);
}
