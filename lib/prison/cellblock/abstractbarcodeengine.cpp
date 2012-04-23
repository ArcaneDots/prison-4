/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

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

#include <numeric>
#include "abstractbarcodeengine.h"

using namespace engineFlags;
using namespace barcodeEngine;

/**
 * @cond PRIVATE
 */
// class AbstractBarengineFlags::Private 
// {
//  public:
//     Private();
//     virtual ~Private();
//     
//   
//     BarcodeSetInfo m_barcodeSymbolInfo;
//     /**
//      * collection of regular expressions used to find each multi-character symbols
//      */
//     mutable shared::RegexpCollection m_regexpCollection;
//     /**
//      * Pointer to symbol index look-up table containing all valid symbols 
//      * 
//      * This points to actual symbol list so they can be accessed via a const getter functions. 
//      * It also allows the same set getter functions to support barcodes with single or 
//      * multiple symbol sets (i.e. Code128) without having to rewritten in the subclass.
//      * @note The getter functions will fail if the pointer is "null".
//      * @note A sub-classed barcode with multiple symbol sets will have to define a 
//      * @note function that moves the pointer between symbol sets. 
//      */
//     QStringList * m_symbolLookup; 
//     /**
//      * Pointer to an indexed look-up table containing all valid symbol encoding 
//      * 
//      * Using a pointer actual symbol list to be accessed via a const getter functions. 
//      * This allows the same set getter functions to support barcodes with single or 
//      * multiple symbol sets (i.e. Code128) without having to rewritten in the subclass.
//      * 
//      * @note A sub-classed barcode with multiple symbol sets has to define a function
//      * @note that switches the pointer between symbol sets. 
//      * @note The getter functions will fail if the pointer is "null".
//      */
//     QStringList * m_encodingLookup;
//     /**
//      * actual list of symbols
//      * 
//      * @sa define local copy in a class that uses a different symbol set and 
//      * @sa point m_symbolLookup so its subclasses can perform symbol conversions. 
//      */
//     QStringList m_localSymbolLookup;     
//};

/**
 * @endcond
 */

// barcodeEngine::base::Private::Private() :
//   m_barcodeSymbolInfo(0)
// {
// 
// }
// 
// barcodeEngine::base::Private::~Private()
// {
//   // empty
// }

barcodeEngine::base::base(const QString &defaultString,
			  int minLength, 
			  int maxLength,
			  bool multiCharSymbols,
			  int checksumModulus,
			  bool internalCheckDigit,
			  int requiredCheckDigits,
			  int maxCheckDigits) : 
	m_defaultString(defaultString),
	m_minLength(minLength), 
	m_maxLength(maxLength),
	m_multiCharSymbols(multiCharSymbols),
	m_internalCheckDigit(internalCheckDigit),
	m_checksumModulus(checksumModulus),
	m_maxCheckDigits(maxCheckDigits),
	m_requiredCheckDigits(requiredCheckDigits),
	m_formatedSymbols(),
	m_encodedSymbols(),
	m_barPositions(),
	m_isValid(engineFlags::UnknownError),
	m_constructionFlags(),
	m_barcodeSymbolInfo()
	//d(new AbstractBarengineFlags::Private)
{
  qDebug("AbstractBarengineFlags constructor");
  //initialize();
}

barcodeEngine::base::~base()
{
  qDebug("AbstractBarengineFlags destructor");
  //delete d;
}

void barcodeEngine::base::initialize()
{
  qDebug("AbstractBarengineFlags initialize()");
  // add symbol sets and thier encodings
  //fillSymbolList();
}


void barcodeEngine::base::setBarcodeString(const QString& userBarcode, 
					    engineFlags::ConstructCodes flags)
{
  qDebug("AbstractBarengineFlags setBarcodeString() : start");
  
  // verify that the symbol related shared pointers actually initialized 
  Q_ASSERT(m_barcodeSymbolInfo.constData()->isValid());
  // ignore the identical input  
  if (userBarcode == m_userInputString) {
    return;
  }  
  // reset failure code and output variables
  m_constructionFlags = flags;
  m_isValid = engineFlags::OK; 
  m_formatedSymbols.clear();
  m_encodedSymbols.clear();  
  // save user input for later comparsion
  m_userInputString = userBarcode;
  // parse string into list of text symbols and validate
  SymbolList userSymbols(getBarcodeSetInfo(), userBarcode);    
  // basic validation
  qDebug() << "expected minLength = " << m_minLength;
  qDebug() << "expected maxLength = " << m_maxLength;
  qDebug() << "expected internal check digit = " << m_internalCheckDigit;
  
  if (!userSymbols.empty()) {
    if ((userSymbols.size() < m_minLength) ) {
      m_isValid |= engineFlags::LengthOutOfRange;
    } 
    // may not have a maximun length
    if (m_maxLength != NOT_FOUND && userSymbols.size() > m_maxLength) {      
      m_isValid |= engineFlags::LengthOutOfRange;
    }
  } else {
    m_isValid |= engineFlags::MissingInput;
  }
  // subclass specific symbol validation/processing  
  if (m_isValid.testFlag(engineFlags::OK)) {
    qDebug("AbstractBarengineFlags setBarcodeString() : processing user symbols");
    userSymbols = this->processSymbolList(userSymbols);
  } 
  // switch to default value in case user's input fails in processSymbolList() 
  if (!m_isValid.testFlag(engineFlags::OK)) {
    qDebug("AbstractBarengineFlags setBarcodeString() : revert to default");
    userSymbols = m_defaultString.split(" ");
    userSymbols = this->processSymbolList(userSymbols);
  }  
  // save and output final set of symbols
  m_userSymbols = userSymbols;
  Q_ASSERT(userSymbols.size() >= m_minLength);
  if (m_maxLength != NOT_FOUND) {
    Q_ASSERT(userSymbols.size() <= m_maxLength);
  }
  this->formatSymbols(userSymbols);  
  this->encodeSymbols(userSymbols);  
  qDebug("AbstractBarengineFlags setBarcodeString() : end");
}

// QStringList AbstractBarengineFlags::parseSymbolString(const QString& symbolString) const
// { 
//   qDebug("AbstractBarengineFlags parseSymbolString() : start");
//   // empty argument  
//   if (symbolString.isEmpty()) {
//     qDebug("AbstractBarengineFlags parseSymbolString() : empty");
//     return QStringList();
//   }  
//   qDebug("AbstractBarengineFlags parseSymbolString() : not empty");
//   // verify whether expecting and defined multi-Charactor Symbol regexp patterns
//   Q_ASSERT_X(m_multiCharSymbols == !d->m_regexpCollection.empty(),
// 	     "Conflicted mult-charactor symbols settings",
// 	     "Inconsistent mult-charactor symbols settings");  
//   // initialize the colection of regexp patterns and their starting indexes 
//   if (!d->m_regexpCollection.empty()) {
//     shared::RegexpCollection::const_iterator currentRegExpItr = 
//       d->m_regexpCollection.begin();
//       
//     while(currentRegExpItr != d->m_regexpCollection.end()){    
// 	currentRegExpItr->setSourceString(symbolString);
// 	currentRegExpItr++;
//     }
//     std::sort(d->m_regexpCollection.begin(), d->m_regexpCollection.end());
//   }
//   qDebug("AbstractBarengineFlags parseSymbolString() : start parsing");
//   // step through input string
//   QStringList userSymbolsList;  
//   for (int userIndex = 0; userIndex < symbolString.size(); ) {      
//     if (!d->m_regexpCollection.empty() && 
//       d->m_regexpCollection.begin()->isValid()) { 
//       userSymbolsList << d->m_regexpCollection.begin()->getMatchingString();   
//       userIndex += userSymbolsList.back().size();
//       d->m_regexpCollection.begin()->nextMatch();
// 
//       //  resort collection so closest matching symbol is first
//       std::sort(d->m_regexpCollection.begin(), d->m_regexpCollection.end());  
//     } 
//     else {
//       userSymbolsList << symbolString.at(userIndex++);
//     }
//     if (getSymbolIndex(userSymbolsList.back()) == NOT_FOUND) {
//       qDebug("AbstractBarengineFlags parseSymbolString() : unknown symbol");
//       qDebug() << "removing : " << userSymbolsList.back();
//       userSymbolsList.pop_back();
//     }
//   } // while    
// 
//   if (userSymbolsList.isEmpty()) {
//     qDebug("AbstractBarengineFlags parseSymbolString() : no valid symbols");
//     m_isValid |= engineFlags::UnknownSymbols;
//   }
//   qDebug("AbstractBarengineFlags parseSymbolString() : end");
//   return userSymbolsList;
// }

SymbolList barcodeEngine::base::processSymbolList(const SymbolList &inputSymbols)/*, 
						     int &firstBlockSize)*/
{
  qDebug("AbstractBarengineFlags processSymbolList() : start");  
  // assumes check digit is at end of symbol "stream"
  Q_ASSERT(m_internalCheckDigit == false);
  
  // check digit are at the end of ths list
  SymbolList parsedSymbols(inputSymbols);
  SymbolList calcSymbolIndexes(parsedSymbols.clone()); 
  std::reverse_copy(parsedSymbols.begin(), 
                     parsedSymbols.begin()+= (parsedSymbols.size() - 1), 
                      std::back_inserter<SymbolList>(calcSymbolIndexes));
  // one check symbol
  Q_ASSERT(m_maxCheckDigits == 1 && m_requiredCheckDigits == m_maxCheckDigits);
  
  int foundCheckValue = this->calculateCheckValue(calcSymbolIndexes);
  
  // special case: handle extend symbol ??
  if (foundCheckValue == NOT_FOUND || 
      foundCheckValue != parsedSymbols.back().getIndex()) {
    Symbol foundCheckSymbol(getCheckSymbol(foundCheckValue));
    if (m_constructionFlags.testFlag(engineFlags::AddCheckDigits)) {
      parsedSymbols << foundCheckSymbol;
    } else if (m_constructionFlags.testFlag(engineFlags::UpdateCheckDigit)){
      parsedSymbols.pop_back();
      parsedSymbols << foundCheckSymbol;
    } else {
      parsedSymbols.clear();
      m_isValid != engineFlags::InvalidCheckDigits;
    }
  }    
  qDebug("AbstractBarengineFlags processSymbolList() : end");
  return inputSymbols;
}


int base::calculateCheckValue(const SymbolList &inputSymbols) const
{
  // check value must be at the far right of the barcode; do not use this function for product codes
  qDebug("AbstractBarengineFlags calculateCheckDigit() : start");
  Q_ASSERT(m_internalCheckDigit == false);  
  qDebug("AbstractBarengineFlags calculateCheckDigit() : end");
  const barcodeEngine::LookupIndexArray symbolArray(inputSymbols.getIndexValues());
  return SimpleRemainderCheckDigit(
    std::accumulate(symbolArray.begin(), symbolArray.end(), 0));
}

int base::CommonChecksumOddEven(
  const SymbolList& inputSymbols, 
  int oddMultipler, 
  int evenMultipler, 
  bool reverse) const
{
  qDebug("AbstractBarengineFlags CommonChecksumOddEven() : start");
  const barcodeEngine::LookupIndexArray symbolArray(inputSymbols.getIndexValues());
  barcodeEngine::LookupIndexArray l_symbolArray;  
  
  if (true == reverse) {
    qDebug("AbstractBarengineFlags CommonChecksumOddEven() : reverse copy");
    std::copy(symbolArray.rbegin(), symbolArray.rend(), 
	      std::back_inserter(l_symbolArray));
  } else {
    qDebug("AbstractBarengineFlags CommonChecksumOddEven() : copy");
    std::copy(symbolArray.begin(), symbolArray.end(), 
	      std::back_inserter(l_symbolArray));
  }
  qDebug("AbstractBarengineFlags CommonChecksumOddEven() : end");
  Q_ASSERT(l_symbolArray.size() == symbolArray.size());
  return NextMultipleCheckDigit(
    std::accumulate(l_symbolArray.begin(), l_symbolArray.end(), 0, 
      barcodeEngine::EvenOddChecksum<int>(oddMultipler, evenMultipler))); 
}

int base::CommonChecksumLinear(
      const SymbolList& inputSymbols, bool reverse) const
{
  qDebug("AbstractBarengineFlags CommonChecksumLinear() : start");
  const barcodeEngine::LookupIndexArray symbolArray(inputSymbols.getIndexValues());
  Q_ASSERT(!symbolArray.empty());
  barcodeEngine::LookupIndexArray l_symbolArray;
  
  if (reverse) {
    qDebug("AbstractBarengineFlags CommonChecksumLinear() : reverse copy");
    std::copy(symbolArray.rbegin(), symbolArray.rend(), 
	      std::back_inserter(l_symbolArray));
  } else {
    qDebug("AbstractBarengineFlags CommonChecksumLinear() : copy");
    std::copy(symbolArray.begin(), symbolArray.end(), 
	      std::back_inserter(l_symbolArray));
  }
  qDebug("AbstractBarengineFlags CommonChecksumLinear() : end");
  Q_ASSERT(!l_symbolArray.size() == symbolArray.size());
  return this->SimpleRemainderCheckDigit(
    std::accumulate(symbolArray.begin(), symbolArray.end(), 
		*symbolArray.begin(), barcodeEngine::LinearMultiple<int>(m_checksumModulus)));
}

int base::NextMultipleCheckDigit(int checksum) const
{
  qDebug("AbstractBarengineFlags NextMultipleCheckDigit()");
  Q_ASSERT(checksum >= 0);
  int checkValue = m_checksumModulus - SimpleRemainderCheckDigit(checksum);
  Q_ASSERT(checkValue > 0 && checkValue <= m_checksumModulus);
  checkValue = (checkValue == m_checksumModulus) ? 0 : checkValue;  
  Q_ASSERT(checkValue >= 0 && checkValue < m_checksumModulus);
  qDebug() << "checkValue : " << checkValue; 
  return checkValue;
}

int base::SimpleRemainderCheckDigit(int checksum) const
{
  Q_ASSERT(checksum >= 0);
  return checksum % m_checksumModulus;
}    

// shared::LookupIndexArray AbstractBarengineFlags::convertSymbolsToIndexes(
//   const QStringList& symbolList) const
// {
//   qDebug("AbstractBarengineFlags convertSymbolsToIndexes() : start");
//   Q_ASSERT(!symbolList.empty());
//   shared::LookupIndexArray symbolArray;
//   QStringList::const_iterator itrSymbol = symbolList.begin();
//   while (itrSymbol != symbolList.end()) {
//     symbolArray.push_back(getSymbolIndex(*itrSymbol++));
//   }
//   Q_ASSERT(symbolList.size() == symbolArray.size());
//   qDebug("AbstractBarengineFlags convertSymbolsToIndexes() : end");
//   return symbolArray;
// }

// int AbstractBarengineFlags::getSymbolIndex(const QString &symbol) const
// {
//   Q_ASSERT(d->m_symbolLookup != 0 && !symbol.isEmpty());
//   return d->m_symbolLookup->indexOf(symbol);
// }
// 
// const QString AbstractBarengineFlags::getSymbolAtIndex(int symbolIndex) const
// {  
//   Q_ASSERT(d->m_symbolLookup != 0 && symbolIndex != NOT_FOUND);
//   return d->m_symbolLookup->value(symbolIndex, QString());
// }
// 
// QString AbstractBarengineFlags::getSymbolEncoding(const QString& symbol) const
// {  
//   Q_ASSERT(d->m_encodingLookup != 0 && !symbol.isEmpty());
//   int symbolIndex = getSymbolIndex(symbol);
//   return convertWidthEncodingToBinary(d->m_encodingLookup->at(symbolIndex));
// }

QString base::getEncodedSymbols(barcodeEngine::BarPositionsMap &barLocations, 
						     int barThicknessMultiple)
{
  qDebug("AbstractBarengineFlags getEncodedSymbols() : start");
  barLocations = m_barPositions;  
  QString encodedSymbols;
  
  if (barThicknessMultiple > 1) {
    QString::const_iterator symbolIter = m_encodedSymbols.constBegin();
    while (symbolIter != m_encodedSymbols.end()) {
      encodedSymbols.append(QString(barThicknessMultiple, *symbolIter++));
    }    
    barcodeEngine::BarPositionsMap::iterator posItr = barLocations.begin();     
    while (posItr != barLocations.end()) {
	(posItr++).value() *= barThicknessMultiple;
    }    
  }  
  qDebug() << "m_barPositions :" << m_barPositions;
  qDebug("AbstractBarengineFlags getEncodedSymbols() : end");
  return m_encodedSymbols;
}

const QString base::convertWidthEncodingToBinary(
  const QString& widthEncodedSymbol) const
{
  // FIXME support all encoding correctly
  qDebug("AbstractBarengineFlags convertWidthEncodingToBinary() : start");  
  Q_ASSERT(!widthEncodedSymbol.isEmpty());
  QRegExp widthEncoded("^[N|W]+$");
  QRegExp binaryEncoded("^[0-9]+$");
  int digits = 1;
  QChar bit = '0';
  QString binaryString;
  
  if (widthEncoded.exactMatch(widthEncodedSymbol)) {
    for (int column = 0; column < widthEncodedSymbol.size(); column++) {
      //  Narrow (N) == 1 digit
      //  Wide (W) == 2 digits
      digits = (widthEncodedSymbol.at(column) == 'W') ? 2 : 1;
    
      // even columns == "1"
      // odd columns == "0"
      bit = (column % 2 == 0) ? '1' : '0';       
      binaryString.append(QString(digits, bit));
    } 
  }
  else if (binaryEncoded.exactMatch(widthEncodedSymbol)) {    
    binaryString = widthEncodedSymbol;	      
  }
  Q_ASSERT(binaryString.size() >= widthEncodedSymbol.size() && 
    binaryString.size() <= widthEncodedSymbol.size());
  qDebug("AbstractBarengineFlags convertWidthEncodingToBinary() : end");  
  return binaryString;
}

// --- setup barcode text and "bar" encoding ---

void base::formatSymbols(const SymbolList& symbolSrc)
{
  qDebug("AbstractBarengineFlags formatSymbols() : start");
  Q_ASSERT(!symbolSrc.empty());
  m_formatedSymbols = QStringList(symbolSrc.toQStringList().join(""));
  qDebug("AbstractBarengineFlags formatSymbols() : end");
}

void base::encodeSymbols(const SymbolList& symbolSrc) 
{
  qDebug("AbstractBarengineFlags encodeSymbols() : start");
  Q_ASSERT(!symbolSrc.empty());
  SymbolList::const_iterator itrSymbol = symbolSrc.begin();
  while(itrSymbol !=  symbolSrc.end()) {
    m_encodedSymbols.append(itrSymbol->getEncoding());
    itrSymbol++;
    //m_encodedSymbols.append(this->getSymbolEncoding(*itrSymbol++));
  }
  Q_ASSERT(!m_encodedSymbols.isEmpty());
  qDebug("AbstractBarengineFlags encodeSymbols() : end");
}

const BarcodeSetInfo& base::getBarcodeSetInfo() const
{
  return m_barcodeSymbolInfo;
}

void barcodeEngine::base::setBarcodeSetInfo (const BarcodeSymbolSet& barcodeSymbolSet )
{
  if (barcodeSymbolSet.isValid()) {
    m_barcodeSymbolInfo = new BarcodeSymbolSet(barcodeSymbolSet);
  }
}

Symbol barcodeEngine::base::getCheckSymbol ( int index ) const
{
  return Symbol( getBarcodeSetInfo(), index);
}

StringTableEntry barcodeEngine::base::fillSymbolList()
{
  qDebug("AbstractBarengineFlags fillSymbolList() : start");
  QList<QString> localSymbolStrings;
  // symbols : 0-9
  for (int index = 0; index < baseSymbols::NUM_SYMBOLS; index++) {
    localSymbolStrings.append(QString(baseSymbols::SYMBOL_LOOKUP[index]));
  }
  qDebug() << "AbstractBarengineFlags fillSymbolList() : symbols " << localSymbolStrings;
  qDebug("AbstractBarengineFlags fillSymbolList() : end");

  return StringTableEntry(localSymbolStrings);
}

// void AbstractBarengineFlags::setSymbolAndEncodingLookupTable(
// 				const QStringList& symbolSet, 
// 				const QStringList& encodingSet)
// {
//   setSymbolLookupTable(symbolSet);
//   setSymbolEncodingLookupTable(encodingSet);
// }

// void AbstractBarengineFlags::setSymbolLookupTable(const QStringList& symbolSet)
// {
//   Q_ASSERT(&symbolSet != 0);
//   d->m_symbolLookup = const_cast<QStringList *>(&symbolSet);
// }

// QStringList AbstractBarengineFlags::getSymbolLookupTable() const
// {
//   return *d->m_symbolLookup;
// }

// void AbstractBarengineFlags::addEncodingLookupTableEntry(
//   const QStringList& symbolEncodingSet)
// {
//   Q_ASSERT(&symbolEncodingSet != 0);
//   d->m_encodingLookup = const_cast<QStringList *>(&symbolEncodingSet);
// }
// 
// void AbstractBarengineFlags::addEncodingLookupTableEntry ( const QString& EntryKey, 
//                                             const QStringList& symbolEncodingSet )
// {
//   Q_ASSERT(&symbolEncodingSet != 0);
//   d->m_encodingLookup = const_cast<QStringList *>(&symbolEncodingSet);
// }


// QStringList AbstractBarengineFlags::getSymbolEncodingLookupTable() const
// {
//   return *d->m_encodingLookup;
// }

void barcodeEngine::base::AddMultiSymbolRegexpString(const QString& symbolPattern)
{
  //d->m_regexpCollection.push_back(IndexedRegExp(symbolPattern));
}
