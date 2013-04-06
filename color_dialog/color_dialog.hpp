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
#ifndef COLOR_DIALOG_HPP
#define COLOR_DIALOG_HPP

#include "ui_color_dialog.h"

class Color_Dialog : public QDialog, private Ui::Color_Dialog
{
    Q_OBJECT

public:
    explicit Color_Dialog(QWidget *parent = 0);

    QColor color() const;

public slots:

    void setColor(QColor c);

signals:
    /**
     * Emitted when the user selects a color or setColor is called
     */
    void colorChanged(QColor);

protected slots:
    void update_widgets();
    /// Update from HSV sliders
    void set_hsv();
    /// Update from RGB sliders
    void set_rgb();
private slots:
    void on_edit_hex_editingFinished();
    void on_edit_hex_textEdited(const QString &arg1);
};

#endif // COLOR_DIALOG_HPP
