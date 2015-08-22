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
#include <cmath>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QPainter>

namespace color_widgets {

class ColorPalette::Private
{
public:
    QVector<QPair<QColor,QString> >   colors;
    int             columns;
    QString         name;
    QString         fileName;
    bool            dirty;

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
    p->columns = 0;
    p->dirty = false;
}

ColorPalette::ColorPalette(const QVector<QPair<QColor,QString> >& colors,
                           const QString& name,
                           int columns)
{
    setName(name);
    setColumns(columns);
    setColors(colors);
    p->dirty = false;
}
ColorPalette::ColorPalette(const ColorPalette& other)
    : p ( new Private(*other.p) )
{
}
ColorPalette& ColorPalette::operator=(const ColorPalette& other)
{
    *p = *other.p;
    emit colorsChanged(p->colors);
    emit columnsChanged(p->columns);
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
    emit columnsChanged(p->columns);
    emit nameChanged(p->name);
    emit fileNameChanged(p->fileName);
    return *this;
}
#endif // Q_COMPILER_RVALUE_REFS

QColor ColorPalette::colorAt(int index) const
{
    return p->valid_index(index) ? p->colors[index].first : QColor();
}

QString ColorPalette::nameAt(int index) const
{
    return p->valid_index(index) ? p->colors[index].second : QString();
}

QVector<QPair<QColor,QString> > ColorPalette::colors() const
{
    return p->colors;
}

int ColorPalette::count() const
{
    return p->colors.size();
}

int ColorPalette::columns()
{
    return p->columns;
}

QString ColorPalette::name() const
{
    return p->name;
}

bool ColorPalette::load(const QString& name)
{
    p->fileName = name;
    emit fileNameChanged(p->fileName);
    p->colors.clear();
    p->columns = 0;
    p->name = QString();
    p->dirty = false;

    QFile file(name);

    if ( !file.open(QFile::ReadOnly|QFile::Text) )
        return false;

    QTextStream stream( &file );

    if ( stream.readLine() != "GIMP Palette" )
        return false;

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
    setName(properties["name"]);
    setColumns(properties["columns"].toInt());

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
        p->colors.push_back(qMakePair(QColor(r, g, b), line));
    }

    emit colorsChanged(p->colors);
    p->dirty = false;

    return true;
}

ColorPalette ColorPalette::fromFile(const QString& name)
{
    ColorPalette p;
    p.load(name);
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
        filename = unnamed(p->name)+".gpl";
    }

    QFile file(filename);
    if ( !file.open(QFile::Text|QFile::WriteOnly) )
        return false;

    QTextStream stream(&file);

    stream << "GIMP Palette\n";
    stream << "Name: " << unnamed(p->name) << '\n';
    if ( p->columns )
        stream << "Columns: " << p->columns << '\n';
    /// \todo Options to add comments
    stream << "#\n";

    for ( int i = 0; i < p->colors.size(); i++ )
    {
        stream << qSetFieldWidth(3) << p->colors[i].first.red() << qSetFieldWidth(0) << ' '
               << qSetFieldWidth(3) << p->colors[i].first.green() << qSetFieldWidth(0) << ' '
               << qSetFieldWidth(3) << p->colors[i].first.blue() << qSetFieldWidth(0) << '\t'
               << unnamed(p->colors[i].second) << '\n';
    }

    if ( !file.error() )
    {
        p->dirty = false;
        return true;
    }

    return false;
}


QString ColorPalette::fileName() const
{
    return p->fileName;
}


void ColorPalette::setColumns(int columns)
{
    if ( columns <= 0 )
        columns = 0;

    if ( columns != p->columns )
    {
        p->dirty = true;
        emit columnsChanged( p->columns = columns );
    }
}

void ColorPalette::setColors(const QVector<QColor>& colors)
{
    p->colors.clear();
    foreach(const QColor& col, colors)
        p->colors.push_back(qMakePair(col,QString()));
    p->dirty = true;
    emit colorsChanged(p->colors);
}

void ColorPalette::setColors(const QVector<QPair<QColor,QString> >& colors)
{
    p->colors = colors;
    p->dirty = true;
    emit colorsChanged(p->colors);
}


void ColorPalette::setColorAt(int index, const QColor& color)
{
    if ( !p->valid_index(index) )
        return;

    p->colors[index].first = color;

    p->dirty = true;
    emit colorsChanged(p->colors);
}

void ColorPalette::setColorAt(int index, const QColor& color, const QString& name)
{
    if ( !p->valid_index(index) )
        return;

    p->colors[index].first = color;
    p->colors[index].second = name;
    p->dirty = true;
    emit colorsChanged(p->colors);
}

void ColorPalette::setNameAt(int index, const QString& name)
{
    if ( !p->valid_index(index) )
        return;

    p->colors[index].second = name;

    p->dirty = true;
    emit colorsChanged(p->colors);
}


void ColorPalette::appendColor(const QColor& color, const QString& name)
{
    p->colors.push_back(qMakePair(color,name));
    p->dirty = true;
    emit colorsChanged(p->colors);
}

void ColorPalette::insertColor(int index, const QColor& color, const QString& name)
{
    if ( index < 0 || index > p->colors.size() )
        return;

    p->colors.insert(index, qMakePair(color, name));

    p->dirty = true;
    emit colorsChanged(p->colors);
}

void ColorPalette::eraseColor(int index)
{
    if ( !p->valid_index(index) )
        return;

    p->colors.remove(index);

    p->dirty = true;
    emit colorsChanged(p->colors);
}

void ColorPalette::setName(const QString& name)
{
    p->dirty = true;
    p->name = name;
}

void ColorPalette::setFileName(const QString& name)
{
    p->dirty = true;
    p->fileName = name;
}

QString ColorPalette::unnamed(const QString& name) const
{
    return name.isEmpty() ? tr("Unnamed") : name;
}


QPixmap ColorPalette::preview(const QSize& size, const QColor& background) const
{
    if ( !size.isValid() || p->colors.empty() )
        return QPixmap();

    QPixmap out( size );
    out.fill(background);
    QPainter painter(&out);

    int count = p->colors.size();
    int columns = p->columns;
    if ( !columns )
        columns = std::ceil( std::sqrt( count * float(size.width()) / size.height() ) );
    int rows = std::ceil( float(count) / columns );
    QSizeF color_size(float(size.width()) / columns, float(size.height()) / rows);

    for ( int y = 0, i = 0; y < rows && i < count; y++ )
    {
        for ( int x = 0; x < columns && i < count; x++, i++ )
        {
            painter.fillRect(QRectF(x*color_size.width(), y*color_size.height(),
                             color_size.width(), color_size.height()),
                             p->colors[i].first
                            );
        }
    }

    return out;
}

bool ColorPalette::dirty() const
{
    return p->dirty;
}

void ColorPalette::setDirty(bool dirty)
{
    p->dirty = dirty;
}

QVector<QColor> ColorPalette::onlyColors() const
{
    QVector<QColor> out;
    out.reserve(p->colors.size());
    for ( int i = 0; i < p->colors.size(); i++ )
        out.push_back(p->colors[i].second);
    return out;
}

} // namespace color_widgets
