/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013-2015 Mattia Basaglia

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

#include "color_selector.hpp"
#include "color_dialog.hpp"
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

class Color_Selector::Private
{
public:
    Update_Mode update_mode;
    Color_Dialog *dialog;
    QColor old_color;

    Private(QWidget *widget) : dialog(new Color_Dialog(widget))
    {}
};

Color_Selector::Color_Selector(QWidget *parent) :
    Color_Preview(parent), p(new Private(this))
{
    setUpdateMode(Continuous);
    p->old_color = color();

    connect(this,SIGNAL(clicked()),this,SLOT(showDialog()));
    connect(this,SIGNAL(colorChanged(QColor)),this,SLOT(update_old_color(QColor)));
    connect(p->dialog,SIGNAL(rejected()),this,SLOT(reject_dialog()));
    connect(p->dialog,SIGNAL(colorSelected(QColor)), this, SLOT(accept_dialog()));
    connect(p->dialog,SIGNAL(wheelFlagsChanged(Color_Wheel::Display_Flags)),
                SIGNAL(wheelFlagsChanged(Color_Wheel::Display_Flags)));

    setAcceptDrops(true);
}

Color_Selector::~Color_Selector()
{
    delete p;
}

Color_Selector::Update_Mode Color_Selector::updateMode() const
{
    return p->update_mode;
}

void Color_Selector::setUpdateMode(Update_Mode m)
{
    p->update_mode = m;
}

Qt::WindowModality Color_Selector::dialogModality() const
{
    return p->dialog->windowModality();
}

void Color_Selector::setDialogModality(Qt::WindowModality m)
{
    p->dialog->setWindowModality(m);
}

Color_Wheel::Display_Flags Color_Selector::wheelFlags() const
{
    return p->dialog->wheelFlags();
}

void Color_Selector::showDialog()
{
    p->old_color = color();
    p->dialog->setColor(color());
    connect_dialog();
    p->dialog->show();
}

void Color_Selector::setWheelFlags(Color_Wheel::Display_Flags flags)
{
    p->dialog->setWheelFlags(flags);
}

void Color_Selector::connect_dialog()
{
    if (p->update_mode == Continuous)
        connect(p->dialog, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)), Qt::UniqueConnection);
    else
        disconnect_dialog();
}

void Color_Selector::disconnect_dialog()
{
    disconnect(p->dialog, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
}

void Color_Selector::accept_dialog()
{
    setColor(p->dialog->color());
    p->old_color = color();
}

void Color_Selector::reject_dialog()
{
    setColor(p->old_color);
}

void Color_Selector::update_old_color(const QColor &c)
{
    if (!p->dialog->isVisible())
        p->old_color = c;
}

void Color_Selector::dragEnterEvent(QDragEnterEvent *event)
{
    if ( event->mimeData()->hasColor() ||
         ( event->mimeData()->hasText() && QColor(event->mimeData()->text()).isValid() ) )
        event->acceptProposedAction();
}


void Color_Selector::dropEvent(QDropEvent *event)
{
    if ( event->mimeData()->hasColor() )
    {
        setColor(event->mimeData()->colorData().value<QColor>());
        event->accept();
    }
    else if ( event->mimeData()->hasText() )
    {
        QColor col(event->mimeData()->text());
        if ( col.isValid() )
        {
            setColor(col);
            event->accept();
        }
    }
}
