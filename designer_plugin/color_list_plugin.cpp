/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "color_list_plugin.hpp"
#include "color_list_widget.hpp"

Color_List_Plugin::Color_List_Plugin(QObject *parent) :
    QObject(parent)
{
}


void Color_List_Plugin::initialize(QDesignerFormEditorInterface *)
{
    initialized = true;
}

bool Color_List_Plugin::isInitialized() const
{
    return initialized;
}

QWidget *Color_List_Plugin::createWidget(QWidget *parent)
{
    return new Color_List_Widget(parent);
}

QString Color_List_Plugin::name() const
{
    return "Color_List_Widget";
}

QString Color_List_Plugin::group() const
{
    return "Color Widgets";
}

QIcon Color_List_Plugin::icon() const
{
    return QIcon::fromTheme("format-stroke-color");
}

QString Color_List_Plugin::toolTip() const
{
    return "An editable list of colors";
}

QString Color_List_Plugin::whatsThis() const
{
    return toolTip();
}

bool Color_List_Plugin::isContainer() const
{
    return false;
}

QString Color_List_Plugin::domXml() const
{

    return "<ui language=\"c++\">\n"
           " <widget class=\"Color_List_Widget\" name=\"Color_List_Widget\">\n"
           " </widget>\n"
            "</ui>\n";
}

QString Color_List_Plugin::includeFile() const
{
    return "Color_List_Widget";
}
