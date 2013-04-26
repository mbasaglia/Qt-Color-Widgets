/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

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

#ifndef COLOR_LIST_MODEL_HPP
#define COLOR_LIST_MODEL_HPP

#include <QAbstractTableModel>
#include <QColor>

/**
 *  Model for a list of colors
 */
class Color_List_Model : public QAbstractTableModel
{
    Q_OBJECT

private:
    QList<QColor> colors;

public:
    explicit Color_List_Model(QObject *parent = 0);
    

    int rowCount (const QModelIndex & m = QModelIndex() ) const;
    int columnCount ( const QModelIndex & m= QModelIndex() ) const;

    QVariant data ( const QModelIndex &index, int role ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role ) const;


    bool insertRows ( int row, int count, const QModelIndex &parent=QModelIndex() );
    bool removeRows ( int row, int count, const QModelIndex &parent=QModelIndex() );
    /// Insert a single default entry at the end
    void insert_new();

    bool setData ( const QModelIndex &index, const QVariant &value, int role=Qt::EditRole );

    Qt::ItemFlags flags ( const QModelIndex &index ) const;
    
};

#endif // COLOR_LIST_MODEL_HPP
