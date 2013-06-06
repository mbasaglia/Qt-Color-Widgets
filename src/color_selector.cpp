#include "color_selector.hpp"
#include "color_dialog.hpp"
#include <QDropEvent>
#include <QDragEnterEvent>

Color_Selector::Color_Selector(QWidget *parent) :
    Color_Preview(parent), dialog(new Color_Dialog(this))
{
    setUpdateMode(Continuous);
    old_color = color();
    connect(this,SIGNAL(clicked()),this,SLOT(showDialog()));
    connect(this,SIGNAL(colorChanged(QColor)),this,SLOT(update_old_color(QColor)));
    connect(dialog,SIGNAL(accepted()),this,SLOT(accept_dialog()));
    connect(dialog,SIGNAL(rejected()),this,SLOT(reject_dialog()));
    setAcceptDrops(true);
}

void Color_Selector::setUpdateMode(Color_Selector::Update_Mode m)
{
    update_mode = m;
}

Qt::WindowModality Color_Selector::dialogModality() const
{
    return dialog->windowModality();
}

void Color_Selector::setDialogModality(Qt::WindowModality m)
{
    dialog->setWindowModality(m);
}

void Color_Selector::showDialog()
{
    old_color = color();
    dialog->setColor(color());
    connect_dialog();
    dialog->show();
}

void Color_Selector::connect_dialog()
{
    if ( update_mode == Continuous )
        connect(dialog,SIGNAL(colorChanged(QColor)),this,SLOT(setColor(QColor)));
    else
        disconnect_dialog();
}

void Color_Selector::disconnect_dialog()
{
    disconnect(dialog,SIGNAL(colorChanged(QColor)),this,SLOT(setColor(QColor)));
}

void Color_Selector::accept_dialog()
{
    disconnect_dialog();
    setColor(dialog->color());
}

void Color_Selector::reject_dialog()
{
    setColor(old_color);
}

void Color_Selector::update_old_color(QColor c)
{
    if ( !dialog->isVisible() )
        old_color = c;
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
