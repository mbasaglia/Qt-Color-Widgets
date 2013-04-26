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

#include "color_list_model.hpp"

Color_List_Model::Color_List_Model(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int Color_List_Model::rowCount(const QModelIndex & m) const
{
    return m.isValid() ? 0 : colors.size();
}

int Color_List_Model::columnCount(const QModelIndex &m) const
{
    return m.isValid() ? 0 : 1;
}

QVariant Color_List_Model::data(const QModelIndex &index, int role) const
{
    if ( index.isValid()  && index.row() <= colors.size() &&
        ( role == Qt::DisplayRole || role == Qt::EditRole ) )
    {
        if ( index.column() == 0 )
            return QVariant::fromValue(colors[index.row()]);
    }
    return QVariant();
}

QVariant Color_List_Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) // column headers
    {
        if ( section == 0 )
            return "Color";
    }
    return QVariant();
}

bool Color_List_Model::insertRows(int row, int count, const QModelIndex &parent)
{
    if ( row < 0 || row > colors.size() )
        return false;

    beginInsertRows ( parent, row, row+count-1 );

    for ( int i = 0; i < count ; i++)
        colors.insert(colors.begin()+row,QColor(Qt::black));

    endInsertRows();

    return true;
}

bool Color_List_Model::removeRows(int row, int count, const QModelIndex &parent)
{

    if ( row < 0 || row > colors.size() )
        return false;

    beginRemoveRows ( parent, row, row+count-1 );

    colors.erase ( colors.begin()+row, colors.begin()+row+count );

    endRemoveRows();

    return true;
}

void Color_List_Model::insert_new()
{
    insertRow(colors.size());
    bool ok = setData ( index(colors.size()-1,0), Qt::black, Qt::EditRole );
    if ( !ok )
    {
        removeRow(colors.size()-1);
        return;
    }
}

bool Color_List_Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid()  && index.row() <= colors.size() && role == Qt::EditRole )
    {
        if ( index.column() == 0 )
        {
            colors[index.row()] = value.value<QColor>();
        }
        else
            return false;

        emit ( dataChanged(index,index) );

        return true;
    }

    return false;
}

Qt::ItemFlags Color_List_Model::flags(const QModelIndex &index) const
{
    Qt::ItemFlags base = QAbstractTableModel::flags(index);

    if ( index.isValid() && index.row() <= colors.size() )
        base |= Qt::ItemIsEditable;

    return base;
}

