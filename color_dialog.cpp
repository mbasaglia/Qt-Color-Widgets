#include "color_dialog.hpp"

Color_Dialog::Color_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

QColor Color_Dialog::color() const
{
    return col;
}

void Color_Dialog::setColor(QColor c)
{
    col = c;
    update_widgets();
    emit colorChanged(c);
}


void Color_Dialog::update_widgets()
{
    blockSignals(true);
    slide_alpha->setValue(col.alpha());
    slide_red->setValue(col.red());
    slide_green->setValue(col.green());
    slide_blue->setValue(col.blue());
    slide_hue->setValue(col.hue());
    slide_saturation->setValue(col.saturation());
    slide_lightness->setValue(col.lightness());
    edit_hex->setText(QString("%1%2%3%4")
                      .arg(col.red(),2,16,QChar('0'))
                      .arg(col.green(),2,16,QChar('0'))
                      .arg(col.blue(),2,16,QChar('0'))
                      .arg(col.alpha(),2,16,QChar('0'))
                     );
    preview->setBackgroundBrush(col);
    preview->repaint();
    wheel->setColor(col);
    blockSignals(false);
}

void Color_Dialog::set_hsl()
{
    if ( !signalsBlocked() )
    {
        col.setHsl(slide_hue->value(),slide_saturation->value(),slide_lightness->value());
        update_widgets();
        emit colorChanged(col);
    }
}

void Color_Dialog::set_rgba()
{
    if ( !signalsBlocked() )
    {
        col.setRgb(slide_red->value(),slide_green->value(),slide_blue->value(),
                   slide_alpha->value());
        update_widgets();
        emit colorChanged(col);
    }
}

void Color_Dialog::on_edit_hex_editingFinished()
{
    QString xs = edit_hex->text().trimmed();
    if ( xs.size() == 3 )
    {
        col.setRgb(QString(2,xs[0]).toInt(0,16),
                   QString(2,xs[1]).toInt(0,16),
                   QString(2,xs[2]).toInt(0,16) );
    }
    else
    {
        if ( xs.size() < 8 )
        {
            xs += QString(8-xs.size(),'f');
        }
        col.setRgb(xs.mid(0,2).toInt(0,16),
                   xs.mid(2,2).toInt(0,16),
                   xs.mid(4,2).toInt(0,16),
                   xs.mid(6,2).toInt(0,16)
                );
    }

    update_widgets();
    emit colorChanged(col);

}

void Color_Dialog::on_edit_hex_textEdited(const QString &arg1)
{

    QString xs = arg1.trimmed();
    if ( xs.size() == 8 )
    {
        col.setRgb(xs.mid(0,2).toInt(0,16),
                   xs.mid(2,2).toInt(0,16),
                   xs.mid(4,2).toInt(0,16),
                   xs.mid(6,2).toInt(0,16)
                );
    }

    update_widgets();
    emit colorChanged(col);
}
