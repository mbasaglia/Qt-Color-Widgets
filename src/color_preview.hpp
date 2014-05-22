/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013-2014 Mattia Basaglia

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
#ifndef COLOR_PREVIEW_HPP
#define COLOR_PREVIEW_HPP

#include <QWidget>

/**
    Simple widget that shows a preview of a color
*/
class Color_Preview : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE true)
    Q_PROPERTY(Alpha_Mode alpha_mode READ alphaMode WRITE setAlphaMode DESIGNABLE true)
    Q_PROPERTY(QBrush background READ getBackground WRITE setBackground DESIGNABLE true)
    Q_ENUMS(Alpha_Mode)
public:
    enum Alpha_Mode
    {
        NoAlpha,    ///< Show only solid color
        SplitAlpha, ///< Show both solid and transparent side by side
        AllAlpha    ///< show only transparent
    };

    explicit Color_Preview(QWidget *parent = 0);
    ~Color_Preview();

    /// Get the background visible under transparent colors
    QBrush background() const;

    //! \deprecated use background()
    QBrush getBackground() const { return background(); }

    /// Change the background visible under transparent colors
    void setBackground(const QBrush &bk);

    /// Get how transparent colors are handled
    Alpha_Mode alphaMode() const;

    /// Set how transparent colors are handled
    void setAlphaMode(Alpha_Mode am);

    /// Get current color
    QColor color() const;

    QSize sizeHint () const;

    void paint(QPainter &painter, QRect rect) const;
    
public slots:
    /// Set current color
    void setColor(const QColor &c);

signals:
    /// Emitted when the user clicks on the widget
    void clicked();

    /// Emitted on setColor
    void colorChanged(QColor);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    class Private;
    Private * const p;
};

#endif // COLOR_PREVIEW_HPP
