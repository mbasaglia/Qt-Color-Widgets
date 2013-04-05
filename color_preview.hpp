#ifndef COLOR_PREVIEW_HPP
#define COLOR_PREVIEW_HPP

#include <QGraphicsView>

class Color_Preview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Color_Preview(QWidget *parent = 0);
    
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

};

#endif // COLOR_PREVIEW_HPP
