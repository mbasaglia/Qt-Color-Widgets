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

#include "color_list_widget.hpp"
#include "color_selector.hpp"

class Color_List_Widget::Private
{
public:
    QList<QColor>               colors;
    QSignalMapper               mapper;
    Color_Wheel::Display_Flags  wheel_flags;
};

Color_List_Widget::Color_List_Widget(QWidget *parent)
    : Abstract_Widget_List(parent), p(new Private)
{
    connect(this, SIGNAL(removed(int)), SLOT(handle_removed(int)));
    connect(&p->mapper, SIGNAL(mapped(int)), SLOT(color_changed(int)));
    p->wheel_flags = Color_Wheel::defaultDisplayFlags();
}

Color_List_Widget::~Color_List_Widget()
{
    delete p;
}

QList<QColor> Color_List_Widget::colors() const
{
    return p->colors;
}

void Color_List_Widget::setColors(const QList<QColor> &colors)
{
    clear();
    p->colors = colors;
    for(int i = 0;i < colors.size();i++ )
        append_widget(i);
    emit colorsChanged(colors);
}

void Color_List_Widget::swap(int a, int b)
{
    Color_Selector* sa = widget_cast<Color_Selector>(a);
    Color_Selector* sb = widget_cast<Color_Selector>(b);
    if ( sa && sb )
    {
        QColor ca = sa->color();
        sa->setColor(sb->color());
        sb->setColor(ca);
        emit colorsChanged(p->colors);
    }
}

void Color_List_Widget::append()
{
    p->colors.push_back(Qt::black);
    append_widget(p->colors.size()-1);
    emit colorsChanged(p->colors);
}

void Color_List_Widget::emit_changed()
{
    emit colorsChanged(p->colors);
}

void Color_List_Widget::handle_removed(int i)
{
    p->colors.removeAt(i);
    emit colorsChanged(p->colors);
}

void Color_List_Widget::color_changed(int row)
{
    Color_Selector *cs = widget_cast<Color_Selector>(row);
    if ( cs )
    {
        p->colors[row] = cs->color();
        emit colorsChanged(p->colors);
    }
}

void Color_List_Widget::append_widget(int col)
{
    Color_Selector* cbs = new Color_Selector;
    cbs->setDisplayMode(Color_Preview::AllAlpha);
    cbs->setColor(p->colors[col]);
    //connect(cbs,SIGNAL(colorChanged(QColor)),SLOT(emit_changed()));
    p->mapper.setMapping(cbs,col);
    connect(cbs,SIGNAL(colorChanged(QColor)),&p->mapper,SLOT(map()));
    connect(this,SIGNAL(wheelFlagsChanged(Color_Wheel::Display_Flags)),
            cbs,SLOT(setWheelFlags(Color_Wheel::Display_Flags)));
    appendWidget(cbs);
    setRowHeight(count()-1,22);
}

Color_Wheel::Display_Flags Color_List_Widget::wheelFlags() const
{
    return p->wheel_flags;
}

void Color_List_Widget::setWheelFlags(Color_Wheel::Display_Flags flags)
{
    if ( p->wheel_flags != flags )
    {
        p->wheel_flags = flags;
        emit wheelFlagsChanged(flags);
    }
}
