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

#ifndef COLOR_LIST_WIDGET_HPP
#define COLOR_LIST_WIDGET_HPP

#include <QTableWidget>
#include "color_selector.hpp"
#include <QToolButton>


class Color_List_Widget;

class Color_List_Widget_Item : public QObject
{
    Q_OBJECT
private:
    Color_List_Widget*  parent;
    int                 row;
    Color_Selector*     selector;
    QToolButton*        up;
    QToolButton*        down;
    QToolButton*        remove;

public:

    Color_List_Widget_Item(int row, QColor color, Color_List_Widget *parent);

    static void create_row( QColor color, Color_List_Widget* parent);

private slots:
    void remove_clicked();
    void up_clicked();
    void down_clicked();
    void color_changed(QColor);
    void row_removed(int);
};

class Color_List_Widget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor default_color READ defaultColor WRITE setDefaultColor)

    friend class Color_List_Widget_Item;

private:
    QTableWidget* table;
    QColor default_color; ///< If invalid, no default color
    QList<Color_List_Widget_Item*> old; ///< Items to be deleted


public:
    explicit Color_List_Widget(QWidget *parent = 0);
    
    void setColor(int row,QColor c);
    QColor color(int row) const;
    /// Get the number of colors
    int count() const;
    /**
     *  \brief remove given row
     *
     *  If the last row is removed and defaultColor is valid, a new row with the
     *  default color is created
    */
    void remove(int row);

    void setDefaultColor(QColor color) { default_color = color; }
    QColor defaultColor() const { return default_color; }

signals:
    void append(QColor c);
    void colorChanged(int row,QColor c);
    void removed(int);
};


#endif // COLOR_LIST_WIDGET_HPP
