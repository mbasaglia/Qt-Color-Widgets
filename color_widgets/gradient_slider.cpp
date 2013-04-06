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
#include "gradient_slider.hpp"
#include <QPainter>
#include <QStyleOptionSlider>


#include "gradient_slider.hpp"
#include <QPainter>
#include <QLinearGradient>

Gradient_Slider::Gradient_Slider(QWidget *parent) :
    QSlider(parent)
{
    back.push_back(Qt::black);
    back.push_back(Qt::white);

    setOrientation(Qt::Horizontal);
}

void Gradient_Slider::setBackground(QVector<QColor> bg)
{
    back = bg;
    repaint();
}

void Gradient_Slider::setBackgroundGradient(QLinearGradient bg)
{
    back.clear();
    foreach(const QGradientStop& gs, bg.stops() )
    {
        back.push_back(gs.second);
    }
}

QLinearGradient Gradient_Slider::backgroundGradient() const
{
    int ior = orientation() == Qt::Horizontal ? 1 : 0;
    QLinearGradient lg(0,0,ior,1-ior);
    lg.setCoordinateMode(QGradient::StretchToDeviceMode);
    for(int i = 0; i < back.size(); i++)
        lg.setColorAt(double(i)/(back.size()-1),back[i]);
    return lg;
}

void Gradient_Slider::setFirstColor(QColor c)
{
    if ( back.empty() )
        back.push_back(c);
    else
        back.front() = c;
}

void Gradient_Slider::setLastColor(QColor c)
{

    if ( back.size() < 2 )
        back.push_back(c);
    else
        back.back() = c;
}

QColor Gradient_Slider::firstColor() const
{
    return back.empty() ? QColor() : back.front();
}

QColor Gradient_Slider::lastColor() const
{
    return back.empty() ? QColor() : back.back();
}


void Gradient_Slider::paintEvent(QPaintEvent *)
{
    QPainter p(this);


    p.fillRect(0,0,geometry().width(),geometry().height(),backgroundGradient());

    QStyleOptionSlider opt;

    initStyleOption(&opt);

    opt.subControls = QStyle::SC_SliderHandle;

    if (isSliderDown())
    {
        opt.state |= QStyle::State_Sunken;
    }
    style()->drawComplexControl(QStyle::CC_Slider, &opt, &p, this);

}
