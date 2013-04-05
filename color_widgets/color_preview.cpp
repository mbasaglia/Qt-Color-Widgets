#include "color_preview.hpp"
#include <QStylePainter>
#include <QStyleOptionFrame>
//#include <QGuiApplication>

Color_Preview::Color_Preview(QWidget *parent) :
    QWidget(parent), col(Qt::red),back( Qt::darkGray, Qt::DiagCrossPattern ),
    alpha_mode(NoAlpha)
{
    //setPalette(QGuiApplication::palette());
}

QSize Color_Preview::sizeHint() const
{
    return QSize(24,24);
}

void Color_Preview::setColor(QColor c)
{
    col = c;
    repaint();
}

void Color_Preview::paintEvent(QPaintEvent *)
{



    QStylePainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor noalpha = col;
    noalpha.setAlpha(255);

    painter.fillRect(1,1,geometry().width()-2,geometry().height()-2, back );

    int w = geometry().width()-2;
    if ( alpha_mode == SplitAlpha )
        w /= 2;
    else if ( alpha_mode == AllAlpha )
        w = 0;
    painter.fillRect(1,1,w,geometry().height()-2,noalpha);
    painter.fillRect(w,1,geometry().width()-w-2,geometry().height()-2,col);


    painter.translate(-geometry().topLeft());


    QStyleOptionFrame opt;

    opt.init(this);

    opt.frameShape = QFrame::StyledPanel;

    opt.rect = geometry();

    opt.lineWidth = 2;
    opt.midLineWidth = 2;


    opt.state = QStyle::State_Sunken;
    opt.features = QStyleOptionFrame::Rounded;

    //opt.palette = palette();
    //opt.palette.setColor(QPalette::Base, Qt::transparent);
    //style()->drawPrimitive(QStyle::PE_PanelLineEdit,&opt,&painter,this);
    painter.drawControl(QStyle::CE_ShapedFrame, opt);
}

void Color_Preview::resizeEvent(QResizeEvent *)
{
    repaint();
}

