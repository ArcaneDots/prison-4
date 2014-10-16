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

#include "symbolnode.h"
#include "abstractsymbology.h"
#include "symbol.h"
//#include "symbol_p.h"
#include "product/upceandefines_p.h"

//#include <QRegExp>



using namespace linearSymbology;
// -------- Symbols ----------
// typedef linearSymbology::hidden::SymbolNode SymbolNode;

//const QString Symbol::ERROR_ENCODING = "1010101";

Symbol::Symbol() :
  m_node(0),
  m_symbology(0)
{

}

Symbol::Symbol(const Symbol& s) :
  m_node(s.m_node),
  m_symbology(s.m_symbology)
{
  
}

Symbol::Symbol(SymbolNode* symbolData) :
  m_node(symbolData),
  m_symbology(0)
{
  if (hasValue()) { m_symbology = const_cast<AbstractSymbology *>(m_node->getSymbology()); } //->getSymbology(); }
}


Symbol::~Symbol()
{
  // don't delete pointers on exit
}



// Symbol::Symbol() : 
//   d_ptr(new SymbolPrivate) 
// {  
//   Q_D(Symbol);
//   d->m_node = 0;
// }
// 
// Symbol::Symbol(const Symbol& other) : 
//   d_ptr(new SymbolPrivate())
// {
//   Q_D(Symbol);
//   d->m_node = other.d_ptr.data()->m_node;
// }
// 
// Symbol::Symbol(const AbstractSymbology& symbology) : 
//   d_ptr(new SymbolPrivate())
// {
//   Q_D(Symbol);
//   d->m_node = symbology.getIndex(NOT_FOUND);
// }
// 
// Symbol::~Symbol()
// {
// }

bool Symbol::isValid() const
{
  return m_symbology != 0;
//   Q_D(const Symbol);
//   return (!d == false &&
// 	  d->isValid());
}

bool Symbol::hasValue() const
{
  
  return m_node != 0;
//   Q_D(const Symbol);
//   return d->hasValue();
}

int Symbol::getIndex() const
{
  return hasValue() ? m_node->index() : shared::NOT_FOUND;
//   Q_D(const Symbol);
//   return d->m_node->Index();
}



QString Symbol::encoding(const QChar& encodingType) const
{
  return encoding(QString(encodingType));
//   Q_D(const Symbol);
//   QString encodingString(encodingType);
//   return (hasValue()) ? d->encoding(encodingType) : d->encoding();
}

QString Symbol::encoding(const QString& encodingType) const
{  
  if (hasValue()) {
    m_node->encoding(encodingType);
  } else if (isValid()) {
    m_symbology->errorEncoding();
  } else { 
    return QString(""); 
  }
//   Q_D(const Symbol);
//   return (hasValue()) ? d->encoding(encodingType) : d->encoding();
}





QString Symbol::toString() const
{
  return (hasValue()) ? m_node->toString() : QString("");
//   Q_D(const Symbol);
//   return (hasValue()) ? d->m_node->toString() : QString("");
}

QList<Symbol> Symbol::parse(const QString& userInput) const
{
  if (!isValid()) { 
    return QList<Symbol>();    
  }
  
  return m_symbology->parse(userInput);
}

QList<Symbol> Symbol::parse(const QStringList& userInput) const
{
  QList<Symbol> parsedSymbols;
  
  QStringList::const_iterator itrString = userInput.begin();
  QStringList::const_iterator itrLast = userInput.end(); 
  for (; itrString != itrLast; ++itrString) {
    parsedSymbols += parse(*itrString);    
  }
  return parsedSymbols;
}

// const Symbol::SymbolNode* Symbol::getNode() const
// {
//   Q_D(const Symbol);
//   return d->m_node;
// }



const Symbol& Symbol::operator=(const Symbol& other)
{ 
  m_node = other.m_node;
  m_symbology = other.m_symbology; 
  
  return *this;
//   Q_D(Symbol);
//   if (d->m_node != other.d_ptr.data()->m_node) {
//     d->m_node = other.d_ptr.data()->m_node;
//   }
//   return *this;
}

bool Symbol::operator==(const Symbol& right) const
{
  return (m_node == right.m_node && m_symbology == right.m_symbology);
//   Q_D(const Symbol);
//   return (d->m_node == right.d_ptr->m_node);
}

bool Symbol::operator==(int index) const
{  
  return (hasValue() && (index == m_node->index()));
//   Q_D(const Symbol);
//   return (d->getIndex() == index);
}


// bool Symbol::isValidString(const QString& symbolString) const
// {
//   Q_D(const Symbol);
//   return (d->symbolString.contains(symbolString));
// }
// 
// int Symbol::convertStringToIndex(const QString& symbolString) const
// {  
//   Q_D(const Symbol);
//   int index = NOT_FOUND;
//   if (isValidString(symbolString)) {
//     index =  d->symbolString.indexOf(symbolString); 
//   } 
//   return index;  
// }
// 
// const QString Symbol::convertIndexToString(const int index) const
// {  
//   Q_D(const Symbol);
//   QString str;
//   int numSymbols = d->m_symbolList.size();  
//   if (index > NOT_FOUND && index < numSymbols) {
//     str = d->m_symbolList.at(index);
//   }
//   return str;
// }

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


const Symbol& Symbol::operator=(int index)
{
  //setSymbolIndex(index);
  return *this;
}

// static operators

bool operator<(const Symbol& left, const Symbol& right) 
{
  return left.getIndex() < right.getIndex();
}

bool operator>(const Symbol& left, const Symbol& right) 
{  
  return left.getIndex() > right.getIndex();
}

// bool Symbol::operator==(const Symbol& left, const Symbol& right)
// {
//   return (left. == right.getNode());
// }
bool linearSymbology::operator<=(const Symbol& left, const Symbol& right)
{
  return (left < right  || left == right);
}

bool linearSymbology::operator>=(const Symbol& left, const Symbol& right) 
{
  return (left > right  || left == right);
}


QList<Symbol> operator+=(const QList<Symbol> & left, const Symbol& right)
{
  QList<Symbol> temp(left);
  temp.append(right);
  return temp;
}




QDebug operator<<(QDebug& dbg, const Symbol& s)
{ 
  dbg << s.toString(); 
  return dbg;
}

QList<Symbol> operator<<(const QList<Symbol>& symbols, const QString& userInput)
{
  QList<Symbol> tempSymbols(symbols);  
  tempSymbols += symbols.front().parse(userInput);   
  return tempSymbols;
}







// QString shared::toStrings(const QList<Symbol> & symbolStrings)
// {
//   QStringList strings; 
//   
//   std::copy(symbolStrings.begin(), symbolStrings.end(),
// 	    std::back_inserter(strings));
//   return strings.join("");
// }
// 
// QStringList shared::toStringList(const QList<Symbol>& symbolStrings)
// {
//   QStringList strings; 
//   
//   QList<Symbol>::const_iterator _first = symbolStrings.begin();
//   QList<Symbol>::const_iterator _last = symbolStrings.end();
//   while (_first != _last) {
//       strings.append(_first->toString());
//       _first++;      
//   }
//   return strings;
// }
// 
// std::vector<int> toIntVector(const QList< Symbol >& symbolStrings)
// {
//   std::vector<int> vecInt; 
//   
//   std::copy<>(symbolStrings.begin(), 
// 	      symbolStrings.end(), 
// 	      std::back_inserter(vecInt));
//   return vecInt;
// }

// QStringList shared::encodeSymbolParity(const QList<Symbol>& symbols, 
// 			       const QString& parityPattern)
// {
//   //const QString Zeros(symbols.size(),'0');
//   
//   QString pattern(parityPattern);
//   if ( symbols.isEmpty() ) { 
//     return QStringList();
//   }
//   QStringList encodedList;
//   if ( !pattern.isEmpty() ) {
//     std::transform(symbols.begin(), symbols.end(),
// 	 std::back_inserter(encodedList),
// 	 PatternEncoder(pattern));
//   } else { 
//     qDebug("Missing Parity Pattern string");
//     int symbolsSize = symbols.size();
//     for (int i = 0; i < symbolsSize; i++) {
//       encodedList.append(ERROR_ENCODING);
//     }
//   }
//   return encodedList;
// }
// 
// 
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
//   


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






  


