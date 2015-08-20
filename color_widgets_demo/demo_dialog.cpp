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

    combo_shape->addItem("Triangle",color_widgets::ColorWheel::SHAPE_TRIANGLE);
    combo_shape->addItem("Square",color_widgets::ColorWheel::SHAPE_SQUARE);
    combo_shape->setCurrentIndex(combo_shape->findData(
        QVariant(color_widgets::ColorWheel::defaultDisplayFlags(color_widgets::ColorWheel::SHAPE_FLAGS))));

    combo_angle->addItem("Fixed",color_widgets::ColorWheel::ANGLE_FIXED);
    combo_angle->addItem("Rotating",color_widgets::ColorWheel::ANGLE_ROTATING);
    combo_angle->setCurrentIndex(combo_angle->findData(
        QVariant(color_widgets::ColorWheel::defaultDisplayFlags(color_widgets::ColorWheel::ANGLE_FLAGS))));

    combo_color->addItem("HSV",color_widgets::ColorWheel::COLOR_HSV);
    combo_color->addItem("HSL",color_widgets::ColorWheel::COLOR_HSL);
    combo_color->addItem("LCH",color_widgets::ColorWheel::COLOR_LCH);
    combo_color->setCurrentIndex(combo_color->findData(
        QVariant(color_widgets::ColorWheel::defaultDisplayFlags(color_widgets::ColorWheel::COLOR_FLAGS))));


    swatch->palette().setColumns(12);
    for ( int i = 0; i < 6; i++ )
    {
        for ( int j = 0; j < swatch->palette().columns(); j++ )
        {
            float f = float(j)/swatch->palette().columns();
            swatch->palette().appendColor(QColor::fromHsvF(i/8.0,1-f,0.5+f/2));
        }
    }
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
        color_widgets::ColorWheel::Display_Enum(combo_shape->itemData(index).toInt()),
        color_widgets::ColorWheel::SHAPE_FLAGS);
    color_widgets::ColorWheel::setDefaultDisplayFlags(color_wheel->displayFlags());
    color_list->setWheelFlags(color_wheel->displayFlags());
}

void Demo_Dialog::on_combo_angle_activated(int index)
{
    color_wheel->setDisplayFlag(
        color_widgets::ColorWheel::Display_Enum(combo_angle->itemData(index).toInt()),
        color_widgets::ColorWheel::ANGLE_FLAGS);
    color_widgets::ColorWheel::setDefaultDisplayFlags(color_wheel->displayFlags());
    color_list->setWheelFlags(color_wheel->displayFlags());
}

void Demo_Dialog::on_combo_color_activated(int index)
{
    color_wheel->setDisplayFlag(
        color_widgets::ColorWheel::Display_Enum(combo_color->itemData(index).toInt()),
        color_widgets::ColorWheel::COLOR_FLAGS);
    color_widgets::ColorWheel::setDefaultDisplayFlags(color_wheel->displayFlags());
    color_list->setWheelFlags(color_wheel->displayFlags());
}
