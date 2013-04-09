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
#include "color_preview.hpp"
#include <QStylePainter>
#include <QStyleOptionFrame>
#include "paint_border.hpp"
//#include <QGuiApplication>

Color_Preview::Color_Preview(QWidget *parent) :
    QWidget(parent), col(Qt::red), back( Qt::darkGray, Qt::DiagCrossPattern ),
    alpha_mode(NoAlpha)
{
    back.setTexture(QPixmap(QString(":/color_widgets/alphaback.png")));
    //setPalette(QGuiApplication::palette());
}

QSize Color_Preview::sizeHint() const
{
    return QSize(24,24);
}

void Color_Preview::setColor(QColor c)
{
    col = c;
    update();
    emit colorChanged(c);
}

void Color_Preview::paintEvent(QPaintEvent *)
{



    QStylePainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);

    QColor noalpha = col;
    noalpha.setAlpha(255);

    painter.fillRect(1,1,geometry().width()-2,geometry().height()-2, back );

    int w = geometry().width()-2;
    if ( alpha_mode == SplitAlpha )
        w /= 2;
    else if ( alpha_mode == AllAlpha )
        w = 0;
    painter.fillRect(1,1,w,geometry().height()-2,noalpha);
    painter.fillRect(w,1,geometry().width()-w-1,geometry().height()-2,col);

    paint_tl_border(painter,size(),palette().color(QPalette::Mid),0);
    paint_tl_border(painter,size(),palette().color(QPalette::Dark),1);

    paint_br_border(painter,size(),palette().color(QPalette::Midlight),1);
    paint_br_border(painter,size(),palette().color(QPalette::Button),0);

    /*painter.translate(-geometry().topLeft());


    QStyleOptionFrameV3 opt;

    opt.init(this);

    opt.rect = geometry();

    opt.lineWidth = 2;
    opt.midLineWidth = 2;

    opt.state = QStyle::State_Sunken;

    opt.frameShape = QFrame::StyledPanel;

    //opt.palette = palette();
    //opt.palette.setColor(QPalette::Base, Qt::transparent);
    //style()->drawPrimitive(QStyle::PE_PanelLineEdit,&opt,&painter,this);
    painter.drawControl(QStyle::CE_ShapedFrame, opt);*/
}

void Color_Preview::resizeEvent(QResizeEvent *)
{
    update();
}

void Color_Preview::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
}

