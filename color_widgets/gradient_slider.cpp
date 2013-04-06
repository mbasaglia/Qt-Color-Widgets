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
    update();
}

void Gradient_Slider::setBackgroundGradient(QLinearGradient bg)
{
    back.clear();
    foreach(const QGradientStop& gs, bg.stops() )
    {
        back.push_back(gs.second);
    }
    update();
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
    update();
}

void Gradient_Slider::setLastColor(QColor c)
{

    if ( back.size() < 2 )
        back.push_back(c);
    else
        back.back() = c;
    update();
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


    p.fillRect(1,1,geometry().width()-2,geometry().height()-2,backgroundGradient());

    QStyleOptionSlider opt_slider;
    initStyleOption(&opt_slider);
    opt_slider.subControls = QStyle::SC_SliderHandle;
    if (isSliderDown())
        opt_slider.state |= QStyle::State_Sunken;
    style()->drawComplexControl(QStyle::CC_Slider, &opt_slider, &p, this);

    QStyleOptionFrame opt_frame;
    opt_frame.init(this);
    opt_frame.frameShape = QFrame::StyledPanel;
    opt_frame.rect = geometry();
    opt_frame.state = QStyle::State_Sunken;
    opt_frame.features = QStyleOptionFrame::Rounded;
    p.translate(-geometry().topLeft());
    style()->drawControl(QStyle::CE_ShapedFrame, &opt_frame, &p, this);

}
