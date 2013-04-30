/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013 Mattia Basaglia

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
#include "color_dialog.hpp"
#include <QApplication>
#include "color_list_widget.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Color_List_Widget c;
    c.show();


    Color_Dialog w;
    w.setColor(QColor(64,172,143,128));
    w.show();

    /*QPixmap p(w.size());
    w.render(&p);
    p.save("screenshot.png");*/
    
    return a.exec();
}
