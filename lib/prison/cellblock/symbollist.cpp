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

#include "symbollist.h"

SymbolList::SymbolList() :
  QList<Symbol>(),
  m_barcodeSymbolSet(),
  m_userBlocks()
{
  // empty
}

SymbolList::SymbolList(const Symbol& i): 
  QList<Symbol>(),
  m_barcodeSymbolSet(i.getFullsymbolSet()),
  m_userBlocks()
{
  if (i.isValid()) {
    this->push_back(i);
  }
}

SymbolList::SymbolList(const QList<Symbol>& l): 
  QList<Symbol>(l),
  m_barcodeSymbolSet(),
  m_userBlocks()
{
  QList<Symbol>::const_iterator itrSymbol = l.begin();
  QSharedDataPointer<BarcodeSymbolSet> tempBarcodeSymbolSet(itrSymbol->getFullsymbolSet());
  while (itrSymbol != l.end()) {
    if (itrSymbol->isValid() && 
      itrSymbol->getFullsymbolSet() == tempBarcodeSymbolSet) {
	this->push_back(*itrSymbol);
    }
    itrSymbol++;      
  }
}

SymbolList::SymbolList(const BarcodeSymbolSet& barcodeSymbol, 
		       const QList< QString >& l): 
  QList<Symbol>(),
  m_barcodeSymbolSet((QSharedDataPointer<BarcodeSymbolSet> &)barcodeSymbol),
  m_userBlocks()  
{
  QList<QString>::const_iterator itrSymbol = l.begin();
  while (itrSymbol != l.end()) {
    if (!itrSymbol->isEmpty()) {
      Symbol possibleSymbol(*itrSymbol, m_barcodeSymbolSet); 
      if(possibleSymbol.isValid()) {
	this->push_back(possibleSymbol);	
      }      
    }
    itrSymbol++;      
  }
}

// SymbolList::SymbolList()
// {
// 
// }

SymbolList::SymbolList(const SymbolList& other) :
  QList<Symbol>(other.mid(0)),
  m_barcodeSymbolSet(other.m_barcodeSymbolSet),
  m_userBlocks(other.m_userBlocks)
{
  //this = other.mid(0);
}

SymbolList::~SymbolList()
{

}

bool SymbolList::isValid()
{
  return m_barcodeSymbolSet->isValid();
}

void SymbolList::insert(int i,const QString &s) { 
  const Symbol symbol(s, m_barcodeSymbolSet);
  if (symbol.isValid()) {
//   Symbol possibleSymbol(s, m_barcodeSymbolSet); 
//   if(possibleSymbol.isValid()) {
    this->QList<Symbol>::insert(i, symbol);	
  }      
}

SymbolList SymbolList::parse(const QString &symbolString)
{
  Q_ASSERT(symbolString.isEmpty());
  
  SymbolList parsedSymbols(m_barcodeSymbolSet);
  // fill regexpCollection from m_barcodeSymbolSet
  RegexpCollection regexpCollection;
  
  // parsing code
  qDebug("SymbolList parseSymbolString() : start");
  // empty argument  
  if (symbolString.isEmpty()) {
    qDebug("SymbolList parseSymbolString() : empty");
    return parsedSymbols;
  }  
  qDebug("SymbolList parseSymbolString() : not empty");
  // verify whether expecting and defined multi-Charactor Symbol regexp patterns
//   Q_ASSERT_X(m_multiCharSymbols == !d->m_regexpCollection.empty(),
// 	     "Conflicted mult-charactor symbols settings",
// 	     "Inconsistent mult-charactor symbols settings");  
  // initialize the colection of regexp patterns and their starting indexes 
  if (!regexpCollection.empty()) {
    shared::RegexpCollection::const_iterator currentRegExpItr = 
      regexpCollection.begin();
      
    while(currentRegExpItr != regexpCollection.end()){    
	currentRegExpItr->setSourceString(symbolString);
	currentRegExpItr++;
    }
    std::sort(regexpCollection.begin(), regexpCollection.end());
  }
  qDebug("SymbolList parseSymbolString() : start parsing");
  // step through input string
  //QStringList userSymbolsList;  
  for (int userIndex = 0; userIndex < symbolString.size(); ) {      
    if (!regexpCollection.empty() && 
      regexpCollection.begin()->isValid()) { 
      QString matchingString(regexpCollection.begin()->getMatchingString());
      parsedSymbols << matchingString;   
      userIndex += matchingString.size();
      regexpCollection.begin()->nextMatch();

      //  resort collection so closest matching symbol is first
      std::sort(regexpCollection.begin(), regexpCollection.end());  
    } 
    else {
      parsedSymbols << symbolString.at(userIndex++);
    }
//     if (getSymbolIndex(userSymbolsList.back()) == NOT_FOUND) {
//       qDebug("BarcodeSymbolSet parseSymbolString() : unknown symbol");
//       qDebug() << "removing : " << userSymbolsList.back();
//       userSymbolsList.pop_back();
//     }
  } // while    

  if (parsedSymbols.isEmpty()) {
    qDebug("BarcodeSymbolSet parseSymbolString() : no valid symbols");
    //m_isValid |= codeEngine::UnknownSymbols;
  }
  qDebug("BarcodeSymbolSet parseSymbolString() : end");
  return parsedSymbols;
}

shared::LookupIndexArray SymbolList::getIndexValues()
{ 
  shared::LookupIndexArray indexArray;
  QList<Symbol>::const_iterator itrSymbol = this->begin();
  while (itrSymbol != this->end()) {
    indexArray;
  }
//   bool extendedSet = m_barcodeSymbolSet->isExtendedSet();
//   if (this->count() == 0) {
//     return indexArray;
//   }
//   int symbolIndex = NOT_FOUND;
//   SymbolList::const_iterator itrSymbol = this->begin();
//   while (itrSymbol != this->end()) {
//     symbolIndex = itrSymbol->getIndex();
//     if (extendedSet){
//       indexArray.append();
//     }
//   }
}


SymbolList& SymbolList::operator=(const SymbolList& other)
{
    return *this;
}

bool SymbolList::operator==(const SymbolList& other) const
{
///TODO: return ...;
}

