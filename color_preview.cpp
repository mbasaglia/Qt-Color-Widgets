#include "color_preview.hpp"

Color_Preview::Color_Preview(QWidget *parent) :
    QGraphicsView(parent)
{

}


void Color_Preview::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect,Qt::red);//backgroundBrush());
}
