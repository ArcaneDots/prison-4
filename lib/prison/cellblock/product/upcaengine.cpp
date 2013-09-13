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

#include <numeric>
#include "upcaengine.h"

using namespace product;

UpcAEngine::UpcAEngine() :
	ProductEngine("",
		CodeEngine::AutoProduct,
		upcA::DEFAULT_VALUE, 
		upcA::MIN, 
		upcA::MAX_LEN, 
		upcA::CHECK_DIGIT_OFFSET, 
		upcA::BLOCK_SIZE,
		upc_common::ENCODE_BLOCK_SIZE,
		upc_common::PS__UPC_A)
{
  qDebug("UpcAEngine default constructor");    
  AbstractBarcodeEngine::setBarcodeString();
}
   
		      
UpcAEngine::UpcAEngine(const QString &userBarcode, 
		       CodeEngine::ConstructCodes flags,
		       const QString& defaultString, 
		       int minLength, 
		       int maxLength, 
		       int checkDigitOffset, 
		       int blockSize, 
		       upc_common::PRODUCT_CODE_VALUES productCode): 
		       ProductEngine(userBarcode,
				  flags,
				  defaultString, 
				  minLength, 
				  maxLength, 
				  checkDigitOffset, 
				  blockSize,
				  upc_common::ENCODE_BLOCK_SIZE,
				  productCode)
{
  qDebug("UpcAEngine constructor");  
  if (m_productCode == upc_common::PS__UPC_A) {
    setBarcodeString();
  }  
}



UpcAEngine::~UpcAEngine()
{
  qDebug("UpcAEngine destructor");
}

QString UpcAEngine::userInput() const
{
   return product::ProductEngine::userInput();
}

void UpcAEngine::setBarcodeString()
{
  ProductEngine::setBarcodeString();
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
  if (m_isValid.testFlag(CodeEngine::OK) &&
    upceMatch.exactMatch(symbolList.join(""))) {    
    QStringList manf(upceMatch.capturedTexts().at(0));
    QStringList product(upceMatch.capturedTexts().at(1));
    qDebug() << "UpcAEngine toUpcE() : manufactor code: " << manf;   
    qDebug() << "UpcAEngine toUpcE() : product Code: " << product;   
  
    if (manf.size() == 3 && product.size() == 3 && lookupSymbolIndex(manf.back()) < 3) {
    qDebug("UpcAEngine toUpcE(): 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]");
	localUpcE << manf.mid(0,2); 
	localUpcE << product << manf.back();	  
    } else if (manf.size() == 4 && product.size() == 2 && lookupSymbolIndex(manf.back()) == 3) {
      qDebug("UpcAEngine toUpcE(): 2. manf ddd300, prod code 000dd -> mmmpp3"); 
	localUpcE << manf.mid(0,3); 
	localUpcE << product << manf.back();	  
    } else if (manf.size() == 4 && product.size() == 1 && lookupSymbolIndex(product.back()) < 5) {
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
  int compressionMethod = lookupSymbolIndex(compressionSymbol);
  qDebug() << "UpcAEngine expandUpc() : Compression Symbol: " << compressionSymbol;
  qDebug() << "UpcAEngine expandUpc() : Compression method: " << compressionMethod;

  expandedUpcA << symbolList.at(upc_common::NUMBER_SYSTEM_INDEX);
  QStringList compressedUpc(symbolList.mid(1, 6));
  qDebug() << "UpcAEngine expandUpc() : compressedUpc: " << compressedUpc;
  QStringList manf;
  QStringList product;
  switch (compressionMethod) {
    case 0:
    case 1:
    case 2:
      qDebug("UpcAEngine expandUpc(): 1. mmppp[0-2] -> manf dd[0-2]00, prod code 00ddd");
      manf << compressedUpc.mid(0, 2) << compressionSymbol << "0" << "0";
      product << "0" << "0"<< compressedUpc.mid(2,3);
      break;
    case 3:
      qDebug("UpcAEngine expandUpc(): 2. mmmpp3 -> manf ddd30, prod code 000dd"); 
      manf << compressedUpc.mid(0, 3) << compressionSymbol << "0";
      product << "0" << "0" << "0" << compressedUpc.mid(3, 2);
      break;
    case 4:
      qDebug("UpcAEngine expandUpc(): 3. mmmmp4 -> manf dddd0, prod code 0000d");
      manf << compressedUpc.mid(0, 4) << "0";
      product << "0" << "0" << "0" << "0"<< compressedUpc.mid(4,1);
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      qDebug("UpcAEngine expandUpc(): 4. mmmmm[5-9] -> manf ddddd, prod code 0000[5-9]"); 
      manf << compressedUpc.mid(0, 5);
      product << "0" << "0" << "0" << "0"<< compressionSymbol;
      break;
    default:
      // should never reach this case; UPC-E -> UPC-A should alway be sucessful 
      qDebug("UpcAEngine expandUpc() : unknown compression method");
      break;
  }
    
  expandedUpcA << manf; 
  expandedUpcA << product; 
  qDebug() << "UpcAEngine expandUpc(): manf: " <<  manf;
  qDebug() << "UpcAEngine expandUpc(): product: " <<  product;  
  qDebug() << "UpcAEngine expandUpc(): expended E -> A: " <<  expandedUpcA;
  Q_ASSERT(expandedUpcA.size() >= upcA::MIN && expandedUpcA.size() <= upcA::MAX);
  qDebug("UpcAEngine expandUpc() : end");
  return expandedUpcA;
}
