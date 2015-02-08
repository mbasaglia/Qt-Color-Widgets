/**

\file

\author Mattia Basaglia

\section License

Copyright (C) 2015  Mattia Basaglia

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef DEMO_DIALOG_HPP
#define DEMO_DIALOG_HPP

#include "ui_demo_dialog.h"

class Demo_Dialog : public QDialog, private Ui::Demo_Dialog
{
    Q_OBJECT

public:
    explicit Demo_Dialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
private slots:
    void on_combo_shape_activated(int index);
    void on_combo_angle_activated(int index);
    void on_combo_color_activated(int index);
};

#endif // DEMO_DIALOG_HPP
