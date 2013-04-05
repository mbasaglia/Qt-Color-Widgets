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
    return QIcon();
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
           " </widget>\n"
            "</ui>\n";
}

QString Color_Wheel_Plugin::includeFile() const
{
    return "color_wheel.hpp";
}


