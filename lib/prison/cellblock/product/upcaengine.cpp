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
#include "upcaengine_p.h"

using namespace product;

UpcAEngine::UpcAEngine() :
	ProductEngine(*new UpcAEnginePrivate)
{
  qDebug("UpcAEngine default constructor");  
  Q_D(UpcAEngine);
  d->setProductCode(upc_common::PS__UPC_A);
  d->m_userInputString = "";
  d->m_constructionFlags = CodeEngine::AutoProduct;
} 		    
  		    
UpcAEngine::UpcAEngine(const QString &userBarcode, 
	CodeEngine::ConstructCodes flags):
	ProductEngine(*new UpcAEnginePrivate)
{
  qDebug("UpcAEngine constructor::string");
  Q_D(UpcAEngine);
  d->setProductCode(upc_common::PS__UPC_A);
  d->m_userInputString = userBarcode;
  d->m_constructionFlags = flags;
}

UpcAEngine::UpcAEngine(const QList< Symbol >& userBarcode, CodeEngine::ConstructCodes flags):
	ProductEngine(*new UpcAEnginePrivate)
{  
  qDebug("UpcAEngine constructor::symbol");
  Q_D(UpcAEngine);
  d->setProductCode(upc_common::PS__UPC_A);
  d->m_userInputString = "";
  d->m_userParsedSymbols = userBarcode;
  d->m_constructionFlags = flags;
}

UpcAEngine::~UpcAEngine()
{
  qDebug("UpcAEngine destructor");
}

const QStringList UpcAEngine::formatedSymbols() const
{
  qDebug("UpcAEngine::formatedSymbols : start"); 
  QStringList formatedString;
  formatedString << numberSystem();
  formatedString << block1().join("");
  formatedString << block2().join("");
  formatedString << checkDigit();
  formatedString << extendedBlock().join("");
  qDebug() << "UpcAEngine::formatedSymbols : " << formatedString;
  qDebug("UpcAEngine::formatedSymbols : end");
  return formatedString;
}

const QList<QStringList> UpcAEngine::encoded() const
{  
  QList<QStringList> encodedBlocks(encodeMainBlock(local_mainBlock()));
  encodedBlocks += encodeExtendedBlock(local_extendedBlock());
  return encodedBlocks;
}

QList<QStringList> UpcAEngine::encodeMainBlock(const QList< Symbol >& mainBlock) const
{
  Q_D(const UpcAEngine);
  qDebug("UpcAEngine encodeMainBlock() : start");    
  int encBlockSize = upcA::ENCODE_BLOCK_SIZE;
  QList<Symbol> l_mainBlock(local_mainBlock());

  QList<QStringList> encodeMainBlock;
  if (l_mainBlock.isEmpty()) {
    QStringList e_block;
    for (int count = 0; count < encBlockSize; count++) {  
      e_block.append(d->m_symbology.data()->errorEncoding());
    }
    encodeMainBlock.append(e_block);
    encodeMainBlock.append(e_block);
  } else {
    QList<Symbol> l_block1 = l_mainBlock.mid(0, encBlockSize);
    QList<Symbol> l_block2 = l_mainBlock.mid(encBlockSize, encBlockSize);
    // "O" and "R"
    QString patternOs = QString(encBlockSize, 'O');
    QString patternRs = QString(encBlockSize, 'R');

    encodeMainBlock.append( d->encodeSymbolParity(l_block1, patternOs) );
    encodeMainBlock.append( d->encodeSymbolParity(l_block2, patternRs) );
    qDebug() << "UpcAEngine::encodeMainBlock() : l_block1" << encodeMainBlock.at(0);
    qDebug() << "UpcAEngine::encodeMainBlock() : l_block2" << encodeMainBlock.at(1);
  }
  qDebug("UpcAEngine encodeMainBlock() : end");  
  return encodeMainBlock;
}

QList< Symbol > UpcAEngine::compressUpc(const QList< Symbol >& inputSymbolList) const
{
  Q_D(const UpcAEngine);
  qDebug("UpcAEngine toUpcE() : start");
  qDebug() << "UpcAEngine toUpcE() : input " << inputSymbolList;
  qDebug() << "UpcAEngine toUpcE() : input size " << inputSymbolList.size();
  Q_ASSERT(inputSymbolList.size() >= upcA::MIN && 
    inputSymbolList.size() <= upcA::MAX);
  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
  
  QList<Symbol> localUpcE;
  // strip off number system and check digit symbols
  
  // -- compress to a UPC-E if possible --
  QString symbolString(toStringList(inputSymbolList).join("")); 
  QRegExp upceMatch("([0|1]\\d{1,5})0{4,5}(\\d{1,3})\\d)");
  if ( upceMatch.exactMatch(symbolString) ) {    
    QStringList captured(upceMatch.capturedTexts());
    
    QList<Symbol> manf = d->m_emptySymbol.parse(captured.at(0));
    QList<Symbol> product = d->m_emptySymbol.parse(captured.at(1));
    qDebug() << "compressUpc : manufactor code: " << manf;   
    qDebug() << "compressUpc : product Code: " << product;   
    
    if (manf.size() == 3 && product.size() == 3 && manf.back() < 3) {
      qDebug("compressUpc : 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]");
    localUpcE << manf.mid(0,2); 
    localUpcE << product << manf.back();	  
      } else if (manf.size() == 4 && product.size() == 2 && manf.back() == 3) {
	qDebug("compressUpc : 2. manf ddd300, prod code 000dd -> mmmpp3"); 
	localUpcE << manf.mid(0,3); 
	localUpcE << product << manf.back();	  
      } else if (manf.size() == 4 && product.size() == 1 && product.back() < 5) {
	qDebug("compressUpc : 3. manf dddd0, prod code 0000d -> mmmmp4");
	localUpcE << manf; 
	localUpcE << product << "4";	  
      } else if (manf.size() == 5 && product.size() == 1) {
	qDebug("compressUpc : 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9]");
	localUpcE << manf; 
	localUpcE << product;	  
      } else {
	qDebug("compressUpc : unknown compression method");
	localUpcE.clear();
      }    
      
      qDebug() << "compressUpc :" << manf << " " << product << " -> " << localUpcE;
      qDebug() << "compressUpc : to UPC-E: " <<  localUpcE;    
      Q_ASSERT(localUpcE.isEmpty() ||
      localUpcE.size() >= upcE::MIN && localUpcE.size() <= upcE::MAX);
      qDebug("compressUpc  : end");
  }
  return localUpcE;
}


QList< Symbol > UpcAEngine::expandUpc(const QList< Symbol >& inputSymbolList) const 
{
  qDebug("UpcEEngine expandUpc() : start");
  Q_ASSERT(inputSymbolList.size() >= upcE::MIN && inputSymbolList.size() <= upcE::MAX);
  qDebug() << "UpcEEngine expandUpc() : input " << inputSymbolList;
  qDebug() << "UpcEEngine expandUpc() : input size " << inputSymbolList.size();
  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
  
  // -- validate by expanding -> UPC-A --
  QList<Symbol> expandedUpcA; 
  
  Symbol compressionSymbol = inputSymbolList.at(upcE::COMPRESS_METHOD_INDEX);
  int compressionMethod = compressionSymbol;
  qDebug() << "UpcAEngine expandUpc() : Compression Symbol: " << compressionSymbol;
  qDebug() << "UpcAEngine expandUpc() : Compression method: " << compressionMethod;

  expandedUpcA << inputSymbolList.at(upc_common::NUMBER_SYSTEM_INDEX);
  QList<Symbol> compressedUpc(inputSymbolList.mid(1, 6));
  qDebug() << "UpcAEngine expandUpc() : compressedUpc: " << compressedUpc;
  QList<Symbol> manf;
  QList<Symbol> product;
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

