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

#include "color_wheel.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QLineF>
#include <qmath.h>

enum Mouse_Status
{
    Nothing,
    Drag_Circle,
    Drag_Square
};

static const Color_Wheel::Display_Flags hard_default_flags = Color_Wheel::SHAPE_TRIANGLE|Color_Wheel::ANGLE_ROTATING|Color_Wheel::COLOR_HSV;
static Color_Wheel::Display_Flags default_flags = hard_default_flags;

class Color_Wheel::Private
{
private:
    Color_Wheel * const w;

public:

    qreal huem, sat, val;
    unsigned int wheel_width;
    Mouse_Status mouse_status;
    QPixmap hue_ring;
    QImage inner_selector;
    Display_Flags display_flags;
    QColor (*color_from)(qreal,qreal,qreal,qreal);

    Private(Color_Wheel *widget)
        : w(widget), huem(0), sat(0), val(0),
        wheel_width(20), mouse_status(Nothing),
        display_flags(FLAGS_DEFAULT), color_from(&QColor::fromHsvF)
    { }

    /// Calculate outer wheel radius from idget center
    qreal outer_radius() const
    {
        return qMin(w->geometry().width(), w->geometry().height())/2;
    }

    /// Calculate inner wheel radius from idget center
    qreal inner_radius() const
    {
        return outer_radius()-wheel_width;
    }

    /// Calculate the edge length of the inner square
    qreal square_size() const
    {
        return inner_radius()*qSqrt(2);
    }

    /// Calculate the height of the inner triangle
    qreal triangle_height() const
    {
        return inner_radius()*3/2;
    }

    /// Calculate the side of the inner triangle
    qreal triangle_side() const
    {
        return inner_radius()*qSqrt(3);
    }

    /// return line from center to given point
    QLineF line_to_point(const QPoint &p) const
    {
        return QLineF (w->geometry().width()/2, w->geometry().height()/2, p.x(), p.y());
    }

    void render_square()
    {
        int sz = square_size();
        inner_selector = QImage(sz, sz, QImage::Format_RGB32);


        for(int i = 0; i < sz; ++i)
        {
            for(int j = 0;j < sz; ++j)
            {
                inner_selector.setPixel( i, j,
                        color_from(huem,double(i)/sz,double(j)/sz,1).rgb());
            }
        }
    }

    /**
     * \brief renders the selector as a triangle
     * \note It's the same as a square with the edge with value=0 collapsed to a single point
     */
    void render_triangle()
    {
        qreal side = triangle_side();
        qreal height = triangle_height();
        inner_selector = QImage(height, side, QImage::Format_RGBA8888);
        for (int y = 0; y < inner_selector.height(); y++ )
        {
            for (int x = 0; x < inner_selector.width(); x++ )
            {
                qreal pval = x / height;
                qreal slice_h = side * pval;

                qreal ycenter = side/2;
                qreal ymin = ycenter-slice_h/2;
                qreal ymax = ycenter+slice_h/2;
                qreal psat = (y-ymin)/slice_h;

                /// \todo antialiasing
                if ( y >= ymin && y <= ymax )
                    inner_selector.setPixel(x,y,color_from(huem,psat,pval,1).rgba());
                else
                    inner_selector.setPixel(x,y,color_from(0,0,0,0).rgba());
            }
        }
    }

    /// Updates the inner image that displays the saturation-value selector
    void render_inner_selector()
    {
        if ( w->displayFlags(Color_Wheel::SHAPE_FLAGS) == Color_Wheel::SHAPE_TRIANGLE )
            render_triangle();
        else
            render_square();
    }

    QPointF selector_image_offset()
    {
        if ( w->displayFlags(SHAPE_FLAGS) == SHAPE_TRIANGLE )
                return QPointF(-inner_radius(),-triangle_side()/2);
        return QPointF(-square_size()/2,-square_size()/2);
    }

    /// Updates the outer ring that displays the hue selector
    void render_ring()
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
};

Color_Wheel::Color_Wheel(QWidget *parent) :
    QWidget(parent), p(new Private(this))
{
}

Color_Wheel::~Color_Wheel()
{
    delete p;
}

QColor Color_Wheel::color() const
{
    return p->color_from(p->huem, p->sat, p->val, 1);
}

QSize Color_Wheel::sizeHint() const
{
    return QSize(p->wheel_width*5, p->wheel_width*5);
}

qreal Color_Wheel::hue() const
{
    return p->huem;
}

qreal Color_Wheel::saturation() const
{
    return p->sat;
}

qreal Color_Wheel::value() const
{
    return p->val;
}

unsigned int Color_Wheel::wheelWidth() const
{
    return p->wheel_width;
}

void Color_Wheel::setWheelWidth(unsigned int w)
{
    p->wheel_width = w;
    p->render_inner_selector();
    update();
}

void Color_Wheel::paintEvent(QPaintEvent * )
{
    double selector_w = 6;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(geometry().width()/2,geometry().height()/2);

    // hue wheel
    if(p->hue_ring.isNull())
        p->render_ring();

    painter.drawPixmap(-p->outer_radius(), -p->outer_radius(), p->hue_ring);

    // hue selector
    painter.setPen(QPen(Qt::black,3));
    painter.setBrush(Qt::NoBrush);
    QLineF ray(0, 0, p->outer_radius(), 0);
    ray.setAngle(p->huem*360);
    QPointF h1 = ray.p2();
    ray.setLength(p->inner_radius());
    QPointF h2 = ray.p2();
    painter.drawLine(h1,h2);

    // lum-sat square
    if(p->inner_selector.isNull())
        p->render_inner_selector();

    QPointF selector_position;
    if ( displayFlags(SHAPE_FLAGS) == SHAPE_SQUARE )
    {
        if ( displayFlags(ANGLE_FLAGS) == ANGLE_ROTATING )
            painter.rotate(-p->huem*360-45);
        else
            painter.scale(1,-1);

        qreal side = p->square_size();
        selector_position = QPointF(p->sat*side, p->val*side);
    }
    else if ( displayFlags(SHAPE_FLAGS) == SHAPE_TRIANGLE )
    {
        if ( displayFlags(ANGLE_FLAGS) == ANGLE_ROTATING )
            painter.rotate(-p->huem*360-60);

        qreal side = p->triangle_side();
        qreal height = p->triangle_height();
        qreal slice_h = side * p->val;
        qreal ymin = side/2-slice_h/2;

        selector_position = QPointF(p->val*height, ymin + p->sat*slice_h);
    }

    painter.drawImage(p->selector_image_offset(),p->inner_selector);

    // lum-sat selector
    //painter.rotate(135);
    painter.setPen(QPen(p->val > 0.5 ? Qt::black : Qt::white, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(selector_position+p->selector_image_offset(), selector_w, selector_w);

}

void Color_Wheel::mouseMoveEvent(QMouseEvent *ev)
{
    if (p->mouse_status == Drag_Circle )
    {
        p->huem = p->line_to_point(ev->pos()).angle()/360.0;
        p->render_inner_selector();

        emit colorSelected(color());
        emit colorChanged(color());
        update();
    }
    else if(p->mouse_status == Drag_Square)
    {
        QLineF glob_mouse_ln = p->line_to_point(ev->pos());
        QLineF center_mouse_ln ( QPointF(0,0),
                                 glob_mouse_ln.p2() - glob_mouse_ln.p1() );

        if ( displayFlags(SHAPE_FLAGS) == SHAPE_SQUARE )
        {
            if ( displayFlags(ANGLE_FLAGS) == ANGLE_ROTATING )
                center_mouse_ln.setAngle(center_mouse_ln.angle()-p->huem*360-45);

            p->sat = qBound(0.0, center_mouse_ln.x2()/p->square_size()+0.5, 1.0);
            p->val = qBound(0.0, center_mouse_ln.y2()/p->square_size()+0.5, 1.0);

            if ( displayFlags(ANGLE_FLAGS) == ANGLE_FIXED )
                p->val = 1 - p->val;
        }
        else if ( displayFlags(SHAPE_FLAGS) == SHAPE_TRIANGLE )
        {
            if ( displayFlags(ANGLE_FLAGS) == ANGLE_ROTATING )
                center_mouse_ln.setAngle(center_mouse_ln.angle()-p->huem*360-60);
            center_mouse_ln.setP2(center_mouse_ln.p2()-p->selector_image_offset());
            QPointF pt = center_mouse_ln.p2();

            qreal side = p->triangle_side();
            p->val = qBound(0.0, pt.x() / p->triangle_height(), 1.0);
            qreal slice_h = side * p->val;

            qreal ycenter = side/2;
            qreal ymin = ycenter-slice_h/2;
            qreal ymax = ycenter+slice_h/2;

            if ( pt.y() >= ymin && pt.y() <= ymax )
                p->sat = (pt.y()-ymin)/slice_h;
        }

        emit colorSelected(color());
        emit colorChanged(color());
        update();
    }
}

void Color_Wheel::mousePressEvent(QMouseEvent *ev)
{
    if ( ev->buttons() & Qt::LeftButton )
    {
        QLineF ray = p->line_to_point(ev->pos());
        if ( ray.length() <= p->inner_radius() )
            p->mouse_status = Drag_Square;
        else if ( ray.length() <= p->outer_radius() )
            p->mouse_status = Drag_Circle;
    }
}

void Color_Wheel::mouseReleaseEvent(QMouseEvent *ev)
{
    mouseMoveEvent(ev);
    p->mouse_status = Nothing;
}

void Color_Wheel::resizeEvent(QResizeEvent *)
{
    p->render_ring();
    p->render_inner_selector();
}

void Color_Wheel::setColor(QColor c)
{
    qreal oldh = p->huem;
    p->huem = qMax(0.0, c.hueF());
    p->sat = c.saturationF();
    p->val = c.valueF();
    if (!qFuzzyCompare(oldh+1, p->huem+1))
        p->render_inner_selector();
    update();
    emit colorChanged(c);
}

void Color_Wheel::setHue(qreal h)
{
    p->huem = qBound(0.0, h, 1.0);
    p->render_inner_selector();
    update();
}

void Color_Wheel::setSaturation(qreal s)
{
    p->sat = qBound(0.0, s, 1.0);
    update();
}

void Color_Wheel::setValue(qreal v)
{
    p->val = qBound(0.0, v, 1.0);
    update();
}


void Color_Wheel::setDisplayFlags(Display_Flags flags)
{
    if ( (flags & COLOR_FLAGS) != (p->display_flags & COLOR_FLAGS) )
    {
        QColor old_col = color();
        p->display_flags = flags;
        if ( displayFlags(Color_Wheel::COLOR_FLAGS) == Color_Wheel::COLOR_HSL )
        {
            p->sat = old_col.saturationF();
            p->val = old_col.lightnessF();
            p->color_from = &QColor::fromHslF;
        }
        else
        {
            p->sat = old_col.hsvSaturationF();
            p->val = old_col.valueF();
            p->color_from = &QColor::fromHsvF;
        }
    }
    else
        p->display_flags = flags;
    p->render_inner_selector();
    update();
}

Color_Wheel::Display_Flags Color_Wheel::displayFlags(Display_Flags mask) const
{
    Display_Flags flags = p->display_flags;

    if ( ! (flags & COLOR_FLAGS) )
        flags |= default_flags & COLOR_FLAGS;
    if ( ! (flags & ANGLE_FLAGS) )
        flags |= default_flags & ANGLE_FLAGS;
    if ( ! (flags & SHAPE_FLAGS) )
        flags |= default_flags & SHAPE_FLAGS;

    return flags & mask;
}

void Color_Wheel::setDefaultDisplayFlags(Display_Flags flags)
{
    if ( !(flags & COLOR_FLAGS) )
        flags |= hard_default_flags & COLOR_FLAGS;
    if ( !(flags & ANGLE_FLAGS) )
        flags |= hard_default_flags & ANGLE_FLAGS;
    if ( !(flags & SHAPE_FLAGS) )
        flags |= hard_default_flags & SHAPE_FLAGS;
    default_flags = flags;
}

Color_Wheel::Display_Flags Color_Wheel::defaultDisplayFlags(Display_Flags mask)
{
    return default_flags & mask;
}

void Color_Wheel::setDisplayFlag(Display_Flags flag, Display_Flags mask)
{
    setDisplayFlags((p->display_flags&~mask)|flag);
}

