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

void UpcAEngine::setBarcodeString(const QString& userBarcode, engineFlags::ConstructCodes flags)
{
  product::ProductEngine::setBarcodeString(userBarcode, flags);
}

void UpcAEngine::setBarcodeString(ProductEngine* ptrProductEngine)
{
  qDebug("UpcAEngine setBarcodeString(ptrProductEngine) : start");  
  if (ptrProductEngine == 0 || 
    !ptrProductEngine->getStatusFlags().testFlag(engineFlags::OK)) {
  qDebug("UpcAEngine setBarcodeString(ptrProductEngine) : bad source");  
    m_isValid = engineFlags::UnknownError;
    return;
  }
  
  //initialize();
  SymbolList userSymbols(getBarcodeSetInfo());
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
  m_isValid = engineFlags::OK; 
  m_formatedSymbols.clear();
  m_encodedSymbols.clear();
  
  // -- assume input is valid --
  if (!userSymbols.empty()) {
    m_userSymbols = userSymbols;
    m_userInputString = m_userSymbols.toQStringList().join("");
    this->formatSymbols(userSymbols);  
    this->encodeSymbols(userSymbols);
  }
  qDebug("UpcAEngine setBarcodeString(ptrProductEngine) : end");    
}

SymbolList UpcAEngine::toEan13()
{
  qDebug("UpcAEngine toEan13");
  SymbolList localEan13(m_userSymbols);
  localEan13.push_front("0");
  //localEan13 << "0" << m_userSymbols;
  return localEan13;
}

SymbolList UpcAEngine::toUpcA()
{
  qDebug("UpcAEngine toUpcA");
  return m_userSymbols;
}

SymbolList UpcAEngine::toUpcE()
{
  qDebug("UpcAEngine toUpcE");
  return compressUpc(m_userSymbols);
}

SymbolList UpcAEngine::compressUpc(const SymbolList& upcASymbols) const
{
  qDebug("UpcAEngine toUpcE() : start");
  qDebug() << "UpcAEngine toUpcE() : input " << upcASymbols;
  qDebug() << "UpcAEngine toUpcE() : input size " << upcASymbols.size();
  Q_ASSERT(upcASymbols.size() >= upcA::MIN && upcASymbols.size() <= upcA::MAX);
  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
  
  SymbolList localUpcE(upcASymbols.clone());
  SymbolList nonZeroCode(upcASymbols.clone());
  //localUpcE.clear();
  // strip off number system and check digit symbols
  
  // -- compress to a UPC-E if possible --
  QRegExp upceMatch("[0|1](\\d{1,5})0{4,5}(\\d{1,3})\\d)");
  if (m_isValid.testFlag(engineFlags::OK) &&
    upceMatch.exactMatch(upcASymbols.toQStringList().join(""))) {   
    nonZeroCode.manf() = upceMatch.capturedTexts().at(0);
    nonZeroCode.product() = upceMatch.capturedTexts().at(1);
//     QString manf(upceMatch.capturedTexts().at(0));
//     QString product(upceMatch.capturedTexts().at(1));
    qDebug() << "UpcAEngine toUpcE() : manufactor code: " << nonZeroCode.manf();   
    qDebug() << "UpcAEngine toUpcE() : product Code: " << nonZeroCode.product();   
  
    if (nonZeroCode.manf().size() == 3 && 
      nonZeroCode.product().size() == 3 && 
      nonZeroCode.manf().back().getIndex() < 3) {
    qDebug("UpcAEngine toUpcE(): 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]");
	localUpcE += nonZeroCode.manf().mid(0,2); 
	localUpcE += nonZeroCode.product() + nonZeroCode.manf().back();	  
    } else if (nonZeroCode.manf().size() == 4 && 
      nonZeroCode.product().size() == 2 && 
      nonZeroCode.manf().back().getIndex() == 3) {
      qDebug("UpcAEngine toUpcE(): 2. manf ddd300, prod code 000dd -> mmmpp3"); 
	localUpcE << nonZeroCode.manf().mid(0,3); 
	localUpcE << nonZeroCode.product() + nonZeroCode.manf().back();	  
    } else if (nonZeroCode.manf().size() == 4 && 
      nonZeroCode.product().size() == 1 && 
      nonZeroCode.product().back().getIndex() < 5) {
    qDebug("UpcAEngine toUpcE(): 3. manf dddd0, prod code 0000d -> mmmmp4");
	localUpcE << nonZeroCode.manf(); 
	localUpcE << nonZeroCode.product() + "4";	  
    } else if (nonZeroCode.manf().size() == 5 && 
      nonZeroCode.product().size() == 1) {
      qDebug("UpcAEngine toUpcE(): 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9]");
	localUpcE << nonZeroCode.manf(); 
	localUpcE << nonZeroCode.product();	  
    } else {
      qDebug("UpcAEngine toUpcE() : unknown compression method");
      localUpcE.clear();
    }    
   
    qDebug() << "UpcAEngine toUpcE():" << nonZeroCode.manf() << " " << nonZeroCode.product() << " -> " << localUpcE;
    qDebug() << "UpcAEngine toUpcE(): to UPC-E: " <<  localUpcE;    
    Q_ASSERT(localUpcE.empty() ||
      localUpcE.size() >= upcE::MIN && localUpcE.size() <= upcE::MAX);
    qDebug("UpcAEngine expandUpc() : end");
  }
  return localUpcE;
}

SymbolList UpcAEngine::expandUpc(const SymbolList& upcESymbols) const 
{
  qDebug("UpcEEngine expandUpc() : start");
  Q_ASSERT(upcESymbols.size() >= upcE::MIN && upcESymbols.size() <= upcE::MAX);
  qDebug() << "UpcEEngine expandUpc() : input " << upcESymbols;
  qDebug() << "UpcEEngine expandUpc() : input size " << upcESymbols.size();
  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
  
  // -- validate by expanding -> UPC-A --
  SymbolList expandedUpcA(upcESymbols);
  expandedUpcA.clear();
  
  Symbol compressionSymbol = upcESymbols.at(upcE::COMPRESS_METHOD_INDEX);
  int compressionMethod = compressionSymbol.getIndex();
  qDebug() << "UpcEEngine expandUpc() : Compression Symbol: " << compressionSymbol;
  qDebug() << "UpcEEngine expandUpc() : Compression method: " << compressionMethod;

  expandedUpcA << upcESymbols.at(upc_common::NUMBER_SYSTEM_INDEX);
  SymbolList compressedUpc(upcESymbols.mid(1, 6));
  qDebug() << "UpcEEngine expandUpc() : compressedUpc: " << compressedUpc;
  SymbolList manf(compressedUpc.clone());
  SymbolList product(compressedUpc.clone());
  switch (compressionMethod) {
    case 0:
    case 1:
    case 2:
      qDebug("UpcEEngine expandUpc(): 1. mmppp[0-2] -> manf dd[0-2]00, prod code 00ddd");
      manf += compressedUpc.mid(0, 2) + compressionSymbol + "0" + "0";
      product += "00" + compressedUpc.mid(2,3);
      break;
    case 3:
      qDebug("UpcEEngine expandUpc(): 2. mmmpp3 -> manf ddd30, prod code 000dd"); 
      manf << compressedUpc.mid(0, 3) + compressionSymbol + "0";
      product += "000" + compressedUpc.mid(3, 2);
      break;
    case 4:
      qDebug("UpcEEngine expandUpc(): 3. mmmmp4 -> manf dddd0, prod code 0000d");
      manf += compressedUpc.mid(0, 4) + "0";
      product += "0000" + compressedUpc.mid(4,1);
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      qDebug("UpcEEngine expandUpc(): 4. mmmmm[5-9] -> manf ddddd, prod code 0000[5-9]"); 
      manf << compressedUpc.mid(0, 5);
      product += "0000" + compressionSymbol;
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








