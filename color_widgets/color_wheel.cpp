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
#include "color_wheel.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QLineF>

Color_Wheel::Color_Wheel(QWidget *parent) :
    QWidget(parent), hue(0), sat(0), val(0), alpha(255),
    wheel_width(20), mouse_status(Nothing)
{
}

QColor Color_Wheel::color() const
{
    return QColor::fromHsv(hue,sat,val,alpha);
}

QSize Color_Wheel::sizeHint() const
{
    return QSize(wheel_width*5,wheel_width*5);
}


void Color_Wheel::paintEvent(QPaintEvent * )
{
    double selector_w = wheel_width/3;

    // hue wheel
    const int hue_stops = 24;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    QConicalGradient gradient_hue(0, 0, 0);
    for ( double a = 0; a < 1.0; a+=1.0/(hue_stops-1) )
    {
        gradient_hue.setColorAt(a,QColor::fromHsvF(a,1,1));
    }
    gradient_hue.setColorAt(1,QColor::fromHsvF(0,1,1));

    painter.translate(geometry().width()/2,geometry().height()/2);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(gradient_hue));
    painter.drawEllipse(QPointF(0,0),outer_radius(),outer_radius());

    painter.setBrush(palette().background());
    painter.drawEllipse(QPointF(0,0),inner_radius(),inner_radius());

    // hue selector
    painter.setPen(QPen(Qt::black,3));
    painter.setBrush(Qt::NoBrush);
    QLineF ray(0,0,outer_radius(),0);
    ray.setAngle(hue);
    QPointF h1 = ray.p2();
    ray.setLength(inner_radius());
    QPointF h2 = ray.p2();
    painter.drawLine(h1,h2);

    // lum-sat square
    if ( sat_val_square.isNull() )
        render_rectangle();

    painter.rotate(-hue-45);
    ray.setLength(inner_radius());
    ray.setAngle(135);
    painter.drawImage(ray.p2(),sat_val_square);

    // lum-sat selector
    painter.rotate(135);
    painter.setPen(QPen( val > 128 ? Qt::black : Qt::white ,3));
    painter.setBrush(Qt::NoBrush);
    double max_dist = qSqrt(2)*square_size();
    painter.drawEllipse(QPointF(val/255.0*max_dist-max_dist/2,
                                max_dist/2-sat/255.0*max_dist),
                        selector_w,selector_w);

}

void Color_Wheel::mouseMoveEvent(QMouseEvent *ev)
{
    if ( mouse_status == Drag_Circle )
    {
        hue = line_to_point(ev->pos()).angle();
        render_rectangle();

        emit colorSelected(color());
        emit colorChanged(color());
        repaint();
    }
    else if ( mouse_status == Drag_Square )
    {
        QLineF ml = line_to_point(ev->pos());
        QVector2D mv(ml.p2()-ml.p1());
        QLineF satl(0,0,0,square_size());
        satl.setAngle(hue);
        QVector2D satv(satl.p2());

        float satF = QVector2D::dotProduct(mv,satv) /
                (square_size()*square_size()*qSqrt(2)) + 0.5;
        sat = satF*255;
        if ( sat > 255 )
            sat = 255;
        else if ( sat < 0 )
            sat = 0;

        satl.setAngle(hue-90);
        QVector2D lumv(satl.p2());
        float lumF = QVector2D::dotProduct(mv,lumv) /
                (square_size()*square_size()*qSqrt(2)) + 0.5;
        val = lumF*255;
        if ( val> 255 )
            val = 255;
        else if ( val < 0 )
            val = 0;

        emit colorSelected(color());
        emit colorChanged(color());
        repaint();
    }
}

void Color_Wheel::mousePressEvent(QMouseEvent *ev)
{
    if ( ev->buttons() & Qt::LeftButton )
    {
        QLineF ray = line_to_point(ev->pos());
        if ( ray.length() <= inner_radius() )
            mouse_status = Drag_Square;
        else if ( ray.length() <= outer_radius() )
            mouse_status = Drag_Circle;
    }
}

void Color_Wheel::mouseReleaseEvent(QMouseEvent *)
{
    mouse_status = Nothing;
}

void Color_Wheel::resizeEvent(QResizeEvent *)
{
    render_rectangle();
}

void Color_Wheel::render_rectangle()
{
    int sz = square_size();
    double huef = hue/360.0;
    sat_val_square = QImage(sz,sz, QImage::Format_RGB32);
    //double max_dist = sz*sz*2;
    for(int i = 0; i < sz; ++i)
    {
        for(int j = 0;j < sz; ++j)
        {
            sat_val_square.setPixel( i,j,
                    QColor::fromHsvF(huef,double(i)/sz,double(j)/sz).rgb());
            /*sat_lum_square.setPixel( i,j, QColor::fromHslF(
                                 huef,
                                 (i*i+(sz-j)*(sz-j))/max_dist,
                                 (i*i+j*j)/max_dist
                            ).rgb());*/
        }
    }
}


void Color_Wheel::setColor(QColor c)
{
    int oldh = hue;
    hue = c.hue();
    sat = c.saturation();
    val = c.value();
    alpha = c.alpha();
    if ( oldh != hue )
        render_rectangle();
    repaint();
    emit colorChanged(c);
}
