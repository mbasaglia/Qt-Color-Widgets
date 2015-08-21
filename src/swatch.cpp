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
#include <QStyleOption>

namespace color_widgets {

class Swatch::Private
{
public:
    ColorPalette palette;    ///< Palette with colors and related metadata
    int          selected;   ///< Current selection index (-1 for no selection)
    QSize        color_size; ///< Preferred size for the color squares

    QPoint  drag_pos;       ///< Point used to keep track of dragging
    int     drag_index;     ///< Index used by drags
    int     drop_index;     ///< Index for a requested drop
    QColor  drop_color;     ///< Dropped color
    bool    drop_overwrite; ///< Whether the drop will overwrite an existing color

    Swatch* owner;

    Private(Swatch* owner)
        : selected(-1),
          color_size(16,16),
          drag_index(-1),
          drop_index(-1),
          drop_overwrite(false),
          owner(owner)
    {}

    /**
     * \brief Number of rows/columns in the palette
     */
    QSize rowcols()
    {
        int count = palette.count();
        if ( count == 0 )
            return QSize();

        int columns = palette.columns();

        if ( columns == -1 )
            columns = qMin(palette.count(), owner->width() / color_size.width());

        int rows = std::ceil( float(count) / columns );

        return QSize(columns, rows);
    }

    /**
     * \brief Sets the drop properties
     */
    void dropEvent(QDropEvent* event)
    {
        // Find the output location
        drop_index = owner->indexAt(event->pos());
        if ( drop_index == -1 )
            drop_index = palette.count();

        // Gather up the color
        if ( event->mimeData()->hasColor() )
        {
            drop_color = event->mimeData()->colorData().value<QColor>();
            drop_color.setAlpha(255);
        }
        else if ( event->mimeData()->hasText() )
        {
            drop_color = QColor(event->mimeData()->text());
        }

        drop_overwrite = false;
        QRectF drop_rect = indexRect(drop_index);
        if ( drop_index < palette.count() && drop_rect.isValid() )
        {
            // Dragged to the last quarter of the size of the square, add after
            if ( event->posF().x() >= drop_rect.left() + drop_rect.width() * 3.0 / 4 )
                drop_index++;
            // Dragged to the middle of the square, overwrite existing color
            else if ( event->posF().x() > drop_rect.left() + drop_rect.width() / 4 &&
                    ( event->dropAction() != Qt::MoveAction || event->source() != owner ) )
                drop_overwrite = true;
        }

        owner->update();
    }

    /**
     * \brief Clears drop properties
     */
    void clearDrop()
    {
        drop_index = -1;
        drop_color = QColor();
        drop_overwrite = false;

        owner->update();
    }

    /**
     * \brief Actual size of a color square
     */
    QSizeF actualColorSize()
    {
        QSize rowcols = this->rowcols();
        if ( !rowcols.isValid() )
            return QSizeF();
        return actualColorSize(rowcols);
    }

    /**
     * \brief Actual size of a color square
     * \pre rowcols.isValid() and obtained via rowcols()
     */
    QSizeF actualColorSize(const QSize& rowcols)
    {
        return QSizeF (float(owner->width()) / rowcols.width(),
                       float(owner->height()) / rowcols.height());
    }


    /**
     * \brief Rectangle corresponding to the color at the given index
     * \pre rowcols.isValid() and obtained via rowcols()
     * \pre color_size obtained via rowlcols(rowcols)
     */
    QRectF indexRect(int index, const QSize& rowcols, const QSizeF& color_size)
    {
        if ( index == -1 )
            return QRectF();

        return QRectF(
            index % rowcols.width() * color_size.width(),
            index / rowcols.width() * color_size.height(),
            color_size.width(),
            color_size.height()
        );
    }
    /**
     * \brief Rectangle corresponding to the color at the given index
     */
    QRectF indexRect(int index)
    {
        QSize rc = rowcols();
        if ( index == -1 || !rc.isValid() )
            return QRectF();
        return indexRect(index, rc, actualColorSize(rc));
    }
};

/**
 * \todo
 *      * event on doubleclick (to allow changing the color)
 */
Swatch::Swatch(QWidget* parent)
    : QWidget(parent), p(new Private(this))
{
    connect(&p->palette, SIGNAL(colorsChanged(QVector<QColor>)),SLOT(paletteModified()));
    connect(&p->palette, SIGNAL(columnsChanged(int)),SLOT(update()));
    setFocusPolicy(Qt::StrongFocus);
    setAcceptDrops(true);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

Swatch::~Swatch()
{
    delete p;
}

QSize Swatch::sizeHint() const
{
    QSize rowcols = p->rowcols();

    if ( !p->color_size.isValid() || !rowcols.isValid() )
        return QSize();

    return QSize(
        p->color_size.width()  * rowcols.width(),
        p->color_size.height() * rowcols.height()
    );
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

    QSizeF color_size = p->actualColorSize(rowcols);

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

    QSizeF color_size = p->actualColorSize(rowcols);
    QPainter painter(this);

    // For some reason QStyle::PE_Frame doesn't work propely with custom widgets
    // (At least on my theme)
    // ie: it doesn't change color depending on the focus status
    // It works fine in GradientSlider because it inherits QSlider.
    // The problem is not in the styleoption, but depends on the last
    // argument to QStyle::drawPrimitive()
    // For these reason, this uses PE_PanelLineEdit,
    // which appears to be working fine.
    QStyleOptionFrame panel;
    panel.initFrom(this);
    panel.lineWidth = 1;
    //panel.state |= QStyle::State_Sunken;
    style()->drawPrimitive(QStyle::PE_PanelLineEdit, &panel, &painter, this);
    QRect r = style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);
    painter.setClipRect(r);

    int count = p->palette.count();
    for ( int y = 0, i = 0; i < count; y++ )
    {
        for ( int x = 0; x < rowcols.width() && i < count; x++, i++ )
        {
            QRectF rect(QPointF(x*color_size.width(), y*color_size.height()), color_size);
            painter.fillRect(rect, p->palette.colorAt(i));
        }
    }

    painter.setClipping(false);

    if ( p->drop_index != -1 )
    {
        QRectF drop_area = p->indexRect(p->drop_index, rowcols, color_size);
        if ( p->drop_overwrite )
        {
            painter.setBrush(p->drop_color);
            painter.setPen(QPen(Qt::gray));
            painter.drawRect(drop_area);
        }
        else
        {
            painter.setPen(QPen(p->drop_color, 2));
            painter.setBrush(Qt::transparent);
            painter.drawLine(drop_area.topLeft(), drop_area.bottomLeft());
            // Draw also on the previous line when the first item of a line is selected
            if ( p->drop_index % rowcols.width() == 0 && p->drop_index != 0 )
            {
                drop_area = p->indexRect(p->drop_index-1, rowcols, color_size);
                drop_area.translate(color_size.width(), 0);
                painter.drawLine(drop_area.topLeft(), drop_area.bottomLeft());
            }
        }
    }

    if ( p->selected != -1 )
    {
        QRectF rect = p->indexRect(p->selected, rowcols, color_size);
        painter.setBrush(Qt::transparent);
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
    p->dropEvent(event);

    if ( p->drop_color.isValid() && p->drop_index != -1 )
    {
        if ( event->proposedAction() == Qt::MoveAction && event->source() == this )
            event->setDropAction(Qt::MoveAction);
        else
            event->setDropAction(Qt::CopyAction);

        event->accept();
    }
}

void Swatch::dragMoveEvent(QDragMoveEvent* event)
{
    p->dropEvent(event);
}

void Swatch::dragLeaveEvent(QDragLeaveEvent *event)
{
    p->clearDrop();
}
/**
 * \todo
 *      * For move select new color or overwrite based on the position
 *      * Show drop location on paint
 */
void Swatch::dropEvent(QDropEvent *event)
{
    QString name;

    // Gather up the color
    if ( event->mimeData()->hasColor() && event->mimeData()->hasText() )
            name = event->mimeData()->text();

    // Not a color, discard
    if ( !p->drop_color.isValid() || p->drop_index == -1 )
        return;

    p->dropEvent(event);

    // Move unto self
    if ( event->dropAction() == Qt::MoveAction && event->source() == this )
    {
        // Not moved => noop
        /// \todo Disallow this instead of accepting
        if ( p->drop_index != p->drag_index + 1 )
        {
            // Erase the old color
            p->palette.eraseColor(p->drag_index);
            if ( p->drop_index > p->drag_index )
                p->drop_index--;
            p->selected = p->drop_index;
            // Insert the dropped color
            p->palette.insertColor(p->drop_index, p->drop_color, name);
        }
    }
    // Move into a color cell
    else if ( p->drop_overwrite )
    {
        p->palette.setColorAt(p->drop_index, p->drop_color, name);
    }
    // Insert the dropped color
    else
    {
        p->palette.insertColor(p->drop_index, p->drop_color, name);
    }

    // Finalize
    event->accept();
    p->drag_index = -1;
    p->clearDrop();
}

void Swatch::paletteModified()
{
    if ( p->selected >= p->palette.count() )
        clearSelection();
    update();
}

/// \todo policy to make this a hint, minimum, or fixed size
QSize Swatch::colorSize() const
{
    return p->color_size;
}

void Swatch::setColorSize(const QSize& colorSize)
{
    emit colorSizeChanged(p->color_size = colorSize);
}

} // namespace color_widgets
