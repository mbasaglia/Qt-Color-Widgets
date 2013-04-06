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
#ifndef GRADIENT_SLIDER_HPP
#define GRADIENT_SLIDER_HPP

#include <QSlider>

class Gradient_Slider : public QSlider
{
    Q_OBJECT
    Q_PROPERTY(QVector<QColor> background READ background WRITE setBackground)
    Q_PROPERTY(QLinearGradient backgroundGradient
               READ backgroundGradient WRITE setBackgroundGradient STORED false)
    Q_PROPERTY(QColor firstColor READ firstColor WRITE setFirstColor STORED false)
    Q_PROPERTY(QColor lastColor READ lastColor WRITE setLastColor STORED false)

private:
    QVector<QColor> back;

public:
    explicit Gradient_Slider(QWidget *parent = 0);

    QVector<QColor> background() const { return back; }
    void setBackground(QVector<QColor> bg);

    void setBackgroundGradient(QLinearGradient bg);
    QLinearGradient backgroundGradient() const ;

    void setFirstColor(QColor c);
    void setLastColor(QColor c);
    QColor firstColor() const;
    QColor lastColor() const;
    
protected:
    void paintEvent(QPaintEvent *ev);
};

#endif // GRADIENT_SLIDER_HPP
