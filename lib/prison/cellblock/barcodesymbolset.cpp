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
#include "barcodesymbolset.h"
//#include "symbollist.h"
#include "guardshack/shareddefines.h"

//const int NOT_FOUND = -1;

BarcodeSymbolSet::BarcodeSymbolSet() : 
  m_visibleSymbolSet(0),
  m_encodableSymbolSet(0),
  m_symbolEncodings(0),
  m_encodingLists(),
  m_expansionMap()
{
  // empty
}

BarcodeSymbolSet::BarcodeSymbolSet(const BarcodeSymbolSet &other) : 
  QSharedData(other), 
  m_visibleSymbolSet(other.m_visibleSymbolSet),
  m_encodableSymbolSet(other.m_encodableSymbolSet),
  m_symbolEncodings(other.m_symbolEncodings),
  m_encodingLists(other.m_encodingLists),
  m_expansionMap(other.m_expansionMap)
{ 
  // empty
}

BarcodeSymbolSet::BarcodeSymbolSet(const QStringList& encodableSymbols, 
				 const QStringList& symbolEncodings) :
  m_visibleSymbolSet(0),
  m_encodableSymbolSet(0),
  m_symbolEncodings(new QStringList(symbolEncodings)),
  m_encodingLists(),
  m_expansionMap()
{
  Q_ASSERT(!encodableSymbols.isEmpty() || 
    encodableSymbols.size() == symbolEncodings.size());
  // point both symbol pointers to the same QStringList object
  m_encodableSymbolSet = new QStringList(encodableSymbols);
  m_visibleSymbolSet = m_encodableSymbolSet;
}

BarcodeSymbolSet::BarcodeSymbolSet(const QStringList& expandedSymbols, 
				   const QStringList& encodableSymbols,   
				   const ExpansionMap &expansionMap, 
				   const QStringList& symbolEncodings):
  m_visibleSymbolSet(new QStringList(expandedSymbols)),
  m_encodableSymbolSet(new QStringList(encodableSymbols)),
  m_symbolEncodings(new QStringList(symbolEncodings)),
  m_encodingLists(),
  m_expansionMap()
{
  Q_ASSERT(!encodableSymbols.isEmpty() || !expandedSymbols.isEmpty());
  Q_ASSERT(encodableSymbols.size() == symbolEncodings.size());
  Q_ASSERT(expandedSymbols.size() == expansionMap.size());
  QStringList::iterator itrExpandedSymbol = m_visibleSymbolSet->begin();
  ExpansionMap::const_iterator itrSourceMap = expansionMap.end();
  while (itrExpandedSymbol != m_visibleSymbolSet->end()) {
    itrSourceMap = expansionMap.find(*itrExpandedSymbol);
    if (itrSourceMap != expansionMap.end()) {
        m_expansionMap[*itrExpandedSymbol] = itrSourceMap->second;
    }
//       m_expansionMap->insert(
// 	std::pair<QStringRef, QStringList>(
// 	  itrExpandedSymbol->midRef(0), itrSourceMap->second));
//    }
  }
  Q_ASSERT(m_visibleSymbolSet->size() == m_expansionMap.size());
}

bool BarcodeSymbolSet::isValid()
{
  return (m_visibleSymbolSet != 0 && m_encodableSymbolSet != 0);
}

// BarcodeSymbolSet::BarcodeSymbolSet(const QStringList& encodableSymbols,
// 				 const EncodingLists& symbolEncodings):
//   m_stringExpandedSet(new QStringList(encodableSymbols)),
//   m_encodableSymbolList(m_stringExpandedSet),
//   m_symbolEncodings(0),
//   m_encodingLists(symbolEncodings),
//   m_expansionMap()
// {
// 
// }

bool BarcodeSymbolSet::isExtendedSet() const
{
  return (m_visibleSymbolSet != m_encodableSymbolSet);
}


// SymbolList BarcodeSymbolSet::parse(const QString &inputString)
// {
//   if (inputString.isEmpty()) {
//     return SymbolList((QSharedDataPointer<BarcodeSymbolSet>&)*this);
//   }
//   return parser(inputString);
// }




int BarcodeSymbolSet::getSymbolIndex(const QString& symbol) const
{
  int returnValue = NOT_FOUND;
  if (!isExtendedSet() && isSymbolEncodable(symbol)) {
    returnValue = m_encodableSymbolSet->indexOf(symbol);
  } else if (isSymbolVisible(symbol)) {
    returnValue = m_visibleSymbolSet->indexOf(symbol);
  }
  return returnValue;
}

LookupIndexArray BarcodeSymbolSet::getExtendedIndex(const QString& currentSymbol) const
{
  LookupIndexArray indexArray;
  int symbolIndex = getSymbolIndex(currentSymbol);
  if (!isExtendedSet() || symbolIndex == NOT_FOUND) {
    indexArray.push_back(symbolIndex);
  } else {
    //indexArray.push_back(m_encodableSymbolList->indexOf(currentSymbol.getString()));
    ExpansionMap::const_iterator itrExpandedSymbolMap = 
      m_expansionMap.find(currentSymbol);
    if (itrExpandedSymbolMap != m_expansionMap.end()) {
      QStringList extendedSymbols(itrExpandedSymbolMap->second);
      for (int i = 0; i < extendedSymbols.size(); i++) {
	indexArray.push_back(m_encodableSymbolSet->indexOf(extendedSymbols[i]));
      }
    }
  }
  return indexArray;
}


const QString BarcodeSymbolSet::getSymbolAtIndex(const SymbolData &symbolData) const
{
  QString symbolString;
  if (symbolData.m_extendedSet) {
    symbolString = m_visibleSymbolSet->value(symbolData.m_index);
  } else {
    symbolString = m_encodableSymbolSet->value(symbolData.m_index);    
  }
  return symbolString;
}

QStringList BarcodeSymbolSet::getSymbolEncoding(const SymbolData& symbolData) const
{
  QStringList encoding;
  
  if (isExtendedSet() && symbolData.m_extendedSet) {
    QStringList stringSymbols(m_expansionMap.at(getSymbol(symbolData)));
    for (int i = 0; i < stringSymbols.size(); i++) {
      int symbolIndex = getSymbolIndex(stringSymbols.at(i));
      encoding.append(m_symbolEncodings->at(symbolIndex));
    }
  } else {
    encoding = QStringList(m_symbolEncodings->at(symbolData.m_index));
  }
  return encoding;
}

QStringList BarcodeSymbolSet::getSymbolEncoding(const SymbolData& symbolData, 
						const QString& encodingSetId) const
{
  QStringList encoding;
  if (!m_symbolEncodings->contains(encodingSetId)) {
    return encoding;
  }
  
  if (isExtendedSet() && symbolData.m_extendedSet) {
    QStringList stringSymbols(m_expansionMap.at(getSymbol(symbolData)));
    for (int i = 0; i < stringSymbols.size(); i++) {
      int symbolIndex = getSymbolIndex(stringSymbols.at(i));
      encoding.append(m_encodingLists.at(encodingSetId).at(symbolIndex));
    }
  } else {
    encoding = 
      QStringList(m_encodingLists.at(encodingSetId).at(symbolData.m_index));
  }
  return encoding;
}

bool BarcodeSymbolSet::isSymbolVisible(const QString& symbolString) const
{
  return m_visibleSymbolSet->contains(symbolString);
}

bool BarcodeSymbolSet::isSymbolEncodable(const QString& symbolString) const
{
  return m_encodableSymbolSet->contains(symbolString);
}

QString BarcodeSymbolSet::getSymbol(const SymbolData& symbolData) const
{
  Q_ASSERT(symbolData.m_index != NOT_FOUND);
  QString returnString;
  if (symbolData.m_extendedSet) {
    returnString = m_visibleSymbolSet->value(symbolData.m_index);
  } else {
    returnString = m_encodableSymbolSet->value(symbolData.m_index);
  }
  return returnString;
}

// BarcodeSymbolSet::BarcodeSymbolSet()
// {
// 
// }
// 
// BarcodeSymbolSet::BarcodeSymbolSet(const BarcodeSymbolSet& other)
// {
// 
// }
// 
// BarcodeSymbolSet::~BarcodeSymbolSet()
// {
// 
// }
// 
// BarcodeSymbolSet& BarcodeSymbolSet::operator=(const BarcodeSymbolSet& other)
// {
//     return *this;
// }
// 
// bool BarcodeSymbolSet::operator==(const BarcodeSymbolSet& other) const
// {
// ///TODO: return ...;
// }

