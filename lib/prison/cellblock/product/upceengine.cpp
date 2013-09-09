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
#include <algorithm>

#include <QRegExp>

#include "upceengine.h"
#include "ean13engine.h"

using namespace product;

UpcEEngine::UpcEEngine() :
	ProductEngine(upcE::DEFAULT_VALUE,
	      CodeEngine::AutoProduct,
	      upc_common::PS__UPC_E),
	m_UpcA(),
	m_fromUpcA(false),
	m_system0EncodingList(),
	m_system1EncodingList()
{
  qDebug("UpcEEngine default constructor");
  localInitialize();
//   m_finalSymbolList = processSymbolList(m_userParsedSymbols);
  populateSections();
}

UpcEEngine::UpcEEngine(const QString& userBarcode, CodeEngine::ConstructCodes flags): 
  ProductEngine(userBarcode, flags, upc_common::PS__UPC_E)
{
    qDebug("UpcEEngine string constructor");
    localInitialize();
//     m_finalSymbolList = processSymbolList(m_userParsedSymbols);
    populateSections();
}


UpcEEngine::UpcEEngine(const QList<Symbol>& userBarcode, CodeEngine::ConstructCodes flags) :
	  ProductEngine(userBarcode, 
			flags, 
		 upc_common::PS__UPC_E),
	m_UpcA(),
	m_fromUpcA(false),
	m_system0EncodingList(),
	m_system1EncodingList()
{
  qDebug("UpcEEngine constructor");
  localInitialize();
  populateSections();
}

UpcEEngine::UpcEEngine(const UpcAEngine& rhUpcA): 
  ProductEngine(toStringList(rhUpcA.compressed()).join(""),
	   CodeEngine::AutoProduct,
	   upc_common::PS__UPC_E),
  m_UpcA(rhUpcA),
  m_fromUpcA(true),
  m_system0EncodingList(),
  m_system1EncodingList()
{  
  localInitialize();
//   m_finalSymbolList = processSymbolList(m_userParsedSymbols);
  populateSections();
}

// UpcEEngine::UpcEEngine(const Ean13Engine& rhsEan13): 
// ProductEngine("",
// 	    CodeEngine::AutoProduct,
// 	    upc_common::PS__UPC_E),
//   m_UpcA(rhsEan13),  
//   m_fromUpcA(true),
//   m_system0EncodingList(),
//   m_system1EncodingList()
// {
//   qDebug("UpcEEngine copy constructor");
//   m_userParsedSymbols = m_UpcA.compressed();
//   localInitialize();
// //   m_finalSymbolList = processSymbolList(m_userParsedSymbols);
//   populateSections();
// }

UpcEEngine::~UpcEEngine()
{
  qDebug("UpcEEngine destructor");
}


UpcEEngine& UpcEEngine::operator=(const UpcEEngine& rhs)
{
  if (this != &rhs) {
    UpcEEngine temp(rhs);
    swap (temp);
  }
  return *this;
}

UpcEEngine& UpcEEngine::operator=(const UpcAEngine& rhs)
{
  if (this->symbols() != rhs.compressed()) {
    UpcEEngine tempE(rhs.compressed());
    swap (tempE);
  }
  return *this;
}

// UpcEEngine& UpcEEngine::operator=(const Ean13Engine& rhs)
// {  
//   UpcEEngine tempEan13(rhs);
//   if (this->symbols() != tempEan13.symbols()) {
//     UpcEEngine temp(rhs);
//     swap (temp);   
//   }
//   return *this;
// }

// const Symbol UpcEEngine::local_numberSystem() const 
// {
//   return m_primaryBlock.at(0);
// }
// const SymbolList UpcEEngine::local_block1() const
// {
//   return m_block1;
// }
// const SymbolList UpcEEngine::local_block2() const 
// {
//   return SymbolList();
// }
// const Symbol UpcEEngine::checkDigit() const
// {
//   return m_UpcA.symbols().at(upca::CHECK_DIGIT_OFFSET);
// }
// const SymbolList UpcEEngine::mainBlock() const
// {
//   return m_primaryBlock;
// }
// const SymbolList UpcEEngine::extendedBlock() const
// {
//   return m_extendedBlock;
// }

const QStringList UpcEEngine::formatedSymbols() const
{
  QStringList formatedString;
  formatedString << numberSystem();
  formatedString << block1().join("");
  formatedString << checkDigit();
  return formatedString;
}

const QStringList UpcEEngine::encoded() const
{  
  QStringList encodedBlocks(encodeMainBlock(m_finalSymbolList));
  encodedBlocks << encodeExtendedBlock(local_extendedBlock()).join("");
}

const UpcAEngine &UpcEEngine::toUpcA() const
{
  qDebug("UpcEEngine toUpcA");
  return m_UpcA;
}

void UpcEEngine::localInitialize()
{
  fillSystemEncodingList();
  
  if (m_fromUpcA = false) {
    m_UpcA = UpcAEngine(expandUpc(m_userParsedSymbols));
  }
  
  SymbolList finalSymbolList = processSymbolList(m_userParsedSymbols);
  Symbol calculatedCheckDigit = CalculateCheckDigit();
  m_isValid = validateCheckDigit(local_checkDigit(), calculatedCheckDigit);
  setCheckDigit(calculatedCheckDigit);
  
  populateSections();
}

void UpcEEngine::populateSections()
{
  
  qDebug() << "MAIN_BLOCK "<< mainBlock(); 
  qDebug() << "EXTENDED_BLOCK "<< extendedBlock();
    
}

Symbol UpcEEngine::CalculateCheckDigit()
{
  const int upcaOffset = upcA::CHECK_DIGIT_OFFSET;
  return m_UpcA.symbols().at(upcaOffset);
}

SymbolList UpcEEngine::processSymbolList(const SymbolList& inputSymbols)
{
  qDebug("UpcEEngine processSymbolList() : start");  
  SymbolList parsed = processSymbolList(inputSymbols);

  
  // UPC-E barcodes must belong to either number systems;
  // RegularUpcCodes_1 ("0") or Reserved_1 ("1")
  Symbol currentNumberSystem = parsed.at(upc_common::NUMBER_SYSTEM_INDEX);
  if ((currentNumberSystem != NAME_SYSTEM0) &&
    (currentNumberSystem != NAME_SYSTEM1)) {
    m_isValid |= CodeEngine::StandardsViolationError;
    return SymbolList();
  }
  if (!m_fromUpcA) {
    m_UpcA = UpcAEngine(expandUpc(parsed));
  }
  const int symbolLength = parsed.size();
  qDebug() << "#symbols: " << symbolLength 
  << "offset: " << checkDigitOffset()
  << "symbols" << parsed;
 
  qDebug("UpcEEngine processSymbolList() : end");
  return parsed;
}


QStringList UpcEEngine::formatMainBlock(const SymbolList& mainBlock) const
{   
  qDebug("UpcEEngine formatMainBlock() : start");  
  Q_ASSERT(productCode() == upc_common::PS__UPC_E);  
  Q_ASSERT(mainBlock.size() == primaryLength());
  
  qDebug() << "UpcEEngine formatMainBlock() : input : " 
	   << mainBlock;
  QStringList formatedSymbols;
  // number system
  formatedSymbols << systemDigit();
  // main block
  formatedSymbols << block1().join("");
  // check digit
  formatedSymbols << checkDigit();
  qDebug("UpcEEngine formatMainBlock() : end");
  return formatedSymbols;
}

// void UpcEEngine::encodeSymbols(const SymbolList &symbolSrc)
// {  
//   qDebug("UpcAEngine encodeSymbols() : start"); 
//   QStringList encodedMainBlocks(encodeMainBlock(symbolSrc.mid(0, primaryLength())));
//   
//   m_encodedSymbols = upc_common::QUIET_ZONE;
//   m_barPositions[shared::BAR_START] = m_encodedSymbols.size();
//   m_encodedSymbols += upc_common::LEFT_HAND_GUARD_BARS;
//   // -- block: 1 --
//   m_barPositions[shared::MAIN_START] = m_encodedSymbols.size();
//   m_barPositions[shared::MAIN_BLOCK_1_START] = m_encodedSymbols.size() + 1;
//   m_encodedSymbols += encodedMainBlocks.at(0);  
//   m_barPositions[shared::MAIN_BLOCK_1_END] = m_encodedSymbols.size() - 1;
//   qDebug() << "block1" << encodedMainBlocks.at(0);
//   m_encodedSymbols += upcE::SPECIAL_GUARD_BARS;
//   m_barPositions[shared::MAIN_END] = m_encodedSymbols.size();
//   // -- block: extended --
//   QStringList encodedExtBlock(encodeExtendedBlock(symbolSrc.mid(primaryLength())));
//   if (!encodedExtBlock.isEmpty()) {
//     qDebug("UpcAEngine encodeSymbols() : encode extended block");  
//     m_encodedSymbols += upc_common::QUIET_ZONE;
//     m_barPositions[shared::EXTENDED_BLOCK_START] = m_encodedSymbols.size() + 1;
//     m_encodedSymbols += upc_common::ADD_ON_GUARD_BARS;
//     m_encodedSymbols += encodedExtBlock.join(upc_common::ADD_ON_SEPERATOR);
//     m_barPositions[shared::EXTENDED_BLOCK_END] = m_encodedSymbols.size();
//   }
//   m_barPositions[shared::BAR_END] = m_encodedSymbols.size();
//   m_encodedSymbols.append(upc_common::QUIET_ZONE);
//   qDebug() << "m_barPositions :" << m_barPositions;    
//   qDebug("UpcAEngine encodeSymbols() : end");  
// }

QStringList UpcEEngine::encodeMainBlock(const SymbolList &mainBlock) const
{  
  qDebug("UpcEEngine encodeMainDigits() : start");
  const int barcodeType = mainBlock.at(upc_common::NUMBER_SYSTEM_INDEX); 
  const int checkDigit = mainBlock.at(upcE::CHECK_DIGIT_OFFSET);
    
  QString blockPattern;  
  if (barcodeType == upc_common::NS__REGULAR_UPC_CODES_1) {
    blockPattern = m_system0EncodingList.at(checkDigit);
  } else {
    blockPattern = m_system1EncodingList.at(checkDigit);
  }
  qDebug() << "UpcEEngine encodeMainDigits() : pattern" << blockPattern;
  
  QStringList workingBlock;
  // --- Encode first block of digits: (first 6 digits)---  
  workingBlock = encodeSymbolParity(mainBlock.mid(0, upcE::CHECK_DIGIT_OFFSET),
				    blockPattern);
  qDebug("UpcEEngine encodeMainDigits() : end");
  return QStringList(workingBlock.join(""));
}

int UpcEEngine::calculateCheckDigit() const
{
  qDebug("UpcEEngine calculateCheckDigit : start");
  Q_ASSERT(productCode() == upc_common::PS__UPC_E);
  SymbolList expandedSymbols(m_UpcA.symbols());
  return expandedSymbols[upcA::CHECK_DIGIT_OFFSET];
}
void UpcEEngine::fillSystemEncodingList()
{
  
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
      m_system0EncodingList.append(upcE::numberSystem0[i]);
      m_system1EncodingList.append(upcE::numberSystem1[i]);
  }  ;
}

SymbolList UpcEEngine::expandUpc(const SymbolList& compressedList) const 
{
  qDebug("expandUpc : start");
  Q_ASSERT(compressedList.size() >= upcE::MIN && 
    compressedList.size() <= upcE::MAX);
  qDebug() << "expandUpc : input " << compressedList;
  qDebug() << "expandUpc : input size " << compressedList.size();  
  // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
  // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
  // 3. manf dddd0, prod code 0000d 	-> mmmmp4
  // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9]   
  // -- validate by expanding -> UPC-A --  
  Symbol compressionSymbol = compressedList.at(upcE::COMPRESS_METHOD_INDEX);
  qDebug() << "expandUpc : Compression Symbol: " << compressionSymbol;
  qDebug() << "expandUpc : Compression method: " << (int)compressionSymbol;  
  SymbolList compressedUpc(compressedList.mid(1, 6));
  qDebug() << "expandUpc : compressedUpc: " << compressedUpc;
//   int compressionMethod = m_Symbols->lookupSymbolIndex(compressionSymbol);
//   qDebug() << "expandUpc : Compression Symbol: " << compressionSymbol;
//   qDebug() << "expandUpc : Compression method: " << compressionMethod;  
//   QStringList compressedUpc(symbolList.mid(1, 6));
//   qDebug() << "expandUpc : compressedUpc: " << compressedUpc;
  SymbolList manf;
  SymbolList product;
  switch ((int)compressionSymbol) {
    case 0:
    case 1:
    case 2:
      qDebug("expandUpc : 1. mmppp[0-2] -> manf dd[0-2]00, prod code 00ddd");
      manf << compressedUpc.mid(0, 2) << compressionSymbol << "0" << "0";
      product << "0" << "0"<< compressedUpc.mid(2,3);
      break;
    case 3:
      qDebug("expandUpc : 2. mmmpp3 -> manf ddd30, prod code 000dd"); 
      manf << compressedUpc.mid(0, 3) << compressionSymbol << "0";
      product << "0" << "0" << "0" << compressedUpc.mid(3, 2);
      break;
    case 4:
      qDebug("expandUpc : 3. mmmmp4 -> manf dddd0, prod code 0000d");
      manf << compressedUpc.mid(0, 4) << "0";
      product << "0" << "0" << "0" << "0"<< compressedUpc.mid(4,1);
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      qDebug("expandUpc : 4. mmmmm[5-9] -> manf ddddd, prod code 0000[5-9]"); 
      manf << compressedUpc.mid(0, 5);
      product << "0" << "0" << "0" << "0"<< compressionSymbol;
      break;
    default:
      // should never reach this case; UPC-E -> UPC-A should alway be sucessful 
      qDebug("expandUpc : unknown compression method");
      break;
  }  
  SymbolList expandedUpcA; 
  expandedUpcA << compressedList.at(upc_common::NUMBER_SYSTEM_INDEX);
  expandedUpcA << manf; 
  expandedUpcA << product; 
  qDebug() << "expandUpc : manf: " <<  manf;
  qDebug() << "expandUpc : product: " <<  product;  
  qDebug() << "expandUpc : expended E -> A: " <<  expandedUpcA;
  Q_ASSERT(expandedUpcA.size() >= upcA::MIN && expandedUpcA.size() <= upcA::MAX);
  qDebug("expandUpc : end");
  return expandedUpcA;
}




// QStringList UpcEEngine::expandUpc(const QStringList& symbolList) const 
// {
//   qDebug("UpcEEngine expandUpc() : start");
//   Q_ASSERT(symbolList.size() >= upcE::MIN && symbolList.size() <= upcE::MAX);
//   qDebug() << "UpcEEngine expandUpc() : input " << symbolList;
//   qDebug() << "UpcEEngine expandUpc() : input size " << symbolList.size();
//   
//   // 1. manf dd[0-2]00, prod code 00ddd -> mmppp[0-2]
//   // 2. manf dd[3-9]00, prod code 000dd -> mmmpp3
//   // 3. manf dddd0, prod code 0000d 	-> mmmmp4
//   // 4. manf ddddd, prod code 0000[5-9] -> mmmmm[5-9] 
//   
//   // -- validate by expanding -> UPC-A --
//   QStringList expandedUpcA; 
//   
//   QString compressionSymbol = symbolList.at(upcE::COMPRESS_METHOD_INDEX);
//   int compressionMethod = lookupSymbolIndex(compressionSymbol);
//   qDebug() << "UpcEEngine expandUpc() : Compression Symbol: " << compressionSymbol;
//   qDebug() << "UpcEEngine expandUpc() : Compression method: " << compressionMethod;
//   
//   expandedUpcA << symbolList.at(upc_common::NUMBER_SYSTEM_INDEX);
//   QStringList compressedUpc(symbolList.mid(1, 6));
//   qDebug() << "UpcEEngine expandUpc() : compressedUpc: " << compressedUpc;
//   QStringList manf;
//   QStringList product;
//   switch (compressionMethod) {
//     case 0:
//     case 1:
//     case 2:
//       qDebug("UpcEEngine expandUpc(): 1. mmppp[0-2] -> manf dd[0-2]00, prod code 00ddd");
//       manf << compressedUpc.mid(0, 2) << compressionSymbol << "0" << "0";
//       product << "0" << "0"<< compressedUpc.mid(2,3);
//       break;
//     case 3:
//       qDebug("UpcEEngine expandUpc(): 2. mmmpp3 -> manf ddd30, prod code 000dd"); 
//       manf << compressedUpc.mid(0, 3) << compressionSymbol << "0";
//       product << "0" << "0" << "0" << compressedUpc.mid(3, 2);
//       break;
//     case 4:
//       qDebug("UpcEEngine expandUpc(): 3. mmmmp4 -> manf dddd0, prod code 0000d");
//       manf << compressedUpc.mid(0, 4) << "0";
//       product << "0" << "0" << "0" << "0"<< compressedUpc.mid(4,1);
//       break;
//     case 5:
//     case 6:
//     case 7:
//     case 8:
//     case 9:
//       qDebug("UpcEEngine expandUpc(): 4. mmmmm[5-9] -> manf ddddd, prod code 0000[5-9]"); 
//       manf << compressedUpc.mid(0, 5);
//       product << "0" << "0" << "0" << "0"<< compressionSymbol;
//       break;
//     default:
//       // should never reach this case; UPC-E -> UPC-A should alway be sucessful 
//       qDebug("UpcEEngine expandUpc() : unknown compression method");
//       break;
//   }
//   
//   expandedUpcA << manf; 
//   expandedUpcA << product; 
//   qDebug() << "UpcEEngine expandUpc(): manf: " <<  manf;
//   qDebug() << "UpcEEngine expandUpc(): product: " <<  product;  
//   qDebug() << "UpcEEngine expandUpc(): expended E -> A: " <<  expandedUpcA;
//   Q_ASSERT(expandedUpcA.size() >= upcA::MIN && expandedUpcA.size() <= upcA::MAX);
//   qDebug("UpcEEngine expandUpc() : end");
//   return expandedUpcA;
// }

