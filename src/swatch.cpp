/**

@author Mattia Basaglia

@section License

    Copyright (C) 2015 Mattia Basaglia

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "swatch.hpp"

#include <QPainter>
#include <cmath>

namespace color_widgets {

class Swatch::Private
{
public:
    ColorPalette palette;
    int selected;
};

Swatch::Swatch(QWidget* parent)
    : QWidget(parent), p(new Private)
{
    p->selected = -1;
}

Swatch::~Swatch()
{
    delete p;
}

const ColorPalette& Swatch::palette() const
{
    return p->palette;
}
ColorPalette& Swatch::palette()
{
    return p->palette;
}
int Swatch::selected() const
{
    return p->selected;
}
QColor Swatch::selectedColor() const
{
    return p->palette.colorAt(p->selected);
}

void Swatch::setPalette(const ColorPalette& palette)
{
    p->palette = palette;
    update();
    emit paletteChanged(p->palette);
}
void Swatch::setSelected(int selected)
{
    if ( selected < 0 || selected >= p->palette.count() )
        selected = -1;
    if ( selected != p->selected )
    {
        emit selectedChanged( p->selected = selected );
        emit colorSelected( p->palette.colorAt(p->selected) );
        update();
    }
}
void Swatch::clearSelection()
{
    setSelected(-1);
}
void Swatch::paintEvent(QPaintEvent* event)
{
    int count = p->palette.count();

    if ( count == 0 )
        return;

    int columns = p->palette.columns();

    /// \todo evaluate the proper number of columns based on the geometry
    if ( columns == -1 )
        columns = 8;

    int rows = std::ceil( float(count) / columns );

    QSizeF color_size(width()/columns, height()/rows);
    QPainter painter(this);
    for ( int y = 0, i = 0; i < count; y++ )
        for ( int x = 0; x < columns && i < count; x++, i++ )
        {
            QRectF rect(QPointF(x*color_size.width(), y*color_size.height()), color_size);
            painter.fillRect(rect, p->palette.colorAt(i));
        }
}

} // namespace color_widgets
