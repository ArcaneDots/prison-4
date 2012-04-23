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
#include <algorithm>

#include "symboldatum.h"
#include "barcodesymbolset.h"

SymbolDatum::SymbolDatum() :
  m_srcSymbolSet(0),
  m_tableIndex(NOT_FOUND),
  m_stringRep(),
  m_encodingPair(),
  m_expansionSymbols()
{
  // empty
}

SymbolDatum::SymbolDatum(const SymbolDatum& other) :
  m_srcSymbolSet(other.m_srcSymbolSet),
  m_tableIndex(other.m_tableIndex),
  m_stringRep(other.m_stringRep),
  m_encodingPair(other.m_encodingPair),
  m_expansionSymbols(other.m_expansionSymbols)
{
  // empty
}

SymbolDatum::SymbolDatum (const BarcodeSymbolSet * parent,
                          const QString& stringSymbol, 
                          const int index, 
                          const String_Pairs& encoding ) :
  m_srcSymbolSet(const_cast<BarcodeSymbolSet *>(parent)),
  m_tableIndex(index),
  m_stringRep(stringSymbol),
  m_encodingPair(encoding),
  m_expansionSymbols()
{
   // empty
}

SymbolDatum::SymbolDatum ( const BarcodeSymbolSet * parent,
                           const QString& stringSymbol,
                           const int index, 
                           const String_Pairs& encoding, 
                           const ExpansionSymbolList encodableSymbols ) :
  m_srcSymbolSet(const_cast<BarcodeSymbolSet *>(parent)),
  m_tableIndex(index),
  m_stringRep(stringSymbol),
  m_encodingPair(encoding),
  m_expansionSymbols(encodableSymbols)
{
  // empty
}


SymbolDatum::~SymbolDatum()
{
  //empty
}

SymbolDatum& SymbolDatum::operator=(const SymbolDatum& other)
{
  m_srcSymbolSet = other.m_srcSymbolSet;
  m_tableIndex = other.m_tableIndex;
  m_stringRep = other.m_stringRep;
  m_encodingPair = other.m_encodingPair;
  m_expansionSymbols = other.m_expansionSymbols;
  return *this;
}

bool SymbolDatum::isValid() const
{
  bool result = (m_srcSymbolSet != 0 && m_srcSymbolSet->isValid());
  const SymbolDatum * setResult = 0;
  if (result && !m_stringRep.isEmpty()) {
    if (m_encodingPair.size() > 0) {
      setResult = m_srcSymbolSet->getSymbol(m_tableIndex);
    } else if (!m_expansionSymbols.isEmpty()) {
      setResult = m_srcSymbolSet->getExpandedSymbol(m_tableIndex); 
    }
    result = (setResult->getString() == m_stringRep);
  }
  
  return result;
}


bool SymbolDatum::operator==(const SymbolDatum& other) const
{
  bool result = (m_srcSymbolSet == other.m_srcSymbolSet);
  if (result){
    result = (m_tableIndex == other.m_tableIndex); 
    result = (result && m_stringRep == other.m_stringRep);
    result = (result && m_encodingPair == other.m_encodingPair);
    result = (result && m_expansionSymbols == other.m_expansionSymbols);
  }
  return result;
}


QDebug operator<<(QDebug debugString, const SymbolDatum& sd)
{  
  debugString << "SymbolDatum #" << sd.getIndex() << " = " << sd.getString();
  
  debugString << sd.getEncodings();
  debugString << sd.getExpandedSymbols();
  return debugString.nospace();
}

QDebug operator<<(QDebug debugString, const SymbolDatum* sd)
{
  debugString << *sd;
  return debugString.nospace(); 
}

/**
 * Get string version of current SymbolDatum pointer object
 */
template<>
QString LookupTableEntry<SymbolDatum *>::getString(SymbolDatum * t) const
{
  return t->getString();
}

template<>
SymbolDatum * LookupTableEntry<SymbolDatum *>::findElement(const QString& searchString, 
		const QString& setName) const
{
  SymbolDatum * result = 0;
  _findElement(result, searchString, setName);
  return result;
}
template<>
SymbolDatum * LookupTableEntry<SymbolDatum *>::findElement(int index,
		const QString &setName) const
{  
  SymbolDatum * result = 0;
  _findElement(result, index, setName);
  return result;
}

