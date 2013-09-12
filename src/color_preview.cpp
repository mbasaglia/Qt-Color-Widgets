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
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

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

void Color_Preview::paint(QPainter &painter, QRect rect) const
{

    QColor noalpha = col;
    noalpha.setAlpha(255);

    painter.fillRect(1,1,rect.width()-2,rect.height()-2, back );

    int w = rect.width()-2;
    if ( alpha_mode == SplitAlpha )
        w /= 2;
    else if ( alpha_mode == AllAlpha )
        w = 0;
    painter.fillRect(1,1,w,rect.height()-2,noalpha);
    painter.fillRect(w,1,rect.width()-w-1,rect.height()-2,col);

    paint_tl_border(painter,size(),palette().color(QPalette::Mid),0);
    paint_tl_border(painter,size(),palette().color(QPalette::Dark),1);

    paint_br_border(painter,size(),palette().color(QPalette::Midlight),1);
    paint_br_border(painter,size(),palette().color(QPalette::Button),0);
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


    paint(painter,geometry());

}

void Color_Preview::resizeEvent(QResizeEvent *)
{
    update();
}

void Color_Preview::mouseReleaseEvent(QMouseEvent * ev)
{
    if ( QRect(QPoint(0,0),size()).contains(ev->pos()) )
        emit clicked();
}

void Color_Preview::mouseMoveEvent(QMouseEvent *ev)
{

    if ( ev->buttons() &Qt::LeftButton && !QRect(QPoint(0,0),size()).contains(ev->pos()) )
    {
        QMimeData *data = new QMimeData;

        data->setColorData(col);
        /*data->setText(col.name());
        data->setData("application/x-oswb-color",
            QString("<paint><color name='%1'>""<sRGB r='%2' g='%3' b='%4' />"
                    "</color></paint>")
            .arg(col.name()).arg(col.redF()).arg(col.greenF()).arg(col.blueF())
            .toUtf8()
        );*/

        QDrag* drag = new QDrag(this);
        drag->setMimeData(data);

        QPixmap preview(24,24);
        preview.fill(col);
        drag->setPixmap(preview);

        drag->exec();
    }
}

