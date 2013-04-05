#ifndef COLOR_PREVIEW_HPP
#define COLOR_PREVIEW_HPP

#include <QWidget>

class Color_Preview : public QWidget
{
    Q_OBJECT
public:
    enum Alpha_Mode
    {
        NoAlpha, ///< Show only solid color
        SplitAlpha, ///< Show both solid and transparent
        AllAlpha ///< show only transparent
    };

private:
    QColor col; ///< color to be viewed
    QBrush back;///< Background brush, visible on transaprent color
    Alpha_Mode alpha_mode;

public:
    explicit Color_Preview(QWidget *parent = 0);

    void setBackground(QBrush bk)
    {
        back = bk;
        repaint();
    }

    QBrush getBackground() const
    {
        return back;
    }

    Alpha_Mode alphaMode() const
    {
        return alpha_mode;
    }

    void setAlphaMode(Alpha_Mode am)
    {
        alpha_mode = am;
        repaint();
    }
    
public slots:
    void setColor(QColor c);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

};

#endif // COLOR_PREVIEW_HPP
