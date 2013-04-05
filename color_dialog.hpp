#ifndef COLOR_DIALOG_HPP
#define COLOR_DIALOG_HPP

#include "ui_color_dialog.h"

class Color_Dialog : public QDialog, private Ui::Color_Dialog
{
    Q_OBJECT

private:
    QColor col;

public:
    explicit Color_Dialog(QWidget *parent = 0);

    QColor color() const;

public slots:

    void setColor(QColor c);

signals:
    /**
     * Emitted when the user selects a color or setColor is called
     */
    void colorChanged(QColor);

protected slots:
    void update_widgets();
    void set_hsl();
    void set_rgba();
private slots:
    void on_edit_hex_editingFinished();
    void on_edit_hex_textEdited(const QString &arg1);
};

#endif // COLOR_DIALOG_HPP
