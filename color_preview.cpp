#include "color_preview.hpp"
#include <QPainter>
#include <QStyleOptionFrame>

Color_Preview::Color_Preview(QWidget *parent) :
    QWidget(parent), back( Qt::darkGray, Qt::DiagCrossPattern ),
    alpha_mode(SplitAlpha)
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
    QColor noalpha = col;
    noalpha.setAlpha(255);

    painter.fillRect(0,0,geometry().width(),geometry().height(), back );

    int w = geometry().width();
    if ( alpha_mode == SplitAlpha )
        w /= 2;
    else if ( alpha_mode == AllAlpha )
        w = 0;
    painter.fillRect(0,0,w,geometry().height(),noalpha);
    painter.fillRect(w,0,geometry().width()-w,geometry().height(),col);


    QStyleOptionFrame opt;

    opt.init(this);

    opt.frameShape = QFrame::StyledPanel;

    opt.rect = geometry();

    opt.lineWidth = 2;
    opt.midLineWidth = 2;


    opt.state = QStyle::State_Sunken;
    opt.features = QStyleOptionFrame::Rounded;

    /*opt.palette = palette();
    opt.palette.setColor(QPalette::Base, col);
    style()->drawPrimitive(QStyle::PE_PanelLineEdit,&opt,&painter,this);*/
    style()->drawControl(QStyle::CE_ShapedFrame, &opt, &painter, this);
}

void Color_Preview::resizeEvent(QResizeEvent *)
{
    repaint();
}

