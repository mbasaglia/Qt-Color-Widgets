/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013 Mattia Basaglia

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
#include "color_wheel.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QLineF>

Color_Wheel::Color_Wheel(QWidget *parent) :
    QWidget(parent), huem(0), sat(0), val(0),
    wheel_width(20), mouse_status(Nothing)
{
}

QColor Color_Wheel::color() const
{
    return QColor::fromHsvF(huem,sat,val);
}

QSize Color_Wheel::sizeHint() const
{
    return QSize(wheel_width*5,wheel_width*5);
}

void Color_Wheel::setWheelWidth(unsigned w)
{
    wheel_width = w;
    render_rectangle();
    update();
}


void Color_Wheel::paintEvent(QPaintEvent * )
{
    double selector_w = 6;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(geometry().width()/2,geometry().height()/2);

    // hue wheel
    if ( hue_ring.isNull() )
        render_ring();

    painter.drawPixmap(-outer_radius(),-outer_radius(),hue_ring);



    // hue selector
    painter.setPen(QPen(Qt::black,3));
    painter.setBrush(Qt::NoBrush);
    QLineF ray(0,0,outer_radius(),0);
    ray.setAngle(huem*360);
    QPointF h1 = ray.p2();
    ray.setLength(inner_radius());
    QPointF h2 = ray.p2();
    painter.drawLine(h1,h2);

    // lum-sat square
    if ( sat_val_square.isNull() )
        render_rectangle();

    painter.rotate(-huem*360-45);
    ray.setLength(inner_radius());
    ray.setAngle(135);
    painter.drawImage(ray.p2(),sat_val_square);

    // lum-sat selector
    //painter.rotate(135);
    painter.setPen(QPen( val > 0.5 ? Qt::black : Qt::white ,3));
    painter.setBrush(Qt::NoBrush);
    double max_dist = square_size();
    painter.drawEllipse(QPointF(sat*max_dist-max_dist/2,
                                val*max_dist-max_dist/2
                               ),
                        selector_w,selector_w);

}

void Color_Wheel::mouseMoveEvent(QMouseEvent *ev)
{
    if ( mouse_status == Drag_Circle )
    {
        huem = line_to_point(ev->pos()).angle()/360.0;
        render_rectangle();

        emit colorSelected(color());
        emit colorChanged(color());
        update();
    }
    else if ( mouse_status == Drag_Square )
    {
        QLineF glob_mouse_ln = line_to_point(ev->pos());
        QLineF center_mouse_ln ( QPointF(0,0),
                                 glob_mouse_ln.p2() - glob_mouse_ln.p1() );
        center_mouse_ln.setAngle(center_mouse_ln.angle()-huem*360-45);

        sat = center_mouse_ln.x2()/square_size()+0.5;

        val = center_mouse_ln.y2()/square_size()+0.5;

        if ( sat > 1 )
            sat = 1;
        else if ( sat < 0 )
            sat = 0;

        if ( val > 1 )
            val = 1;
        else if ( val < 0 )
            val = 0;

        emit colorSelected(color());
        emit colorChanged(color());
        update();
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

void Color_Wheel::mouseReleaseEvent(QMouseEvent *ev)
{
    mouseMoveEvent(ev);
    mouse_status = Nothing;
}

void Color_Wheel::resizeEvent(QResizeEvent *)
{
    render_ring();
    render_rectangle();
}

void Color_Wheel::render_rectangle()
{
    int sz = square_size();
    sat_val_square = QImage(sz,sz, QImage::Format_RGB32);

    for(int i = 0; i < sz; ++i)
    {
        for(int j = 0;j < sz; ++j)
        {
            sat_val_square.setPixel( i,j,
                    QColor::fromHsvF(huem,double(i)/sz,double(j)/sz).rgb());
        }
    }
}

void Color_Wheel::render_ring()
{

    hue_ring = QPixmap(outer_radius()*2,outer_radius()*2);
    hue_ring.fill(Qt::transparent);
    QPainter painter(&hue_ring);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Source);


    const int hue_stops = 24;
    static QConicalGradient gradient_hue(0, 0, 0);
    if ( gradient_hue.stops().size() < hue_stops )
    {
        for ( double a = 0; a < 1.0; a+=1.0/(hue_stops-1) )
        {
            gradient_hue.setColorAt(a,QColor::fromHsvF(a,1,1));
        }
        gradient_hue.setColorAt(1,QColor::fromHsvF(0,1,1));
    }

    painter.translate(outer_radius(),outer_radius());

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(gradient_hue));
    painter.drawEllipse(QPointF(0,0),outer_radius(),outer_radius());

    painter.setBrush(Qt::transparent);//palette().background());
    painter.drawEllipse(QPointF(0,0),inner_radius(),inner_radius());

}


void Color_Wheel::setColor(QColor c)
{
    qreal oldh = huem;
    huem = c.hueF();
    sat = c.saturationF();
    val = c.valueF();
    if ( !qFuzzyCompare(oldh+1,huem+1) )
        render_rectangle();
    update();
    emit colorChanged(c);
}

void Color_Wheel::setHue(qreal h)
{
    huem = qMax(0.0,qMin(1.0,h));
    render_rectangle();
    update();
}

void Color_Wheel::setSaturation(qreal s)
{
    sat = qMax(0.0,qMin(1.0,s));
    update();
}

void Color_Wheel::setValue(qreal v)
{
    val = qMax(0.0,qMin(1.0,v));
    update();
}

