/**

@author Mattia Basaglia

@section License

    Copyright (C) 2015 Mattia Basaglia

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "color_palette_model.hpp"
#include <QDir>
#include <QList>

namespace color_widgets {

class ColorPaletteModel::Private
{
public:
    /// \todo Keep sorted by name
    QList<ColorPalette> palettes;
    QSize icon_size;
    QStringList search_paths;
    QString     save_path;

    Private()
        : icon_size(32, 32)
    {}

    bool acceptable(const QModelIndex& index) const
    {
        return index.row() >= 0 && index.row() <= palettes.count();
    }

    QList<ColorPalette>::iterator find(const QString& name)
    {
        return std::find_if(palettes.begin(), palettes.end(),
            [&name](const ColorPalette& palette) {
                return palette.name() == name;
        });
    }
};

ColorPaletteModel::ColorPaletteModel()
    : p ( new Private )
    {}

ColorPaletteModel::~ColorPaletteModel()
{
    delete p;
}

int ColorPaletteModel::rowCount(const QModelIndex &) const
{
    return p->palettes.count();
}

QVariant ColorPaletteModel::data(const QModelIndex &index, int role) const
{
    if ( !p->acceptable(index) )
        return QVariant();

    const ColorPalette& palette = p->palettes[index.row()];
    switch( role )
    {
        case Qt::DisplayRole:
            return palette.name();
        case Qt::DecorationRole:
            return palette.preview(p->icon_size);
        case Qt::ToolTipRole:
            return tr("%1 (%2 colors)").arg(palette.name()).arg(palette.count());
    }

    return QVariant();
}

QSize ColorPaletteModel::iconSize() const
{
    return p->icon_size;
}

void ColorPaletteModel::setIconSize(const QSize& iconSize)
{
    if ( p->icon_size != iconSize )
        emit iconSizeChanged( p->icon_size = iconSize );
}

QString ColorPaletteModel::savePath() const
{
    return p->save_path;
}

QStringList ColorPaletteModel::searchPaths() const
{
    return p->search_paths;
}

void ColorPaletteModel::setSavePath(const QString& savePath)
{
    if ( p->save_path != savePath )
        emit savePathChanged( p->save_path = savePath );
}

void ColorPaletteModel::setSearchPaths(const QStringList& searchPaths)
{
    if ( p->search_paths != searchPaths )
        emit searchPathsChanged( p->search_paths = searchPaths );
}

void ColorPaletteModel::addSearchPath(const QString& path)
{
    /// \todo Should compare absolute canonical paths,
    /// and these checks should also be made in setSearchPaths
    if ( !p->search_paths.contains(path) )
    {
        p->search_paths.push_back(path);
        emit searchPathsChanged( p->search_paths );
    }
}

void ColorPaletteModel::load()
{
    beginResetModel();
    p->palettes.clear();
    QStringList filters;
    filters << "*.gpl";
    for ( const QString& directory_name : p->search_paths )
    {
        QDir directory(directory_name);
        directory.setNameFilters(filters);
        directory.setFilter(QDir::Files|QDir::Readable);
        directory.setSorting(QDir::Name);
        for ( const QFileInfo& file : directory.entryInfoList() )
        {
            ColorPalette palette;
            if ( palette.load(file.absoluteFilePath()) )
            {
                p->palettes.push_back(palette);
            }
        }
    }
    endResetModel();
}

void ColorPaletteModel::addPalette(const ColorPalette& palette)
{
    p->palettes.push_back(palette);
    if ( p->palettes.back().name().isEmpty() )
        p->palettes.back().setName(tr("Unnamed"));
}

bool ColorPaletteModel::hasPalette(const QString& name) const
{
    return p->find(name) != p->palettes.end();
}

const ColorPalette& ColorPaletteModel::palette(const QString& name) const
{
    return *p->find(name);
}

const ColorPalette& ColorPaletteModel::palette(int index) const
{
    return p->palettes[index];
}

ColorPalette& ColorPaletteModel::palette(int index)
{
    return p->palettes[index];
}

} // namespace color_widgets
