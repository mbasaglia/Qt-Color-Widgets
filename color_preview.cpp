#include "color_preview.hpp"
#include <QPainter>
#include <QStyleOptionFrame>

Color_Preview::Color_Preview(QWidget *parent) :
    QWidget(parent)
{

}

void Color_Preview::setColor(QColor c)
{
    col = c;
    repaint();
}

void Color_Preview::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.fillRect(geometry(),col);




    QStyleOptionFrame opt;

    opt.init(this);

    opt.frameShape = QFrame::Panel;

    opt.rect = geometry();

    opt.lineWidth = 2;
    opt.midLineWidth = 2;


    opt.state = QStyle::State_Sunken;


    style()->drawControl(QStyle::CE_ShapedFrame, &opt, &painter, this);
}

void Color_Preview::resizeEvent(QResizeEvent *)
{
    repaint();
}

