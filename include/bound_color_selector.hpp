/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2015  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef BOUND_COLOR_SELECTOR_HPP
#define BOUND_COLOR_SELECTOR_HPP

#include "color_selector.hpp"

/**
 *  A color selector bound to a color reference
 */
class QCP_EXPORT Bound_Color_Selector : public Color_Selector
{
    Q_OBJECT
private:
    QColor* ref;
public:
    explicit Bound_Color_Selector(QColor* reference, QWidget *parent = 0);

private slots:
    void update_reference(QColor);
};

#endif // BOUND_COLOR_SELECTOR_HPP
