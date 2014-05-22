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
#include "color_preview_plugin.hpp"
#include "color_preview.hpp"
#include <QtPlugin>

Color_Preview_Plugin::Color_Preview_Plugin(QObject *parent)
    : QObject(parent), initialized(false)
{
}


void Color_Preview_Plugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized)
        return;

    initialized = true;
}

bool Color_Preview_Plugin::isInitialized() const
{
    return initialized;
}

QWidget *Color_Preview_Plugin::createWidget(QWidget *parent)
{
    return new Color_Preview(parent);
}

QString Color_Preview_Plugin::name() const
{
    return "Color_Preview";
}

QString Color_Preview_Plugin::group() const
{
    return "Color Widgets";
}

QIcon Color_Preview_Plugin::icon() const
{
    return QIcon();
}

QString Color_Preview_Plugin::toolTip() const
{
    return "Display a color";
}

QString Color_Preview_Plugin::whatsThis() const
{
    return toolTip();
}

bool Color_Preview_Plugin::isContainer() const
{
    return false;
}

QString Color_Preview_Plugin::domXml() const
{

    return "<ui language=\"c++\">\n"
           " <widget class=\"Color_Preview\" name=\"Color_Preview\">\n"
           " </widget>\n"
            "</ui>\n";
}

QString Color_Preview_Plugin::includeFile() const
{
    return "Color_Preview";
}

//Q_EXPORT_PLUGIN2(color_widgets, Color_Preview_Plugin);



