/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013 Mattia Basaglia

    This file is part of Color Widgets.

    Color Widgets is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Color Widgets is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef COLOR_PREVIEW_HPP
#define COLOR_PREVIEW_HPP

#include <QWidget>

/**
    Simple widget that shows a preview of a color
*/
class Color_Preview : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor DESIGNABLE true)
    Q_PROPERTY(Alpha_Mode alpha_mode READ alphaMode WRITE setAlphaMode DESIGNABLE true)
    Q_PROPERTY(QBrush background READ getBackground WRITE setBackground DESIGNABLE true)
    Q_ENUMS(Alpha_Mode)
public:
    enum Alpha_Mode
    {
        NoAlpha, ///< Show only solid color
        SplitAlpha, ///< Show both solid and transparent
        AllAlpha ///< show only transparent
    };

private:
    QColor col; ///< color to be viewed
    QBrush back;///< Background brush, visible on transaprent color
    Alpha_Mode alpha_mode;

public:
    explicit Color_Preview(QWidget *parent = 0);

    void setBackground(QBrush bk)
    {
        back = bk;
        update();
    }

    QBrush getBackground() const
    {
        return back;
    }

    Alpha_Mode alphaMode() const
    {
        return alpha_mode;
    }

    void setAlphaMode(Alpha_Mode am)
    {
        alpha_mode = am;
        update();
    }

    QColor color() const
    {
        return col;
    }

    QSize sizeHint () const;
    
public slots:
    void setColor(QColor c);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

};

#endif // COLOR_PREVIEW_HPP
