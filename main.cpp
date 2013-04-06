#include "color_dialog.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Color_Dialog w;
    w.setColor(QColor(172,169,64,128));
    w.show();

    /*QPixmap p(w.size());
    w.render(&p);
    p.save("screenshot.png");*/
    
    return a.exec();
}
