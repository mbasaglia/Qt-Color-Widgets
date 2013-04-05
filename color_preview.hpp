#ifndef COLOR_PREVIEW_HPP
#define COLOR_PREVIEW_HPP

#include <QWidget>

class Color_Preview : public QWidget
{
    Q_OBJECT
private:
    QColor col;

public:
    explicit Color_Preview(QWidget *parent = 0);
    
public slots:
    void setColor(QColor c);
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

};

#endif // COLOR_PREVIEW_HPP
