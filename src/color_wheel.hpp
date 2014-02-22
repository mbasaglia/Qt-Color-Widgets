/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013-2014 Mattia Basaglia

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
#ifndef COLOR_WHEEL_HPP
#define COLOR_WHEEL_HPP

#include <QWidget>
#include <qmath.h>

/**
 * \brief Display an analog widget that allows the selection of a HSV color
 *
 * It has an outer wheel to select the Hue and an intenal square to select
 * Saturation and Lightness.
 */
class Color_Wheel : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE true STORED false )
    Q_PROPERTY(qreal hue READ hue WRITE setHue DESIGNABLE false )
    Q_PROPERTY(qreal saturation READ saturation WRITE setSaturation DESIGNABLE false )
    Q_PROPERTY(qreal value READ value WRITE setValue DESIGNABLE false )
    Q_PROPERTY(unsigned wheelWidth READ wheelWidth WRITE setWheelWidth DESIGNABLE true )

private:
    qreal huem, sat, val;
    unsigned wheel_width;
    enum Mouse_Status
    {
        Nothing,
        Drag_Circle,
        Drag_Square
    } mouse_status;
    QPixmap hue_ring;
    QImage sat_val_square;


public:
    explicit Color_Wheel(QWidget *parent = 0);

    /// Get current color
    QColor color() const;

    QSize sizeHint () const;

    /// Get current hue in the range [0-1]
    qreal hue() const { return huem; }

    /// Get current saturation in the range [0-1]
    qreal saturation() const { return sat; }

    /// Get current value in the range [0-1]
    qreal value() const { return val; }

    /// Get the width in pixels of the outer wheel
    unsigned wheelWidth() const { return wheel_width; }

    /// Set the width in pixels of the outer wheel
    void setWheelWidth(unsigned w);

public slots:

    /// Set current color
    void setColor(QColor c);

    /**
     * @param h Hue [0-1]
    */
    void setHue(qreal h);
    /**
     * @param s Saturation [0-1]
    */
    void setSaturation(qreal s);
    /**
     * @param v Value [0-1]
    */
    void setValue(qreal v);

signals:
    /**
     * Emitted when the user selects a color or setColor is called
     */
    void colorChanged(QColor);
    /**
     * Emitted when the user selects a color
     */
    void colorSelected(QColor);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    /// Calculate outer wheel radius from idget center
    qreal outer_radius() const
    {
        return qMin(geometry().width(), geometry().height())/2;
    }
    /// Calculate inner wheel radius from idget center
    qreal inner_radius() const
    {
        return outer_radius()-wheel_width;
    }
    /// return line from center to given point
    QLineF line_to_point(QPoint p) const
    {
        return QLineF (geometry().width()/2,geometry().height()/2, p.x(),p.y());
    }
    /// Calculate the edge length of the inner square
    qreal square_size() const
    {
        return inner_radius()*qSqrt(2);
    }
    /**
     * Updates the inner square that displays the saturation-value selector
     */
    void render_rectangle();
    /**
     * Updates the outer ring that displays the hue selector
     */
    void render_ring();

};

#endif // COLOR_WHEEL_HPP
