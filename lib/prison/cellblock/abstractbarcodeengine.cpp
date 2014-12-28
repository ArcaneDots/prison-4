/*
 *  Copyright (C) 2011  Ian Hollander <ianhollander at gmail dot com>
 * 
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 * 
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */

#include <algorithm>
#include <numeric>
#include "abstractbarcodeengine_p.h"

namespace linearBarcode
{

// WidthEncoding: number of contigous segments that are in a bar or space
// BarEncoding: actual value for each segment 

  
// -------------- AbstractBarcodeEngine --------------

AbstractBarcodeEngine::AbstractBarcodeEngine() : 
  d_ptr(new AbstractBarcodeEnginePrivate)
{
  qDebug("AbstractBarcodeEngine constructor");
}

AbstractBarcodeEngine::AbstractBarcodeEngine(AbstractBarcodeEnginePrivate& d) : 
  d_ptr(&d)
{

}


AbstractBarcodeEngine::~AbstractBarcodeEngine()
{
  qDebug("AbstractBarcodeEngine destructor");
}


int AbstractBarcodeEngine::calculateCheckDigit() const
{  
  qDebug("AbstractBarcodeEngine calculateCheckDigit()");
  return 0;
}

// // ------------  -----
// RawEncoding::RawEncoding(){}
// 
// QString RawEncoding::operator()(Symbol& s1, Symbol& s2) {
//     return s1.encoding();
// }


// QStringList encodePattern(const QList<Symbol>& symbols, 
// 			  int Wide, int Narrow, 
// 			  bool LeftBar1)
// {
//   if ( symbols.isEmpty() ) { 
//     return QStringList();
//   }
//   
//   QList<Symbol>& l_symbols(const_cast<QList<Symbol> &>(symbols));
//   QStringList rawEncoding;
//   rawEncoding << toRawEncoding(symbols);
// //   std::copy(l_symbols.begin(), l_symbols.end(), 
// // 			  rawEncoding.begin(),RawEncoding());
// 
//   QStringList encodedList;
//   std::transform<QString>(rawEncoding.begin(), rawEncoding.end(), 
// 			  encodedList.begin(), 
// 			  WidthModulationEncoder(Wide, Narrow. leftBar1));
//   
//   return encodedList;
// }

// ------- PatternEncoder ------
WidthModulationEncoder::WidthModulationEncoder(int Wide, int Narrow, bool leftBar1) :
  m_wide(Wide),
  m_narrow(Narrow),
  m_leftBar1( m_leftBar1 ), 
  m_index(0),
  m_W("W"),
  m_N("N")
{ /* Empty */ }

int WidthModulationEncoder::decodeCharWidth(QString& str)
{  
  bool ok;
  bool isWide;
  int parity = str.toInt(&ok, 10);
  if (ok) {
    if ( parity != 0 && parity != 1) { return shared::NOT_FOUND; }
    isWide = (parity == 1) ? true : false;
  } else if (str.toUpper() == m_W || str.toUpper() == m_N) {
    isWide = (str.toUpper() == m_W) ? true : false;
  } else { 
    return shared::NOT_FOUND; 
  }
  
  return (isWide) ? m_wide : m_narrow;
}

QList<int> WidthModulationEncoder::decodeWidth(Symbol& s)
{
  QStringList encoding = s.encoding().split("");
  QList<int> numEncodings;
  for(QStringList::iterator itEncoding = encoding.begin();
   encoding.end() !=  itEncoding; 
    ++itEncoding)
  {
    numEncodings.append(decodeCharWidth(*itEncoding));
  }
}

QString WidthModulationEncoder::encodeBarModulation(QList<int>  widthLengths)
{
  
  QString result = "";
  
  for (int i = 0; i < widthLengths.length(); ++i)
  {
    bool Even = (i % 2 == 0 );
    bool isWide = false;
    QChar barDigit = (
	(Even && m_leftBar1) || 
	(!Even && !m_leftBar1 )) ? '1' : '0';
    result.append( QString(widthLengths.at(i), barDigit) );
  }
  return result;
}


QString WidthModulationEncoder::operator()(Symbol& s)
{ 
  return  encodeBarModulation( decodeWidth(s) );  
}





// --- helper functions ---

int CommonChecksumOddEven(int checksumModulus, const QList<Symbol>& symbols, int oddMultipler, int evenMultipler, bool reverse)
{
  qDebug("CommonChecksumOddEven() : start");
  int l_accum = 0;
  if (symbols.isEmpty() || (oddMultipler > 1 && evenMultipler > 1)) {
    return l_accum;
  }  
  EvenOddChecksum l_eoChecksum(oddMultipler, evenMultipler);
  
  if (true == reverse) {
    std::reverse_iterator<QList<Symbol>::const_iterator> rend(symbols.begin()) ;
    std::reverse_iterator<QList<Symbol>::const_iterator> rbegin(symbols.end());
    qDebug("CommonChecksumOddEven() : reverse copy");
    l_accum = std::accumulate(rbegin, rend, 0, l_eoChecksum);
  } else {
    qDebug("CommonChecksumOddEven() : copy");
    l_accum = std::accumulate(symbols.begin(), symbols.end(), 0, l_eoChecksum);
  }
  qDebug() << "CommonChecksumOddEven() : total = " << l_accum;
  qDebug("CommonChecksumOddEven() : end");
  
  return NextMultipleCheckDigit(checksumModulus, l_accum); 
}

int CommonChecksumLinear(int checksumModulus, 
			 const QList<Symbol>& symbols,
			 bool reverse)
{
  qDebug("CommonChecksumLinear() : start");
  int total = 0;
  if (symbols.isEmpty()) { return total; }
  
  if (reverse) {
    qDebug("CommonChecksumLinear() : reverse copy");
    std::reverse_iterator<QList<Symbol>::const_iterator> rend(symbols.begin()) ;
    std::reverse_iterator<QList<Symbol>::const_iterator> rbegin(symbols.end());
    total = std::accumulate(rbegin, rend, 0,		
		    LinearMultiple(checksumModulus));
  } else {
    std::accumulate(symbols.begin(), symbols.end(), 0,		
		    LinearMultiple(checksumModulus));
  }
  qDebug("CommonChecksumLinear() : end");
  return  (total != 0) ? (checksumModulus % total) : checksumModulus;
}

int NextMultipleCheckDigit(int modulusValue, int checksum)
{
  qDebug("NextMultipleCheckDigit()");
  int checkValue = 0;
  // invert value = m - (m % x)
  checkValue = (checksum % modulusValue);
  if (checkValue == 0) {
    return checkValue;
  }
  checkValue = modulusValue - checkValue;
  return checkValue;
}

int SimpleRemainderCheckDigit(int checksumModulus, int checksum)
{
  // prevent divide by zero
  if (checksumModulus <= 0)  { return checksum; }
  return checksum % checksumModulus;
}    

}