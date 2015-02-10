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

#include "demo_dialog.hpp"

Demo_Dialog::Demo_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    QColor democolor = QColor(64,172,143,128);
    color_wheel->setColor(democolor);
    QList<QColor> l;
    l << democolor;
    color_list->setColors(l);

    combo_shape->addItem("Triangle",Color_Wheel::SHAPE_TRIANGLE);
    combo_shape->addItem("Square",Color_Wheel::SHAPE_SQUARE);
    combo_shape->setCurrentIndex(combo_shape->findData(
        QVariant(Color_Wheel::defaultDisplayFlags(Color_Wheel::SHAPE_FLAGS))));

    combo_angle->addItem("Fixed",Color_Wheel::ANGLE_FIXED);
    combo_angle->addItem("Rotating",Color_Wheel::ANGLE_ROTATING);
    combo_angle->setCurrentIndex(combo_angle->findData(
        QVariant(Color_Wheel::defaultDisplayFlags(Color_Wheel::ANGLE_FLAGS))));

    combo_color->addItem("HSV",Color_Wheel::COLOR_HSV);
    combo_color->addItem("HSL",Color_Wheel::COLOR_HSL);
    combo_color->addItem("LCH",Color_Wheel::COLOR_LCH);
    combo_color->setCurrentIndex(combo_color->findData(
        QVariant(Color_Wheel::defaultDisplayFlags(Color_Wheel::COLOR_FLAGS))));
}

void Demo_Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void Demo_Dialog::on_combo_shape_activated(int index)
{
    color_wheel->setDisplayFlag(
        Color_Wheel::Display_Enum(combo_shape->itemData(index).toInt()),
        Color_Wheel::SHAPE_FLAGS);
    Color_Wheel::setDefaultDisplayFlags(color_wheel->displayFlags());
    color_list->setWheelFlags(color_wheel->displayFlags());
}

void Demo_Dialog::on_combo_angle_activated(int index)
{
    color_wheel->setDisplayFlag(
        Color_Wheel::Display_Enum(combo_angle->itemData(index).toInt()),
        Color_Wheel::ANGLE_FLAGS);
    Color_Wheel::setDefaultDisplayFlags(color_wheel->displayFlags());
    color_list->setWheelFlags(color_wheel->displayFlags());
}

void Demo_Dialog::on_combo_color_activated(int index)
{
    color_wheel->setDisplayFlag(
        Color_Wheel::Display_Enum(combo_color->itemData(index).toInt()),
        Color_Wheel::COLOR_FLAGS);
    Color_Wheel::setDefaultDisplayFlags(color_wheel->displayFlags());
    color_list->setWheelFlags(color_wheel->displayFlags());
}
