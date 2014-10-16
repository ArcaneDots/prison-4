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

#include "abstractsymbology_p.h"
#include "product/upceandefines_p.h"

#include <QRegExp>


//namespace shared {

  
// // -------- Private ----------
// class Symbol::Private : public QSharedData
// {
// public:
//   Private();
//   Private(const Private &other);
//   virtual ~Private();
//   
//   /// Initialize
//   void init();
//   /// Valid object
//   bool isValid() const;
//   /// Is set to a valid value
//   bool hasValue() const;
//   /// string version of integer value
//   QString str() const;
//   // parse symbols - no "empty" symbols
//   QList< Symbol > parse(const QString& src) const;
//   // get symbol encoding
//   QString encoding(const QString& encodingType) const;
//   // current symbol index
//   int m_index;
//   // Symbols "0-9"
//   QStringList m_symbolList;    
//   // Encoding : Lefthand "Even"  
//   QStringList m_encodingLE;  
//   // Encoding : Lefthand "Odd"  
//   QStringList m_encodingLO;  
//   // Encoding : Righthand  
//   QStringList m_encodingR;
// };
// 
// Symbol::Private::Private() : 
//   m_index(NOT_FOUND),
//   m_symbolList(),
//   m_encodingLE(),
//   m_encodingLO(),
//   m_encodingR()
// {  
//   //qDebug("Symbol::Private default constructor");
//   init();
// }
// 
// Symbol::Private::Private(const Private &other) : 
//   QSharedData(other), 
//   m_index(other.m_index),
//   m_symbolList(other.m_symbolList),
//   m_encodingLE(other.m_encodingLE),
//   m_encodingLO(other.m_encodingLO),
//   m_encodingR(other.m_encodingR)
// {
//   // empty
// }
// 
// void Symbol::Private::init()
// {
//   for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
//     m_symbolList.append(upc_common::SYMBOL_TABLE[i]);
//     m_encodingLE.append(upc_common::LEFT_EVEN_ENCODE_TABLE[i]);
//     m_encodingLO.append(upc_common::LEFT_ODD_ENCODE_TABLE[i]);
//     m_encodingR.append(upc_common::RIGHT_HAND_ENCODE_TABLE[i]);
//   }    
// }
// 
// bool Symbol::Private::isValid() const
// {
//   qDebug("Symbol::Private isValid()");
//   int numSymbols = m_symbolList.size();
//   if (numSymbols == 10) {
//     return (numSymbols == m_encodingLE.size() && 
//       numSymbols == m_encodingLO.size() && 
//       numSymbols == m_encodingR.size());
//   }
//   return false;
// }
// 
// bool Symbol::Private::hasValue() const
// {
//   return m_index != NOT_FOUND;
// }
// 
// QString Symbol::Private::str() const
// {
//   if (hasValue()) {
//     return m_symbolList.at(m_index);
//   }
//   return QString("");
// }
// 
// 
// QList< Symbol > Symbol::Private::parse(const QString & src) const
// {
//   int length = src.size();
//   QList<Symbol> indexList;
//   for (int i = 0; i < length; i++) {
//     QString possSymbol(src.at(i));
//     if (m_symbolList.contains(possSymbol)) {
//       indexList.append(Symbol((Symbol::Private *)this));
//       indexList.back().setSymbolIndex(m_symbolList.indexOf(possSymbol)); 
//     }
//   }
//   return indexList;
// }
// 
// QString Symbol::Private::encoding(const QString& encodingType) const
// {
//   qDebug() << " Symbol " << m_symbolList.value(m_index);
//   qDebug() << " encoding type " <<  encodingType;
//   QString encodedSymbol;
//   if (encodingType == "O" || encodingType == "o") {
//     encodedSymbol = m_encodingLO.at(m_index);
//     qDebug() << "encode LeftOdd: " << encodedSymbol;
//   }
//   else if (encodingType == "E" || encodingType == "e") {
//     encodedSymbol = m_encodingLE.at(m_index);
//     qDebug() << "encode LeftEven: " << encodedSymbol;
//   }
//   else if (encodingType == "R" || encodingType == "r") {
//     encodedSymbol = m_encodingR.at(m_index);
//     qDebug() << "encode Right: " << encodedSymbol;
//   }
//   // output all zeros when no pattern is available
//   else {
//     encodedSymbol = ERROR_ENCODING;
//     qDebug() << "encode ERROR: " << ERROR_ENCODING;
//   }
//   return encodedSymbol;
// }
// 
// Symbol::Private::~Private()
// {
//   // empty
// }
// 
// };

using namespace linearSymbology;
// -------- Symbols ----------



AbstractSymbology::AbstractSymbology() : 
  d_ptr(new AbstractSymbologyPrivate) 
{
  
}

AbstractSymbology::AbstractSymbology(const AbstractSymbology& other) : 
  //QSharedData(other),
  d_ptr(new AbstractSymbologyPrivate(other.d_ptr))
{
}

// AbstractSymbology::AbstractSymbology(const QString& string) :
//   d_ptr(new AbstractSymbologyPrivate) 
// { 
//   Q_D(Symbol);  
//   d->setSymbolIndex(string);
// }
// 
// AbstractSymbology::AbstractSymbology(int index) :
//   d_ptr(new AbstractSymbologyPrivate) 
// {
//   Q_D(Symbol); 
//   d->setSymbolIndex(index);
// }


// AbstractSymbology::AbstractSymbology(const AbstractSymbology& other) : 
//   d_ptr(new AbstractSymbologyPrivate)
// {
// }



// Symbol::Symbol(SymbolPrivate& sharedData) : d_ptr(&sharedData)
// {
// }


AbstractSymbology::~AbstractSymbology()
{
}

// int AbstractSymbology::getIndex() const
// {
//   Q_D(const Symbol);
//   return d->getIndex();
// }

// QString Symbol::encoding(const QChar& encodingType) const
// {
//   QString encodingString(encodingType);
//   return (hasValue()) ? d_ptr.constData()->encoding(encodingType) : ERROR_ENCODING;
// }
// 
// QString Symbol::encoding(const QString& encodingType) const
// {  
//   return (hasValue()) ? d_ptr.constData()->encoding(encodingType) : ERROR_ENCODING;
// }

// const AbstractSymbology& AbstractSymbology::operator=(const AbstractSymbology& other)
// {
//   Q_D(Symbol);
//   if (d_ptr != other.d_ptr) {
//     d->copy(other.d_ptr); 
//   }
//   return *this;
// }
// 
// const AbstractSymbology& AbstractSymbology::operator=(int index)
// {
//   Q_D(Symbol);
//   d->setSymbolIndex(index);
//   return *this;
// }

// bool AbstractSymbology::isValid() const
// {
//   Q_D(const Symbol);
//   return (d_ptr->isValid());
// }
// 
// bool AbstractSymbology::hasValue() const
// {  
//   Q_D(const Symbol);
//   return d_ptr->hasValue();
// }
// 
// QString AbstractSymbology::toString() const
// {
//   Q_D(const Symbol);
//   return (hasValue()) ? d_ptr->str() : QString("");
// }

SymbolNode* AbstractSymbology::findNode(const QString& userSymbol,
					QString symbolSet) const
{  
  Q_D(const AbstractSymbology);
  return d->findNode(userSymbol, symbolSet);
}

SymbolNode * AbstractSymbology::findNode(int index, 
					 QString symbolSet) const
{ 
  Q_D(const AbstractSymbology);
  return d->findNode(index, symbolSet);
}

QList<Symbol> AbstractSymbology::parse(const QString& userInput) const
{
  Q_D(const AbstractSymbology);
  return d->parse(userInput);
     
 
}

QList<Symbol> AbstractSymbology::parse(const QStringList& userInput) const
{
   Q_D(const AbstractSymbology);
   return d->parse(userInput);
}

Symbol AbstractSymbology::getSymbol(int index, QString symbolSet)
{

}


Symbol AbstractSymbology::getSymbol(QString string, QString symbolSet)
{

}

QString AbstractSymbology::errorEncoding()
{
  return "1010101";
}

QDebug operator<<(QDebug& dbg, const AbstractSymbology& s)
{ 
  dbg << s.getName(); 
  return dbg;
}




PatternEncoder::PatternEncoder(const QString& pattern) : 
    m_pattern( pattern ), 
    m_index(0)
{

}

QString PatternEncoder::operator()(const Symbol & s)
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






  


