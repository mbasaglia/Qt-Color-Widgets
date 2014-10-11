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
#ifndef COLOR_WIDGET_PLUGIN_COLLECTION_HPP
#define COLOR_WIDGET_PLUGIN_COLLECTION_HPP

#include <QDesignerCustomWidgetCollectionInterface>

class Color_Widget_Plugin_Collection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
#ifdef FRAMEWORK_QT5
    Q_PLUGIN_METADATA(IID "mattia.basaglia.Color_Widgets_Plugin")
#endif
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    explicit Color_Widget_Plugin_Collection(QObject *parent = 0);

    QList<QDesignerCustomWidgetInterface*> customWidgets() const;

   private:
       QList<QDesignerCustomWidgetInterface*> widgets;
    
};

#endif // COLOR_WIDGET_PLUGIN_COLLECTION_HPP
