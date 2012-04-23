/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Ian gmail <ianhollander@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "lookuptableentry.h"
#include <algorithm>

// -- QString --
template<>
QString LookupTableEntry<QString>::getString(QString t) const
{
  return t;
}
template<>
QString LookupTableEntry<QString>::findElement(const QString& searchString, 
		const QString& setName) const
{
  QString result;
  _findElement(result, searchString, setName);
  return result;
}
template<>
QString LookupTableEntry<QString>::findElement(int index,
		const QString &setName) const
{  
  QString result;
  _findElement(result, index, setName);
  return result;
}
// -- LookupTableEntry<QString> --
template<>
QString LookupTableEntry<LookupTableEntry<QString> >::getString(LookupTableEntry<QString> t) const
{
  return QStringList(t.getRowNames()).join(" ");
}
template<>
LookupTableEntry<QString> LookupTableEntry<LookupTableEntry<QString> >::findElement(const QString& searchString, 
		const QString& setName) const
{
  LookupTableEntry<QString> result;
  _findElement(result, searchString, setName);
  return result;
}
template<>
LookupTableEntry<QString>  LookupTableEntry<LookupTableEntry<QString> >::findElement(int index,
		const QString &setName) const
{  
  LookupTableEntry<QString> result;
  _findElement(result, index, setName);
  return result;
}



