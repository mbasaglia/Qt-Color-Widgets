/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013 Mattia Basaglia

    This file is part of Color Widgets.

    Color Widgets is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Color Widgets is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "color_wheel_plugin.hpp"
#include "color_wheel.hpp"

Color_Wheel_Plugin::Color_Wheel_Plugin(QObject *parent) :
    QObject(parent), initialized(false)
{
}

void Color_Wheel_Plugin::initialize(QDesignerFormEditorInterface *)
{
    initialized = true;
}

bool Color_Wheel_Plugin::isInitialized() const
{
    return initialized;
}

QWidget *Color_Wheel_Plugin::createWidget(QWidget *parent)
{
    return new Color_Wheel(parent);
}

QString Color_Wheel_Plugin::name() const
{
    return "Color_Wheel";
}

QString Color_Wheel_Plugin::group() const
{
    return "Color Widgets";
}

QIcon Color_Wheel_Plugin::icon() const
{
    Color_Wheel w;
    w.resize(64,64);
    w.setWheelWidth(8);
    QPixmap pix(64,64);
    w.render(&pix);
    return QIcon(pix);
}

QString Color_Wheel_Plugin::toolTip() const
{
    return "Color selector";
}

QString Color_Wheel_Plugin::whatsThis() const
{
    return "A widget that allows an intuitive selection of HSL parameters for a QColor";
}

bool Color_Wheel_Plugin::isContainer() const
{
    return false;
}

QString Color_Wheel_Plugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"Color_Wheel\" name=\"Color_Wheel\">\n"
           "  <property name=\"sizePolicy\">\n"
           "   <sizepolicy hsizetype=\"Minimum\" vsizetype=\"Minimum\">\n"
           "    <horstretch>0</horstretch>\n"
           "    <verstretch>0</verstretch>\n"
           "   </sizepolicy>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString Color_Wheel_Plugin::includeFile() const
{
    return "Color_Wheel";
}


