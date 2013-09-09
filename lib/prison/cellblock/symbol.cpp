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

namespace shared {

// -------- Private ----------
class Symbol::Private : public QSharedData
{
public:
  Private();
  Private(const Private &other);
  virtual ~Private();
  
  bool isValid() const;
  void init();
  
  int m_index;
  
  QStringList m_symbolList;    
  QStringList m_encodingLE;
  QStringList m_encodingLO;
  QStringList m_encodingR;
};

Symbol::Private::Private() : 
  m_index(NOT_FOUND),
  m_symbolList(),
  m_encodingLE(),
  m_encodingLO(),
  m_encodingR()
{  
  qDebug("Symbol::Private default constructor");
  init();
}

// Symbol::Private::Private() : 
//   m_symbolList(symbolData.m_symbolList),
//   m_encodingLE(symbolData.m_encodingLE),
//   m_encodingLO(symbolData.m_encodingLO),
//   m_encodingR(symbolData.m_encodingR)
// {
// }

Symbol::Private::Private(const Private &other) : 
  QSharedData(other), 
  m_index(other.m_index),
  m_symbolList(other.m_symbolList),
  m_encodingLE(other.m_encodingLE),
  m_encodingLO(other.m_encodingLO),
  m_encodingR(other.m_encodingR)
{
  qDebug("Symbol::Private copy constructor");
}

void Symbol::Private::init()
{
  qDebug("Symbol::Private init() : start");
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    m_symbolList.append(upc_common::SYMBOL_TABLE[i]);
    m_encodingLE.append(upc_common::LEFT_EVEN_ENCODE_TABLE[i]);
    m_encodingLO.append(upc_common::LEFT_ODD_ENCODE_TABLE[i]);
    m_encodingR.append(upc_common::RIGHT_HAND_ENCODE_TABLE[i]);
  }    
  qDebug("Symbol::Private init() : end");
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

Symbol::Private::~Private()
{
  qDebug("Symbol::Private destructor");
}

};

using namespace shared;
// -------- Symbols ----------

Symbol::Symbol() : d(0) //: d(new Symbol::Private())
{
  qDebug("Symbol default constructor");
  d = new Symbol::Private;
  qDebug("Symbol default constructor end");
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
  qDebug("Symbol::Symbol copy constructor");
}

// Symbol::Symbol(const SymbolInfo& symbolInfo) : 
// 		d(new Symbol::Private(symbolInfo)),
// 		m_symbolIndex(NOT_FOUND)
// {  
// }

Symbol::~Symbol()
{
  qDebug("Symbol::Symbol destructor");
}

int Symbol::getIndex() const
{
  return d.constData()->m_index;
}

QString Symbol::encoding(const QChar& encodingType) const
{
  QString encodingString(encodingType);
  return encoding(encodingString);
}

QString Symbol::encoding(const QString& encodingType) const
{  
  const QString Zeros(upc_common::ENCODE_LENGTH,'0');
  
  qDebug() << " Symbol " << convertIndexToString(getIndex());
  qDebug() << " encoding type " <<  encodingType;
  QString encodedSymbol;
  if (encodingType == "O") {
    encodedSymbol = d.constData()->m_encodingLO.at(getIndex());
    qDebug() << "encode LeftOdd: " << encodedSymbol;
  }
  else if (encodingType == "E") {
    encodedSymbol = d.constData()->m_encodingLE.at(getIndex());
    qDebug() << "encode LeftEven: " << encodedSymbol;
  }
  else if (encodingType == "R") {
    encodedSymbol = d.constData()->m_encodingLE.at(getIndex());
    qDebug() << "encode Right: " << encodedSymbol;
  }
  // output all zeros when no pattern is available
  else {
    encodedSymbol = Zeros;
    qDebug() << "encode Zeros: " << Zeros;
  }
  return encodedSymbol;
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

QList<Symbol> Symbol::parse(const QString& userInput) const
{
  qDebug("Symbol::parse(string) : start");
  if (!d.constData()->isValid()) { return QList<Symbol>(); }
  
  QList<Symbol> parsedSymbols;
  QRegExp parseDigits("(\\d)");
  int pos = 0;
  
  Symbol blankSymbol(*this);
  
  while ((pos = parseDigits.indexIn(userInput, pos)) != -1) {
    int index = convertStringToIndex(QString(userInput.at(pos)));
    if (index != NOT_FOUND) {
      parsedSymbols.append(Symbol(*this));
      parsedSymbols.back().setSymbolIndex(index);
    }
    pos += parseDigits.matchedLength();
  }
  qDebug("Symbol::parse(string) : end");
  return parsedSymbols;
}

QList<Symbol> Symbol::parse(const QStringList& userInput) const
{
  if (!d.constData()->isValid()) { return QList<Symbol>(); }
  
  QList<Symbol> parsedSymbols;
  
  QStringList::const_iterator itrString = userInput.begin();
  QStringList::const_iterator itrLast = userInput.end(); 
  while (itrString != itrLast) {
    parsedSymbols += parse(*itrString++);
  }
  return parsedSymbols;
}

QList<Symbol> Symbol::operator+=(const Symbol& other)
{
  QList<Symbol> tempSymbols;
  tempSymbols.append(*this);
  tempSymbols += this->parse(other);
  return tempSymbols;
}

bool Symbol::operator==(const Symbol& other) const
{
  return (d == other.d);
}

bool Symbol::operator==(int index) const
{
  return (getIndex() == index);
}

bool Symbol::isValid() const
{
  return true;
}
bool Symbol::hasValue() const
{
  return (d != 0 && getIndex() != NOT_FOUND);
}

QString Symbol::toString() const
{
  QString result = convertIndexToString(getIndex()); 
  if (result.isNull()) {
    result = "";
  }
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
  if (index >= 0 && index < numSymbols) {
    str = d.constData()->m_symbolList.at(index);
  }
  return str;
}

void Symbol::setSymbolIndex(int index) 
{  
  int finalIndex = index;
  QString string = convertIndexToString(index);
  if (string.isNull()) {
    finalIndex = shared::NOT_FOUND;
  }
  if (finalIndex != d.constData()->m_index) {
    d->m_index = finalIndex; 
  }
}

void Symbol::setSymbolIndex(const QString & string) 
{  
  int finalIndex = convertStringToIndex(string);
  if (finalIndex != d.constData()->m_index)  {
    d->m_index = finalIndex; 
  }
}

QDebug shared::operator<<(QDebug& dbg, const Symbol& s)
{ 
  dbg << s.toString(); 
  return dbg;
}

QList<Symbol> shared::operator<<(const QList<Symbol>& symbols, const QString& userInput)
{
  SymbolList tempSymbols(symbols);  
  tempSymbols += tempSymbols.front().parse(userInput);   
  return tempSymbols;
}

QString shared::toStrings(const QList<Symbol> & symbolStrings)
{
  QStringList strings; 
  
  std::copy(symbolStrings.begin(), symbolStrings.end(),
	    std::back_inserter(strings));
  return strings.join("");
}

QStringList shared::toStringList(const QList< Symbol >& symbolStrings)
{
  QList<QString> strings; 
  
  std::copy<>(symbolStrings.begin(), 
	      symbolStrings.end(), 
	      std::back_inserter(strings));
  return strings;
}

// shared::LookupIndexArray convertSymbolsToIndexes(
//   const QList<Symbol>& symbolList)
// {
//   qDebug("Symbols convertSymbolsToIndexes() : start");
//   shared::LookupIndexArray symbolArray;
//   QList<Symbol>::const_iterator itrSymbol = symbolList.begin();
//   while (itrSymbol != symbolList.end()) {
//     symbolArray.push_back(*itrSymbol++));
//   }
//   qDebug("Symbols convertSymbolsToIndexes() : end");
//   return symbolArray;
// }

std::vector<int> toIntVector(const QList< Symbol >& symbolStrings)
{
  std::vector<int> vecInt; 
  
  std::copy<>(symbolStrings.begin(), 
	      symbolStrings.end(), 
	      std::back_inserter(vecInt));
  return vecInt;
}

PatternEncoder::PatternEncoder(const QString& pattern) : 
    m_pattern( pattern ), 
    m_index(0)
{

}

QString PatternEncoder::operator()(Symbol s)
{ 
  const int patternSize = m_pattern.size();
  QString result = s.encoding('O');
  if (m_index < patternSize) {
    result = s.encoding(m_pattern.at(m_index++));    
  }
  return result;
}
  
QStringList shared::encodeSymbolParity(const QList<Symbol>& symbols, 
			       const QString& parityPattern)
{
  const QString Zeros(symbols.size(),'0');
  
  QString pattern(parityPattern);
  if ( symbols.isEmpty() ) { 
    return QStringList();
  }
  if ( pattern.isEmpty() ) { 
    qDebug("Missing Parity Pattern string");
    pattern = Zeros;
  }
  QStringList encodedList;
  std::transform(symbols.begin(), symbols.end(), std::back_inserter(encodedList),
		 PatternEncoder(pattern));
  return encodedList;
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






  


