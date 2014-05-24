/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013 Mattia Basaglia
    Copyright (C) 2014 Calle Laakkonen

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
#include "hue_slider_plugin.hpp"
#include "hue_slider.hpp"
#include <QtPlugin>

Hue_Slider_Plugin::Hue_Slider_Plugin(QObject *parent)
    : QObject(parent), initialized(false)
{
}


void Hue_Slider_Plugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized)
        return;

    initialized = true;
}

bool Hue_Slider_Plugin::isInitialized() const
{
    return initialized;
}

QWidget *Hue_Slider_Plugin::createWidget(QWidget *parent)
{
    return new Hue_Slider(parent);
}

QString Hue_Slider_Plugin::name() const
{
    return "Hue_Slider";
}

QString Hue_Slider_Plugin::group() const
{
    return "Color Widgets";
}

QIcon Hue_Slider_Plugin::icon() const
{
    Hue_Slider w;
    w.resize(64,16);
    QPixmap pix(64,64);
    pix.fill(Qt::transparent);
    w.render(&pix,QPoint(0,16));
    return QIcon(pix);
}

QString Hue_Slider_Plugin::toolTip() const
{
    return "Slider over a hue gradient";
}

QString Hue_Slider_Plugin::whatsThis() const
{
    return toolTip();
}

bool Hue_Slider_Plugin::isContainer() const
{
    return false;
}

QString Hue_Slider_Plugin::domXml() const
{

    return "<ui language=\"c++\">\n"
           " <widget class=\"Hue_Slider\" name=\"Hue_Slider\">\n"
           " </widget>\n"
            "</ui>\n";
}

QString Hue_Slider_Plugin::includeFile() const
{
    return "Hue_Slider";
}


