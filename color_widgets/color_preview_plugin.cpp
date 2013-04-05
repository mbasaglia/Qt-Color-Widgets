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
    return "color_preview.hpp";
}

//Q_EXPORT_PLUGIN2(color_widgets, Color_Preview_Plugin);



