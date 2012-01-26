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

#include <numeric>
#include "upcaengine.h"

using namespace product;

UpcAEngine::UpcAEngine(const QString& defaultString, 
		       int minLength, 
		       int maxLength, 
		       int checkDigitOffset, 
		       int blockSize, 
		       upc_common::PRODUCT_CODE_VALUES productCode): 
		       ProductEngine(defaultString, 
				  minLength, 
				  maxLength, 
				  checkDigitOffset, 
				  blockSize,
				  upc_common::ENCODE_BLOCK_SIZE,
				  productCode)
{
  qDebug("UpcAEngine constructor");
}

UpcAEngine::~UpcAEngine()
{
  qDebug("UpcAEngine destructor");
}

void UpcAEngine::setBarcodeString(const QString& userBarcode, codeEngine::ConstructCodes flags)
{
    product::ProductEngine::setBarcodeString(userBarcode, flags);
}

void UpcAEngine::setBarcodeString(ProductEngine* ptrProductEngine)
{
  qDebug("UpcAEngine setBarcodeString(ptrProductEngine) : start");  
  if (ptrProductEngine == 0 || 
    !ptrProductEngine->getStatusFlags().testFlag(codeEngine::OK)) {
  qDebug("UpcAEngine setBarcodeString(ptrProductEngine) : bad source");  
    m_isValid = codeEngine::UnknownError;
    return;
  }
  
  initialize();
  QStringList userSymbols;
  switch(getProductCode()) {
    case upc_common::PS__EAN_13:
      userSymbols = ((UpcAEngine *)ptrProductEngine)->toEan13();
      break;      
    case upc_common::PS__UPC_A:
      userSymbols = ((UpcAEngine *)ptrProductEngine)->toUpcA();
      break;      
    case upc_common::PS__UPC_E:
      userSymbols = ((UpcAEngine *)ptrProductEngine)->toUpcE();
      break;
    default:
      qDebug("UpcAEngine setBarcodeString(ptrProductEngine) : unexpected class");  
      break;
  }
  
  // reset members
  m_isValid = codeEngine::OK; 
  m_formatedSymbols.clear();
  m_encodedSymbols.clear();
  
  // -- assume input is valid --
  if (!userSymbols.isEmpty()) {
    m_userSymbols = userSymbols;
    m_userInputString = m_userSymbols.join("");
    this->formatSymbols(userSymbols);  
    this->encodeSymbols(userSymbols);
  }
  qDebug("UpcAEngine setBarcodeString(ptrProductEngine) : end");    
}

QStringList UpcAEngine::toEan13()
{
  qDebug("UpcAEngine toEan13");
  QStringList localEan13;
  localEan13 << "0" << m_userSymbols;
  return localEan13;
}

QStringList UpcAEngine::toUpcA()
{
  qDebug("UpcAEngine toUpcA");
  return m_userSymbols;
}

QStringList UpcAEngine::toUpcE()
{
  qDebug("UpcAEngine toUpcE");
  return compressUpc(m_userSymbols);
}

QStringList UpcAEngine::compressUpc(const QStringList& symbolList) const
{
  qDebug("UpcAEngine toUpcE() : start");
  qDebug() << "UpcAEngine toUpcE() : input " << symbolList;
  qDebug() << "UpcAEngine toUpcE() : input size " << symbolList.size();
  Q_ASSERT(symbolList.size() >= upcA::MIN && symbolList.size() <= upcA::MAX);
  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
  
  QStringList localUpcE;
  // strip off number system and check digit symbols
  
  // -- compress to a UPC-E if possible --
  QRegExp upceMatch("[0|1](\\d{1,5})0{4,5}(\\d{1,3})\\d)");
  if (m_isValid.testFlag(codeEngine::OK) &&
    upceMatch.exactMatch(symbolList.join(""))) {    
    QStringList manf(upceMatch.capturedTexts().at(0));
    QStringList product(upceMatch.capturedTexts().at(1));
    qDebug() << "UpcAEngine toUpcE() : manufactor code: " << manf;   
    qDebug() << "UpcAEngine toUpcE() : product Code: " << product;   
  
    if (manf.size() == 3 && product.size() == 3 && getSymbolIndex(manf.back()) < 3) {
    qDebug("UpcAEngine toUpcE(): 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]");
	localUpcE << manf.mid(0,2); 
	localUpcE << product << manf.back();	  
    } else if (manf.size() == 4 && product.size() == 2 && getSymbolIndex(manf.back()) == 3) {
      qDebug("UpcAEngine toUpcE(): 2. manf ddd300, prod code 000dd -> mmmpp3"); 
	localUpcE << manf.mid(0,3); 
	localUpcE << product << manf.back();	  
    } else if (manf.size() == 4 && product.size() == 1 && getSymbolIndex(product.back()) < 5) {
    qDebug("UpcAEngine toUpcE(): 3. manf dddd0, prod code 0000d -> mmmmp4");
	localUpcE << manf; 
	localUpcE << product << "4";	  
    } else if (manf.size() == 5 && product.size() == 1) {
      qDebug("UpcAEngine toUpcE(): 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9]");
	localUpcE << manf; 
	localUpcE << product;	  
    } else {
      qDebug("UpcAEngine toUpcE() : unknown compression method");
      localUpcE.clear();
    }    
   
    qDebug() << "UpcAEngine toUpcE():" << manf << " " << product << " -> " << localUpcE;
    qDebug() << "UpcAEngine toUpcE(): to UPC-E: " <<  localUpcE;    
    Q_ASSERT(localUpcE.isEmpty() ||
      localUpcE.size() >= upcE::MIN && localUpcE.size() <= upcE::MAX);
    qDebug("UpcAEngine expandUpc() : end");
  }
  return localUpcE;
}

QStringList UpcAEngine::expandUpc(const QStringList& symbolList) const 
{
  qDebug("UpcEEngine expandUpc() : start");
  Q_ASSERT(symbolList.size() >= upcE::MIN && symbolList.size() <= upcE::MAX);
  qDebug() << "UpcEEngine expandUpc() : input " << symbolList;
  qDebug() << "UpcEEngine expandUpc() : input size " << symbolList.size();
  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
  
  // -- validate by expanding -> UPC-A --
  QStringList expandedUpcA; 
  
  QString compressionSymbol = symbolList.at(upcE::COMPRESS_METHOD_INDEX);
  int compressionMethod = getSymbolIndex(compressionSymbol);
  qDebug() << "UpcEEngine expandUpc() : Compression Symbol: " << compressionSymbol;
  qDebug() << "UpcEEngine expandUpc() : Compression method: " << compressionMethod;

  expandedUpcA << symbolList.at(upc_common::NUMBER_SYSTEM_INDEX);
  QStringList compressedUpc(symbolList.mid(1, 6));
  qDebug() << "UpcEEngine expandUpc() : compressedUpc: " << compressedUpc;
  QStringList manf;
  QStringList product;
  switch (compressionMethod) {
    case 0:
    case 1:
    case 2:
      qDebug("UpcEEngine expandUpc(): 1. mmppp[0-2] -> manf dd[0-2]00, prod code 00ddd");
      manf << compressedUpc.mid(0, 2) << compressionSymbol << "0" << "0";
      product << "0" << "0"<< compressedUpc.mid(2,3);
      break;
    case 3:
      qDebug("UpcEEngine expandUpc(): 2. mmmpp3 -> manf ddd30, prod code 000dd"); 
      manf << compressedUpc.mid(0, 3) << compressionSymbol << "0";
      product << "0" << "0" << "0" << compressedUpc.mid(3, 2);
      break;
    case 4:
      qDebug("UpcEEngine expandUpc(): 3. mmmmp4 -> manf dddd0, prod code 0000d");
      manf << compressedUpc.mid(0, 4) << "0";
      product << "0" << "0" << "0" << "0"<< compressedUpc.mid(4,1);
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      qDebug("UpcEEngine expandUpc(): 4. mmmmm[5-9] -> manf ddddd, prod code 0000[5-9]"); 
      manf << compressedUpc.mid(0, 5);
      product << "0" << "0" << "0" << "0"<< compressionSymbol;
      break;
    default:
      // should never reach this case; UPC-E -> UPC-A should alway be sucessful 
      qDebug("UpcEEngine expandUpc() : unknown compression method");
      break;
  }
    
  expandedUpcA << manf; 
  expandedUpcA << product; 
  qDebug() << "UpcEEngine expandUpc(): manf: " <<  manf;
  qDebug() << "UpcEEngine expandUpc(): product: " <<  product;  
  qDebug() << "UpcEEngine expandUpc(): expended E -> A: " <<  expandedUpcA;
  Q_ASSERT(expandedUpcA.size() >= upcA::MIN && expandedUpcA.size() <= upcA::MAX);
  qDebug("UpcEEngine expandUpc() : end");
  return expandedUpcA;
}








