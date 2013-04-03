/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Ian gmail <ianhollander at gmail.com>

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

using namespace shared;

/**
 * @cond PRIVATE
 */
class AbstractBarcodeEngine::Private 
{
 public:
    Private();
    virtual ~Private();
    
    /**
     * collection of regular expressions used to find each multi-character symbols
     */
    mutable shared::RegexpCollection m_regexpCollection;
    /**
     * Pointer to symbol index look-up table containing all valid symbols 
     * 
     * This points to actual symbol list so they can be accessed via a const getter functions. 
     * It also allows the same set getter functions to support barcodes with single or 
     * multiple symbol sets (i.e. Code128) without having to rewritten in the subclass.
     * @note The getter functions will fail if the pointer is "null".
     * @note A sub-classed barcode with multiple symbol sets will have to define a 
     * @note function that moves the pointer between symbol sets. 
     */
    QStringList * m_symbolLookup; 
    /**
     * Pointer to an indexed look-up table containing all valid symbol encoding 
     * 
     * Using a pointer actual symbol list to be accessed via a const getter functions. 
     * This allows the same set getter functions to support barcodes with single or 
     * multiple symbol sets (i.e. Code128) without having to rewritten in the subclass.
     * 
     * @note A sub-classed barcode with multiple symbol sets has to define a function
     * @note that switches the pointer between symbol sets. 
     * @note The getter functions will fail if the pointer is "null".
     */
    QStringList * m_encodingLookup;
    /**
     * actual list of symbols
     * 
     * @sa define local copy in a class that uses a different symbol set and 
     * @sa point m_symbolLookup so its subclasses can perform symbol conversions. 
     */
    QStringList m_localSymbolLookup;     
};

/**
 * @endcond
 */

shared::AbstractBarcodeEngine::Private::Private() :
  m_symbolLookup(0),
  m_encodingLookup(0),
  m_localSymbolLookup()
{

}

shared::AbstractBarcodeEngine::Private::~Private()
{
  // empty
}

AbstractBarcodeEngine::AbstractBarcodeEngine(const QString &defaultString,
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
	m_isValid(CodeEngine::UnknownError),
	m_constructionFlags(),
	d(new AbstractBarcodeEngine::Private)
{
  qDebug("AbstractBarcodeEngine constructor");
  initialize();
}

AbstractBarcodeEngine::~AbstractBarcodeEngine()
{
  qDebug("AbstractBarcodeEngine destructor");
  delete d;
}

void AbstractBarcodeEngine::initialize()
{
  qDebug("AbstractBarcodeEngine initialize");
  // add symbol sets and thier encodings
  fillSymbolList();
}


void AbstractBarcodeEngine::setBarcodeString(const QString& userBarcode, 
					    CodeEngine::ConstructCodes flags)
{
  qDebug("AbstractBarcodeEngine setBarcodeString() : start");
  
  // verify that the symbol related shared pointers actually initialized 
  //initialize();  
  Q_ASSERT(d->m_symbolLookup != 0);
  Q_ASSERT(d->m_encodingLookup != 0);   
  // ignore the identical input  
  if (userBarcode == m_userInputString) {
    return;
  }  
  // reset failure code and output variables
  m_constructionFlags = flags;
  m_isValid = CodeEngine::OK; 
  m_formatedSymbols.clear();
  m_encodedSymbols.clear();  
  // save user input for later comparsion
  m_userInputString = userBarcode;
  QStringList userSymbols;  
  // parse string into list of text symbols and validate
  userSymbols = parseSymbolString(userBarcode);    
  // basic validation
  qDebug() << "expected minLength = " << m_minLength;
  qDebug() << "expected maxLength = " << m_maxLength;
  qDebug() << "expected internal check digit = " << m_internalCheckDigit;
  
  if (!userSymbols.isEmpty()) {
    if ((userSymbols.size() < m_minLength) ) {
      m_isValid |= CodeEngine::LengthOutOfRange;
    } 
    // may not have a maximun length
    if (m_maxLength != NOT_FOUND && userSymbols.size() > m_maxLength) {      
      m_isValid |= CodeEngine::LengthOutOfRange;
    }
  } else {
    m_isValid |= CodeEngine::MissingInput;
  }
  // subclass specific symbol validation/processing  
  if (m_isValid.testFlag(CodeEngine::OK)) {
    qDebug("AbstractBarcodeEngine setBarcodeString() : processing user symbols");
    userSymbols = this->processSymbolList(userSymbols);
  } 
  // switch to default value in case user's input fails in processSymbolList() 
  if (!m_isValid.testFlag(CodeEngine::OK)) {
    qDebug("AbstractBarcodeEngine setBarcodeString() : revert to default");
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
  qDebug("AbstractBarcodeEngine setBarcodeString() : end");
}

QStringList AbstractBarcodeEngine::parseSymbolString(const QString& symbolString) const
{ 
  qDebug("AbstractBarcodeEngine parseSymbolString() : start");
  // empty argument  
  if (symbolString.isEmpty()) {
    qDebug("AbstractBarcodeEngine parseSymbolString() : empty");
    return QStringList();
  }  
  qDebug("AbstractBarcodeEngine parseSymbolString() : not empty");
  // verify whether expecting and defined multi-Charactor Symbol regexp patterns
  Q_ASSERT_X(m_multiCharSymbols == !d->m_regexpCollection.empty(),
	     "Conflicted mult-charactor symbols settings",
	     "Inconsistent mult-charactor symbols settings");  
  // initialize the colection of regexp patterns and their starting indexes 
  if (!d->m_regexpCollection.empty()) {
    shared::RegexpCollection::const_iterator currentRegExpItr = 
      d->m_regexpCollection.begin();
      
    while(currentRegExpItr != d->m_regexpCollection.end()){    
	currentRegExpItr->setSourceString(symbolString);
	currentRegExpItr++;
    }
    std::sort(d->m_regexpCollection.begin(), d->m_regexpCollection.end());
  }
  qDebug("AbstractBarcodeEngine parseSymbolString() : start parsing");
  // step through input string
  QStringList userSymbolsList;  
  for (int userIndex = 0; userIndex < symbolString.size(); ) {      
    if (!d->m_regexpCollection.empty() && 
      d->m_regexpCollection.begin()->isValid()) { 
      userSymbolsList << d->m_regexpCollection.begin()->getMatchingString();   
      userIndex += userSymbolsList.back().size();
      d->m_regexpCollection.begin()->nextMatch();

      //  resort collection so closest matching symbol is first
      std::sort(d->m_regexpCollection.begin(), d->m_regexpCollection.end());  
    } 
    else {
      userSymbolsList << symbolString.at(userIndex++);
    }
    if (getSymbolIndex(userSymbolsList.back()) == NOT_FOUND) {
      qDebug("AbstractBarcodeEngine parseSymbolString() : unknown symbol");
      qDebug() << "removing : " << userSymbolsList.back();
      userSymbolsList.pop_back();
    }
  } // while    

  if (userSymbolsList.isEmpty()) {
    qDebug("AbstractBarcodeEngine parseSymbolString() : no valid symbols");
    m_isValid |= CodeEngine::UnknownSymbols;
  }
  qDebug("AbstractBarcodeEngine parseSymbolString() : end");
  return userSymbolsList;
}

QStringList AbstractBarcodeEngine::processSymbolList(const QStringList& userSymbols)
{
  qDebug("AbstractBarcodeEngine processSymbolList() : start");  
  // assumes check digit is at end of symbol "stream"
  Q_ASSERT(m_internalCheckDigit == false);
  
  // check digit are at the end of ths list
  int numberCheckDigitsFound = 0;  
  shared::LookupIndexArray symbolIndexes(
    this->convertSymbolsToIndexes(userSymbols));
  shared::LookupIndexArray calcSymbolIndexes;  
  // start at second to last index
  shared::LookupIndexArray::reverse_iterator itrCheckdigit = symbolIndexes.rbegin();    
  while (itrCheckdigit != symbolIndexes.rend() && 
    numberCheckDigitsFound < m_maxCheckDigits) { 
    // get next list of symbol indexes minus the possible check digit value
    std::copy(itrCheckdigit - 1, symbolIndexes.rend(), 
      std::back_inserter(calcSymbolIndexes));
    if (*(itrCheckdigit) != this->calculateCheckDigit(calcSymbolIndexes)) {
      break;
    }
    calcSymbolIndexes.clear();
    numberCheckDigitsFound++;
    itrCheckdigit++;
  }  
  m_isValid |= (numberCheckDigitsFound < m_requiredCheckDigits) ?
     CodeEngine::MissingRequiredCheckDigits : CodeEngine::OK;
  qDebug("AbstractBarcodeEngine processSymbolList() : end");
  return userSymbols;
}


int AbstractBarcodeEngine::calculateCheckDigit(const LookupIndexArray &symbolArray) const
{
  // do not use this function for product codes
  qDebug("AbstractBarcodeEngine calculateCheckDigit() : start");
  Q_ASSERT(m_internalCheckDigit == false);  
  qDebug("AbstractBarcodeEngine calculateCheckDigit() : end");
  
  return SimpleRemainderCheckDigit(
    std::accumulate(symbolArray.begin(), symbolArray.end(), 0));
}

int AbstractBarcodeEngine::CommonChecksumOddEven(
  const shared::LookupIndexArray& symbolArray, 
  int oddMultipler, 
  int evenMultipler, 
  bool reverse) const
{
  qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : start");
  Q_ASSERT(!symbolArray.empty());
  Q_ASSERT(oddMultipler > 0 && evenMultipler > 0);
  shared::LookupIndexArray l_symbolArray;  
  
  if (true == reverse) {
    qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : reverse copy");
    std::copy(symbolArray.rbegin(), symbolArray.rend(), 
	      std::back_inserter(l_symbolArray));
  } else {
    qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : copy");
    std::copy(symbolArray.begin(), symbolArray.end(), 
	      std::back_inserter(l_symbolArray));
  }
  qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : end");
  Q_ASSERT(l_symbolArray.size() == symbolArray.size());
  return NextMultipleCheckDigit(
    std::accumulate(l_symbolArray.begin(), l_symbolArray.end(), 0, 
      shared::EvenOddChecksum<int>(oddMultipler, evenMultipler))); 
}

int AbstractBarcodeEngine::CommonChecksumLinear(
      const shared::LookupIndexArray& symbolArray, bool reverse) const
{
  qDebug("AbstractBarcodeEngine CommonChecksumLinear() : start");
  Q_ASSERT(!symbolArray.empty());
  shared::LookupIndexArray l_symbolArray;
  
  if (reverse) {
    qDebug("AbstractBarcodeEngine CommonChecksumLinear() : reverse copy");
    std::copy(symbolArray.rbegin(), symbolArray.rend(), 
	      std::back_inserter(l_symbolArray));
  } else {
    qDebug("AbstractBarcodeEngine CommonChecksumLinear() : copy");
    std::copy(symbolArray.begin(), symbolArray.end(), 
	      std::back_inserter(l_symbolArray));
  }
  qDebug("AbstractBarcodeEngine CommonChecksumLinear() : end");
  Q_ASSERT(!l_symbolArray.size() == symbolArray.size());
  return this->SimpleRemainderCheckDigit(
    std::accumulate(symbolArray.begin(), symbolArray.end(), 
		*symbolArray.begin(), shared::LinearMultiple<int>(m_checksumModulus)));
}

int AbstractBarcodeEngine::NextMultipleCheckDigit(int checksum) const
{
  qDebug("AbstractBarcodeEngine NextMultipleCheckDigit()");
  Q_ASSERT(checksum >= 0);
  int checkValue = m_checksumModulus - SimpleRemainderCheckDigit(checksum);
  Q_ASSERT(checkValue > 0 && checkValue <= m_checksumModulus);
  checkValue = (checkValue == m_checksumModulus) ? 0 : checkValue;  
  Q_ASSERT(checkValue >= 0 && checkValue < m_checksumModulus);
  qDebug() << "checkValue : " << checkValue; 
  return checkValue;
}

int AbstractBarcodeEngine::SimpleRemainderCheckDigit(int checksum) const
{
  Q_ASSERT(checksum >= 0);
  return checksum % m_checksumModulus;
}    

shared::LookupIndexArray AbstractBarcodeEngine::convertSymbolsToIndexes(
  const QStringList& symbolList) const
{
  qDebug("AbstractBarcodeEngine convertSymbolsToIndexes() : start");
  Q_ASSERT(!symbolList.empty());
  shared::LookupIndexArray symbolArray;
  QStringList::const_iterator itrSymbol = symbolList.begin();
  while (itrSymbol != symbolList.end()) {
    symbolArray.push_back(getSymbolIndex(*itrSymbol++));
  }
  Q_ASSERT(symbolList.size() == symbolArray.size());
  qDebug("AbstractBarcodeEngine convertSymbolsToIndexes() : end");
  return symbolArray;
}

int AbstractBarcodeEngine::getSymbolIndex(const QString &symbol) const
{
  Q_ASSERT(d->m_symbolLookup != 0 && !symbol.isEmpty());
  return d->m_symbolLookup->indexOf(symbol);
}

const QString AbstractBarcodeEngine::getSymbolAtIndex(int symbolIndex) const
{  
  Q_ASSERT(d->m_symbolLookup != 0 && symbolIndex != NOT_FOUND);
  return d->m_symbolLookup->value(symbolIndex, QString());
}

QString AbstractBarcodeEngine::getSymbolEncoding(const QString& symbol) const
{  
  Q_ASSERT(d->m_encodingLookup != 0 && !symbol.isEmpty());
  int symbolIndex = getSymbolIndex(symbol);
  return convertWidthEncodingToBinary(d->m_encodingLookup->at(symbolIndex));
}

QString AbstractBarcodeEngine::getEncodedSymbols(shared::BarPositionsMap &barLocations, 
						     int barThicknessMultiple)
{
  qDebug("AbstractBarcodeEngine getEncodedSymbols() : start");
  barLocations = m_barPositions;  
  QString encodedSymbols;
  
  if (barThicknessMultiple > 1) {
    QString::const_iterator symbolIter = m_encodedSymbols.constBegin();
    while (symbolIter != m_encodedSymbols.end()) {
      encodedSymbols.append(QString(barThicknessMultiple, *symbolIter++));
    }    
    shared::BarPositionsMap::iterator posItr = barLocations.begin();     
    while (posItr != barLocations.end()) {
	(posItr++).value() *= barThicknessMultiple;
    }    
  }  
  qDebug() << "m_barPositions :" << m_barPositions;
  qDebug("AbstractBarcodeEngine getEncodedSymbols() : end");
  return m_encodedSymbols;
}

const QString AbstractBarcodeEngine::convertWidthEncodingToBinary(
  const QString& widthEncodedSymbol) const
{
  // FIXME support all encoding correctly
  qDebug("AbstractBarcodeEngine convertWidthEncodingToBinary() : start");  
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
  qDebug("AbstractBarcodeEngine convertWidthEncodingToBinary() : end");  
  return binaryString;
}

// --- setup barcode text and "bar" encoding ---

void AbstractBarcodeEngine::formatSymbols(const QStringList& symbolSrc)
{
  qDebug("AbstractBarcodeEngine formatSymbols() : start");
  Q_ASSERT(!symbolSrc.isEmpty());
  m_formatedSymbols = QStringList(symbolSrc.join(""));
  qDebug("AbstractBarcodeEngine formatSymbols() : end");
}

void AbstractBarcodeEngine::encodeSymbols(const QStringList& symbolSrc) 
{
  qDebug("AbstractBarcodeEngine encodeSymbols() : start");
  Q_ASSERT(!symbolSrc.isEmpty());
  QStringList::const_iterator itrSymbol = symbolSrc.constBegin();
  while(itrSymbol !=  symbolSrc.end()) {
    m_encodedSymbols.append(this->getSymbolEncoding(*itrSymbol++));
  }
  Q_ASSERT(!m_encodedSymbols.isEmpty());
  qDebug("AbstractBarcodeEngine encodeSymbols() : end");
}

void AbstractBarcodeEngine::fillSymbolList()
{
  qDebug("AbstractBarcodeEngine fillSymbolList() : start");
  // symbols : 0-9
  if (d->m_localSymbolLookup.isEmpty()) {
    for (int index = 0; index < baseSymbols::NUM_SYMBOLS; index++) {
      d->m_localSymbolLookup << QString(baseSymbols::SYMBOL_LOOKUP[index]);
    }
    qDebug("AbstractBarcodeEngine fillSymbolList() : symbols added"); 
    setSymbolLookupTable(d->m_localSymbolLookup); 
  }
}

void AbstractBarcodeEngine::setSymbolAndEncodingLookupTable(
				const QStringList& symbolSet, 
				const QStringList& encodingSet)
{
  setSymbolLookupTable(symbolSet);
  setSymbolEncodingLookupTable(encodingSet);
}

void AbstractBarcodeEngine::setSymbolLookupTable(const QStringList& symbolSet)
{
  Q_ASSERT(&symbolSet != 0);
  d->m_symbolLookup = const_cast<QStringList *>(&symbolSet);
}

QStringList AbstractBarcodeEngine::getSymbolLookupTable() const
{
  return *d->m_symbolLookup;
}

void AbstractBarcodeEngine::setSymbolEncodingLookupTable(
  const QStringList& symbolEncodingSet)
{
  Q_ASSERT(&symbolEncodingSet != 0);
  d->m_encodingLookup = const_cast<QStringList *>(&symbolEncodingSet);
}

QStringList AbstractBarcodeEngine::getSymbolEncodingLookupTable() const
{
  return *d->m_encodingLookup;
}

void AbstractBarcodeEngine::AddMultiSymbolRegexpString(const QString& symbolPattern)
{
  d->m_regexpCollection.push_back(IndexedRegExp(symbolPattern));
}
