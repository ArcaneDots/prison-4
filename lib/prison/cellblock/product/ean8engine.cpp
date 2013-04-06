/*
    Copyright (C) 2011  Ian gmail <ianhollander at gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#include <QtDebug>
#include "ean8engine.h"

using namespace product;
using namespace shared;

Ean8Engine::Ean8Engine(const QString& defaultString, 
		       int minLength, 
		       int maxLength, 
		       int checkDigitOffset, 
		       int blockSize,
		       int encodedBlockSize,
		       upc_common::PRODUCT_CODE_VALUES productCode): 	
	 ProductEngine(defaultString,
		 minLength,
		 maxLength,
		 checkDigitOffset,
		 blockSize,
		 encodedBlockSize,
		 productCode)
{
  qDebug("Ean8Engine constructor");
}

Ean8Engine::~Ean8Engine()
{
  qDebug("Ean8Engine destructor");
}

void Ean8Engine::setBarcodeString(const QString& userBarcode, CodeEngine::ConstructCodes flags)
{
    product::ProductEngine::setBarcodeString(userBarcode, flags);
}

void Ean8Engine::setBarcodeString(ProductEngine* ptrProductEngine)
{  
  qDebug("Ean8Engine setBarcodeString(ptrProductEngine) : start");   
  if (ptrProductEngine == 0 || 
    !ptrProductEngine->getStatusFlags().testFlag(CodeEngine::OK)) {
    qDebug("Ean8Engine setBarcodeString(ptrProductEngine) : bad source engine");   
    m_isValid = CodeEngine::UnknownError;
    return;
  }
  Q_ASSERT(productCode() == upc_common::PS__EAN_8 && 
    ptrProductEngine->productCode() == productCode());
  initialize();
  QStringList userSymbols(((Ean8Engine *)ptrProductEngine)->getSymbolList());
  
  // reset members
  m_isValid = CodeEngine::OK; 
  m_formatedSymbols.clear();
  m_encodedSymbols.clear();
  
  // -- assume input is valid --
  m_userSymbols = userSymbols;
  m_userInputString = m_userSymbols.join("");
  this->formatSymbols(userSymbols);  
  this->encodeSymbols(userSymbols);
  qDebug("Ean8Engine setBarcodeString(ptrProductEngine) : end");    
}

QStringList Ean8Engine::getSymbolList() const
{
  return m_userSymbols;
}

// QString Ean8Engine::toEan13()
// {
//   qDebug("Ean8Engine toEan13");
//   return QString();
// }
// QString Ean8Engine::toUpcA()
// {
//   qDebug("Ean8Engine toUpcA");
//   return QString();
// }
// QString Ean8Engine::toUpcE()
// {
//   qDebug("Ean8Engine toUpcE");
//   return QString();
// }
