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
#include <QKeyEvent>

namespace color_widgets {

class Swatch::Private
{
public:
    ColorPalette palette; ///< Palette with colors and related metadata
    int selected; ///< Current selection index (-1 for no selection)

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
 *      * event on doubleclick (to allow changing the color)
 */
Swatch::Swatch(QWidget* parent)
    : QWidget(parent), p(new Private)
{
    p->selected = -1;
    connect(&p->palette, SIGNAL(colorsChanged(QVector<QColor>)),SLOT(paletteModified()));
    connect(&p->palette, SIGNAL(columnsChanged(int)),SLOT(update()));
    setFocusPolicy(Qt::StrongFocus);
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

    QSizeF color_size(float(width()) / rowcols.width(),
                      float(height()) / rowcols.height());
    QPoint point(
        qBound<int>(0, pt.x() / color_size.width(), rowcols.width() - 1),
        qBound<int>(0, pt.y() / color_size.height(), rowcols.height() - 1)
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

    QSizeF color_size(float(width())/rowcols.width(), float(height())/rowcols.height());
    QPainter painter(this);
    painter.setBrush(Qt::transparent);

    int count = p->palette.count();
    for ( int y = 0, i = 0; i < count; y++ )
    {
        for ( int x = 0; x < rowcols.width() && i < count; x++, i++ )
        {
            QRectF rect(QPointF(x*color_size.width(), y*color_size.height()), color_size);
            painter.fillRect(rect, p->palette.colorAt(i));
        }
    }

    if ( hasFocus() )
    {
        QColor outline = QWidget::palette().color(QPalette::Highlight);
        painter.setPen(QPen(outline, 0));
        painter.drawRect(rect().adjusted(0,0,-1,-1));
    }

    if ( p->selected != -1 )
    {
        int x = p->selected % rowcols.width();
        int y = p->selected / rowcols.width();
        QRectF rect(QPointF(x*color_size.width(), y*color_size.height()), color_size);
        painter.setPen(QPen(Qt::darkGray, 2));
        painter.drawRect(rect);
        painter.setPen(QPen(Qt::gray, 2, Qt::DotLine));
        painter.drawRect(rect);
    }
}

void Swatch::keyPressEvent(QKeyEvent* event)
{
    if ( p->palette.count() == 0 )
        QWidget::keyPressEvent(event);

    int selected = p->selected;
    int count = p->palette.count();
    int columns = p->palette.columns();
    switch ( event->key() )
    {
        default:
            QWidget::keyPressEvent(event);
            return;

        case Qt::Key_Left:
            if ( selected == -1 )
                selected = count - 1;
            else if ( selected > 0 )
                selected--;
            break;

        case Qt::Key_Right:
            if ( selected == -1 )
                selected = 0;
            else if ( selected < count - 1 )
                selected++;
            break;

        case Qt::Key_Up:
            if ( selected == -1 )
                selected = count - 1;
            else if ( selected >= columns )
                selected -= columns;
            break;

        case Qt::Key_Down:
            if ( selected == -1 )
                selected = 0;
            else if ( selected < count - columns )
                selected += columns;
            break;

        case Qt::Key_Home:
            if ( event->modifiers() & Qt::ControlModifier )
                selected = 0;
            else
                selected -= selected % columns;
            break;

        case Qt::Key_End:
            if ( event->modifiers() & Qt::ControlModifier )
                selected = count - 1;
            else
                selected += columns - (selected % columns) - 1;
            break;

    }
    setSelected(selected);

}

void Swatch::mouseReleaseEvent(QMouseEvent *event)
{
    setSelected(indexAt(event->pos()));
}

void Swatch::paletteModified()
{
    if ( p->selected >= p->palette.count() )
        clearSelection();
    update();
}

} // namespace color_widgets
