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
#include "ean13engine.h"
#include "upceengine.h"

using namespace product;

UpcAEngine::UpcAEngine() :
	ProductEngine("",
		CodeEngine::AutoProduct,
		upc_common::PS__UPC_A)
{
  qDebug("UpcAEngine default constructor");  
  localInitialize();
} 		    
  		    
UpcAEngine::UpcAEngine(const QString &userBarcode, 
	CodeEngine::ConstructCodes flags,
	upc_common::PRODUCT_CODE_VALUES productCode): 
	ProductEngine(userBarcode,
 		  flags,
		  productCode)
{
  qDebug("UpcAEngine::string constructor : start");  
  localInitialize();
}

UpcAEngine::UpcAEngine(const SymbolList& userBarcode, CodeEngine::ConstructCodes flags, 
upc_common::PRODUCT_CODE_VALUES productCode): 
		       ProductEngine(toStringList(userBarcode).join(""), 
				     flags, productCode)
{
  
  qDebug("UpcAEngine::symbol constructor : start"); 
  localInitialize();
}


// UpcAEngine::UpcAEngine(const UpcEEngine& existingUpcE) : 
//   ProductEngine(existingUpcE.expanded())
// {	
//   qDebug("UpcAEngine \"UpcEEngine\" constructor");
//   swap(existingUpcE.toUpcA()); 
//   localInitialize();
// }
// 
// UpcAEngine::UpcAEngine(const Ean13Engine& existingEan13): 
//     ProductEngine(existingEan13.finalSymbolList().join(","),
// 		  CodeEngine::AutoProduct,
// 		  upc_common::PS__UPC_A)
// {
//   const QStringList ean13DefaultSymbols(existingEan13.codeDefault());
//   QStringList copiedSymbols(existingEan13.finalSymbolList());
//   if ((ean13DefaultSymbols == copiedSymbols) || 
//     (copiedSymbols.at(0) != "0") )
//   {
//     m_isValid |= CodeEngine::MissingInput;
//     copiedSymbols = codeDefault();
//   } else {
//     copiedSymbols.pop_front();
//   }
//   UpcAEngine tempA = UpcAEngine(copiedSymbols.join(""));
//   swap(tempA);
//   localInitialize();
// }

UpcAEngine::~UpcAEngine()
{
  qDebug("UpcAEngine destructor");
}

const QStringList UpcAEngine::formatedSymbols() const
{
  QStringList formatedString;
  formatedString << numberSystem();
  formatedString << block1().join("");
  formatedString << block2().join("");
  formatedString << checkDigit();
  return formatedString;
}

const QStringList UpcAEngine::encoded() const
{  
  SymbolList l_symbols = symbols();
  QStringList encodedBlocks(encodeMainBlock(l_symbols));
  encodedBlocks += encodeExtendedBlock(local_extendedBlock()).join("");
  return encodedBlocks;
}

void UpcAEngine::localInitialize()
{
  qDebug("UpcAEngine::localInitialize() - start");
  m_finalSymbolList = processSymbolList(m_userParsedSymbols);
  Symbol calculatedCheckDigit = CalculateCheckDigit();
  m_isValid = validateCheckDigit(checkDigit(), calculatedCheckDigit);
  setCheckDigit(calculatedCheckDigit);
  populateSections();
  qDebug("UpcAEngine::localInitialize() - end");
}

void UpcAEngine::populateSections()
{  
  qDebug() << "MAIN_BLOCK "<< mainBlock(); 
  qDebug() << "EXTENDED_BLOCK "<< extendedBlock();
  
  qDebug() << "MAIN_SYSTEM " << systemDigit();  
  qDebug() << "MAIN_BLOCK_1 "<< block1();
  qDebug() << "MAIN_BLOCK_2 "<< block2();    
  qDebug() << "MAIN_CHECK_DIGIT "<< checkDigit();
}

void UpcAEngine::swap(const UpcAEngine& other)
{
  qDebug("UpcAEngine::swap - start");
  using std::swap;
  
  // FIXME
  this->swap(other);
  qDebug("UpcAEngine::swap - end");
}


QStringList UpcAEngine::encodeMainBlock(const SymbolList& mainBlock) const
{
  qDebug("UpcAEngine encodeSymbols() : start");    
  int blockSize = upcA::ENCODE_BLOCK_SIZE;
  
  SymbolList l_block1 = m_finalSymbolList.mid(0, encBlockSize()); 
  SymbolList l_block2 = m_finalSymbolList.mid(encBlockSize(), encBlockSize());
  // "O" and "R"
  QString patternOs = QString(blockSize, 'O');
  QString patternRs = QString(blockSize, 'R');
  
  QStringList encodeMainBlock;
  encodeMainBlock.reserve(2);
  encodeMainBlock += encodeSymbolParity(l_block1, patternOs).join("");  
  encodeMainBlock += encodeSymbolParity(l_block2, patternRs).join("");
  qDebug("UpcAEngine encodeSymbols() : end");  
  return encodeMainBlock;
}

SymbolList UpcAEngine::compressUpc(const SymbolList& inputSymbolList) const
{
  qDebug("UpcAEngine toUpcE() : start");
  qDebug() << "UpcAEngine toUpcE() : input " << inputSymbolList;
  qDebug() << "UpcAEngine toUpcE() : input size " << inputSymbolList.size();
  Q_ASSERT(inputSymbolList.size() >= upcA::MIN && 
    inputSymbolList.size() <= upcA::MAX);
  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
  
  SymbolList localUpcE;
  // strip off number system and check digit symbols
  
  // -- compress to a UPC-E if possible --
  QString symbolString(toStringList(inputSymbolList).join("")); 
  QRegExp upceMatch("([0|1]\\d{1,5})0{4,5}(\\d{1,3})\\d)");
  if ( upceMatch.exactMatch(symbolString) ) {    
    QStringList captured(upceMatch.capturedTexts());
    
    SymbolList manf = Symbol().parse(captured.at(0));
    SymbolList product = Symbol().parse(captured.at(1));
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



