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
#ifndef COLOR_WIDGETS_COLOR_PALETTE_MODEL_HPP
#define COLOR_WIDGETS_COLOR_PALETTE_MODEL_HPP

#include <QAbstractListModel>
#include "color_palette.hpp"

namespace color_widgets {

class ColorPaletteModel : public QAbstractListModel
{
    Q_OBJECT
    /**
     * \brief List of directories to be scanned for palette files
     */
    Q_PROPERTY(QStringList searchPaths READ searchPaths WRITE setSearchPaths NOTIFY searchPathsChanged)

    /**
     * \brief Default directory to be used when saving a palette
     */
    Q_PROPERTY(QString savePath READ savePath WRITE setSavePath NOTIFY savePathChanged)

    /**
     * \brief Size of the icon used for the palette previews
     */
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)


public:
    ColorPaletteModel();
    ~ColorPaletteModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QString savePath() const;
    QStringList searchPaths() const;
    QSize iconSize() const;

    /**
     * \brief Returns a reference to the first palette with the given name
     * \pre hasPalette(name)
     */
    const ColorPalette& palette(const QString& name) const;

    /**
     * \brief Whether a palette with the given name exists in the model
     */
    bool hasPalette(const QString& name) const;

    /**
     * \brief Get the palette at the given index (row)
     * \pre 0 <= index < rowCount()
     */
    const ColorPalette& palette(int index) const;

    /**
     * \brief Get the palette at the given index (row)
     * \pre 0 <= index < rowCount()
     * \todo Maybe it's unsafe to be able to modify them outside the model
     */
    ColorPalette& palette(int index);

public slots:
    void setSavePath(const QString& savePath);
    void setSearchPaths(const QStringList& searchPaths);
    void addSearchPath(const QString& path);
    void setIconSize(const QSize& iconSize);
    void addPalette(const ColorPalette& palette);

    /**
     * \brief Load palettes files found in the search paths
     */
    void load();

signals:
    void savePathChanged(const QString& savePath);
    void searchPathsChanged(const QStringList& searchPaths);
    void iconSizeChanged(const QSize& iconSize);

private:
    class Private;
    Private* p;
};

} // namespace color_widgets

#endif // COLOR_WIDGETS_COLOR_PALETTE_MODEL_HPP
