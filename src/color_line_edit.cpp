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

#include "color_line_edit.hpp"
#include <QRegularExpression>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

namespace color_widgets {

static QRegularExpression regex_3hex_rgb ("^#[[:xdigit]]{3}$");
static QRegularExpression regex_func_rgb (R"(^rgb\s*\(\s*([0-9]+)\s*,\s*([0-9]+)\s*,\s*([0-9]+)\s*\)$)");
static QRegularExpression regex_hex_rgba ("^#[[:xdigit]]{8}$");
static QRegularExpression regex_func_rgba (R"(^rgba?\s*\(\s*([0-9]+)\s*,\s*([0-9]+)\s*,\s*([0-9]+)\s*,\s*([0-9]+)\s*\)$)");

class ColorLineEdit::Private
{
public:
    QColor color;
    bool show_alpha = false;

    QString stringFromColor(const QColor& c)
    {
        if ( !show_alpha )
            return c.name();
        return c.name()+QString::number(c.alpha(),16);
    }

    QColor colorFromString(const QString& s)
    {
        QString xs = s.trimmed();
        QRegularExpressionMatch match;

        match = regex_3hex_rgb.match(xs);
        if ( match.hasMatch() )
        {
            return QColor(
                QString(2,xs[1]).toInt(nullptr,16),
                QString(2,xs[2]).toInt(nullptr,16),
                QString(2,xs[3]).toInt(nullptr,16)
            );
        }

        match = regex_func_rgb.match(xs);
        if ( match.hasMatch() )
        {
            return QColor(
                match.captured(1).toInt(),
                match.captured(2).toInt(),
                match.captured(3).toInt()
            );
        }

        if ( show_alpha )
        {
            match = regex_hex_rgba.match(xs);
            if ( match.hasMatch() )
            {
                return QColor(
                    xs.mid(1,2).toInt(nullptr,16),
                    xs.mid(3,2).toInt(nullptr,16),
                    xs.mid(5,2).toInt(nullptr,16),
                    xs.mid(7,2).toInt(nullptr,16)
                );
            }

            match = regex_func_rgba.match(xs);
            if ( match.hasMatch() )
            {
                return QColor(
                    match.captured(1).toInt(),
                    match.captured(2).toInt(),
                    match.captured(3).toInt(),
                    match.captured(4).toInt()
                );
            }
        }

        return QColor(xs);
    }
};

ColorLineEdit::ColorLineEdit(QWidget* parent)
    : QLineEdit(parent), p(new Private)
{
    /// \todo determine if having this connection might be useful
    /*connect(this, &QLineEdit::textChanged, [this](const QString& text){
        QColor color = p->colorFromString(text);
        if ( color.isValid() )
            emit colorChanged(color);
    });*/
    connect(this, &QLineEdit::textEdited, [this](const QString& text){
        QColor color = p->colorFromString(text);
        if ( color.isValid() )
        {
            emit colorEdited(color);
            emit colorChanged(color);
        }
    });
    connect(this, &QLineEdit::editingFinished, [this](){
        QColor color = p->colorFromString(text());
        if ( color.isValid() )
        {
            p->color = color;
            emit colorEditingFinished(color);
            emit colorChanged(color);
        }
        else
        {
            setText(p->color.name());
            emit colorEditingFinished(p->color);
            emit colorChanged(color);
        }
    });
}

ColorLineEdit::~ColorLineEdit()
{
    delete p;
}

QColor ColorLineEdit::color() const
{
    return p->color;
}

void ColorLineEdit::setColor(const QColor& color)
{
    if ( color != p->color )
    {
        p->color = color;
        setText(p->stringFromColor(p->color));
        emit colorChanged(p->color);
    }
}

void ColorLineEdit::setShowAlpha(bool showAlpha)
{
    if ( p->show_alpha )
    {
        p->show_alpha = showAlpha;
        setText(p->stringFromColor(p->color));
        emit showAlphaChanged(p->show_alpha);
    }
}

bool ColorLineEdit::showAlpha() const
{
    return p->show_alpha;
}

void ColorLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if ( event->mimeData()->hasColor() ||
         ( event->mimeData()->hasText() && p->colorFromString(event->mimeData()->text()).isValid() ) )
        event->acceptProposedAction();
}


void ColorLineEdit::dropEvent(QDropEvent *event)
{
    if ( event->mimeData()->hasColor() )
    {
        setColor(event->mimeData()->colorData().value<QColor>());
        event->accept();
    }
    else if ( event->mimeData()->hasText() )
    {
        QColor col =  p->colorFromString(event->mimeData()->text());
        if ( col.isValid() )
        {
            setColor(col);
            event->accept();
        }
    }
}

} // namespace color_widgets
