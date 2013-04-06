#ifndef COLOR_DIALOG_HPP
#define COLOR_DIALOG_HPP

#include "ui_color_dialog.h"

class Color_Dialog : public QDialog, private Ui::Color_Dialog
{
    Q_OBJECT

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
    /// Update from HSV sliders
    void set_hsv();
    /// Update from RGB sliders
    void set_rgb();
private slots:
    void on_edit_hex_editingFinished();
    void on_edit_hex_textEdited(const QString &arg1);
};

#endif // COLOR_DIALOG_HPP
