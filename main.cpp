#include "color_dialog.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Color_Dialog w;
    w.setColor(QColor(128,255,16,128));
    w.show();
    
    return a.exec();
}
