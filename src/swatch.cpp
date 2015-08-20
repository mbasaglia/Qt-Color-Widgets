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

#include <cmath>
#include <QPainter>
#include <QMouseEvent>

namespace color_widgets {

class Swatch::Private
{
public:
    ColorPalette palette;
    int selected;

    /**
     * \brief Number of rows/columns in the palette
     */
    QSize rowcols()
    {
        int count = palette.count();
        if ( count == 0 )
            return QSize();

        int columns = palette.columns();

        /// \todo evaluate the proper number of columns based on the geometry
        if ( columns == -1 )
            columns = 8;

        int rows = std::ceil( float(count) / columns );

        return QSize(columns, rows);
    }
};

/**
 * \todo
 *      * Drag/drop
 *      * connect p->palette to update() and change selected
 *      * event on doubleclick (to allow changing the color)
 */
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

int Swatch::indexAt(const QPoint& pt)
{
    QSize rowcols = p->rowcols();
    if ( rowcols.isEmpty() )
        return -1;

    QRect geo = rect();
    QSize color_size(geo.width()/rowcols.width(), geo.height()/rowcols.height());
    QPoint point(
        qBound(geo.left(), pt.x(), geo.right()) / color_size.width(),
        qBound(geo.top(), pt.y(), geo.bottom()) / color_size.height()
    );

    int index = point.y() * rowcols.width() + point.x();
    if ( index >= p->palette.count() )
        return -1;
    return index;
}

QColor Swatch::colorAt(const QPoint& pt)
{
    return p->palette.colorAt(indexAt(pt));
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
    QSize rowcols = p->rowcols();
    if ( rowcols.isEmpty() )
        return;

    QSizeF color_size(width()/rowcols.width(), height()/rowcols.height());
    QPainter painter(this);
    int count = p->palette.count();
    for ( int y = 0, i = 0; i < count; y++ )
    {
        for ( int x = 0; x < rowcols.width() && i < count; x++, i++ )
        {
            QRectF rect(QPointF(x*color_size.width(), y*color_size.height()), color_size);
            painter.fillRect(rect, p->palette.colorAt(i));
        }
    }

    if ( p->selected != -1 )
    {
        int x = p->selected % rowcols.width();
        int y = p->selected / rowcols.width();
        QRectF rect(QPointF(x*color_size.width(), y*color_size.height()), color_size);
        painter.setBrush(Qt::transparent);
        painter.setPen(QPen(Qt::darkGray, 2));
        painter.drawRect(rect);
        painter.setPen(QPen(Qt::gray, 2, Qt::DotLine));
        painter.drawRect(rect);
    }
}

void Swatch::mouseReleaseEvent(QMouseEvent *event)
{
    setSelected(indexAt(event->pos()));
}

} // namespace color_widgets
