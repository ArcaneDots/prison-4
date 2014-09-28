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
#include "abstractbarcodeengine.h"

//using namespace barcodeEngine;

int barcodeEngine::CommonChecksumOddEven(int checksumModulus, const barcodeEngine::SymbolList& symbols, int oddMultipler, int evenMultipler, bool reverse)
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

int barcodeEngine::CommonChecksumLinear(int checksumModulus, const barcodeEngine::SymbolList& symbols, bool reverse)
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

int barcodeEngine::NextMultipleCheckDigit(int modulusValue, int checksum)
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
using namespace barcodeEngine;

// -------------- AbstractBarcodeEngine --------------

AbstractBarcodeEngine::AbstractBarcodeEngine() : 
  d_ptr(new AbstractBarcodeEnginePrivate())
//  d_ptr(new AbstractBarcodeEnginePrivate(this))
{
  qDebug("AbstractBarcodeEngine constructor");
}

AbstractBarcodeEngine::AbstractBarcodeEngine(AbstractBarcodeEnginePrivate &d):
  d_ptr(&d)
{

}

AbstractBarcodeEngine::~AbstractBarcodeEngine()
{
  qDebug("AbstractBarcodeEngine destructor");
}

QString AbstractBarcodeEngine::userInput() const
{
  Q_D(const AbstractBarcodeEngine);
  return d->m_userInputString;
}

const CodeEngine::ErrorCodes& AbstractBarcodeEngine::statusFlags() const
{
  Q_D(const AbstractBarcodeEngine);
  return d->m_isValid;
}   

const CodeEngine::ErrorCodes& AbstractBarcodeEngine::addFlags(CodeEngine::ErrorCode flags)
{
  Q_D(const AbstractBarcodeEngine);
  return d->m_isValid |= flags;
}

const CodeEngine::ErrorCodes& AbstractBarcodeEngine::removeFlags(CodeEngine::ErrorCode flags)
{
  Q_D(const AbstractBarcodeEngine);
  return d->m_isValid ^= flags;
}

CodeEngine::ConstructCodes AbstractBarcodeEngine::constructionFlags() const
{
  Q_D(const AbstractBarcodeEngine);
  return d->m_constructionFlags;
}