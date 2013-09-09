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
#include <QtCore/QByteArray>
#include <QtCore/QBitArray>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

#include "productengine.h"
//#include "symbol.h"


using namespace product;
using namespace shared;
/**
 * @cond PRIVATE
 */
class ProductEngine::Private 
{
public:  
  Private();
  Private(upc_common::PRODUCT_CODE_VALUES productCode);
  virtual ~Private();
   
  bool m_hasNumberSystem;
  /**
    * starting index of first text block
    * 
    * @note All  = 1 but EAN-8 = 0
    */
  int m_fmtFirstBlockOffset;
  /**
   * text block size
   * 
   * @note format blockSize may/not be equal to m_encodeBlockSize    
   */
  int m_fmtBlockSize;
  /**
   * Has a second text block
   * 
   * @note UPC-E and EAN-8 do not contain a second block(s)
   */
  bool m_fmtHasSecondBlock;
  /**
   * starting index of first encoded block
   * 
   * @note All  = 1 but EAN-8 = 0
   */
  int m_encFirstBlockOffset;
  /**
  * encoded block size
  * 
  * @note format blockSize may/not be equal to m_encodeBlockSize    
  */
  int m_encBlockSize;
  /**
   * Has a second encoded block
   * 
   * @note UPC-E and EAN-8 do not contain a second block(s)
   */
  bool m_encHasSecondBlock;
//   /**
//    * encoding patterns for EAN-2 
//    */
//   QStringList m_parity2WidthEncoding;
//   /**
//    * encoding patterns for EAN-5
//    */
//   QStringList m_parity5WidthEncoding;    
 
 /**
  * The product code number system digit is used by object
  * 
  * @note product code specific
  */
  upc_common::PRODUCT_CODE_VALUES m_productCode;
  /**
   * default string used if user input is invalid
   * 
   * @note product code specific
   */
  QString m_defaultString;    
  /** 
   * barcode's minimum number of symbols 
   * 
   * @note product code specific,
   * @note NOT_FOUND == no minimum
   */ 
  int m_minLength;
  /** 
   * barcode's maximum number of symbols
   * 
   * @note product code specific,
   * @note NOT_FOUND == no maximum
   */ 
  int m_maxLength;
  /**
  * Zero-based index of the product code's check digit
  * 
  * 
  * @note UPC-E: uses checkDigit from uncompressed UPC-A for encoding only
  * @note product code specific,
  * @note "-1" == all check digit appear at end of barcode
  * 
  */
  int m_checkDigitOffset;     
  /** 
  * block size
  * 
  * @note product code specific
  */
  int m_blockSize;
};

/**
 * @endcond
 */


ProductEngine::Private::Private() :
  m_productCode(upc_common::PS__UNKNOWN),
  m_defaultString(),
  m_minLength(0),
  m_maxLength(0),
  m_checkDigitOffset(0),
  m_blockSize(0),
  m_hasNumberSystem(false),
  m_fmtFirstBlockOffset(1),
  m_fmtHasSecondBlock(true),
  m_encFirstBlockOffset(0),
  m_encBlockSize(upc_common::ENCODE_BLOCK_SIZE)
{
  qDebug("ProductEngine::Private default constructor");
}

ProductEngine::Private::Private(upc_common::PRODUCT_CODE_VALUES productCode) :
  m_productCode(productCode),
  m_defaultString(""),
  m_minLength(0),
  m_maxLength(0),
  m_checkDigitOffset(0),
  m_blockSize(0),
  m_hasNumberSystem(false),
  m_fmtFirstBlockOffset(1),
  m_fmtHasSecondBlock(true),
  m_encFirstBlockOffset(0),
  m_encBlockSize(0),
  m_encHasSecondBlock(true)
{
  if (productCode ==  upc_common::PS__EAN_13) {
      m_defaultString = ean13::DEFAULT_VALUE;
      m_minLength = ean13::MIN;
      m_maxLength = ean13::MAX_LEN;
      m_checkDigitOffset = ean13::CHECK_DIGIT_OFFSET;
      // [systemDigit][block1][block2]
      m_fmtBlockSize = ean13::BLOCK_SIZE;
      m_fmtFirstBlockOffset = 1;
      m_fmtHasSecondBlock = true;
      // [systemDigit][block1][block2]
      m_encBlockSize = ean13::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  } else if (productCode == upc_common::PS__UPC_A) {
      m_defaultString = upcA::DEFAULT_VALUE;
      m_minLength = upcA::MIN;
      m_maxLength = upcA::MAX_LEN;
      m_checkDigitOffset = upcA::CHECK_DIGIT_OFFSET;
      // [systemDigit][block1][block2][checkDigit]
      m_fmtBlockSize = upcA::BLOCK_SIZE; 
      m_fmtFirstBlockOffset = 1;
      m_fmtHasSecondBlock = true;
      // [block1][block2]
      m_encBlockSize = upcA::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  } else if (productCode == upc_common::PS__EAN_8) {
      m_defaultString = ean8::DEFAULT_VALUE;
      m_minLength = ean8::MIN;
      m_maxLength = ean8::MAX_LEN;
      m_checkDigitOffset = ean8::CHECK_DIGIT_OFFSET;
      // [block1][block2]
      m_fmtBlockSize = ean8::BLOCK_SIZE;
      m_fmtFirstBlockOffset = 0;
      m_fmtHasSecondBlock = true;
      // [block1][block2]
      m_encBlockSize = ean8::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  }  else if (productCode == upc_common::PS__UPC_E) {
      m_defaultString = upcE::DEFAULT_VALUE,
      m_minLength = upcE::MIN,
      m_maxLength = upcE::MAX_LEN,
      m_checkDigitOffset = upcE::CHECK_DIGIT_OFFSET;
      // [systemDigit][block1][checkDigit]
      m_fmtBlockSize = upcE::BLOCK_SIZE;
      m_fmtFirstBlockOffset = 0;
      m_fmtHasSecondBlock = false;
      // [systemDigit] X [checkDigit]->[block1
      m_encBlockSize = upcE::ENCODE_BLOCK_SIZE;
      m_encFirstBlockOffset = 1;
      m_encHasSecondBlock = true;
  }
}

ProductEngine::Private::~Private()
{  
  qDebug("ProductEngine::Private destructor");
}

Symbol ProductEngine::m_emptySymbol;

ProductEngine::ProductEngine(const QString &userBarcode, 			     
	CodeEngine::ConstructCodes flags, 
		upc_common::PRODUCT_CODE_VALUES productCode) :
	AbstractBarcodeEngine(upc_common::CHECKSUM_MODULUS_VALUE),
	d(new Private(productCode)),
	m_constructionFlags(flags),
	m_isValid(CodeEngine::OK),
	m_userInputString(userBarcode),
	m_userParsedSymbols(),
	m_finalSymbolList(),
	m_encodedSymbols()
{
  qDebug("ProductEngine constructor");
  
  switch (d->m_productCode) {
    case upc_common::PS__EAN_13:
      qDebug("------------ ProductEngine : ean-13 start");
      break;
    case upc_common::PS__UPC_A:
      qDebug("------------ ProductEngine : upc-A start");
      break;
    case upc_common::PS__UPC_E:
      qDebug("------------ ProductEngine : upc-E start");
      break;
    case upc_common::PS__EAN_8:
      qDebug("------------ ProductEngine : ean-8 start");
      break;
    default:
      qDebug("------------ ProductEngine : unknown start");
      break;
  }
  qDebug() << " ProductEngine : user input " << m_userInputString;
  if (m_userInputString.isEmpty()) {
    qDebug() << " ProductEngine : default value " << generateDefaultInputString(d->m_minLength + 1);
    m_userInputString = generateDefaultInputString(d->m_minLength + 1);
  }   
  m_userParsedSymbols = m_emptySymbol.parse(m_userInputString);
  qDebug() << " ProductEngine : parsed " << parsedSymbolList();
  // add symbol sets and thier encodings
  initialize();
  
  switch (d->m_productCode) {
    case upc_common::PS__EAN_13:
      qDebug("------------ ProductEngine : ean-13 end");
      break;
    case upc_common::PS__UPC_A:
      qDebug("------------ ProductEngine : upc-A end");
      break;
    case upc_common::PS__UPC_E:
      qDebug("------------ ProductEngine : upc-E end");
      break;
    case upc_common::PS__EAN_8:
      qDebug("------------ ProductEngine : ean-8 end");
      break;
    default:
      qDebug("------------ ProductEngine : unknown end");
      break;
  }
}

ProductEngine::ProductEngine(const shared::SymbolList& barcodeSymbols, CodeEngine::ConstructCodes 
flags, upc_common::PRODUCT_CODE_VALUES productCode) :
		AbstractBarcodeEngine(upc_common::CHECKSUM_MODULUS_VALUE),
		d(new Private(productCode)),
		m_constructionFlags(flags),
		m_isValid(CodeEngine::OK),
		m_userInputString(""),
		m_userParsedSymbols(barcodeSymbols),
		m_finalSymbolList(),
		m_encodedSymbols()
{
  qDebug("ProductEngine constructor");
  
  switch (d->m_productCode) {
    case upc_common::PS__EAN_13:
      qDebug("------------ ProductEngine : ean-13 start");
      break;
    case upc_common::PS__UPC_A:
      qDebug("------------ ProductEngine : upc-A start");
      break;
    case upc_common::PS__UPC_E:
      qDebug("------------ ProductEngine : upc-E start");
      break;
    case upc_common::PS__EAN_8:
      qDebug("------------ ProductEngine : ean-8 start");
      break;
    default:
      qDebug("------------ ProductEngine : unknown start");
      break;
  }
  m_userParsedSymbols = barcodeSymbols;
  qDebug() << " ProductEngine : parsed " << toStringList(m_userParsedSymbols);
  // add symbol sets and thier encodings
  initialize();
  
  switch (d->m_productCode) {
    case upc_common::PS__EAN_13:
      qDebug("------------ ProductEngine : ean-13 end");
      break;
    case upc_common::PS__UPC_A:
      qDebug("------------ ProductEngine : upc-A end");
      break;
    case upc_common::PS__UPC_E:
      qDebug("------------ ProductEngine : upc-E end");
      break;
    case upc_common::PS__EAN_8:
      qDebug("------------ ProductEngine : ean-8 end");
      break;
    default:
      qDebug("------------ ProductEngine : unknown end");
      break;
  }
}

ProductEngine::~ProductEngine()
{
  qDebug("ProductEngine destructor");
  if (d != 0) {
    delete d;
  }
}

QStringList ProductEngine::codeDefault() const
{
  return QStringList(generateDefaultInputString(d->m_minLength + 1).split("", 
QString::SkipEmptyParts) );
}

CodeEngine::ConstructCodes ProductEngine::constructionFlags() const
{
  return m_constructionFlags;
}

QString ProductEngine::userInput() const
{
  return m_userInputString;
}

QStringList ProductEngine::parsedSymbolList() const
{
  return toStringList(m_userParsedSymbols);
}

QStringList ProductEngine::finalSymbolList() const
{
  return toStringList(m_finalSymbolList) ;
}

int ProductEngine::digitsPerBlock() const
{
  return d->m_blockSize;
}

int ProductEngine::primaryLength() const
{
  return d->m_minLength + 1; //m_mainBlockSize;
}

const QStringList ProductEngine::mainBlock() const
{
  return toStringList(m_finalSymbolList.mid(0, d->m_checkDigitOffset + 1));
}

const QString ProductEngine::checkDigit() const
{
  return m_finalSymbolList.at(d->m_checkDigitOffset);
}

const QStringList ProductEngine::extendedBlock() const
{
  return toStringList(m_finalSymbolList.mid(0, d->m_checkDigitOffset));
}

const shared::SymbolList & ProductEngine::symbols() const
{
  return m_finalSymbolList;
}

const QString ProductEngine::numberSystem() const
{
  return local_numberSystem();
}

const QStringList ProductEngine::block1() const
{
  return toStringList(local_block1());
}

const QStringList ProductEngine::block2() const
{
  return toStringList(local_block2());
}

const QString ProductEngine::systemDigit() const
{
  return local_numberSystem();
}

const CodeEngine::ErrorCodes& ProductEngine::statusFlags() const
{
  return m_isValid;
}   

const CodeEngine::ErrorCodes& ProductEngine::addFlags(CodeEngine::ErrorCode flags)
{
  return m_isValid |= flags;
}

const CodeEngine::ErrorCodes& ProductEngine::removeFlags(CodeEngine::ErrorCode flags)
{
  return m_isValid ^= flags;
}


upc_common::PRODUCT_CODE_VALUES ProductEngine::productCode() const
{
  return d->m_productCode;
}

int ProductEngine::fmtBlockSize() const
{
  return d->m_blockSize;
}


int ProductEngine::encBlockSize() const
{
  return d->m_encBlockSize;
}

int ProductEngine::checkDigitOffset() const
{
  return d->m_checkDigitOffset;
}

int ProductEngine::minLength() const
{
  return d->m_minLength;
}

int ProductEngine::maxLength() const
{
  return d->m_maxLength;
}

void ProductEngine::swap(ProductEngine& other)
{
  using std::swap;  
  swap(d, other.d);
}

// ---  ----

// QStringList ProductEngine::parity2WidthEncoding() const
// {
//   return m_parity2WidthEncoding;
// }
// 
// QStringList ProductEngine::parity5WidthEncoding() const
// {
//   return m_parity5WidthEncoding;
// }

// -- protected --

void ProductEngine::initialize()
{
  qDebug("ProductEngine initialize");
  
  fillExtendedEncodingList();
}

shared::SymbolList ProductEngine::processSymbolList(const shared::SymbolList& inputSymbols)
{
  qDebug("ProductEngine::processSymbolList - start");
  const int inputLength = inputSymbols.size();
  const int minLength = d->m_minLength;
  const int maxLength = d->m_maxLength;
  const int validLength = minLength  + 1;
  const int validEan2Length = validLength + ean2::BLOCK_SIZE;
  const int validEan5Length = validLength + ean5::BLOCK_SIZE;
  
  SymbolList finalSymbols;
  // missing check digit
  if (inputLength == minLength) {
    finalSymbols = inputSymbols;
    finalSymbols.append(m_emptySymbol);
  }
  // completely out of range
  else if (inputLength < minLength || inputLength > maxLength) {
    m_isValid |= CodeEngine::LengthOutOfRange;
    return generateDefaultInputSymbols(validLength);
  } else {  
    SymbolList inputSecondHalf(inputSymbols.mid(d->m_checkDigitOffset));
    int secondHalfSize = inputSecondHalf.size();
    switch (secondHalfSize) {
      case 1: // check digit only
      case 3: // EAN-2 and check digit
      case 6: // EAN-5 and check digit
	finalSymbols = inputSymbols;
	break;
      case 0: // no check digit
      case 2: // EAN-2, no check digit 
      case 5: // EAN-5, no check digit 
	finalSymbols = inputSymbols;
	finalSymbols.append(m_emptySymbol);
	break;
      default: // other 
	finalSymbols = generateDefaultInputSymbols(validLength);
	break;
    }
  }
  qDebug("ProductEngine::processSymbolList - end");
  return finalSymbols;  
}

CodeEngine::ErrorCodes ProductEngine::validateCheckDigit(const shared::Symbol& foundDigit, const 
shared::Symbol& calculated) const
{
  
  qDebug() << "CheckDigit : calculated "<< calculated;
  qDebug() << "CheckDigit : found "<< foundDigit;
  
  CodeEngine::ErrorCodes checkDigitFlags;
  // match
  if (foundDigit == calculated) {      
    qDebug("ProductEngine::validateCheckDigit : check digits correct ");
  } else if (foundDigit != calculated) {
    if (m_constructionFlags.testFlag(CodeEngine::UpdateCheckDigit)) {
      qDebug("ProductEngine::validateCheckDigit : replacing bad check digit");
    } else {
      qDebug("ProductEngine::validateCheckDigit : bad check digit");
      checkDigitFlags |= CodeEngine::InvalidCheckDigits;	
    }
  } else if (foundDigit == shared::NOT_FOUND) {
    if (m_constructionFlags.testFlag(CodeEngine::AddCheckDigits)) {
      qDebug("ProductEngine::validateCheckDigit : Inserting missing checkDigit");	
    } else {
      qDebug("ProductEngine::validateCheckDigit : missing check digit");
      checkDigitFlags |= CodeEngine::MissingRequiredCheckDigits;
    } 
  }  
  return checkDigitFlags;
}

shared::Symbol ProductEngine::CalculateCheckDigit()
{
  qDebug("ProductEngine CalculateCheckDigit");
  const int MODULUS = upc_common::CHECKSUM_MODULUS_VALUE;
  Symbol checkSymbol = m_emptySymbol;
  checkSymbol = CommonChecksumOddEven(MODULUS, 
	m_finalSymbolList.mid(0, d->m_checkDigitOffset), 3, 1, true);
  return checkSymbol;
}

int ProductEngine::calculateEan2CheckDigit(const QList< shared::Symbol >& symbolArray) const
{
  qDebug("ProductEngine calculateEan2CheckDigit()");
  if (symbolArray.size() == 2) ;
  int checksum = (symbolArray[0] * 10 + symbolArray[1]) % ean2::CHECKSUM_MODULUS_VALUE;
  Q_ASSERT((checksum >= 0)&&(checksum <= 3));
  return checksum;
}

int ProductEngine::calculateEan5CheckDigit(const QList<shared::Symbol> &symbolArray) const
{
  qDebug("ProductEngine calculateEan5CheckDigit()");
  if (symbolArray.size() != 5) { return 0;}
  const int MODULUS_VALUE = upc_common::CHECKSUM_MODULUS_VALUE;
  return CommonChecksumOddEven(MODULUS_VALUE, symbolArray, 3, 9);
}

//void ProductEngine::formatSymbols(const QStringList &symbolSrc)
// void ProductEngine::formatSymbols()
// {
//   qDebug("ProductEngine formatSymbols() : start");  
//    
//   //m_formatedSymbols << formatMainBlock(m_primaryBlock);
//   m_symbolPositions = formatMainBlock(m_primaryBlock);
//   qDebug() << "ProductEngine formatSymbols() : main block" << formatedSymbols();
//   if (!m_extendedBlock.isEmpty()) {
//     m_symbolPositions[shared::EXTENDED_BLOCK] = m_extendedBlock;
//     //m_formatedSymbols << formatExtendedBlock(m_extendedBlock); 
//   }  
//   qDebug() << "m_formatedSymbols" << formatedSymbols();
//   qDebug("ProductEngine formatSymbols() : end");  

//   qDebug() << "symbolSrc " << symbolSrc;
//   Q_ASSERT(symbolSrc.size() >= minLength() && symbolSrc.size() <= maxLength());    
//   
//   QStringList mainBlock(symbolSrc.mid(0, primaryCodeLength()));
//   QStringList extendedBlock(symbolSrc.mid(primaryCodeLength()));  
//   qDebug() << "mainBlock " << mainBlock;
//   qDebug() << "extendedBlock " << extendedBlock;
//   Q_ASSERT(mainBlock.size() == primaryCodeLength());
//   Q_ASSERT((extendedBlock.isEmpty()) || 
//     (extendedBlock.size() == ean2::BLOCK_SIZE) || 
//     (extendedBlock.size() == ean5::BLOCK_SIZE));   
//   m_formatedSymbols << formatMainBlock(mainBlock);
//   qDebug() << "ProductEngine formatSymbols() : main block" << formatedSymbols();
//   if (!extendedBlock.isEmpty()) {
//     m_formatedSymbols << formatExtendedBlock(extendedBlock); 
//   }  
//   qDebug() << "m_formatedSymbols" << formatedSymbols();
//   qDebug("ProductEngine formatSymbols() : end");  
// }

// shared::SymbolPositionsMap ProductEngine::formatMainBlock(const QStringList &primaryBlock) const
// {
//   qDebug("ProductEngine formatMainBlock() : start");  
//   const int primaryBlockSize = primaryBlock.size();
//   Q_ASSERT(productCode() != upc_common::PS__UPC_E);  
//   Q_ASSERT(primaryBlockSize == primaryCodeLength());
//   
//   if (productCode() != upc_common::PS__EAN_8) {
//     Q_ASSERT((primaryBlockSize - 1) / 2 == d->m_blockSize);
//   } else {    
//     Q_ASSERT(primaryBlockSize / 2 == d->m_blockSize);
//   }  
//   qDebug() << "ProductEngine formatMainBlock() : input : " 
//   << primaryBlock;
//   shared::SymbolPositionsMap formatedSymbols;
//   QStringList formatedBlock;
//   QStringList::const_iterator itrSymbols = primaryBlock.constBegin();
//   // EAN-8 is not shown with a leading digit
//   if (productCode() != upc_common::PS__EAN_8) {
//     formatedSymbols[shared::MAIN_SYSTEM] << *itrSymbols++;
//   } else {
//     formatedSymbols[shared::MAIN_SYSTEM] << "";
//   }
//   qDebug() << "ProductEngine formatMainBlock() : found leading digit : " 
//   << formatedSymbols[shared::MAIN_SYSTEM];  
//   
//   qDebug() << "ProductEngine formatMainBlock() : block size: " << d->m_blockSize;
//   // first block
//   QStringList::const_iterator iterBlockEnd = itrSymbols + digitsPerBlock();
//   while (itrSymbols != iterBlockEnd) {
//     formatedBlock << *itrSymbols++;
//   }
//   formatedSymbols[shared::MAIN_BLOCK_1] = formatedBlock;
// 	 qDebug() << "ProductEngine formatMainBlock() : found first block : " 
// 	 << formatedBlock;
//   formatedBlock.clear();
// 	
//   // second block
//   iterBlockEnd = itrSymbols + digitsPerBlock();
//   while (itrSymbols != iterBlockEnd) {
//     formatedBlock << *itrSymbols++;
//   }
//   formatedSymbols[shared::MAIN_BLOCK_2] = formatedBlock;
//   qDebug() << "ProductEngine formatMainBlock() : found second block : " 
//   << formatedBlock;
//   formatedBlock.clear();
// 	 
//   // EAN-13 and EAN-8 should not have a trailing digit
//   if (itrSymbols != primaryBlock.end() && 
//     (productCode() != upc_common::PS__EAN_13 || 
//     productCode() != upc_common::PS__EAN_8)) {
//     formatedSymbols[shared::MAIN_CHECK_DIGIT] << *itrSymbols;
//     qDebug("ProductEngine formatMainBlock() : found check digit");
//   }
//   qDebug() << "ProductEngine formatMainBlock() : output : " 
//   << formatedSymbols[shared::MAIN_CHECK_DIGIT];
//   qDebug("ProductEngine formatMainBlock() : end");
//   return formatedSymbols;
//   qDebug("ProductEngine formatMainBlock() : start");  
//   Q_ASSERT(productCode() != upc_common::PS__UPC_E);  
//   Q_ASSERT(mainBlock.size() == primaryCodeLength());
//   
//   if (productCode() != upc_common::PS__EAN_8) {
//     Q_ASSERT((mainBlock.size() - 1) / 2 == d->m_blockSize);
//   } else {    
//     Q_ASSERT(mainBlock.size() / 2 == d->m_blockSize);
//   }  
//   qDebug() << "ProductEngine formatMainBlock() : input : " 
// 	   << mainBlock;
//   QStringList formatedSymbols;
//   QStringList formatedBlock;
//   QStringList::const_iterator itrSymbols = mainBlock.constBegin();
//   // EAN-8 is not shown with a leading digit
//   if (productCode() != upc_common::PS__EAN_8) {
//     formatedSymbols << *itrSymbols++;
//   } else {
//     formatedSymbols << "<";
//   }
//   qDebug() << "ProductEngine formatMainBlock() : found leading digit : " 
// 	   << formatedSymbols.at(0);  
// 	   qDebug() << "ProductEngine formatMainBlock() : block size: " << d->m_blockSize;
//   // first block
//   QStringList::const_iterator iterBlockEnd = itrSymbols + digitsPerBlock();
//   while (itrSymbols != iterBlockEnd) {
//     formatedBlock << *itrSymbols++;
//   }
//   formatedSymbols << formatedBlock.join("");
//   qDebug() << "ProductEngine formatMainBlock() : found first block : " 
// 	   << formatedBlock;
//   formatedBlock.clear();
//   
//   // second block
//   iterBlockEnd = itrSymbols + digitsPerBlock();
//   while (itrSymbols != iterBlockEnd) {
//     formatedBlock << *itrSymbols++;
//   }
//   formatedSymbols << formatedBlock.join("");
//   qDebug() << "ProductEngine formatMainBlock() : found second block : " 
// 	   << formatedBlock;
//   formatedBlock.clear();
//   
//   // EAN-13 and EAN-8 should not have a trailing digit
//   if (itrSymbols != mainBlock.end() && 
//     (productCode() != upc_common::PS__EAN_13 || 
//     productCode() != upc_common::PS__EAN_8)) {
//     formatedSymbols << *itrSymbols;
//     qDebug("ProductEngine formatMainBlock() : found check digit");
//   }
//   qDebug() << "ProductEngine formatMainBlock() : output : " 
// 	 << formatedSymbols;
//   qDebug("ProductEngine formatMainBlock() : end");
//   return formatedSymbols;
//}




// QStringList ProductEngine::encodeMainBlock(const SymbolList& mainBlock) const
// {
//   qDebug("ProductEngine encodeMainBlock() : start");  
//   
//   const int symbolsPerBlock = d->m_blockSize;
//   qDebug() << "mainBlock =" << mainBlock;
//   // UPC-E should be handled by its own specific encoding function
//   Q_ASSERT(productCode() != upc_common::PS__UPC_E);  
//   Q_ASSERT(mainBlock.size() == primaryLength());
//   if (productCode() != upc_common::PS__EAN_8) {
//     Q_ASSERT((mainBlock.size() - 1) / 2 == symbolsPerBlock);
//   } else {    
//     Q_ASSERT(mainBlock.size() / 2 == symbolsPerBlock);
//   }  
//   QStringList encodedDigits;  
//   QStringList workingBlock; 
//   QString firstBlockPattern(symbolsPerBlock, 'O');
//   QString secondBlockPattern(symbolsPerBlock, 'R');
//   
//   SymbolList::const_iterator itrSymbols = mainBlock.constBegin();
//   // --- Encode first block of digits:--- 
//   // formated block size equals encoded block size (EAN-13)
//   if (productCode() == upc_common::PS__EAN_13) {
//     firstBlockPattern = getFirstBlockEncodePattern(*itrSymbols++); 
//   }  
//   qDebug() << "encodingBlockSize=" << symbolsPerBlock;
//   qDebug() << "mainBlock size=" << mainBlock.size();
//   qDebug() << "firstBlockPattern=" << firstBlockPattern;
//   qDebug() << "secondBlockPattern=" << secondBlockPattern;  
//   
//   // bars per symbol
//   const int encodedSymbolLength = upc_common::ENCODE_LENGTH;
//  
//   qDebug("ProductEngine encodeMainBlock() : encode first block");
//   std::transform(itrSymbols, itrSymbols + symbolsPerBlock, 
// 		     std::back_inserter(workingBlock),
// 		     PatternEncoder(firstBlockPattern));
//   encodedDigits << workingBlock.join("");
//   workingBlock.clear();
//   itrSymbols += symbolsPerBlock;
//   // --- Encode second block of digits:---  
//   qDebug("ProductEngine encodeMainBlock() : encode second block");
//   std::transform(itrSymbols, mainBlock.end(), 
// 		      std::back_inserter(workingBlock),
// 		      PatternEncoder(secondBlockPattern));
//   encodedDigits << workingBlock.join("");
//   qDebug() << "ENCODE_LENGTH = " << upc_common::ENCODE_LENGTH; 
//   qDebug() <<  "m_encodeBlockSize " << symbolsPerBlock; 
//   qDebug() <<  "encodedDigits.size " << encodedDigits.size();
//   qDebug() <<  "encodedDigits " << encodedDigits;
//   Q_ASSERT(encodedSymbolLength * symbolsPerBlock == 
//   encodedDigits.at(0).size());  
//   Q_ASSERT(encodedSymbolLength * symbolsPerBlock == 
//   encodedDigits.at(1).size());
//   qDebug("ProductEngine encodeMainBlock() : end");
//   return encodedDigits;
// }

QStringList ProductEngine::encodeExtendedBlock(const shared::SymbolList& extendedBlock) const
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
//   shared::LookupIndexArray indexArray(toIndexArray(extendedBlock));
//   shared::LookupIndexArray indexArray(
//     m_Symbols->convertSymbolsToIndexes(extendedBlock));  
  if (extendedBlock.size() == 2) {
    qDebug("ProductEngine encodeExtendedDigits() : EAN-2");
    workingPattern = 
    m_parity2WidthEncoding.at(calculateEan2CheckDigit(extendedBlock));
    qDebug() << "EAN-2 -> " << workingPattern;
  }
  else if (extendedBlock.size() == 5) {
    qDebug("ProductEngine encodeExtendedDigits() : EAN-5");
    workingPattern = 
    m_parity5WidthEncoding.at(calculateEan5CheckDigit(extendedBlock));
    qDebug() << "EAN-5 -> " << workingPattern;
  } else {
    return QStringList();
  }
  
  Q_ASSERT_X(workingPattern.size() == extendedBlock.size(),
	     "size mismatch",
	     "Encoding pattern length does not match number of digits");
  
  workingBlock = encodeSymbolParity(extendedBlock, workingPattern);
  
  qDebug("ProductEngine encodeExtendedDigits() : end");
  return QStringList(workingBlock);
}

QString ProductEngine::generateDefaultInputString(int size) const
{
  return QString(size,'0');
}

QList< shared::Symbol > ProductEngine::generateDefaultInputSymbols(int size) const
{
  return m_emptySymbol.parse(generateDefaultInputString(size)); 
}

const shared::SymbolList ProductEngine::local_primaryBlock() const
{  
  return m_finalSymbolList.mid(0, d->m_checkDigitOffset);
}

const shared::SymbolList ProductEngine::local_extendedBlock() const
{
  return m_finalSymbolList.mid(d->m_checkDigitOffset + 1);
}

const shared::Symbol ProductEngine::setCheckDigit(const shared::Symbol& s)
{
  
}

const shared::Symbol ProductEngine::local_checkDigit() const
{
  return m_finalSymbolList.at(d->m_checkDigitOffset);
}

const shared::Symbol ProductEngine::local_numberSystem() const
{  
  if (d->m_hasNumberSystem && 
    !m_finalSymbolList.isEmpty()) {
    return m_finalSymbolList.at(0);
  }
  return Symbol();
}

const shared::SymbolList ProductEngine::local_block1() const
{
  return m_finalSymbolList.mid(d->m_fmtFirstBlockOffset,
			  d->m_fmtBlockSize);
}

const shared::SymbolList ProductEngine::local_block2() const
{
  if (d->m_fmtHasSecondBlock) {
  return m_finalSymbolList.mid(d->m_fmtFirstBlockOffset + d->m_blockSize,
			  d->m_fmtBlockSize);
  }
  return SymbolList();
}

QImage ProductEngine::image(const QSizeF &requestedSize, QSizeF &minimumSize, 
			      const QColor &foreground, const QColor &background)
{
  QImage eanImage;
  Q_ASSERT(requestedSize.isValid() && foreground.isValid() && background.isValid());
  // not user input or valid encoding
  if (m_encodedSymbols.isEmpty()) {
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
  QStringList encodedMainBlock(this->encodeMainBlock(m_finalSymbolList));
  QStringList encodedExtendedBlock(
    this->encodeExtendedBlock(local_extendedBlock()));
  
  qDebug() << formatedSymbols(); 
  qDebug() << encoded();   
  
  // image width = final barcode pixel width
  // ---------------------------------------------------------------------
  const int ONE_BLOCK_SIZE =	upc_common::QUIET_ZONE.size() + 
				upc_common::LEFT_HAND_GUARD_BARS.size() + 
				upc_common::RIGHT_HAND_GUARD_BARS.size() +
				upc_common::QUIET_ZONE.size();
				
  const int TWO_BLOCK_SIZE =	upc_common::QUIET_ZONE.size() + 
				upc_common::LEFT_HAND_GUARD_BARS.size() + 
				upc_common::CENTER_GUARD_BARS.size() +
				upc_common::RIGHT_HAND_GUARD_BARS.size() +
				upc_common::QUIET_ZONE.size();
				
  const int EXT_BLOCK_SIZE =	upc_common::ADD_ON_GUARD_BARS.size() + 
				upc_common::QUIET_ZONE.size();
				
				
  const int ENCODE_SYMBOL_LEN = upc_common::ENCODE_BLOCK_SIZE;	
  const int EXT_SEP_LEN = upc_common::ADD_ON_SEPERATOR.size();
  int actualSize = 0;
  int sizeBlock2 = local_block2().size() * ENCODE_SYMBOL_LEN;
  int numExtendedBlock = local_extendedBlock().size();
  int sizeExtendedBlock = numExtendedBlock * ENCODE_SYMBOL_LEN +
		(numExtendedBlock - 1) * EXT_SEP_LEN;
  if (sizeBlock2 > 0) {
    actualSize = TWO_BLOCK_SIZE ;
    actualSize += (2 * sizeBlock2);
  } else {
    actualSize = ONE_BLOCK_SIZE;
    actualSize += sizeBlock2;
  }
  if (sizeExtendedBlock > 0) {
    actualSize += EXT_BLOCK_SIZE;
    actualSize += sizeExtendedBlock;
  }
  
  QSize defaultImageSize(actualSize + margin * 2, 
			 fullHeight + margin * 4);
  minimumSize = QSizeF(defaultImageSize);
  eanImage = QImage(defaultImageSize,  QImage::Format_ARGB32);  
				
  QList<QRect> textBlocks;   
  
  int runningIndex = 0;
  int systemDigit_start = runningIndex;
  QPainter * painter = new QPainter(&eanImage); 
  // paint background 
  painter->fillRect(eanImage.rect(), background); // was Qt::white
  // set draw and text color
  painter->setPen(QPen(foreground));
  painter->setFont(font);
  
  // -- Encode main block (EAN-13/UPC-A/UPC-E) --
  runningIndex = drawBars(painter, margin, fullHeight, upc_common::QUIET_ZONE);
  int systemDigit_end = runningIndex;
  textBlocks.append(
    QRect(systemDigit_start, textLowerTopY,
	  systemDigit_end, fontHeight));
  runningIndex += drawBars(painter, margin, fullHeight, 
			   upc_common::LEFT_HAND_GUARD_BARS);
  // -- block: 1 --
  qDebug() << "block1" << encodedMainBlock.at(0);
  int block1_start = runningIndex + 1;
  runningIndex += drawBars(painter, margin, fullHeight, encodedMainBlock.at(0));  
  int block1_end = runningIndex;  
  textBlocks.append(
    QRect(block1_start, textLowerTopY,
	  block1_end - block1_start, fontHeight));
  if (sizeBlock2 > 0) {
    qDebug() << "block2" << encodedMainBlock.at(1);
    runningIndex += drawBars(painter, margin, fullHeight, 
			    upc_common::CENTER_GUARD_BARS);  
    // -- block: 2 --
    int block2_start = runningIndex + 1;
    runningIndex += drawBars(painter, margin, fullHeight, encodedMainBlock.at(1));
    int block2_end = runningIndex;
    textBlocks.append(
      QRect(block2_start, textLowerTopY,
	    block2_end - block2_start, fontHeight));
  }
  runningIndex += drawBars(painter, margin, fullHeight, 
			   upc_common::RIGHT_HAND_GUARD_BARS);
  int checkDigit_start = runningIndex;
  runningIndex += drawBars(painter, margin, fullHeight, upc_common::QUIET_ZONE);
  int checkDigit_end = runningIndex;  
  textBlocks.append(
    QRect(checkDigit_start, textLowerTopY,
	  checkDigit_end - checkDigit_start, fontHeight));
  
  // -- block: extended (EAN-2/EAN-5)--
  if (!encodedExtendedBlock.isEmpty()) {
    qDebug("ProductEngine encodeSymbols() : encode extended block");
    QString encodedExtBlocks(encodedExtendedBlock.join(upc_common::ADD_ON_SEPERATOR));
    qDebug() << "extended block" << encodedExtBlocks;  
    runningIndex += drawBars(painter, margin, fullHeight, upc_common::QUIET_ZONE);
    int extended_start = runningIndex - 2;
    runningIndex += drawBars(painter, margin, fullHeight, 
			     upc_common::ADD_ON_GUARD_BARS);
//     encodedSymbols  encodedExtBlocks.join(upc_common::ADD_ON_SEPERATOR);
    runningIndex += drawBars(painter, margin, fullHeight, encodedExtBlocks);
    int extended_end = runningIndex + 2;
    textBlocks.append(
      QRect(extended_start, textLowerTopY,
	    extended_end - extended_start, fontHeight));
  }
  runningIndex += drawBars(painter, margin, fullHeight, upc_common::QUIET_ZONE);
  
  
  
//   // "number system" digit (EAN-13/EAN-8("<")/UPC-A/UPC-E)
//   //textBlocks.append(QRect(QPoint(0, textLowerTopY), sizeChar));      
//   // first block
//   qDebug() << "add first and second block \"text\"";
// //   textBlocks.append(
// //   QRect(
// //     m_barPositions[shared::MAIN_BLOCK_1_START],
// //     textLowerTopY,
// //     m_barPositions[shared::MAIN_BLOCK_1_END] - m_barPositions[shared::MAIN_BLOCK_1_START],
// //     fontHeight));
//   Q_ASSERT(textBlocks.size() == 2);
//   // second block (EAN-13/EAN-8/UPC-A)
//   if (upc_common::PS__UPC_E != d->m_productCode) {
//     qDebug() << "add second block \"text\"";
//     textBlocks.append(
//       QRect(
// 	m_barPositions[shared::MAIN_BLOCK_2_START], 
// 	textLowerTopY,
// 	m_barPositions[shared::MAIN_BLOCK_2_END] - m_barPositions[shared::MAIN_BLOCK_2_START],
// 	fontHeight));       
//   }
//     // seperated check digit (UPC-A/UPC-E)
//     if (upc_common::PS__UPC_A == productCode() || 
//       upc_common::PS__UPC_E == productCode()) {
//       textBlocks.append(QRect(
// 	QPoint(m_barPositions[shared::MAIN_END], textLowerTopY), sizeChar));
//     }
    
//     // text boxes : asserts
//     switch (productCode()) {
//       case upc_common::PS__EAN_13: // first "system" digit, 2 blocks(6), checkDigit
//       case upc_common::PS__UPC_A: // second "system" digit, 2 blocks(5), checkDigit
// 	Q_ASSERT(textBlocks.size() == 4);
// 	break;
//       case upc_common::PS__UPC_E:  // visual "<" charactor, 1 blocks(6), checkDigit
// 	Q_ASSERT(textBlocks.size() == 3);
// 	break;
//       case upc_common::PS__EAN_8:  // 2 blocks(4)
// 	Q_ASSERT(textBlocks.size() == 2);
// 	break;
//       default:
// 	Q_ASSERT(false);
// 	break;
//     }
    
//     // ean-2, ean-5; extended code 
//     if (!ExtendedBlock().isEmpty()) {   
//       // text on top of bars
//       textBlocks.append(QRect(
// 	m_barPositions[shared::EXTENDED_BLOCK_START], 
// 	0,
// 	m_barPositions[shared::EXTENDED_BLOCK_END] - m_barPositions[shared::EXTENDED_BLOCK_START] + 
// 	1,
// 	fontHeight));
//     }     
//     if (!m_userParsedSymbols.mid(primaryCodeLength()).isEmpty()) {   
//       // text on top of bars
//       textBlocks.append(QRect(
// 	m_barPositions[shared::EXTENDED_BLOCK_START], 
// 	0,
// 	m_barPositions[shared::EXTENDED_BLOCK_END] - m_barPositions[shared::EXTENDED_BLOCK_START] + 
// 1,
// 	fontHeight));
//     }     
  
 
  // Draw text 
  // -------------------------------------------------------------
    qDebug() << "text boxes: " << textBlocks.size();
//     Q_ASSERT(textBlocks.size() >= formatedSymbols().size());
    /*QSize defaultImageSize(m_encodedSymbols.size() + margin * 2, 
			   fullHeight + margin * 4);
    minimumSize = QSizeF(defaultImageSize);
    eanImage = QImage(defaultImageSize,  QImage::Format_ARGB32);  */  
    
    // start drawing, set forground = black, background = white
    /*QPainter * painter = new QPainter(&eanImage); */  /*
    painter->setPen(QPen(foreground));
    painter->setFont(font);*/
//     painter->fillRect(eanImage.rect(), background); // was Qt::white	
    
//     qDebug() << " -- draw the barcode lines -- ";
//     QLine drawBar(0, margin, 0, fullHeight); 
//     for (int index = 0; index < m_encodedSymbols.size(); index++) {
//       if (m_encodedSymbols.at(index) == '1') {
// 	painter->drawLine(drawBar.translated(index + margin, margin));	
//       }
//     }    
    QStringList l_formatedSymbols(formatedSymbols());
    int sections = l_formatedSymbols.size();
    qDebug() << " --  add human readable digits to barcode image -- ";
    for (int index = 0; index < sections; index++) {      
      painter->fillRect(textBlocks[index].translated(index + margin, margin * 2), 
			                       background); //normally Qt::white
      painter->drawText(textBlocks[index].translated(index + margin, margin * 2), 
			Qt::AlignCenter, 
			l_formatedSymbols.at(index));
    }   
    delete painter;    
    
    // resize
    // -----------------------------------------------------------------
    minimumSize = QSizeF(eanImage.width(), 
eanImage.height()*1.5);//setMinimumSize(QSize(eanImage.width(), eanImage.height()*1.5));

  // resize the image to prevent the individual bars from becoming distorted 
  // to barcode readers
  qreal heightFinal = qMax<qreal>(minimumSize.height(),requestedSize.height()); 
  //qreal heightFinal = qMax<qreal>(minimumSize().height(),size.height());
  qreal widthFinal = minimumSize.width(); 
  if (requestedSize.width() > minimumSize.width()) {
    int widthMultipler = requestedSize.width() / minimumSize.width();
    widthFinal *= widthMultipler;
  } 
  return eanImage.scaled(widthFinal, heightFinal);
}

// ----------------------------------------------------------------------------
//                                    private
// ----------------------------------------------------------------------------
int ProductEngine::drawBars(QPainter * painter, int width, int height, QString strBars) const {
  qDebug() << " -- draw the barcode lines -- ";
  int index = 0;
  if (painter == 0) {
    qDebug("painter object missing");
    return index;
  }
  QLine bar(0, width, 0, height); 
  int strBarsSize = strBars.size();
  while (index < strBarsSize) {
    if (strBars.at(index) == '1') {
      painter->drawLine(bar.translated(index + width, width));	
    }
    index++;
  }
}    

void ProductEngine::fillExtendedEncodingList()
{
  qDebug("ProductEngine fillExtendedEncodingList() : start");
  if (m_parity2WidthEncoding.isEmpty()) {
    m_parity2WidthEncoding.append(ean2::PARITY_2[0]);
    m_parity2WidthEncoding.append(ean2::PARITY_2[1]);
    m_parity2WidthEncoding.append(ean2::PARITY_2[2]);
    m_parity2WidthEncoding.append(ean2::PARITY_2[3]);

    for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
      m_parity5WidthEncoding.append(ean5::PARITY_5[i]);
    }    
    qDebug("ProductEngine fillExtendedEncodingList() : set");
  }
  qDebug("ProductEngine fillExtendedEncodingList() : end");
}


