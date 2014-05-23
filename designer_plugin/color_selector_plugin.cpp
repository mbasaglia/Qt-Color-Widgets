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
#include "color_selector_plugin.hpp"
#include "color_selector.hpp"
#include <QtPlugin>

Color_Selector_Plugin::Color_Selector_Plugin(QObject *parent)
    : QObject(parent), initialized(false)
{
}


void Color_Selector_Plugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized)
        return;

    initialized = true;
}

bool Color_Selector_Plugin::isInitialized() const
{
    return initialized;
}

QWidget *Color_Selector_Plugin::createWidget(QWidget *parent)
{
    return new Color_Selector(parent);
}

QString Color_Selector_Plugin::name() const
{
    return "Color_Selector";
}

QString Color_Selector_Plugin::group() const
{
    return "Color Widgets";
}

QIcon Color_Selector_Plugin::icon() const
{
    return QIcon();
}

QString Color_Selector_Plugin::toolTip() const
{
    return "Display a color and opens a color dialog on click";
}

QString Color_Selector_Plugin::whatsThis() const
{
    return toolTip();
}

bool Color_Selector_Plugin::isContainer() const
{
    return false;
}

QString Color_Selector_Plugin::domXml() const
{

    return "<ui language=\"c++\">\n"
           " <widget class=\"Color_Selector\" name=\"Color_Selector\">\n"
           " </widget>\n"
            "</ui>\n";
}

QString Color_Selector_Plugin::includeFile() const
{
    return "Color_Selector";
}



