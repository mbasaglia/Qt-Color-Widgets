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
#ifndef COLOR_WIDGETS_SWATCH_HPP
#define COLOR_WIDGETS_SWATCH_HPP

#include <QWidget>
#include "color_palette.hpp"

namespace color_widgets {

/**
 * \brief A widget drawing a palette
 */
class Swatch : public QWidget
{
    Q_OBJECT

    /**
     * \brief Palette shown by the widget
     */
    Q_PROPERTY(const ColorPalette& palette READ palette WRITE setPalette NOTIFY paletteChanged)
    /**
     * \brief Currently selected color (-1 if no color is selected)
     */
    Q_PROPERTY(int selected READ selected WRITE setSelected NOTIFY selectedChanged)


public:
    Swatch(QWidget* parent = 0);
    ~Swatch();

    const ColorPalette& palette() const;
    ColorPalette& palette();
    int selected() const;
    /**
     * \brief Color at the currently selected index
     */
    QColor selectedColor() const;

    /**
     * \brief Color index at the given position within the widget
     * \param p Point in local coordinates
     * \returns -1 if the position doesn't represent any color
     */
    int indexAt(const QPoint& p);

    /**
     * \brief Color at the given position within the widget
     * \param p Point in local coordinates
     */
    QColor colorAt(const QPoint& p);

public slots:
    void setPalette(const ColorPalette& palette);
    void setSelected(int selected);
    void clearSelection();

signals:
    void paletteChanged(const ColorPalette& palette);
    void selectedChanged(int selected);
    void colorSelected(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    class Private;
    Private* p;
};


} // namespace color_widgets
#endif // COLOR_WIDGETS_SWATCH_HPP
