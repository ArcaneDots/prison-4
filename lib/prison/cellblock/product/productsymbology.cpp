/*
 * Copyright (c) 2013 ian hollander @ gmail dot com
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

#include "productsymbology_p.h"
#include "productsymbology.h"
//#include "product/upceandefines_p.h"

#include <QRegExp>


using namespace product;

const QString ERROR_ENCODING = "1010101";

ProductSymbology::ProductSymbology() : 
  AbstractSymbology(*new ProductSymbologyPrivate)
{
}

ProductSymbology::ProductSymbology(const ProductSymbologyPrivate& other) : 
  AbstractSymbology(*new ProductSymbologyPrivate)
{
  Q_D(ProductSymbology);
}

ProductSymbology::~ProductSymbology()
{
}


QString ProductSymbology::errorEncoding() const
{
    return "1010101";
}

// QString ProductSymbology::encoding(const QChar& encodingType) const
// {
//   Q_D(ProductSymbology);
//   
//   QString encodingString(encodingType);
//   return (hasValue()) ? d->encoding(encodingType) : ERROR_ENCODING;
// }
// 
// QString ProductSymbology::encoding(const QString& encodingType) const
// {  
//   return (hasValue()) ? d->encoding(encodingType) : ERROR_ENCODING;
// }
// 
// const ProductSymbology& ProductSymbology::operator=(const Symbol& other)
// {
//   if (d != other.d) {
//     d = other.d;
//   }
//   return *this;
// }
// 
// const ProductSymbology& ProductSymbology::operator=(int index)
// {
//   setSymbolIndex(index);
//   return *this;
// }
// 
// QList< ProductSymbology > ProductSymbology::operator+=(const ProductSymbology& other)
// {
//   return QList<ProductSymbology>(linearSymbology::Symbol::operator+=(other));
// }
// 
// 
// bool ProductSymbology::operator==(const ProductSymbology& other) const
// {
//   return linearSymbology::Symbol::operator==(other);
// }
// 
// bool ProductSymbology::operator==(int index) const
// {
//   return linearSymbology::Symbol::operator==(index);
// }
// 
// 
// bool ProductSymbology::isValid() const
// {
//   return (!d == false &&
// 	  d.constData()->isValid());
// }
// 
// bool ProductSymbology::hasValue() const
// {
//   Q_D(const ProductSymbol);
//   return d->hasValue();
// }
// 
// QString ProductSymbology::toString() const
// {
//   Q_D(const ProductSymbol);
//   return (hasValue()) ? d->str() : QString("");
// }

// QList< Symbol > ProductSymbology::parse(const QString& userInput) const
// {
//   Q_D(const ProductSymbol);
//   if (!isValid()) { 
//     return QList<ProductSymbology>();    
//   }  
//   QList<ProductSymbology> parsedSymbols(d->parse(userInput));
//   return parsedSymbols;
// }
// 
// QList< Symbol > ProductSymbology::parse(const QStringList& userInput) const
// {
//   Q_D(const ProductSymbol);
//   QList<ProductSymbology> parsedSymbols;
//   if (d->isValid() == false) { 
//     return QList<ProductSymbology>();  
//   }
//      
//   QStringList::const_iterator itrString = userInput.begin();
//   QStringList::const_iterator itrLast = userInput.end(); 
//   while (itrString != itrLast) {
//     parsedSymbols += d->parse(*itrString++);
//   }
//   return parsedSymbols;
// }
// 
// QList<Symbol> Symbol::operator+=(const Symbol& other)
// {
//   QList<Symbol> parsedSymbols;
//   parsedSymbols.append(*this);
//   parsedSymbols += parse(other);
//   return parsedSymbols;
// }
// 
// bool Symbol::operator==(const Symbol& other) const
// {
//   // Remove second check once product subclasses symbol::private
//   return (d == other.d || d->m_index == other.d->m_index);
// }
// 
// bool Symbol::operator==(int index) const
// {
//   return (d->m_index == index);
// }
// 
// 
// bool Symbol::isValidString(const QString& symbolString) const
// {
//   return (d.constData()->m_symbolList.contains(symbolString));
// }
// 
// int Symbol::convertStringToIndex(const QString& symbolString) const
// {  
//   int index = NOT_FOUND;
//   if (isValidString(symbolString)) {
//     index =  d.constData()->m_symbolList.indexOf(symbolString); 
//   } 
//   return index;  
// }
// 
// const QString Symbol::convertIndexToString(const int index) const
// {  
//   QString str;
//   int numSymbols = d.constData()->m_symbolList.size();  
//   if (index > NOT_FOUND && index < numSymbols) {
//     str = d.constData()->m_symbolList.at(index);
//   }
//   return str;
// }
// 
// void Symbol::setSymbolIndex(int index) 
// {  
//   int finalIndex = index;
//   if (d.constData()->m_index == finalIndex) {
//     return;
//   }
//   if (finalIndex > shared::NOT_FOUND && 
//       finalIndex < d.constData()->m_symbolList.size()) {
//     d->m_index = finalIndex;
//   }
// }
// 
// void Symbol::setSymbolIndex(const QString & string) 
// {  
//   int finalIndex = convertStringToIndex(string);
//   if (finalIndex != shared::NOT_FOUND &&
//       finalIndex != d.constData()->m_index)  {
//     d->m_index = finalIndex; 
//   }
// }


QDebug operator<<(QDebug& dbg, const ProductSymbology& s)
{ 
  dbg << s.getName(); 
  return dbg;
}

// PatternEncoder::PatternEncoder(const QString& pattern) : 
//     m_pattern( pattern ), 
//     m_index(0)
// {
// 
// }
// 
// QString PatternEncoder::operator()(Symbol s)
// { 
//   const int patternSize = m_pattern.size();
//   QString result = "1010101";
//   if (m_index < patternSize) {
//     result = s.encoding(m_pattern.at(m_index++));    
//   }
//   return result;
// }
  


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

