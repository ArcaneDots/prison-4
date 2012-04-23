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
#include "symboldatum.h"


BarcodeSymbolSet::BarcodeSymbolSet() :
  m_visibleSymbolSet(0),
  m_encodableSymbolSet(0)
{
  // empty
}

BarcodeSymbolSet::BarcodeSymbolSet(const BarcodeSymbolSet &other) : 
  QSharedData(other), 
  m_visibleSymbolSet(other.m_visibleSymbolSet),
  m_encodableSymbolSet(other.m_encodableSymbolSet)
{ 
  // empty
}

BarcodeSymbolSet::BarcodeSymbolSet(const EncodableSymbols& encodableSymbols,
                                   const EncodingMap& symbolEncodings) :
  m_visibleSymbolSet(0),
  m_encodableSymbolSet(0)
{
  Q_ASSERT(!encodableSymbols.empty() || !symbolEncodings.isEmpty());
  // point both symbol pointers to the same SymbolTableEntry
  qDebug("BarcodeSymbolSet encoding constructor : start");
  m_encodableSymbolSet = generateEncodableSymbols(encodableSymbols, 
                                                  symbolEncodings);  
  m_visibleSymbolSet = m_encodableSymbolSet;
  qDebug("BarcodeSymbolSet encoding constructor : end");
}

BarcodeSymbolSet::BarcodeSymbolSet(const EncodableSymbols &encodableSymbols, 
                                   const EncodingMap& symbolEncodings,
                                   const VisibleSymbols &visibleSymbols,   
				   const ExpansionMap &expansionMap):
  m_visibleSymbolSet(0),
  m_encodableSymbolSet(0)
{
  qDebug("BarcodeSymbolSet expanded constructor : start");
  Q_ASSERT(!visibleSymbols.empty() || !encodableSymbols.empty());
  Q_ASSERT(encodableSymbols.size() == symbolEncodings.size());
 
  qDebug("BarcodeSymbolSet expanded constructor : generate encodable symbols");
  m_encodableSymbolSet = generateEncodableSymbols(encodableSymbols, symbolEncodings);
  qDebug("BarcodeSymbolSet expanded constructor : generate expanded symbols");
  m_visibleSymbolSet = generateExpandedSymbols(m_encodableSymbolSet,
                                               visibleSymbols,
                                               expansionMap);
  qDebug("BarcodeSymbolSet expanded constructor : end");
}

SymbolTableEntry* BarcodeSymbolSet::generateEncodableSymbols ( 
                                          const EncodableSymbols& symbolList,
                                          const EncodingMap& symbolEncodings )
{
  qDebug("BarcodeSymbolSet generateEncodableSymbols() : start");
  qDebug() << "BarcodeSymbolSet generateEncodableSymbols() : symbols " << symbolList;
  qDebug() << "BarcodeSymbolSet generateEncodableSymbols() : encoding " << symbolEncodings;
  Q_ASSERT(!symbolList.isEmpty());
  Q_ASSERT(!symbolEncodings.isEmpty());
  SymbolTableEntry * encodableSymbols = new SymbolTableEntry;
  
  bool matched = symbolList.matchingKeys (symbolEncodings);
  qDebug() << "BarcodeSymbolSet generateEncodableSymbols() : matched " << matched;
  
  // key == Symbol sets
  StringTableEntry::const_iterator itrSymbolSet = symbolList.begin();
  while (itrSymbolSet != symbolList.end()) {
    SymbolDataList symbolData; 
    String_Pairs encodingPair;
    // --- match symbol <=> encoding by index ---
    for (int index = 0; index < itrSymbolSet->second.count(); index++) {
      if (matched) {
	// --- one encoding  ---
	encodingPair.insert(std::pair<const QString, const QString>( 
	  defaultSet, 
	  symbolEncodings.findElement(index, itrSymbolSet->first)));         
      } else {
	// --- multiple encodings
	encodingPair = getSymbolEncoding(symbolEncodings, index);          
      }     
      qDebug() << "BarcodeSymbolSet generateEncodableSymbols : " << encodingPair;
      symbolData.push_back(
	new SymbolDatum(
	  this,
	 const_cast<const QString&>(itrSymbolSet->second.at(index)), 
	 index,
	 const_cast<const String_Pairs&>(encodingPair)));
    }      
    encodableSymbols->addRow(itrSymbolSet->first, symbolData);
    qDebug() << "BarcodeSymbolSet generateEncodableSymbols : " << symbolData;
    //qDebug() << "BarcodeSymbolSet generateEncodableSymbols : " << encodableSymbols;
    itrSymbolSet++;
  }
  qDebug("BarcodeSymbolSet generateEncodableSymbols : end");
  return encodableSymbols;
}

SymbolDataList * BarcodeSymbolSet::getVisibleSymbolExpansionList (
      const SymbolTableEntry * encodedSymbolDatum,
      const ExpansionMap& expansionMap,
      const QString& symbolString,
      const QString &symbolSet )
{
  qDebug("BarcodeSymbolSet getVisibleSymbolExpansionList : start");
  SymbolDataList * expandedDatum = new SymbolDataList();
  QStringList expansionList(expansionMap.findElement(symbolString, symbolSet).getRow());
  QStringList::const_iterator itrExpandedSymbol = expansionList.begin();
  while (itrExpandedSymbol != expansionList.end()) {
    expandedDatum->append(encodedSymbolDatum->findElement(*itrExpandedSymbol, symbolSet));
  }
  qDebug("BarcodeSymbolSet getVisibleSymbolExpansionList : end");
  return expandedDatum;
}

SymbolTableEntry* BarcodeSymbolSet::generateExpandedSymbols ( 
                                const SymbolTableEntry* encodedableSymbolData, 
                                const StringTableEntry& visibleSymbols, 
                                const ExpansionMap& expansionMap )
{
  //qDebug("BarcodeSymbolSet generateExpandedSymbols : start");
  Q_ASSERT(visibleSymbols.getRowNames() == expansionMap.getRowNames());
  bool matched = 
    (visibleSymbols.getRowNames() == expansionMap.getRowNames());
  
  SymbolTableEntry * visibleSymbolDatum = new SymbolTableEntry;
  StringTableEntry::const_iterator itrVisibleSymbolSet = visibleSymbols.begin();
  
  // set level of visibleSymbols
  while ( itrVisibleSymbolSet != visibleSymbols.end()) {   
    QList<QString>::const_iterator itrVisibleSymbol = itrVisibleSymbolSet->second.begin();
    SymbolDataList symbolData; 
    // each symbol in visibleSymbols
    for (int index = 0; index < itrVisibleSymbolSet->second.count(); index++) {
    //while (itrVisibleSymbol != itrVisibleSymbolSet->second.end()) { 
      const QString symbolString( *(itrVisibleSymbol + index));
      ExpansionSymbolList * expansionList(getVisibleSymbolExpansionList(encodedableSymbolData,
                                                                      expansionMap, 
                                                                      symbolString, 
                                                                      itrVisibleSymbolSet->first));
      symbolData.push_back(
        new SymbolDatum(
         this,
         symbolString, 
         index,
         String_Pairs(),
         *expansionList));
       itrVisibleSymbolSet++;
      }      
      visibleSymbolDatum->addRow( itrVisibleSymbolSet->first, symbolData);
    }
  qDebug("BarcodeSymbolSet generateExpandedSymbols : end");
  return visibleSymbolDatum;
}


bool BarcodeSymbolSet::isValid() const
{
  return (m_visibleSymbolSet != 0 && m_encodableSymbolSet != 0);
}

SymbolDataPtrList  BarcodeSymbolSet::parse( const QString& symbolString ) const
{
  qDebug("BarcodeSymbolSet parse() : start");
  Q_ASSERT(symbolString.isEmpty());
  Q_ASSERT(isValid());
  
   SymbolDataPtrList parsedSymbols;
  // fill regexpCollection from m_barcodeSymbolSet
  RegexpCollection regexpCollection;
  
  // parsing code
  qDebug("BarcodeSymbolSet parse() : start");
  // empty argument  
  if (symbolString.isEmpty()) {
    qDebug("BarcodeSymbolSet parse() : empty");
    return parsedSymbols;
  }  
  qDebug("BarcodeSymbolSet parse() : not empty");
  if (!regexpCollection.empty()) {
    RegexpCollection::const_iterator currentRegExpItr = regexpCollection.begin();
      
    while(currentRegExpItr != regexpCollection.end()){    
        currentRegExpItr->setSourceString(symbolString);
        currentRegExpItr++;
    }
    std::sort(regexpCollection.begin(), regexpCollection.end());
  }
  
  qDebug("BarcodeSymbolSet parse() : start parsing");
  // step through input string
  for (int userIndex = 0; userIndex < symbolString.size(); ) {      
    if (!regexpCollection.empty() && 
      regexpCollection.begin()->isValid()) { 
      QString matchingString(regexpCollection.begin()->getMatchingString());
      parsedSymbols.push_back(getVisibleSymbol(const_cast<const QString &>(matchingString)));   
      userIndex += matchingString.size();
      regexpCollection.begin()->nextMatch();

      //  resort collection so closest matching symbol is first
      std::sort(regexpCollection.begin(), regexpCollection.end());  
    } 
    else {
      parsedSymbols.append(getVisibleSymbol(symbolString.at(userIndex++)));
    }
  } // while    

  if (parsedSymbols.isEmpty()) {
    qDebug("BarcodeSymbolSet parse() : no valid symbols");
  }
  qDebug("BarcodeSymbolSet parse() : end");
  return parsedSymbols;
}

bool BarcodeSymbolSet::operator== ( const BarcodeSymbolSet& other ) const
{ 
  // prevent self comparison
  bool result = (this == &other);
  if (!result && m_visibleSymbolSet != 0 && m_encodableSymbolSet != 0) {
    // object are direct copies
    result = (m_visibleSymbolSet == other.m_visibleSymbolSet && 
      m_encodableSymbolSet == other.m_encodableSymbolSet);    
  }
  return (result && m_regexpCollection == other.m_regexpCollection);
}

bool BarcodeSymbolSet::isExtendedSet() const
{
  qDebug("BarcodeSymbolSet isExtendedSet()");
  return (m_visibleSymbolSet != m_encodableSymbolSet);
}

const SymbolDatum* BarcodeSymbolSet::getSymbol( const QString& symbol ) const
{
  //qDebug("BarcodeSymbolSet getSymbol(string)");
  return getVisibleSymbol(symbol);
}
const SymbolDatum* BarcodeSymbolSet::getExpandedSymbol( const QString& symbol ) const
{
  //qDebug("BarcodeSymbolSet getExpandedSymbol(string)");
  return getEncodableSymbol(symbol);
}

const SymbolDatum* BarcodeSymbolSet::getSymbol( int symbolIndex)  const
{ 
  //qDebug("BarcodeSymbolSet getSymbol(index)");
  return m_visibleSymbolSet->findElement(symbolIndex);
}
const SymbolDatum* BarcodeSymbolSet::getExpandedSymbol(int symbolIndex) const
{
  //qDebug("BarcodeSymbolSet getExpandedSymbol(index)");
  return m_encodableSymbolSet->findElement(symbolIndex);
}

String_Pairs BarcodeSymbolSet::getSymbolEncoding(EncodableSymbols encodableSymbols, int index) const
{
  //qDebug("BarcodeSymbolSet getExpandedNamedSymbol(string)");
   std::map<const QString, QString> encodingPairs(encodableSymbols.getMatchPairsAtIndex(index));
   std::map<const QString, QString>::const_iterator itrEncodingSet = encodingPairs.begin();
   String_Pairs resultPairs;
   while(itrEncodingSet != encodingPairs.end()) {
     qDebug("BarcodeSymbolSet getExpandedNamedSymbol(string) : found emcoding");
     resultPairs.insert(
       std::pair<const QString, const QString>(
         itrEncodingSet->first,
         itrEncodingSet->second));
     itrEncodingSet++;
   }
   return resultPairs;
}

const SymbolDatum *BarcodeSymbolSet::getVisibleSymbol (const QString& symbolString) const
{  
  return m_visibleSymbolSet->findElement(symbolString);
}

const SymbolDatum* BarcodeSymbolSet::getEncodableSymbol ( const QString& symbolString ) const
{ 
  return m_encodableSymbolSet->findElement(symbolString);
}
