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




using namespace linearSymbology;
// -------- Symbols ----------

AbstractSymbology::AbstractSymbology() : 
  d_ptr(new AbstractSymbologyPrivate) 
{ /* Empty */ }

AbstractSymbology::AbstractSymbology(const AbstractSymbology& other) : 
  //QSharedData(other),
  d_ptr(new AbstractSymbologyPrivate(other.d_ptr))
{ /* Empty */ }

AbstractSymbology::AbstractSymbology(AbstractSymbologyPrivate& d) : d_ptr(&d)
{ /* Empty */ }


AbstractSymbology::~AbstractSymbology()
{ /* Empty */ }

QString AbstractSymbology::getName() const {return QString("Abstract");}

QString AbstractSymbology::errorEncoding() const { return "1010101"; }

Symbol AbstractSymbology::getSymbol(int userIndex, QString symbolSet)
{
  Q_D(const AbstractSymbology);
  return Symbol(d->findNode(userIndex, symbolSet));
}


Symbol AbstractSymbology::getSymbol(QString userString, QString symbolSet)
{
  Q_D(const AbstractSymbology);
  return Symbol(d->findNode(userString, symbolSet));
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

QDebug operator<<(QDebug& dbg, const AbstractSymbology& s)
{ 
  dbg << s.getName(); 
  return dbg;
}



QList<Symbol> operator<<(const AbstractSymbology& symbols,
			 const QString& userInput)
{
  return symbols.parse(userInput);
}

QList<Symbol> operator<<(const AbstractSymbology& symbols, 
			 const QStringList& userInput)
{
  return symbols.parse(userInput);
}

QStringList encodeSymbolParity(const QList<Symbol>& symbols, 
			       const QString& parityPattern)
{
  if ( symbols.isEmpty() ) { 
    return QStringList();
  }
  
  QString pattern(parityPattern);
  QStringList encodedList;
  
  if ( !pattern.isEmpty() ) {
    std::transform(symbols.begin(), symbols.end(),
	 std::back_inserter(encodedList),
	 PatternEncoder(pattern));
  } else { 
    qDebug("Missing Parity Pattern string");
    int symbolsSize = symbols.size();
    QString errorEncoding(symbols.constBegin()->encoding(shared::NOT_FOUND));
    for (int i = 0; i < symbolsSize; i++) {
      encodedList.append(errorEncoding);
    }
  }
  return encodedList;
}

PatternEncoder::PatternEncoder(const QString& pattern) : 
    m_pattern( pattern ), 
    m_index(0)
{

}

QString PatternEncoder::operator()(const Symbol & s)
{ 
  const int patternSize = m_pattern.size();
  QString result = s.encoding(shared::NOT_FOUND);
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






  


