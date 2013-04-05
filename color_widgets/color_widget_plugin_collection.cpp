#include "color_widget_plugin_collection.hpp"
#include "color_preview_plugin.hpp"
#include "color_wheel_plugin.hpp"

Color_Widget_Plugin_Collection::Color_Widget_Plugin_Collection(QObject *parent) :
    QObject(parent)
{
    widgets.push_back(new Color_Preview_Plugin(this));
    widgets.push_back(new Color_Wheel_Plugin(this));
}

QList<QDesignerCustomWidgetInterface *> Color_Widget_Plugin_Collection::customWidgets() const
{
    return widgets;
}
