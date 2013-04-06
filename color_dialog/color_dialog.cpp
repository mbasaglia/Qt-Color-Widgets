#include "color_dialog.hpp"

Color_Dialog::Color_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QVector<QColor> rainbow;
    for ( int i = 0; i < 360; i+= 360/6 )
        rainbow.push_back(QColor::fromHsv(i,255,255));
    rainbow.push_back(Qt::red);
    slide_hue->setBackground(rainbow);
}

QColor Color_Dialog::color() const
{
    QColor col = wheel->color();
    col.setAlpha(slide_alpha->value());
    return col;
}

void Color_Dialog::setColor(QColor c)
{
    wheel->setColor(c);
    slide_alpha->setValue(c.alpha());
    update_widgets();
}


void Color_Dialog::update_widgets()
{
    blockSignals(true);

    QColor col = color();

    slide_red->setValue(col.red());
    slide_red->setFirstColor(QColor(0,col.green(),col.blue()));
    slide_red->setLastColor(QColor(255,col.green(),col.blue()));

    slide_green->setValue(col.green());
    slide_green->setFirstColor(QColor(col.red(),0,col.blue()));
    slide_green->setLastColor(QColor(col.red(),255,col.blue()));

    slide_blue->setValue(col.blue());
    slide_blue->setFirstColor(QColor(col.red(),col.green(),0));
    slide_blue->setLastColor(QColor(col.red(),col.green(),255));

    slide_hue->setValue(wheel->hue());

    slide_saturation->setValue(wheel->saturation());
    slide_saturation->setFirstColor(QColor::fromHsv(wheel->hue(),0,wheel->value()));
    slide_saturation->setLastColor(QColor::fromHsv(wheel->hue(),255,wheel->value()));

    slide_value->setValue(wheel->value());
    slide_value->setFirstColor(QColor::fromHsv(wheel->hue(),wheel->saturation(),0));
    slide_value->setLastColor(QColor::fromHsv(wheel->hue(),wheel->saturation(),255));


    edit_hex->setText(QString("%1%2%3%4")
                      .arg(col.red(),2,16,QChar('0'))
                      .arg(col.green(),2,16,QChar('0'))
                      .arg(col.blue(),2,16,QChar('0'))
                      .arg(col.alpha(),2,16,QChar('0'))
                     );

    preview->setColor(col);

    blockSignals(false);

    emit colorChanged(col);
}

void Color_Dialog::set_hsv()
{
    if ( !signalsBlocked() )
    {
        wheel->setColor(QColor::fromHsv(
                slide_hue->value(),
                slide_saturation->value(),
                slide_value->value()
            ));
        update_widgets();
    }
}

void Color_Dialog::set_rgb()
{
    if ( !signalsBlocked() )
    {
        QColor c ( slide_red->value(), slide_green->value(), slide_blue->value() );
        if ( c.saturation() == 0 )
            c = QColor::fromHsv(slide_hue->value(),0,c.value());
        wheel->setColor(c);
        update_widgets();
    }
}


void Color_Dialog::on_edit_hex_editingFinished()
{
    QString xs = edit_hex->text().trimmed();
    if ( xs.size() == 3 )
    {
        slide_red->setValue(QString(2,xs[0]).toInt(0,16));
        slide_green->setValue(QString(2,xs[1]).toInt(0,16));
        slide_blue->setValue(QString(2,xs[2]).toInt(0,16));
        slide_alpha->setValue(255);
    }
    else
    {
        if ( xs.size() < 8 )
        {
            xs += QString(8-xs.size(),'f');
        }
        slide_red->setValue(xs.mid(0,2).toInt(0,16));
        slide_green->setValue(xs.mid(2,2).toInt(0,16));
        slide_blue->setValue(xs.mid(4,2).toInt(0,16));
        slide_alpha->setValue(xs.mid(6,2).toInt(0,16));
    }

    set_rgb();

}

void Color_Dialog::on_edit_hex_textEdited(const QString &arg1)
{

    QString xs = arg1.trimmed();
    if ( xs.size() == 8 )
    {
        slide_red->setValue(xs.mid(0,2).toInt(0,16));
        slide_green->setValue(xs.mid(2,2).toInt(0,16));
        slide_blue->setValue(xs.mid(4,2).toInt(0,16));
        slide_alpha->setValue(xs.mid(6,2).toInt(0,16));
    }

    set_rgb();
}
