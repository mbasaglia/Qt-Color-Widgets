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
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <QDragEnterEvent>

namespace color_widgets {

class Swatch::Private
{
public:
    ColorPalette palette; ///< Palette with colors and related metadata
    int selected; ///< Current selection index (-1 for no selection)

    QPoint drag_pos; ///< Point used to keep track of dragging
    int drag_index; ///< Index used by drags

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
 *      * event on doubleclick (to allow changing the color)
 */
Swatch::Swatch(QWidget* parent)
    : QWidget(parent), p(new Private)
{
    p->selected = -1;
    p->drag_index = -1;
    connect(&p->palette, SIGNAL(colorsChanged(QVector<QColor>)),SLOT(paletteModified()));
    connect(&p->palette, SIGNAL(columnsChanged(int)),SLOT(update()));
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
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

void Swatch::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        setSelected(indexAt(event->pos()));
        p->drag_pos = event->pos();
        p->drag_index = indexAt(event->pos());
    }
}

void Swatch::mouseMoveEvent(QMouseEvent *event)
{
    if ( p->drag_index != -1 &&  (event->buttons() & Qt::LeftButton) &&
        (p->drag_pos - event->pos()).manhattanLength() >= QApplication::startDragDistance() )
    {
        QColor color = p->palette.colorAt(p->drag_index);

        QPixmap preview(24,24);
        preview.fill(color);

        QMimeData *mimedata = new QMimeData;
        mimedata->setColorData(color);
        mimedata->setText(p->palette.nameAt(p->drag_index));

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimedata);
        drag->setPixmap(preview);
        drag->exec(Qt::CopyAction|Qt::MoveAction);
    }
}

void Swatch::mouseReleaseEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        p->drag_index = -1;
    }
}

void Swatch::dragEnterEvent(QDragEnterEvent *event)
{
    if ( event->mimeData()->hasColor() ||
         ( event->mimeData()->hasText() && QColor(event->mimeData()->text()).isValid() ) )
    {
        if ( event->proposedAction() == Qt::MoveAction )
            event->setDropAction(Qt::MoveAction);
        else
            event->setDropAction(Qt::CopyAction);

        event->accept();
    }
}

void dragMoveEvent(QDropEvent* event)
{
}

/**
 * \todo
 *      * For move select new color or overwrite based on the position
 *      * Show drop location on paint
 */
void Swatch::dropEvent(QDropEvent *event)
{
    QColor color;
    QString name;

    // Gather up the color
    if ( event->mimeData()->hasColor() )
    {
        color = event->mimeData()->colorData().value<QColor>();
        if ( event->mimeData()->hasText() )
            name = event->mimeData()->text();
    }
    else if ( event->mimeData()->hasText() )
    {
        color = QColor(event->mimeData()->text());
    }

    // Not a color, discard
    if ( !color.isValid() )
        return;

    // Find the output location
    int drop_index = indexAt(event->pos());
    if ( drop_index == -1 )
        drop_index = p->palette.count();

    // Move unto self
    if ( event->dropAction() == Qt::MoveAction && event->source() == this )
    {
        // Not moved => noop
        /// \todo Disallow this instead of accepting
        if ( drop_index == p->drag_index + 1 )
        {
            // Finalize
            event->accept();
            p->drag_index = -1;
            return;
        }
        // Erase the old color
        p->palette.eraseColor(p->drag_index);
        if ( drop_index > p->drag_index )
            drop_index--;
        p->selected = drop_index;
        // Insert the dropped color
        p->palette.insertColor(drop_index, color, name);
    }
    /*// Move into a color cell
    else if ( event->dropAction() == Qt::MoveAction && drop_index < p->palette.count() )
    {
        p->palette.setColorAt(drop_index, color, name);
    }*/
    // Insert the dropped color
    else
    {
        p->palette.insertColor(drop_index, color, name);
    }

    // Finalize
    event->accept();
    p->drag_index = -1;
}

void Swatch::paletteModified()
{
    if ( p->selected >= p->palette.count() )
        clearSelection();
    update();
}

} // namespace color_widgets
