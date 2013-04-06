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
#ifndef COLOR_WHEEL_HPP
#define COLOR_WHEEL_HPP

#include <QWidget>
#include <qmath.h>


class Color_Wheel : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE true STORED false )
    Q_PROPERTY(int hue READ hue WRITE setHue DESIGNABLE false )
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation DESIGNABLE false )
    Q_PROPERTY(int value READ value WRITE setValue DESIGNABLE false )
    Q_PROPERTY(unsigned wheelWidth READ wheelWidth WRITE setWheelWidth DESIGNABLE true )

private:
    int huem, sat, val;
    unsigned wheel_width;
    enum Mouse_Status
    {
        Nothing,
        Drag_Circle,
        Drag_Square
    } mouse_status;
    QImage sat_val_square;


public:
    explicit Color_Wheel(QWidget *parent = 0);

    QColor color() const;

    QSize sizeHint () const;

    int hue() const { return huem; }
    int saturation() const { return sat; }
    int value() const { return val; }
    unsigned wheelWidth() const { return wheel_width; }
    void setWheelWidth(unsigned w);

public slots:

    void setColor(QColor c);
    /**
     * @param h Hue [0-359]
    */
    void setHue(int h);
    /**
     * @param s Saturation [0-255]
    */
    void setSaturation(int s);
    /**
     * @param v Value [0-255]
    */
    void setValue(int v);

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
    qreal outer_radius() const
    {
        return qMin(geometry().width(), geometry().height())/2;
    }
    qreal inner_radius() const
    {
        return outer_radius()-wheel_width;
    }
    /// return line from center to given point
    QLineF line_to_point(QPoint p) const
    {
        return QLineF (geometry().width()/2,geometry().height()/2, p.x(),p.y());
    }
    qreal square_size() const
    {
        return inner_radius()*qSqrt(2);
    }
    /**
     * Updates the internal square that displays the saturation-value selector
     */
    void render_rectangle();

};

#endif // COLOR_WHEEL_HPP
