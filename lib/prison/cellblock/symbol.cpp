/*
 * Copyright (c) 2013 ian <email>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <algorithm>
#include <iterator>
#include <numeric>

//#include "symbol_p.h"
#include "symbol.h"
#include "product/upceandefines_p.h"

#include <QRegExp>

namespace barcodeEngine {

  
// -------- Private ----------
class Symbol::Private : public QSharedData
{
public:
  Private();
  Private(const Private &other);
  virtual ~Private();
  
  /// Initialize
  void init();
  /// Valid object
  bool isValid() const;
  /// Is set to a valid value
  bool hasValue() const;
  /// string version of integer value
  QString str() const;
  // parse symbols - no "empty" symbols
  QList< Symbol > parse(const QString& src) const;
  // get symbol encoding
  QString encoding(const QString& encodingType) const;
  // current symbol index
  int m_index;
  // Symbols "0-9"
  QStringList m_symbolList;    
  // Encoding : Lefthand "Even"  
  QStringList m_encodingLE;  
  // Encoding : Lefthand "Odd"  
  QStringList m_encodingLO;  
  // Encoding : Righthand  
  QStringList m_encodingR;
};

Symbol::Private::Private() : 
  m_index(NOT_FOUND),
  m_symbolList(),
  m_encodingLE(),
  m_encodingLO(),
  m_encodingR()
{  
  //qDebug("Symbol::Private default constructor");
  init();
}

Symbol::Private::Private(const Private &other) : 
  QSharedData(other), 
  m_index(other.m_index),
  m_symbolList(other.m_symbolList),
  m_encodingLE(other.m_encodingLE),
  m_encodingLO(other.m_encodingLO),
  m_encodingR(other.m_encodingR)
{
  // empty
}

void Symbol::Private::init()
{
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    m_symbolList.append(upc_common::SYMBOL_TABLE[i]);
    m_encodingLE.append(upc_common::LEFT_EVEN_ENCODE_TABLE[i]);
    m_encodingLO.append(upc_common::LEFT_ODD_ENCODE_TABLE[i]);
    m_encodingR.append(upc_common::RIGHT_HAND_ENCODE_TABLE[i]);
  }    
}

bool Symbol::Private::isValid() const
{
  qDebug("Symbol::Private isValid()");
  int numSymbols = m_symbolList.size();
  if (numSymbols == 10) {
    return (numSymbols == m_encodingLE.size() && 
      numSymbols == m_encodingLO.size() && 
      numSymbols == m_encodingR.size());
  }
  return false;
}

bool Symbol::Private::hasValue() const
{
  return m_index != NOT_FOUND;
}

QString Symbol::Private::str() const
{
  if (hasValue()) {
    return m_symbolList.at(m_index);
  }
  return QString("");
}


QList< Symbol > Symbol::Private::parse(const QString & src) const
{
  int length = src.size();
  QList<Symbol> indexList;
  for (int i = 0; i < length; i++) {
    QString possSymbol(src.at(i));
    if (m_symbolList.contains(possSymbol)) {
      indexList.append(Symbol((Symbol::Private *)this));
      indexList.back().setSymbolIndex(m_symbolList.indexOf(possSymbol)); 
    }
  }
  return indexList;
}

QString Symbol::Private::encoding(const QString& encodingType) const
{
  qDebug() << " Symbol " << m_symbolList.value(m_index);
  qDebug() << " encoding type " <<  encodingType;
  QString encodedSymbol;
  if (encodingType == "O" || encodingType == "o") {
    encodedSymbol = m_encodingLO.at(m_index);
    qDebug() << "encode LeftOdd: " << encodedSymbol;
  }
  else if (encodingType == "E" || encodingType == "e") {
    encodedSymbol = m_encodingLE.at(m_index);
    qDebug() << "encode LeftEven: " << encodedSymbol;
  }
  else if (encodingType == "R" || encodingType == "r") {
    encodedSymbol = m_encodingR.at(m_index);
    qDebug() << "encode Right: " << encodedSymbol;
  }
  // output all zeros when no pattern is available
  else {
    encodedSymbol = ERROR_ENCODING;
    qDebug() << "encode ERROR: " << ERROR_ENCODING;
  }
  return encodedSymbol;
}

Symbol::Private::~Private()
{
  // empty
}

};

using namespace barcodeEngine;
// -------- Symbols ----------

const QString Symbol::ERROR_ENCODING = "1010101";
const int Symbol::NOT_FOUND = -1;

Symbol::Symbol() : d(0) //: d(new Symbol::Private())
{
  d = new Symbol::Private;
}

Symbol::Symbol(const QString& string) : d(0)// : d(new Symbol::Private())
{  
  d = new Symbol::Private;
  setSymbolIndex(string);
}

Symbol::Symbol(int index)
{
  d = new Symbol::Private;
  setSymbolIndex(index);
}


Symbol::Symbol(const Symbol& other) : d(other.d)
{
}

Symbol::Symbol(Symbol::Private* sharedData) : d(sharedData)
{
}


Symbol::~Symbol()
{
}

int Symbol::getIndex() const
{
  return d.constData()->m_index;
}

QString Symbol::encoding(const QChar& encodingType) const
{
  QString encodingString(encodingType);
  return (hasValue()) ? d.constData()->encoding(encodingType) : ERROR_ENCODING;
}

QString Symbol::encoding(const QString& encodingType) const
{  
  return (hasValue()) ? d.constData()->encoding(encodingType) : ERROR_ENCODING;
}

const Symbol& Symbol::operator=(const Symbol& other)
{
  if (d != other.d) {
    d = other.d;
  }
  return *this;
}

const Symbol& Symbol::operator=(int index)
{
  setSymbolIndex(index);
  return *this;
}

bool Symbol::isValid() const
{
  return (!d == false &&
	  d.constData()->isValid());
}

bool Symbol::hasValue() const
{
  return d.constData()->hasValue();
}

QString Symbol::toString() const
{
  return (hasValue()) ? d.constData()->str() : QString("");
}

QList<Symbol> Symbol::parse(const QString& userInput) const
{
  if (!isValid()) { 
    return QList<Symbol>();    
  }  
  QList<Symbol> parsedSymbols(d.constData()->parse(userInput));
  return parsedSymbols;
}

QList<Symbol> Symbol::parse(const QStringList& userInput) const
{
  QList<Symbol> parsedSymbols;
  if (d.constData()->isValid() == false) { 
    return QList<Symbol>();  
  }
     
  QStringList::const_iterator itrString = userInput.begin();
  QStringList::const_iterator itrLast = userInput.end(); 
  while (itrString != itrLast) {
    parsedSymbols += d.constData()->parse(*itrString++);
  }
  return parsedSymbols;
}

QList<Symbol> Symbol::operator+=(const Symbol& other)
{
  QList<Symbol> parsedSymbols;
  parsedSymbols.append(*this);
  parsedSymbols += parse(other);
  return parsedSymbols;
}

bool Symbol::operator==(const Symbol& other) const
{
  // Remove second check once product subclasses symbol::private
  return (d == other.d || d->m_index == other.d->m_index);
}

bool Symbol::operator==(int index) const
{
  return (d->m_index == index);
}


bool Symbol::isValidString(const QString& symbolString) const
{
  return (d.constData()->m_symbolList.contains(symbolString));
}

int Symbol::convertStringToIndex(const QString& symbolString) const
{  
  int index = NOT_FOUND;
  if (isValidString(symbolString)) {
    index =  d.constData()->m_symbolList.indexOf(symbolString); 
  } 
  return index;  
}

const QString Symbol::convertIndexToString(const int index) const
{  
  QString str;
  int numSymbols = d.constData()->m_symbolList.size();  
  if (index > NOT_FOUND && index < numSymbols) {
    str = d.constData()->m_symbolList.at(index);
  }
  return str;
}

void Symbol::setSymbolIndex(int index) 
{  
  int finalIndex = index;
  if (d.constData()->m_index == finalIndex) {
    return;
  }
  if (finalIndex > shared::NOT_FOUND && 
      finalIndex < d.constData()->m_symbolList.size()) {
    d->m_index = finalIndex;
  }
}

void Symbol::setSymbolIndex(const QString & string) 
{  
  int finalIndex = convertStringToIndex(string);
  if (finalIndex != shared::NOT_FOUND &&
      finalIndex != d.constData()->m_index)  {
    d->m_index = finalIndex; 
  }
}

QDebug barcodeEngine::operator<<(QDebug& dbg, const Symbol& s)
{ 
  dbg << s.toString(); 
  return dbg;
}

QList<Symbol> barcodeEngine::operator<<(const QList<Symbol>& symbols, const QString& userInput)
{
  SymbolList tempSymbols(symbols);  
  tempSymbols += tempSymbols.front().parse(userInput);   
  return tempSymbols;
}

QString barcodeEngine::toStrings(const QList<Symbol> & symbolStrings)
{
  QStringList strings; 
  
  std::copy(symbolStrings.begin(), symbolStrings.end(),
	    std::back_inserter(strings));
  return strings.join("");
}

QStringList barcodeEngine::toStringList(const QList<Symbol>& symbolStrings)
{
  QStringList strings; 
  
  QList<Symbol>::const_iterator _first = symbolStrings.begin();
  QList<Symbol>::const_iterator _last = symbolStrings.end();
  while (_first != _last) {
      strings.append(_first->toString());
      _first++;      
  }
  return strings;
}

std::vector<int> toIntVector(const QList< Symbol >& symbolStrings)
{
  std::vector<int> vecInt; 
  
  std::copy<>(symbolStrings.begin(), 
	      symbolStrings.end(), 
	      std::back_inserter(vecInt));
  return vecInt;
}

QStringList barcodeEngine::encodeSymbolParity(const QList<Symbol>& symbols, 
			       const QString& parityPattern)
{
  //const QString Zeros(symbols.size(),'0');
  
  QString pattern(parityPattern);
  if ( symbols.isEmpty() ) { 
    return QStringList();
  }
  QStringList encodedList;
  if ( !pattern.isEmpty() ) {
    std::transform(symbols.begin(), symbols.end(),
	 std::back_inserter(encodedList),
	 PatternEncoder(pattern));
  } else { 
    qDebug("Missing Parity Pattern string");
    int symbolsSize = symbols.size();
    for (int i = 0; i < symbolsSize; i++) {
      encodedList.append(Symbol::ERROR_ENCODING);
    }
  }
  return encodedList;
}


PatternEncoder::PatternEncoder(const QString& pattern) : 
    m_pattern( pattern ), 
    m_index(0)
{

}

QString PatternEncoder::operator()(Symbol s)
{ 
  const int patternSize = m_pattern.size();
  QString result = "1010101";
  if (m_index < patternSize) {
    result = s.encoding(m_pattern.at(m_index++));    
  }
  return result;
}
  


// int calculateCheckDigit(int modulusValue, const QList<Symbol>& symbolArray) 
// {
//   // do not use this function for product codes
//   qDebug("calculateCheckDigit() : start");
//   return CommonChecksumOddEven(modulusValue, symbolArray,3,1, true);
// //   EvenOddChecksum l_eoChecksum<int>(oddMultipler, evenMultipler);
// //   int total = std::accumulate<int>(symbolArray.begin(), symbolArray.end(), 0);
// //   return std::modulus<int>(modulusValue, total);
// //   return SimpleRemainderCheckDigit(checksumModulus, 
// // 	std::accumulate(symbolArray.begin(), symbolArray.end(), 0));
// }
// 
// int shared::CommonChecksumOddEven(int checksumModulus, 
// 				  const SymbolList& symbols, 
// 				  int oddMultipler, 
// 				  int evenMultipler, 
// 				  bool reverse) 
// {
//   qDebug("CommonChecksumOddEven() : start");
//   int l_accum = 0;
//   if (symbols.isEmpty() || (oddMultipler > 1 && evenMultipler > 1)) {
//     return l_accum;
//   }  
//   EvenOddChecksum l_eoChecksum(oddMultipler, evenMultipler);
//   
//   if (true == reverse) {
//     std::reverse_iterator<QList<Symbol>::const_iterator> rend(symbols.begin()) ;
//     std::reverse_iterator<QList<Symbol>::const_iterator> rbegin(symbols.end());
//     qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : reverse copy");
//     l_accum = std::accumulate(rbegin, rend, 0, l_eoChecksum);
//   } else {
//     qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : copy");
//     l_accum = std::accumulate(symbols.begin(), symbols.end(), 0, l_eoChecksum);
//   }
//   qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : end");
//   
//   return NextMultipleCheckDigit(checksumModulus, l_accum); 
// }

//   shared::LookupIndexArray l_symbolArray;  
//   
//   if (true == reverse) {
//     qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : reverse copy");
//     std::copy(symbols.rbegin(), symbols.rend(), 
// 	      std::back_inserter(l_symbolArray));
//   } else {
//     qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : copy");
//     std::copy(symbols.begin(), symbols.end(), 
// 	      std::back_inserter(l_symbolArray));
//   }
//   qDebug("AbstractBarcodeEngine CommonChecksumOddEven() : end");
//   Q_ASSERT(l_symbolArray.size() == symbolArray.size());
//   return NextMultipleCheckDigit(checksumModulus, 
// 				std::accumulate(l_symbolArray.begin(), l_symbolArray.end(), 0, 
// 				shared::EvenOddChecksum<int>(oddMultipler, 
// evenMultipler))); 
// }

// int shared::CommonChecksumLinear(int checksumModulus, const SymbolList& symbols, bool reverse) 
// {
//   qDebug("CommonChecksumLinear() : start");
//   int total = 0;
//   if (symbols.isEmpty()) { return total; }
//   
//   if (reverse) {
//     qDebug("CommonChecksumLinear() : reverse copy");
//     std::reverse_iterator<QList<Symbol>::const_iterator> rend(symbols.begin()) ;
//     std::reverse_iterator<QList<Symbol>::const_iterator> rbegin(symbols.end());
//     total = std::accumulate(rbegin, rend, 0,		
// 		    LinearMultiple(checksumModulus));
//   } else {
//     std::accumulate(symbols.begin(), symbols.end(), 0,		
// 		    LinearMultiple(checksumModulus));
//   }
//   qDebug("AbstractBarcodeEngine CommonChecksumLinear() : end");
//   return SimpleRemainderCheckDigit(checksumModulus, total);
// }
// 
// int shared::NextMultipleCheckDigit(int modulusValue, int checksum)
// {
//   qDebug("AbstractBarcodeEngine NextMultipleCheckDigit()");
//   // invert value = m - (m % x)
//   int checkValue = modulusValue - SimpleRemainderCheckDigit(modulusValue, checksum);
//   // case : x == m -> x = 0
//   checkValue = (checkValue == modulusValue) ? 0 : checkValue; 
//   qDebug() << "checkValue : " << checkValue; 
//   return checkValue;
// }

// int shared::SimpleRemainderCheckDigit(int checksumModulus, int checksum)
// {
//   // prevent divide by zero
//   if (checksumModulus <= 0)  { return checksum; }
//   return checksum % checksumModulus;
// }    






  


