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

#include <QRegExp>
#include "upceengine.h"

using namespace product;

UpcEEngine::UpcEEngine(const QString& defaultString,
			     int minLength, 
			     int maxLength, 
			     int checkDigitOffset, 
			     int blockSize, 
			     upc_common::PRODUCT_CODE_VALUES productCode): 
			     UpcAEngine(defaultString, 
					minLength, 
					maxLength, 
				        checkDigitOffset, 
					blockSize, 
					productCode)
{
  qDebug("UpcEEngine constructor");
}

UpcEEngine::~UpcEEngine()
{
  qDebug("UpcEEngine destructor");
}

int UpcEEngine::calculateCheckDigit(const LookupIndexArray &symbolArray) const
{
  qDebug("UpcEEngine calculateCheckDigit : start");
  Q_ASSERT(m_productCode == upc_common::PS__UPC_E);
  int checkValue = NOT_FOUND;
  QStringList upceSymbolList;
  
  // expand UPC-E => UPC-A
  LookupIndexArray::const_iterator itrSymbolIndex = symbolArray.begin();
  while (itrSymbolIndex != symbolArray.end()) {
    upceSymbolList.append(getSymbolAtIndex(*itrSymbolIndex++));    
  }
  Q_ASSERT(upceSymbolList.size() == m_checkDigitOffset);
  const QStringList expandedUpcA(expandUpc(upceSymbolList));
  qDebug() << "UpcEEngine calculateCheckDigit() : expandedUpcA : " 
    << expandedUpcA;
  return ProductEngine::calculateCheckDigit(
    convertSymbolsToIndexes(expandedUpcA));
}

QStringList UpcEEngine::toEan13() const
{
  qDebug("UpcEEngine toEan13");
  QStringList symbolListEan13("0");
  symbolListEan13 << toUpcA();
  return symbolListEan13;
}

QStringList UpcEEngine::toUpcA() const
{
  qDebug("UpcEEngine toUpcA");
  return expandUpc(m_userSymbols);
}

QStringList UpcEEngine::toUpcE() const
{
  qDebug("UpcEEngine toUpcE");
  return m_userSymbols;
}

QStringList UpcEEngine::formatMainBlock(const QStringList& mainBlock) const
{   
  qDebug("UpcEEngine formatMainBlock() : start");  
  Q_ASSERT(m_productCode == upc_common::PS__UPC_E);  
  Q_ASSERT(mainBlock.size() == m_mainBlockSize);
  
  qDebug() << "UpcEEngine formatMainBlock() : input : " 
	   << mainBlock;
  QStringList formatedSymbols;
  // number system
  formatedSymbols << mainBlock.at(0);
  // main block
  formatedSymbols << QStringList(
    mainBlock.mid(1, m_blockSize)).join("");
  // check digit
  formatedSymbols << mainBlock.at(upcE::CHECK_DIGIT_OFFSET);
  qDebug("UpcEEngine formatMainBlock() : end");
  return formatedSymbols;
}

void UpcEEngine::encodeSymbols(const QStringList &symbolSrc)
{  
  qDebug("UpcAEngine encodeSymbols() : start"); 
  QStringList encodedMainBlocks(encodeMainBlock(symbolSrc.mid(0, m_mainBlockSize)));
  
  m_encodedSymbols = upc_common::QUIET_ZONE;
  m_barPositions[shared::BAR_START] = m_encodedSymbols.size();
  m_encodedSymbols += upc_common::LEFT_HAND_GUARD_BARS;
  // -- block: 1 --
  m_barPositions[shared::MAIN_START] = m_encodedSymbols.size();
  m_barPositions[shared::MAIN_BLOCK_1_START] = m_encodedSymbols.size() + 1;
  m_encodedSymbols += encodedMainBlocks.at(0);  
  m_barPositions[shared::MAIN_BLOCK_1_END] = m_encodedSymbols.size() - 1;
  qDebug() << "block1" << encodedMainBlocks.at(0);
  m_encodedSymbols += upcE::SPECIAL_GUARD_BARS;
  m_barPositions[shared::MAIN_END] = m_encodedSymbols.size();
  // -- block: extended --
  QStringList encodedExtBlock(encodeExtendedBlock(symbolSrc.mid(m_mainBlockSize)));
  if (!encodedExtBlock.isEmpty()) {
    qDebug("UpcAEngine encodeSymbols() : encode extended block");  
    m_encodedSymbols += upc_common::QUIET_ZONE;
    m_barPositions[shared::EXTENDED_BLOCK_START] = m_encodedSymbols.size() + 1;
    m_encodedSymbols += upc_common::ADD_ON_GUARD_BARS;
    m_encodedSymbols += encodedExtBlock.join(upc_common::ADD_ON_SEPERATOR);
    m_barPositions[shared::EXTENDED_BLOCK_END] = m_encodedSymbols.size();
  }
  m_barPositions[shared::BAR_END] = m_encodedSymbols.size();
  m_encodedSymbols.append(upc_common::QUIET_ZONE);
  qDebug() << "m_barPositions :" << m_barPositions;    
  qDebug("UpcAEngine encodeSymbols() : end");  
}

QStringList UpcEEngine::encodeMainBlock(const QStringList &mainBlock) const
{  
  qDebug("UpcEEngine encodeMainDigits() : start");
  const int barcodeType = 
    getSymbolIndex(mainBlock.at(upc_common::NUMBER_SYSTEM_INDEX)); 
  const int checkDigit = 
    getSymbolIndex(mainBlock.at(upcE::CHECK_DIGIT_OFFSET));
    
  QString blockPattern;  
  if (barcodeType == upc_common::NS__REGULAR_UPC_CODES_1) {
    blockPattern = m_system0EncodingList.at(checkDigit);
  } else {
    blockPattern = m_system1EncodingList.at(checkDigit);
  }
  qDebug() << "UpcEEngine encodeMainDigits() : pattern" << blockPattern;
  
  QStringList workingBlock;
  // --- Encode first block of digits: (first 6 digits)---  
  encodeSymbolParity(mainBlock.begin() + 1,
		 mainBlock.begin() + upcE::CHECK_DIGIT_OFFSET,
		 std::back_inserter(workingBlock),
		 blockPattern);
  qDebug("UpcEEngine encodeMainDigits() : end");
  return QStringList(workingBlock.join(""));
}

void UpcEEngine::fillWidthEncodingList()
{
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
      m_system0EncodingList.append(upcE::numberSystem0[i]);
      m_system1EncodingList.append(upcE::numberSystem1[i]);
  }  
  UpcAEngine::fillWidthEncodingList();
}
