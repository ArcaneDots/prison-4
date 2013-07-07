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

#include <QtCore/QtDebug>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

#include "productengine.h"

using namespace product;

ProductEngine::ProductEngine(const QString &userBarcode, 			     
		  CodeEngine::ConstructCodes flags, 
		  const QString& defaultString, 
		  int minLength, 
		  int maxLength, 
		  int checkDigitOffset, 
		  int blockSize,
		  int encodedBlockSize,
		  upc_common::PRODUCT_CODE_VALUES productCode): 		       
	AbstractBarcodeEngine(userBarcode,
			flags,
			defaultString, 
			minLength, 
			maxLength, 
		        false,
		        upc_common::CHECKSUM_MODULUS_VALUE, // all product codes
		        true,
			NOT_FOUND,
 			NOT_FOUND), 
		m_checkDigitOffset(checkDigitOffset),	
		m_mainBlockSize(),
		m_blockSize(blockSize),
		m_encodeBlockSize(encodedBlockSize),
		m_productCode(productCode),	
		m_firstBlockStartIndex(0),			     
		m_parity2WidthEncoding(),
		m_parity5WidthEncoding(),
		m_leftOddEncodingList(),
		m_leftEvenEncodingList(),
		m_rightEncodingList()
{
  qDebug("ProductEngine constructor");
  
  // add symbol sets and thier encodings
  initialize();
  
  if (m_productCode != upc_common::PS__EAN_8) { 
    m_firstBlockStartIndex = 1;
  }
  m_secondBlockStartIndex = m_blockSize + m_firstBlockStartIndex;  
  m_mainBlockSize = minLength + 1; 
}

ProductEngine::~ProductEngine()
{
  qDebug("ProductEngine destructor");
}
upc_common::PRODUCT_CODE_VALUES ProductEngine::productCode() const
{
  return m_productCode;
}

QString ProductEngine::userInput() const
{
    return shared::AbstractBarcodeEngine::userInput();
}

void ProductEngine::initialize()
{
  qDebug("ProductEngine initialize");
  AbstractBarcodeEngine::initialize();
  this->fillWidthEncodingList();
}

void ProductEngine::setBarcodeString()
{
  shared::AbstractBarcodeEngine::setBarcodeString();
}

QStringList ProductEngine::parseSymbolString(const QString& symbolString) const
{
  QStringList parsedSymbols;
  QRegExp parseDigits("(\\d)");
  int pos = 0;
  
  while ((pos = parseDigits.indexIn(symbolString, pos)) != -1) {
    parsedSymbols << symbolString[pos];
    pos += parseDigits.matchedLength();
  }
  return parsedSymbols;
}


// overloaded for EAN-2, EAN-5 : no check digit
QStringList ProductEngine::processSymbolList(const QStringList &userSymbols)
{
  qDebug("ProductEngine processSymbolList() : start");  
  // do not process UPC-E (has overloaded version) EAN-2 or EAN-5 only numbers

  Q_ASSERT (upc_common::PS__EAN_2 != m_productCode &&
	    upc_common::PS__EAN_5 != m_productCode);
  
  const int symbolLength = userSymbols.size();
  const int wCheckDigitLength = m_minLength + 1;  
  qDebug() << "#symbols: " << symbolLength 
    << "offset: " << m_checkDigitOffset
    << "symbols" << userSymbols.join(" ");
  
  // basic length sanity check
  if ((symbolLength < m_minLength) || (symbolLength > m_maxLength)) {
    qDebug("ProductEngine processSymbolList() : length out of range");
    m_isValid |= CodeEngine::LengthOutOfRange;
    return QStringList();
  }
  
  // product code specific processing
  if (m_productCode == upc_common::PS__UPC_E) {
    // UPC-E barcodes must belong to either number systems;
    // RegularUpcCodes_1 ("0") or Reserved_1 ("1")
    const int currentNumberSystem =
      lookupSymbolIndex(userSymbols.at(upc_common::NUMBER_SYSTEM_INDEX));
    if ((currentNumberSystem != upc_common::NS__REGULAR_UPC_CODES_1) &&
	(currentNumberSystem != upc_common::NS__RESERVED_1)) {
      m_isValid |= CodeEngine::StandardsViolationError;
      return QStringList();
    }
    Q_ASSERT((currentNumberSystem == upc_common::NS__REGULAR_UPC_CODES_1) ||
	  (currentNumberSystem == upc_common::NS__RESERVED_1));
  }
  
  //get main block w/o check digit to calculate the check digit
  QStringList mainBlock_wo_Check(userSymbols.mid(0, m_checkDigitOffset));
  QStringList extendedBlock(userSymbols.mid(m_checkDigitOffset));
  qDebug() << "mainBlock_wo_Check: " << mainBlock_wo_Check; 
  qDebug() << "m_minLength: " << m_minLength;
  qDebug() << "m_checkDigitOffset  " << m_checkDigitOffset;
  Q_ASSERT(mainBlock_wo_Check.size() == m_minLength);
  
  int foundCheckDigit = NOT_FOUND;
  switch (extendedBlock.size()) {
    case 0: // no check digit
    case 2: // EAN-2 
    case 5: // EAN-5  
      qDebug("ProductEngine processSymbolList() : no check digit found");
      break;
    case 1: // check digit found
    case 3: // EAN-2 
    case 6: // EAN-5 
      foundCheckDigit = lookupSymbolIndex(extendedBlock.front());
      qDebug() << "ProductEngine processSymbolList() : found check digit " 
	<< foundCheckDigit;
      extendedBlock.pop_front();
      break;
    case 4:
    default:   
      qDebug() << "ProductEngine processSymbolList() : bad length";
      m_isValid != CodeEngine::LengthOutOfRange;
      return QStringList();
      break;
  }   
  qDebug() << "extendedBlock "<< extendedBlock;
  Q_ASSERT((extendedBlock.isEmpty()) || 
    (extendedBlock.size() == ean2::BLOCK_SIZE) || 
    (extendedBlock.size() == ean5::BLOCK_SIZE)); 
  
  const int calcuatedCheckDigit = this->calculateCheckDigit(
    convertSymbolsToIndexes(mainBlock_wo_Check));  
  qDebug() << "ProductEngine processSymbolList() : calculated check symbol" 
   << calcuatedCheckDigit;
   
  QStringList fullMainBlock;
  fullMainBlock << mainBlock_wo_Check;  
  
  if (foundCheckDigit == NOT_FOUND) {
    if (m_constructionFlags.testFlag(CodeEngine::AddCheckDigits)) {
      qDebug("ProductEngine processSymbolList() : Inserting missing checkDigit");	  
      fullMainBlock << lookupSymbolAtIndex(calcuatedCheckDigit);  
    } else {
      qDebug("ProductEngine processSymbolList() : missing check digit");
      m_isValid |= CodeEngine::MissingRequiredCheckDigits;
      return QStringList();
    } 
  } else if (foundCheckDigit == calcuatedCheckDigit) {      
      qDebug("ProductEngine processSymbolList() : append valid included check digit");
      fullMainBlock << lookupSymbolAtIndex(foundCheckDigit);
  } else {
    if (m_constructionFlags.testFlag(CodeEngine::UpdateCheckDigit)) {
      qDebug("ProductEngine processSymbolList() : replacing bad check digit");
      fullMainBlock << lookupSymbolAtIndex(calcuatedCheckDigit);
    } else {
      qDebug("ProductEngine processSymbolList() : bad check digit");
      m_isValid |= CodeEngine::InvalidCheckDigits;	
      return QStringList();
    }
  }      
  qDebug() << "fullMainBlock "<< fullMainBlock;
  Q_ASSERT(fullMainBlock.size() == m_mainBlockSize);
  QStringList updatedUserSymbols(fullMainBlock);
  updatedUserSymbols << extendedBlock;   
  Q_ASSERT(updatedUserSymbols.size() == fullMainBlock.size() 
    + extendedBlock.size());
  qDebug() << "updatedUserSymbols "<< updatedUserSymbols; 

  qDebug("ProductEngine processSymbolList() : end");
  return updatedUserSymbols;
}

int ProductEngine::calculateCheckDigit(const shared::LookupIndexArray& symbolArray) const
{  
  qDebug("ProductEngine calculateCheckDigit() -> CommonChecksumOddEven(3,1)"); 
  if (m_productCode != upc_common::PS__UPC_E) {
    Q_ASSERT(symbolArray.size() == m_minLength);
  } else { // calculate check value based on expanded UPC-A code
    Q_ASSERT(symbolArray.size() == upcA::MIN);
  }
  return AbstractBarcodeEngine::CommonChecksumOddEven(symbolArray, 3, 1, true);
}

int ProductEngine::calculateEan2CheckDigit(const shared::LookupIndexArray& symbolArray) const
{
  qDebug("ProductEngine calculateEan2CheckDigit()");
  Q_ASSERT(symbolArray.size() == 2);
  int checksum = (symbolArray[0] * 10 + symbolArray[1]) % ean2::CHECKSUM_MODULUS_VALUE;
  Q_ASSERT((checksum >= 0)&&(checksum <= 3));
  return checksum;
}

int ProductEngine::calculateEan5CheckDigit(const shared::LookupIndexArray& symbolArray) const
{
  qDebug("ProductEngine calculateEan5CheckDigit()");
  Q_ASSERT(symbolArray.size() == 5);
  return std::accumulate(symbolArray.rbegin(), symbolArray.rend(), 
	0, shared::EvenOddChecksum<int>(3, 9)) % 
	      upc_common::CHECKSUM_MODULUS_VALUE;	
}

void ProductEngine::formatSymbols(const QStringList &symbolSrc)
{
  qDebug("ProductEngine formatSymbols() : start");  
  
  qDebug() << "symbolSrc " << symbolSrc;
  Q_ASSERT(symbolSrc.size() >= m_minLength && symbolSrc.size() <= m_maxLength);    
  
  QStringList mainBlock(symbolSrc.mid(0, m_mainBlockSize));
  QStringList extendedBlock(symbolSrc.mid(m_mainBlockSize));  
  qDebug() << "mainBlock " << mainBlock;
  qDebug() << "extendedBlock " << extendedBlock;
  Q_ASSERT(mainBlock.size() == m_mainBlockSize);
  Q_ASSERT((extendedBlock.isEmpty()) || 
    (extendedBlock.size() == ean2::BLOCK_SIZE) || 
    (extendedBlock.size() == ean5::BLOCK_SIZE));   
  m_formatedSymbols << this->formatMainBlock(mainBlock);
  qDebug() << "ProductEngine formatSymbols() : main block" << m_formatedSymbols;
  if (!extendedBlock.isEmpty()) {
    m_formatedSymbols << formatExtendedBlock(extendedBlock); 
  }  
  qDebug() << "m_formatedSymbols" << m_formatedSymbols;
  qDebug("ProductEngine formatSymbols() : end");  
}

QStringList ProductEngine::formatMainBlock(const QStringList& mainBlock) const
{
  qDebug("ProductEngine formatMainBlock() : start");  
  Q_ASSERT(m_productCode != upc_common::PS__UPC_E);  
  Q_ASSERT(mainBlock.size() == m_mainBlockSize);
  if (m_productCode != upc_common::PS__EAN_8) {
    Q_ASSERT((mainBlock.size() - 1) / 2 == m_blockSize);
  } else {    
    Q_ASSERT(mainBlock.size() / 2 == m_blockSize);
  }  
  qDebug() << "ProductEngine formatMainBlock() : input : " 
	   << mainBlock;
  QStringList formatedSymbols;
  QStringList formatedBlock;
  QStringList::const_iterator itrSymbols = mainBlock.constBegin();
  // EAN-8 is not shown with a leading digit
  if (m_productCode != upc_common::PS__EAN_8) {
    formatedSymbols << *itrSymbols++;
  } else {
    formatedSymbols << "<";
  }
  qDebug() << "ProductEngine formatMainBlock() : found leading digit : " 
	   << formatedSymbols.at(0);
  
  qDebug() << "ProductEngine formatMainBlock() : block size: " << m_blockSize;
  // first block
  QStringList::const_iterator iterBlockEnd = itrSymbols + m_blockSize;
  while (itrSymbols != iterBlockEnd) {
    formatedBlock << *itrSymbols++;
  }
  formatedSymbols << formatedBlock.join("");
  qDebug() << "ProductEngine formatMainBlock() : found first block : " 
	   << formatedBlock;
  formatedBlock.clear();
  
  // second block
  iterBlockEnd = itrSymbols + m_blockSize;
  while (itrSymbols != iterBlockEnd) {
    formatedBlock << *itrSymbols++;
  }
  formatedSymbols << formatedBlock.join("");
  qDebug() << "ProductEngine formatMainBlock() : found second block : " 
	   << formatedBlock;
  formatedBlock.clear();
  
  // EAN-13 and EAN-8 should not have a trailing digit
  if (itrSymbols != mainBlock.end() && 
    (m_productCode != upc_common::PS__EAN_13 || 
    m_productCode != upc_common::PS__EAN_8)) {
    formatedSymbols << *itrSymbols;
    qDebug("ProductEngine formatMainBlock() : found check digit");
  }
  qDebug() << "ProductEngine formatMainBlock() : output : " 
	 << formatedSymbols;
  qDebug("ProductEngine formatMainBlock() : end");
  return formatedSymbols;
}

QString ProductEngine::formatExtendedBlock(const QStringList& extendedBlock) const
{
  qDebug("ProductEngine formatExtendedBlock()");
  qDebug() << "ProductEngine formatExtendedBlock() : " << extendedBlock;
  return extendedBlock.join("");
}

void ProductEngine::encodeSymbols(const QStringList& symbolSrc)
{ 
  qDebug("ProductEngine encodeSymbols() : start");    
  Q_ASSERT(upc_common::PS__UPC_E != m_productCode);
  Q_ASSERT(symbolSrc.size() >= m_minLength && symbolSrc.size() <= m_maxLength);
  
  QStringList mainBlock(encodeMainBlock(
    symbolSrc.mid(0, m_mainBlockSize)));
  QString encodedBlock1(mainBlock.at(0));
  qDebug() << "ProductEngine encodeSymbols() : found first block : " 
	   << encodedBlock1;
  QString encodedBlock2(mainBlock.at(1));
  qDebug() << "ProductEngine encodeSymbols() : found second block : " 
	   << encodedBlock2;  
  QStringList encodedExtBlocks(
    encodeExtendedBlock(symbolSrc.mid(m_mainBlockSize)));

  
  // -- Encode main block (EAN-13/UPC-A/UPC-E) --
  m_encodedSymbols = upc_common::QUIET_ZONE;
  m_barPositions[shared::BAR_START] = m_encodedSymbols.size();
  m_encodedSymbols += upc_common::LEFT_HAND_GUARD_BARS;
  // -- block: 1 --
  m_barPositions[shared::MAIN_BLOCK_1_START] = m_encodedSymbols.size() + 1;
  m_encodedSymbols += encodedBlock1;  
  m_barPositions[shared::MAIN_BLOCK_1_END] = m_encodedSymbols.size();
  qDebug() << "block1" << encodedBlock1;
  m_encodedSymbols += upc_common::CENTER_GUARD_BARS;  
  // -- block: 2 --
  m_barPositions[shared::MAIN_BLOCK_2_START] = m_encodedSymbols.size() + 1;
  m_encodedSymbols += encodedBlock2;
  m_barPositions[shared::MAIN_BLOCK_2_END] = m_encodedSymbols.size();
  qDebug() << "block2" << encodedBlock2;
  m_encodedSymbols += upc_common::RIGHT_HAND_GUARD_BARS;
  m_barPositions[shared::MAIN_END] = m_encodedSymbols.size();
  
  // -- block: extended (EAN-2/EAN-5)--
  if (!encodedExtBlocks.isEmpty()) {
    qDebug("ProductEngine encodeSymbols() : encode extended block");  
    m_encodedSymbols += upc_common::QUIET_ZONE;
    m_barPositions[shared::EXTENDED_BLOCK_START] = m_encodedSymbols.size() -2;
    m_encodedSymbols += upc_common::ADD_ON_GUARD_BARS;
    m_encodedSymbols += encodedExtBlocks.join(upc_common::ADD_ON_SEPERATOR);
    m_barPositions[shared::EXTENDED_BLOCK_END] = m_encodedSymbols.size() +2;
    qDebug() << "extended block" 
      << upc_common::ADD_ON_GUARD_BARS << encodedExtBlocks.join(upc_common::ADD_ON_SEPERATOR);
  }
  m_barPositions[shared::BAR_END] = m_encodedSymbols.size();
  m_encodedSymbols.append(upc_common::QUIET_ZONE);
  qDebug() << "m_barPositions :" << m_barPositions;
  
  qDebug("ProductEngine encodeSymbols() : end");  
}

QStringList ProductEngine::encodeMainBlock(const QStringList& mainBlock) const
{
  qDebug("ProductEngine encodeMainBlock() : start");  
  // UPC-E should be handled by its own specific encoding function
  Q_ASSERT(m_productCode != upc_common::PS__UPC_E);  
  Q_ASSERT(mainBlock.size() == m_mainBlockSize);
  if (m_productCode != upc_common::PS__EAN_8) {
    Q_ASSERT((mainBlock.size() - 1) / 2 == m_blockSize);
  } else {    
    Q_ASSERT(mainBlock.size() / 2 == m_blockSize);
  }  
  QStringList encodedDigits;  
  QStringList workingBlock; 
  QString firstBlockPattern(m_encodeBlockSize, 'O');
  QString secondBlockPattern(m_encodeBlockSize, 'R');
  
  QStringList::const_iterator itrSymbols = mainBlock.constBegin();
  // --- Encode first block of digits:--- 
  // formated block size equals encoded block size (EAN-13)
  if (m_productCode == upc_common::PS__EAN_13) {
    firstBlockPattern = getFirstBlockEncodePattern(lookupSymbolIndex(*itrSymbols++)); 
  }  
  qDebug() << "encodingBlockSize=" << m_encodeBlockSize;
  qDebug() << "mainBlock size=" << mainBlock.size();
  qDebug() << "firstBlockPattern=" << firstBlockPattern;
  qDebug() << "secondBlockPattern=" << secondBlockPattern;  
  qDebug("ProductEngine encodeMainBlock() : encode first block");
  encodeSymbolParity(itrSymbols, itrSymbols + m_encodeBlockSize, 
		     std::back_inserter(workingBlock),
		     firstBlockPattern);
  encodedDigits << workingBlock.join("");
  workingBlock.clear();
  itrSymbols += m_encodeBlockSize;
  // --- Encode second block of digits:---  
  qDebug("ProductEngine encodeMainBlock() : encode second block");
  encodeSymbolParity(itrSymbols, mainBlock.end(), 
		     std::back_inserter(workingBlock),
		     secondBlockPattern);
  encodedDigits << workingBlock.join("");
  qDebug() << "ENCODE_LENGTH = " << upc_common::ENCODE_LENGTH; 
  qDebug() <<  "m_encodeBlockSize " << m_encodeBlockSize; 
  qDebug() <<  "encodedDigits.size " << encodedDigits.size();
  qDebug() <<  "encodedDigits " << encodedDigits;
  Q_ASSERT((upc_common::ENCODE_LENGTH - 1) * m_encodeBlockSize == 
    encodedDigits.at(0).size());  
  Q_ASSERT((upc_common::ENCODE_LENGTH - 1) * m_encodeBlockSize == 
    encodedDigits.at(1).size());
  qDebug("ProductEngine encodeMainBlock() : end");
  return encodedDigits;
}

QStringList ProductEngine::encodeExtendedBlock(const QStringList& extendedBlock) const
{
  qDebug("ProductEngine encodeExtendedDigits() : start");
  if (extendedBlock.size() != ean2::BLOCK_SIZE && 
    extendedBlock.size() != ean5::BLOCK_SIZE) {
    return QStringList();
  }
  Q_ASSERT(extendedBlock.size() == ean2::BLOCK_SIZE || 
    extendedBlock.size() == ean5::BLOCK_SIZE); 
  QStringList encodedSymbols;
  QStringList workingBlock;  
  QString workingPattern(extendedBlock.size(),'O');    
  shared::LookupIndexArray indexArray(convertSymbolsToIndexes(extendedBlock));
  
  if (extendedBlock.size() == 2) {
    qDebug("ProductEngine encodeExtendedDigits() : EAN-2");
    workingPattern = 
      m_parity2WidthEncoding.at(calculateEan2CheckDigit(indexArray));
    qDebug() << "EAN-2 -> " << workingPattern;
  }
  else if (extendedBlock.size() == 5) {
    qDebug("ProductEngine encodeExtendedDigits() : EAN-5");
    workingPattern = 
      m_parity5WidthEncoding.at(calculateEan5CheckDigit(indexArray));
    qDebug() << "EAN-5 -> " << workingPattern;
  } else {
    return QStringList();
  }
  
  Q_ASSERT_X(workingPattern.size() == extendedBlock.size(),
	     "size mismatch",
	     "Encoding pattern length does not match number of digits");
  
  encodeSymbolParity(extendedBlock.begin(), extendedBlock.end(),
	     std::back_inserter(workingBlock),
	     workingPattern);
  
  qDebug("ProductEngine encodeExtendedDigits() : end");
  return QStringList(workingBlock);
}

QString ProductEngine::getFirstBlockEncodePattern(int indexedPattern) const
{
  return QString(m_encodeBlockSize, 'O');
}


template<class InIt, class OutIt>
OutIt ProductEngine::encodeSymbolParity(InIt __first1, InIt __last1, 
				     OutIt __result, 
				     const QString& parityPattern) const
{       
  const QString Zeros(upc_common::ENCODE_LENGTH,'0');
  Q_ASSERT_X(!parityPattern.isEmpty(),
	 "Empty string",
	 "Missing Parity Pattern string");
  qDebug() << "parityPattern size" << parityPattern.size();
  QString encodedPattern; //(upc_common::encodeLength, '0');
  int index = 0;
  while(__first1 != __last1 && index < parityPattern.size()) {
    int curSymbolIndex = lookupSymbolIndex(const_cast<const QString&>(*__first1++));
    if (curSymbolIndex != NOT_FOUND) {
	// encode each digit based on matching parity type
	qDebug() << "current index" << index;
	qDebug() << "symbol index" << curSymbolIndex;
	if (parityPattern.at(index) == 'O') {
	  qDebug() << "encode LeftOdd: " << m_leftOddEncodingList.at(curSymbolIndex);
	    *__result++ = m_leftOddEncodingList.at(curSymbolIndex);
	}
	else if (parityPattern.at(index) == 'E') {
	  qDebug() << "encode LeftEven: " << m_leftEvenEncodingList.at(curSymbolIndex);
	  *__result++ = m_leftEvenEncodingList.at(curSymbolIndex);
	}
	else if (parityPattern.at(index) == 'R') {
	  qDebug() << "encode Right: " << m_rightEncodingList.at(curSymbolIndex);
	  *__result++ = m_rightEncodingList.at(curSymbolIndex);
	}
	// output all zeros when no pattern is available
	else {
	  qDebug() << "encode Zeros: " << Zeros;
	  *__result++ = Zeros;
	}
    }
    index++;
  }  
  return __result;    
}

QImage ProductEngine::image(const QSizeF &requestedSize, QSizeF &minimumSize, 
			      const QColor &foreground, const QColor &background)
{
  QImage eanImage;
  Q_ASSERT(requestedSize.isValid() && foreground.isValid() && background.isValid());
  // not user input or valid encoding
  if (m_userInputString.isEmpty() || m_encodedSymbols.isEmpty()) {
    return eanImage;
  }    
    // font size 
    QFont font("OCRA", 7, QFont::Bold); 
    const int fontHeight = QFontMetrics(font).height();
    const int fontWidth = QFontMetrics(font).width("<");
    const int fontHalfHeight = fontHeight / 2;  
    const int yFontMultiplier = 4;
    const int fullHeight = fontHeight * yFontMultiplier;    
    const int textLowerTopY = fontHeight * (yFontMultiplier - 1);
    const int imageHeight = fontHeight * yFontMultiplier;
    const int margin = 6;
    
    // size of a single charactor
    QSize sizeChar(fontWidth, fontHeight);      
    qDebug() << m_formatedSymbols; 
    qDebug() << m_encodedSymbols; 
    qDebug() << m_barPositions; 
    //Q_ASSERT(m_formatedSymbols.size() >= 3);   
    
    QList<QRect> textBlocks;  
    // "number system" digit (EAN-13/EAN-8("<")/UPC-A/UPC-E)
    textBlocks.append(QRect(QPoint(0, textLowerTopY), sizeChar));      
    // first block
    qDebug() << "add first and second block \"text\"";
    textBlocks.append(
    QRect(
      m_barPositions[shared::MAIN_BLOCK_1_START],
      textLowerTopY,
      m_barPositions[shared::MAIN_BLOCK_1_END] - m_barPositions[shared::MAIN_BLOCK_1_START],
      fontHeight));
    Q_ASSERT(textBlocks.size() == 2);
    // second block (EAN-13/EAN-8/UPC-A)
    if (upc_common::PS__UPC_E != m_productCode) {
      qDebug() << "add second block \"text\"";
      textBlocks.append(
	QRect(
	  m_barPositions[shared::MAIN_BLOCK_2_START], 
	  textLowerTopY,
	  m_barPositions[shared::MAIN_BLOCK_2_END] - m_barPositions[shared::MAIN_BLOCK_2_START],
	  fontHeight));       
    }
    // seperated check digit (UPC-A/UPC-E)
    if (upc_common::PS__UPC_A == m_productCode || 
      upc_common::PS__UPC_E == m_productCode) {
      textBlocks.append(QRect(
	QPoint(m_barPositions[shared::MAIN_END], textLowerTopY), sizeChar));
    }
    
    // text boxes : asserts
    switch (m_productCode) {
      case upc_common::PS__EAN_13:
      case upc_common::PS__EAN_8:
      case upc_common::PS__UPC_E:	
	Q_ASSERT(textBlocks.size() == 3);
	break;
      case upc_common::PS__UPC_A:
	Q_ASSERT(textBlocks.size() == 4);
	break;
      default:
	Q_ASSERT(false);
	break;
    }
    
    // ean-2, ean-5; extended code 
    if (!m_userParsedSymbols.mid(m_mainBlockSize).isEmpty()) {   
      // text on top of bars
      textBlocks.append(QRect(
	m_barPositions[shared::EXTENDED_BLOCK_START], 
	0,
	m_barPositions[shared::EXTENDED_BLOCK_END] - m_barPositions[shared::EXTENDED_BLOCK_START] + 1,
	fontHeight));
    }     
    qDebug() << "text boxes: " << textBlocks.size();
    Q_ASSERT(textBlocks.size() >= m_formatedSymbols.size());
    QSize defaultImageSize(m_encodedSymbols.size() + margin * 2, 
			   fullHeight + margin * 4);
    minimumSize = QSizeF(defaultImageSize);
    eanImage = QImage(defaultImageSize,  QImage::Format_ARGB32);    
    // start drawing, set background to white
    QPainter * painter = new QPainter(&eanImage);   
    painter->setPen(QPen(foreground));
    painter->setFont(font);
    painter->fillRect(eanImage.rect(), background); // was Qt::white	
    
    qDebug() << " -- draw the barcode lines -- ";
    QLine drawBar(0, margin, 0, fullHeight); 
    for (int index = 0; index < m_encodedSymbols.size(); index++) {
      if (m_encodedSymbols.at(index) == '1') {
	painter->drawLine(drawBar.translated(index + margin, margin));	
      }
    }    
     // -- add human readable digits to barcode image --
    for (int index = 0; index < m_formatedSymbols.size(); index++) {      
      painter->fillRect(textBlocks[index].translated(index + margin, margin * 2), 
			background); //normally Qt::white
      painter->drawText(textBlocks[index].translated(index + margin, margin * 2), 
			Qt::AlignCenter, 
			m_formatedSymbols[index]);
    }   
    delete painter;    
    
    minimumSize = QSizeF(eanImage.width(), eanImage.height()*1.5);//setMinimumSize(QSize(eanImage.width(), eanImage.height()*1.5));
 
  // prevent the widths of individual bars from being distorted 
  // by stretching the image 
  qreal heightFinal = qMax<qreal>(minimumSize.height(),requestedSize.height()); //qreal heightFinal = qMax<qreal>(minimumSize().height(),size.height());
  qreal widthFinal = minimumSize.width(); 
  if (requestedSize.width() > minimumSize.width()) {
    int widthMultipler = requestedSize.width() / minimumSize.width();
    widthFinal *= widthMultipler;
  } 
  return eanImage.scaled(widthFinal, heightFinal);
}

void ProductEngine::fillWidthEncodingList()
{
  qDebug("ProductEngine fillWidthEncodingList() : start");
  if (m_parity2WidthEncoding.isEmpty()) {
    m_parity2WidthEncoding.append(ean2::PARITY_2[0]);
    m_parity2WidthEncoding.append(ean2::PARITY_2[1]);
    m_parity2WidthEncoding.append(ean2::PARITY_2[2]);
    m_parity2WidthEncoding.append(ean2::PARITY_2[3]);

    for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
      m_leftEvenEncodingList.append(upc_common::LEFT_EVEN_ENCODE_TABLE[i]);
      m_leftOddEncodingList.append(upc_common::LEFT_ODD_ENCODE_TABLE[i]);
      m_rightEncodingList.append(upc_common::RIGHT_HAND_ENCODE_TABLE[i]);
      m_parity5WidthEncoding.append(ean5::PARITY_5[i]);
    }    
    setSymbolEncodingLookupTable(m_leftOddEncodingList);
    qDebug("ProductEngine fillWidthEncodingList() : set");
  }
  qDebug("ProductEngine fillWidthEncodingList() : end");
}
