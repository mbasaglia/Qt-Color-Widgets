#ifndef COLOR_WHEEL_HPP
#define COLOR_WHEEL_HPP

#include <QWidget>
#include <qmath.h>


class Color_Wheel : public QWidget
{
    Q_OBJECT
private:
    int hue, sat, lum, alpha;
    qreal wheel_width;
    enum Mouse_Status
    {
        Nothing,
        Drag_Circle,
        Drag_Square
    } mouse_status;
    QImage sat_lum_square;


public:
    explicit Color_Wheel(QWidget *parent = 0);

    QColor color() const;

public slots:

    void setColor(QColor c);

signals:
    /**
     * Emitted when the user selects a color or setColor is called
     */
    void colorChanged(QColor);
    /**
     * Emitted when the user selects a color
     */
    void colorSelected(QColor);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    qreal outer_radius() const
    {
        return qMin(geometry().width(), geometry().height())/2;
    }
    qreal inner_radius() const
    {
        return outer_radius()-wheel_width;
    }
    /// return line from center to given point
    QLineF line_to_point(QPoint p) const
    {
        return QLineF (geometry().width()/2,geometry().height()/2, p.x(),p.y());
    }
    qreal square_size() const
    {
        return inner_radius()*qSqrt(2);
    }
    void render_rectangle();

};

#endif // COLOR_WHEEL_HPP
