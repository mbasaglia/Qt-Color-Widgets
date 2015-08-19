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

#include "color_palette.hpp"
#include <QFile>
#include <QTextStream>
#include <QHash>

namespace color_widgets {

class ColorPalette::Private
{
public:
    QVector<QColor>   colors;
    QVector<QString>  names;
    int             columns;
    QString         name;
    QString         fileName;

    bool valid_index(int index)
    {
        return index >= 0 && index < colors.size();
    }
};

ColorPalette::ColorPalette(const QVector<QColor>& colors,
                           const QString& name,
                           int columns)
    : p ( new Private )
{
    setName(name);
    setColumns(columns);
    setColors(colors);
}

ColorPalette::ColorPalette(const QString& name)
    : p ( new Private )
{
    setName(name);
    p->columns = -1;
}

ColorPalette::ColorPalette(const QVector<QPair<QColor,QString> >& colors,
                           const QString& name,
                           int columns)
{
    setName(name);
    setColumns(columns);
    setColors(colors);
}
ColorPalette::ColorPalette(const ColorPalette& other)
    : p ( new Private(*other.p) )
{
}
ColorPalette& ColorPalette::operator=(const ColorPalette& other)
{
    *p = *other.p;
    emit colorsChanged(p->colors);
    emit namesChanged(p->names);
    emit columnsChanged(p->columns);
    emit rowsChanged(rows());
    emit nameChanged(p->name);
    emit fileNameChanged(p->fileName);
    return *this;
}

ColorPalette::~ColorPalette()
{
    delete p;
}

#ifdef Q_COMPILER_RVALUE_REFS
ColorPalette::ColorPalette(ColorPalette&& other)
    : p ( other.p )
{
    other.p = nullptr;
}
ColorPalette& ColorPalette::operator=(ColorPalette&& other)
{
    std::swap(p, other.p);
    emit colorsChanged(p->colors);
    emit namesChanged(p->names);
    emit columnsChanged(p->columns);
    emit rowsChanged(rows());
    emit nameChanged(p->name);
    emit fileNameChanged(p->fileName);
    return *this;
}
#endif // Q_COMPILER_RVALUE_REFS

QColor ColorPalette::colorAt(int index) const
{
    return p->valid_index(index) ? p->colors[index] : QColor();
}

QString ColorPalette::nameAt(int index) const
{
    return p->valid_index(index) ? p->names[index] : unnamed();
}

QVector<QColor> ColorPalette::colors() const
{
    return p->colors;
}

QVector<QString> ColorPalette::names() const
{
    return p->names;
}

int ColorPalette::count() const
{
    return p->colors.size();
}

int ColorPalette::rows() const
{
    return p->columns <= 0 ? -1 : p->colors.size() / p->columns;
}

int ColorPalette::columns()
{
    return p->columns;
}

QString ColorPalette::name() const
{
    return p->name;
}

ColorPalette ColorPalette::fromFile(const QString& name)
{
    ColorPalette p;
    p.setFileName( name );

    QFile file(name);

    if ( !file.open(QFile::ReadOnly|QFile::Text) )
        return p;

    QTextStream stream( &file );

    if ( stream.readLine() != "GIMP Palette" )
        return p;

    QString line;

    // parse properties
    QHash<QString,QString> properties;
    while( !stream.atEnd() )
    {
        line = stream.readLine();
        if ( line.isEmpty() )
            continue;
        if ( line[0] == '#' )
            break;
        int colon = line.indexOf(':');
        if ( colon != -1 )
            properties[line.left(colon).toLower()] =
                line.right(line.size() - colon - 1).trimmed();
    }
    /// \todo Store extra properties in the palette object
    p.setName(properties["name"]);
    p.setColumns(properties["columns"].toInt());

    // Skip comments
    while( !stream.atEnd() )
    {
        qint64 pos = stream.pos();
        line = stream.readLine();
        if ( !line.isEmpty() && line[0] != '#' )
        {
            stream.seek(pos);
            break;
        }
    }

    while( !stream.atEnd() )
    {
        int r = 0, g = 0, b = 0;
        stream >> r >> g >> b;
        line = stream.readLine().trimmed();
        p.appendColor(QColor(r, g, b), line);
    }

    return p;
}

bool ColorPalette::save(const QString& filename)
{
    setFileName(filename);
    return save();
}

bool ColorPalette::save() const
{
    QString filename = p->fileName;
    if ( filename.isEmpty() )
    {
        if ( !p->name.isEmpty() )
            filename = p->name;
        else
            filename = unnamed();
        filename += ".gpl";
    }

    QFile file(filename);
    if ( !file.open(QFile::Text|QFile::WriteOnly) )
        return false;

    QTextStream stream(&file);

    stream << "GIMP Palette\n";
    stream << "Name: " << p->name << '\n';
    if ( p->columns != -1 )
        stream << "Columns: " << p->columns << '\n';
    /// \todo Options to add comments
    stream << "#\n";

    for ( int i = 0; i < p->colors.size(); i++ )
    {
        stream << qSetFieldWidth(3) << p->colors[i].red() << qSetFieldWidth(0) << ' '
               << qSetFieldWidth(3) << p->colors[i].green() << qSetFieldWidth(0) << ' '
               << qSetFieldWidth(3) << p->colors[i].blue() << qSetFieldWidth(0) << '\t'
               << p->names[i] << '\n';
    }

    return !file.error();
}


QString ColorPalette::fileName() const
{
    return p->fileName;
}


void ColorPalette::setColumns(int columns)
{
    if ( columns <= 0 )
        columns = -1;

    if ( columns != p->columns )
    {
        int oldrows = rows();

        p->columns = columns;

        emit columnsChanged( p->columns );
        if ( rows() != oldrows )
            emit rowsChanged(rows());
    }
}

void ColorPalette::setColors(const QVector<QColor>& colors)
{
    int oldrows = rows();

    p->colors = colors;
    p->names = QVector<QString>(p->colors.size());

    emit colorsChanged(p->colors);
    emit namesChanged(p->names);
    if ( rows() != oldrows )
        emit rowsChanged(rows());
}

void ColorPalette::setColors(const QVector<QPair<QColor,QString> >& colors)
{
    int oldrows = rows();

    p->colors.clear();
    p->colors.reserve(colors.size());
    p->names.clear();
    p->names.reserve(colors.size());

    for(int i = 0; i < colors.size(); i++)
    {
        p->colors.push_back(colors[i].first);
        p->names.push_back(unnamed(colors[i].second));
    }

    emit colorsChanged(p->colors);
    emit namesChanged(p->names);
    if ( rows() != oldrows )
        emit rowsChanged(rows());
}

void ColorPalette::setColorAt(int index, const QColor& color, const QString& name)
{
    if ( !p->valid_index(index) )
        return;

    p->colors[index] = color;
    emit colorsChanged(p->colors);

    if ( !name.isNull() )
    {
        p->names[index] = name;
        emit namesChanged(p->names);
    }
}

void ColorPalette::setNameAt(int index, const QString& name)
{
    if ( !p->valid_index(index) )
        return;

    p->names[index] = unnamed(name);

    emit namesChanged(p->names);
}


void ColorPalette::appendColor(const QColor& color, const QString& name)
{
    int oldrows = rows();

    p->colors.push_back(color);
    p->names.push_back(unnamed(name));

    emit colorsChanged(p->colors);
    emit namesChanged(p->names);
    if ( rows() != oldrows )
        emit rowsChanged(rows());
}

void ColorPalette::insertColor(int index, const QColor& color, const QString& name)
{
    if ( index < 0 || index > p->colors.size() )
        return;

    int oldrows = rows();

    p->colors.insert(index, color);
    p->names.insert(index, unnamed(name));

    emit colorsChanged(p->colors);
    emit namesChanged(p->names);
    if ( rows() != oldrows )
        emit rowsChanged(rows());
}

void ColorPalette::eraseColor(int index)
{
    if ( !p->valid_index(index) )
        return;

    int oldrows = rows();

    p->colors.remove(index);
    p->names.remove(index);

    emit colorsChanged(p->colors);
    emit namesChanged(p->names);
    if ( rows() != oldrows )
        emit rowsChanged(rows());
}

void ColorPalette::setName(const QString& name)
{
    p->name = unnamed(name);
}

void ColorPalette::setFileName(const QString& name)
{
    p->fileName = name;
}

QString ColorPalette::unnamed(const QString& name) const
{
    return name.isNull() ? tr("Unnamed") : name;
}

} // namespace color_widgets
