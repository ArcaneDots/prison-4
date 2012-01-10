/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

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
  Q_ASSERT(getProductCode() == upc_common::PS__EAN_8 && 
    ptrProductEngine->getProductCode() == getProductCode());
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
