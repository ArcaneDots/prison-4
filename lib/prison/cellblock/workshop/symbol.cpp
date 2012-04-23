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


#include "symbol.h"
#include "symboldatum.h"

Symbol::Symbol() :
  QSharedData(),
  m_symbolDatum (0),
  m_parsableSymbol(),
  m_barcodeSetInfo(new BarcodeSymbolSet),
  m_dataState(UNKOWN)
{
  // invalid and empty
  //qDebug("Symbol - default constructor");
}
Symbol::Symbol(const Symbol& other) :
  QSharedData(other),
  m_symbolDatum(other.m_symbolDatum),
  m_parsableSymbol(other.m_parsableSymbol),
  m_barcodeSetInfo(other.m_barcodeSetInfo), 
  m_dataState(other.m_dataState)
{
  //qDebug("Symbol - copy constructor");
}

Symbol::Symbol(const BarcodeSetInfo &barcodeSymbolSetInfo) : 
  QSharedData(),
  m_symbolDatum (0),
  m_parsableSymbol(),
  m_barcodeSetInfo(barcodeSymbolSetInfo),
  m_dataState(UNKOWN)
{
  //qDebug("Symbol - constructor(BarcodeSetInfo) : start");
  if (m_barcodeSetInfo->isValid()) {
    qDebug("Symbol - constructor(BarcodeSetInfo) : valid");
    m_dataState = BARCODESET;
  }
  //qDebug("Symbol - constructor(BarcodeSetInfo) : end");
}

Symbol::Symbol (const SymbolDatum* symbolInfo ) :
  QSharedData(),
  m_symbolDatum (0),
  m_parsableSymbol(),
  m_barcodeSetInfo(new BarcodeSymbolSet),
  m_dataState(UNKOWN)
{
  //qDebug("Symbol constructor(symbolInfo) : start");
  if (symbolInfo != 0) {
    qDebug("Symbol constructor(symbolInfo) : valid");
    m_symbolDatum = const_cast<SymbolDatum *>(symbolInfo);
    m_dataState = DATUM;
  }
  //qDebug("Symbol constructor(symbolInfo) : end");
}

Symbol::Symbol(const BarcodeSetInfo& barcodeSymbolSetInfo, int index, bool extended) :  
  QSharedData(),
  m_symbolDatum (0), 
  m_parsableSymbol(),
  m_barcodeSetInfo(barcodeSymbolSetInfo),
  m_dataState(UNKOWN)
{
  //qDebug("Symbol constructor(BarcodeSymbolSet, index, extended) : start");
  if (!barcodeSymbolSetInfo->isValid()) {
    return;
  }
  if (extended) {
    qDebug("Symbol - constructor : BarcodeSymbolSet(index, extended)");
    m_symbolDatum = 
      const_cast<SymbolDatum *>(barcodeSymbolSetInfo->getExpandedSymbol(index));
  } else {
    qDebug("Symbol - constructor : BarcodeSymbolSet(index, visible)");
    m_symbolDatum = 
      const_cast<SymbolDatum *>(barcodeSymbolSetInfo->getSymbol(index));
  }
  if (m_symbolDatum != 0) {
    m_dataState = DATUM;
  }
  else {
    m_dataState = BARCODESET;
  }
  //qDebug("Symbol constructor(BarcodeSymbolSet, index, extended) : end");
}

Symbol::Symbol(const BarcodeSetInfo& barcodeSymbolSetInfo, const QString& string, bool extended) : 
  QSharedData(),
  m_symbolDatum (0), 
  m_parsableSymbol(),
  m_barcodeSetInfo(barcodeSymbolSetInfo),
  m_dataState(UNKOWN)
{
  //qDebug("Symbol constructor(BarcodeSymbolSet, string, extended) : start");
  if (m_barcodeSetInfo->isValid()) {
    if (extended) {
      qDebug("Symbol - constructor : BarcodeSymbolSet(string, extended)");
      m_symbolDatum = 
	const_cast<SymbolDatum *>(m_barcodeSetInfo->getExpandedSymbol(string));
    } else {
      qDebug("Symbol - constructor : BarcodeSymbolSet(string, visible)");
      m_symbolDatum = 
	const_cast<SymbolDatum *>(m_barcodeSetInfo->getSymbol(string));
    }
    m_dataState = (m_symbolDatum != 0) ? DATUM : BARCODESET;
  }
  //qDebug("Symbol constructor(BarcodeSymbolSet, string, extended) : end");
}

Symbol::Symbol(const QString& string) : 
  QSharedData(),
  m_symbolDatum (0), 
  m_parsableSymbol(string),
  m_barcodeSetInfo(new BarcodeSymbolSet),
  m_dataState(UNKOWN)
{
  //qDebug("Symbol constructor(string) : start");
  if (!m_parsableSymbol.isEmpty()) {
    qDebug("Symbol constructor(string) : valid");
    m_dataState = STRING;
  }
  //qDebug("Symbol constructor(string) : end");
}
 
Symbol::~Symbol()
{
  //qDebug("Symbol destructor");
}

bool Symbol::isValid() const
{
  return (m_dataState == BARCODESET || m_dataState == DATUM);
}

bool Symbol::hasValue() const
{
  return (m_dataState == STRING || m_dataState == DATUM);
}


QString Symbol::toString() const
{
  QString str;
  if (m_dataState == DATUM) {
    str = m_symbolDatum->getString();
  } else if (m_dataState == STRING) {
    str = m_parsableSymbol;
  }
  return str;
}

void Symbol::setBarcodeSymbolSet(const BarcodeSymbolSet* symbolSet)
{
  if (symbolSet != 0 && symbolSet->isValid()) {
    m_barcodeSetInfo = const_cast<BarcodeSymbolSet *>(symbolSet);
    m_dataState = BARCODESET;
  }
}

const BarcodeSymbolSet* Symbol::getBarcodeSymbolSet() const
{
  const BarcodeSymbolSet * result = 0;
  if (m_dataState == DATUM) {
    result = m_symbolDatum->getBarcodeSet();
    //result = const_cast<BarcodeSymbolSet *>(m_symbolDatum->getBarcodeSet());
  } else if (m_dataState == BARCODESET) {
    result = m_barcodeSetInfo.constData();
  }
  return result;
}


void Symbol::setSymbolDatum(const SymbolDatum* symboldatum)
{
  if (symboldatum != 0) {
    m_symbolDatum = const_cast<SymbolDatum *>(symboldatum);
    m_dataState = DATUM;
  }
}

const SymbolDatum* Symbol::getSymbolDatum() const
{
  SymbolDatum * result = 0;
  if (m_dataState == DATUM) {
    result = m_symbolDatum;
  }
  return result;
}

int Symbol::getIndex() const
{
  return (m_dataState == DATUM) ? m_symbolDatum->getIndex() : NOT_FOUND;
}
bool Symbol::isExtendedSet() const
{
  return !m_symbolDatum->getExpandedSymbols().empty();
}
barcodeEngine::LookupIndexArray Symbol::getIndexes() const 
{
  barcodeEngine::LookupIndexArray indexes;
  ExpansionSymbolList expanedSymbols = m_symbolDatum->getExpandedSymbols(); 
  if (!expanedSymbols.empty()) {
    qDebug("Symbol getIndexes() : valid input");
    ExpansionSymbolList::const_iterator itrExpandedSymbols = expanedSymbols.begin();
    while (itrExpandedSymbols != expanedSymbols.end()) {
      indexes.push_back(((SymbolDatum *)*itrExpandedSymbols)->getIndex());
    }
  }
  return indexes;
}

QString Symbol::getEncoding ( const QString& encodingKey ) const
{
  QString result;
  String_Pairs::iterator itrEncodingSet = 
    m_symbolDatum->getEncodings().find(encodingKey);
  if (itrEncodingSet != m_symbolDatum->getEncodings().end()) {
    result = itrEncodingSet->second;
  }
  return result;
}
String_Pairs Symbol::getEncodings() const
{
  return m_symbolDatum->getEncodings();
}

QList<Symbol> Symbol::toSymbol (const Symbol & symbolInfo ) const
{ 
  QList<Symbol> symbols;
  if (symbolInfo.isValid()) { 
    symbols.append(symbolInfo); 
  } else { 
    symbols.append(toSymbol(symbolInfo.toString()));
  }
  return symbols;
 /* QSharedDataPointer<Symbol> s(const_cast<Symbol *>(&symbolInfo));
  return (s->isValid() ? (*s.constData())toSymbol(s->toString()) : )*/;
}

QList<Symbol> Symbol::toSymbol ( int symbolInfo ) const
{
  QList<Symbol> result;
  result.append(Symbol(getFullsymbolSet()->getSymbol(symbolInfo)));
  return result;
}

QList<Symbol> Symbol::toSymbol ( const QString& symbolInfo) const
{
  QList<Symbol> symbols;
  symbols.append(Symbol(getFullsymbolSet()->getSymbol(symbolInfo)));
  return symbols;  
}

QList<Symbol> Symbol::toSymbol( const QStringList& symbolInfo ) const
{
  QList<Symbol> result;  
  QStringList::const_iterator itrSymbolDatum = symbolInfo.begin();
  while (itrSymbolDatum != symbolInfo.end()) {
    result.append(toSymbol(*itrSymbolDatum));
  }
  return result;
}

void Symbol::clear(){
  if (m_dataState == DATUM){     
    m_barcodeSetInfo = const_cast<BarcodeSymbolSet *>(m_symbolDatum->getBarcodeSet());
    m_symbolDatum = 0;
    m_dataState = BARCODESET;
  }
  if (m_dataState == STRING) {
    m_parsableSymbol.clear();
    m_dataState = UNKOWN;
  } 
}

const BarcodeSetInfo Symbol::getFullsymbolSet() const 
{  
  BarcodeSetInfo barcodeSetInfoPtr = m_barcodeSetInfo;
  if (m_dataState == DATUM){  
    barcodeSetInfoPtr = const_cast<BarcodeSymbolSet *>(m_symbolDatum->getBarcodeSet());
  }
  return barcodeSetInfoPtr;
}

bool Symbol::isSameSymbolSet ( const Symbol& s ) const
{
  return (getFullsymbolSet() == s.getFullsymbolSet());
}

// operator(s)

Symbol& Symbol::operator=(const Symbol& other)
{
  if (*this != other) {
    m_parsableSymbol = other.m_parsableSymbol;
    m_symbolDatum = other.m_symbolDatum;
    m_barcodeSetInfo = other.m_barcodeSetInfo;
  }
  return *this;
}

bool operator==(const Symbol& lhs, const Symbol& rhs) 
{
  bool result = (lhs == rhs);
  if (result == false) {
     result = (lhs.m_barcodeSetInfo == rhs.m_barcodeSetInfo && lhs.m_symbolDatum == rhs.m_symbolDatum );
  }
  return result;
}
bool operator<(const Symbol& lhs, const Symbol& rhs)
{
  bool isLessor = false;
  if (lhs.isExtendedSet() && !rhs.isExtendedSet()) {
    isLessor = true;
  } else {
    isLessor = (lhs.getIndex() < rhs.getIndex());
  }
  return isLessor;
}


  

  


QList<Symbol> operator+ (const QList<Symbol>& symbols, const Symbol& s )
{
  QList<Symbol> tempSymbols(symbols);
  if (s.isValid() && !s.hasValue()) { tempSymbols.append(s); }
  return tempSymbols;
}

QList< Symbol >& operator<< ( QList< Symbol >& symbolList, const Symbol& s )
{
  if ((symbolList.isEmpty() && s.isValid()) || !s.hasValue()) {
    symbolList.append(s);
  } 
  return symbolList;
}

QStringList& operator<<(QStringList &stringList, const Symbol &s) 
{ 
  return stringList << s.toString();
}
// std::ostream& operator<<( std::ostream& os, const Symbol &s) 
// {   
//   os << s.toString(); 
//   return os;
// }
QDebug& operator<<(QDebug &debugString, const Symbol &s) 
{ 
  return debugString << s.toString();
}



