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

#include "productengine_p.h"

using namespace linearBarcode;
using namespace product;
//using namespace shared;

typedef QMap<shared::CODE_SECTIONS, QList<AbstractSymbology> >CodeSections;


ProductEngine::ProductEngine() : AbstractBarcodeEngine(*new ProductEnginePrivate)
{
  Q_D(ProductEngine);
  d->setProductCode(upc_common::PS__UNKNOWN);
}
ProductEngine::ProductEngine(ProductEnginePrivate& d) :
 AbstractBarcodeEngine(d)
{
}



 

ProductEngine::~ProductEngine()
{
  qDebug("ProductEngine destructor");
}

const QStringList ProductEngine::codeDefault() const
{
  Q_D(const ProductEngine);
  return QStringList(defaultInputString(d->m_minLength + 1).split("", 
QString::SkipEmptyParts) );
}

CodeEngine::ConstructCodes ProductEngine::constructionFlags() const
{
  Q_D(const ProductEngine);
  return d->m_constructionFlags;
}

QString ProductEngine::userInput() const
{
  Q_D(const ProductEngine);
  return d->m_userInputString;
}

QString ProductEngine::parsedSymbolList() const
{
  Q_D(const ProductEngine);
  return toStrings(local_rawInput());
}

QString ProductEngine::finalSymbolList() const
{ 
  Q_D(const ProductEngine);
  return toStrings(symbols()) ;
}

int ProductEngine::primaryLength() const
{
  Q_D(const ProductEngine);
  return d->m_minLength + 1;
}

const QStringList ProductEngine::mainBlock() const
{
  return toStringList(local_mainBlock());
}

const QString ProductEngine::checkDigit() const
{
  return local_checkDigit();
}

const QStringList ProductEngine::extendedBlock() const
{
  return toStringList(local_extendedBlock());
}

const QList<ProductSymbology> ProductEngine::symbols() const
{
  QList<ProductSymbology> result(local_primaryBlock());
  if (local_checkDigit().hasValue()) {
    result += local_checkDigit();
  }
  if (!local_extendedBlock().isEmpty()) {
    result +=  local_extendedBlock();
  }
  return result;
}

const QString ProductEngine::numberSystem() const
{
  return local_numberSystem();
}

const QStringList ProductEngine::block1() const
{
  return toStringList(fmt_block1());
}

const QStringList ProductEngine::block2() const
{
  return toStringList(fmt_block2());
}

const CodeEngine::ErrorCodes& ProductEngine::statusFlags() const
{
  Q_D(const ProductEngine);
  return d->m_isValid;
}   

const CodeEngine::ErrorCodes& ProductEngine::addFlags(CodeEngine::ErrorCode flags)
{
  Q_D(const ProductEngine);
  return d->m_isValid |= flags;
}

const CodeEngine::ErrorCodes& ProductEngine::removeFlags(CodeEngine::ErrorCode flags)
{
  Q_D(const ProductEngine);
  return d->m_isValid ^= flags;
}


upc_common::PRODUCT_CODE_VALUES ProductEngine::productCode() const
{
  Q_D(const ProductEngine);
  return d->m_productCode;
}

int ProductEngine::fmtBlockSize() const
{
  Q_D(const ProductEngine);
  return d->m_fmtBlockSize;
}


int ProductEngine::encBlockSize() const
{
  Q_D(const ProductEngine);
  return d->m_encBlockSize;
}



void ProductEngine::swap(ProductEngine& other)
{
  Q_D(const ProductEngine);
  using std::swap;  
  swap(d_ptr, other.d_ptr);
}

// -- protected --
void ProductEngine::initialize()
{
  Q_D(ProductEngine);
  qDebug("ProductEngine initialize");  
  d->initialize();
}

void ProductEngine::processSymbolList(const QList< ProductSymbology >& inputSymbols)
{
  Q_D(ProductEngine);
  qDebug("ProductEngine::processSymbolList - start");
  qDebug() << "input " << inputSymbols;
  const int inputLength = inputSymbols.size();
  const int minLength = d->m_minLength;
  const int maxLength = d->m_maxLength;
  const int validLength = minLength  + 1;
  const int validEan2Length = validLength + ean2::BLOCK_SIZE;
  const int validEan5Length = validLength + ean5::BLOCK_SIZE;
  QList<ProductSymbology> localSymbols(inputSymbols);
  
  // completely out of range
  if (inputLength < minLength || inputLength > maxLength) {
    qDebug("ProductEngine::processSymbolList - input out of range");
    d->m_isValid |= CodeEngine::LengthOutOfRange;
    return;
  }
  QList<ProductSymbology> posPrimaryBlock(localSymbols.mid(0, d->m_checkDigitOffset));
  
  // missing check digit
  if (inputLength == minLength) {    
    qDebug("ProductEngine::processSymbolList - input == primary only");
    setPrimaryBlock(posPrimaryBlock);
  } else if (inputLength > minLength) {
    QList<ProductSymbology> inputSecondHalf(localSymbols.mid(d->m_checkDigitOffset));
    int secondHalfSize = inputSecondHalf.size();
    switch (secondHalfSize) {
      case 1: // check digit only
      case 3: // EAN-2 and check digit
      case 6: // EAN-5 and check digit
	qDebug("ProductEngine::processSymbolList - found checksum digit");
	setCheckDigit(inputSecondHalf.front());
	inputSecondHalf.pop_front();
      case 2: // EAN-2, no check digit 
      case 5: // EAN-5, no check digit 			
	if (inputSecondHalf.size() == 2 || inputSecondHalf.size() == 5) {
	  qDebug("ProductEngine::processSymbolList - also found extended block");
	  setExtendedBlock(inputSecondHalf);
	}
      case 0: // no check digit or extended block
	qDebug("ProductEngine::processSymbolList - save primary block");
	setPrimaryBlock(posPrimaryBlock);
	break;
      default: // other 
	break;
    }
  }
  qDebug("ProductEngine::processSymbolList - end");
}

CodeEngine::ErrorCodes ProductEngine::validateCheckDigit(
    const ProductSymbology& foundDigit, const ProductSymbology& calculated) const
{
  Q_D(const ProductEngine);
  qDebug() << "CheckDigit : calculated "<< calculated;
  qDebug() << "CheckDigit : found "<< foundDigit;
  
  CodeEngine::ErrorCodes checkDigitFlags;
  // match
  if (foundDigit == calculated) {      
    qDebug("ProductEngine::validateCheckDigit : check digits correct ");
  } else if (foundDigit != calculated) {
    if (d->m_constructionFlags.testFlag(CodeEngine::UpdateCheckDigit)) {
      qDebug("ProductEngine::validateCheckDigit : replacing bad check digit");
    } else {
      qDebug("ProductEngine::validateCheckDigit : bad check digit");
      checkDigitFlags |= CodeEngine::InvalidCheckDigits;	
    }
  } else if (foundDigit == shared::NOT_FOUND) {
    if (d->m_constructionFlags.testFlag(CodeEngine::AddCheckDigits)) {
      qDebug("ProductEngine::validateCheckDigit : Inserting missing checkDigit");	
    } else {
      qDebug("ProductEngine::validateCheckDigit : missing check digit");
      checkDigitFlags |= CodeEngine::MissingRequiredCheckDigits;
    } 
  }  
  return checkDigitFlags;
}

int ProductEngine::calculateCheckDigit() const
{
  Q_D(const ProductEngine);
  qDebug("ProductEngine calculateCheckDigit");
  const int MODULUS = upc_common::CHECKSUM_MODULUS_VALUE;
  QList<ProductSymbology> symbolArray(local_primaryBlock());
  if (symbolArray.size() < d->m_minLength) {
    qDebug("ProductEngine calculateCheckDigit : bad size");
    return 0;
  }
  return CommonChecksumOddEven(MODULUS, symbolArray, 3, 1, true);
}

int ProductEngine::calculateEan2CheckDigit(
    const QList< ProductSymbology >& symbolArray) const
{
  qDebug("ProductEngine calculateEan2CheckDigit()");
  if (symbolArray.size() != 2) { 
    qDebug("ProductEngine calculateEan2CheckDigit() : incorrect length");
    return shared::NOT_FOUND; 
  }
  int checksum = (symbolArray[0] * 10 + symbolArray[1]) % ean2::CHECKSUM_MODULUS_VALUE;
  Q_ASSERT((checksum >= 0)&&(checksum <= 3));
  return checksum;
}

int ProductEngine::calculateEan5CheckDigit(const QList< ProductSymbology >& symbolArray) const
{
  qDebug("ProductEngine calculateEan5CheckDigit()");
  if (symbolArray.size() != 5) { 
    qDebug("ProductEngine calculateEan5CheckDigit() : incorrect length");
    return shared::NOT_FOUND; 
  }
  const int MODULUS_VALUE = upc_common::CHECKSUM_MODULUS_VALUE;
  return CommonChecksumOddEven(MODULUS_VALUE, symbolArray, 3, 9);
}

QStringList ProductEngine::encodeExtendedBlock(const QList<ProductSymbology>& extendedBlock) const
{
  Q_D(const ProductEngine);
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
  if (extendedBlock.size() == 2) {
    qDebug("ProductEngine encodeExtendedDigits() : EAN-2");
    workingPattern = 
    d->m_parity2WidthEncoding.at(calculateEan2CheckDigit(extendedBlock));
    qDebug() << "EAN-2 -> " << workingPattern;
  }
  else if (extendedBlock.size() == 5) {
    qDebug("ProductEngine encodeExtendedDigits() : EAN-5");
    workingPattern = 
    d->m_parity5WidthEncoding.at(calculateEan5CheckDigit(extendedBlock));
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

QString ProductEngine::defaultInputString(int size) const
{
  return QString(size,'0');
}

QList< AbstractSymbology > ProductEngine::defaultInputSymbols(int size) const
{
  Q_D(const ProductEngine);
  return d->m_emptySymbol.parse(defaultInputString(size)); 
}

void ProductEngine::setRawInput(QList<ProductSymbology> symbolBlock)
{
  Q_D(ProductEngine);
  qDebug() << " ProductEngine::setRawInput " << symbolBlock;
  QList<AbstractSymbology> fake_symbols(symbolBlock);
  if (!fake_symbols.isEmpty()) {
    d->m_symbolSections[shared::RAW] = symbolBlock;
  }
}

const QList< AbstractSymbology > ProductEngine::local_rawInput() const
{
  Q_D(const ProductEngine);
  return d->m_symbolSections.value(shared::RAW);
}


void ProductEngine::setPrimaryBlock(QList<ProductSymbology> symbolBlock)
{
  Q_D(const ProductEngine);
  qDebug() << " ProductEngine::setPrimaryBlock " << symbolBlock;
  int inputLength = symbolBlock.size();
  QList<AbstractSymbology> fake_symbols(symbolBlock);
  // reset the symbol list in case not a valid length
  if (inputLength != d->m_minLength) {
    fake_symbols.clear();
  }
  d->m_symbolSections[shared::PRIMARY] = symbolBlock;
}

const QList< ProductSymbology > ProductEngine::local_primaryBlock() const
{  
  Q_D(const ProductEngine);
  return d->m_symbolSections.value(shared::PRIMARY);
}
void ProductEngine::setExtendedBlock(QList<ProductSymbology> symbolBlock)
{
  Q_D(const ProductEngine);
  qDebug() << " ProductEngine::setExtendedBlock " << symbolBlock;
  QList<ProductSymbology> fake_symbols(symbolBlock);
  int inputLength = fake_symbols.size();
  // reset the symbol list in case not a valid length
  if (inputLength == 2 || inputLength == 5) {
    d->m_symbolSections[shared::EXTENDED] = fake_symbols;
  }  
}

const QList< ProductSymbology > ProductEngine::local_extendedBlock() const
{
  QList<AbstractSymbology> fake_symbols(
    d->m_symbolSections.value(shared::EXTENDED));
  return fake_symbols;
}

const QList<ProductSymbology> ProductEngine::local_mainBlock() const
{
  SymbolList result(local_primaryBlock());
  if (local_checkDigit().hasValue()) {
    result += local_checkDigit();
  }
  return result;
}

void ProductEngine::setCheckDigit(const ProductSymbology& s)
{
  Q_D(const ProductEngine);
  qDebug("ProductEngine::setCheckDigit - start");
  QList<ProductSymbology> fake_symbols;
  fake_symbols.append(s);
  qDebug() << "saving " << fake_symbols;
  d->m_symbolSections[shared::CHECK_DIGIT] = fake_symbols;
}

const ProductSymbology & ProductEngine::local_checkDigit() const
{
  Q_D(const ProductEngine);
  QList<ProductSymbology> fake_symbols(
    d->m_symbolSections.value(shared::CHECK_DIGIT));
  if (!fake_symbols.isEmpty()) {
    return fake_symbols.front();
  }
  return d->m_emptySymbol;
}

const ProductSymbology ProductEngine::local_numberSystem() const
{  
  Q_D(const ProductEngine);
  QList<ProductSymbology> fake_symbols(
    d->m_symbolSections.value(shared::PRIMARY));
  if (d->m_hasNumberSystem && fake_symbols.size() >= d->m_minLength) {
    return fake_symbols.at(0);
  }
  return d->m_emptySymbol;
}

const QList<ProductSymbology> ProductEngine::fmt_block1() const
{
  Q_D(const ProductEngine);
  QList<ProductSymbology> fake_symbols(
    d->m_symbolSections.value(shared::PRIMARY));
  if (!fake_symbols.isEmpty()) {
    return fake_symbols.mid(d->m_fmtFirstBlockOffset, d->m_fmtBlockSize);
  }
  return fake_symbols;
}

const QList<ProductSymbology> ProductEngine::fmt_block2() const
{
  Q_D(const ProductEngine);
  QList<ProductSymbology> results;
  QList<ProductSymbology> fake_symbols(
    d->m_symbolSections.value(shared::PRIMARY));
  if (!fake_symbols.isEmpty() && d->m_fmtHasSecondBlock) {
    results = fake_symbols.mid(d->m_fmtFirstBlockOffset + d->m_fmtBlockSize, 
			  d->m_fmtBlockSize);
    if (upc_common::PS__EAN_13 == d->m_productCode) {
      results << local_checkDigit();      
    }
  }
  return results;
}

QImage ProductEngine::image(const QSizeF &requestedSize, QSizeF &minimumSize, 
			      const QColor &foreground, const QColor &background)
{
  Q_D(const ProductEngine);
  QImage eanImage;

  // dimension constants
  // expand/contract by 1/13
  //
  // -- GS1 printing info -- 
  // bar pixel width - spec: 0.330 mm or 0.0313in (5.2.1.4.1 -> 5.2.3.6)
  const float mmPixelSize = 0.33; // 0.33
  const int mmPerMeter = 1000;	// 1000 mm = 1 meter
  // vertical
  /// white space above bar - guessed
  const float mmVerticalBuffer = 2.5;
  /// full bar height - spec (22.85 / 18.23 (EAN-8))
  const float mmBarHeight = 24.50; 	
  /// bar height when "cut-off" for the text box - spec
  const float mmTextBarHeight = 22.85;
  /// height of bar in extended block section - spec
  const float mmExtBarHeight = 21.9;
  // space between bar bottom and top of the text
  const float mmExtBottomText_BarSeperation = 0.5 * mmPixelSize;
  // horizontal
  /// white space at before and after main barcode - spec (2.97 - 3.96)
  const float mmQuietZone = 2.97;
  /// white space after extended block - spec (2.97 - 3.96)
  const float mmExtQuietZone = 1.65; 	// 1.65

  // sizes at x1.00 magnifacition (bar width of 0.330)
  ///  figure 5.3.3.6 - 2
  const float mmX1MaxLenUpcA = 37.29;
  ///  figure 5.3.3.6 - 4
  const float mmX1MaxLenEan8 = 22.11;
  ///  figure 5.3.3.6 - 5
  const float mmX1MaxLenUpcE = 22.11;
  ///  figure 5.3.3.6 - 6
  const float mmX1MaxLenUpcAEan2 = 45.54;
  ///  figure 5.3.3.6 - 5
  const float mmX1MaxLenEan13Ean5 = 54.45;

  // font size info);
  QFont font("OCRA", 10, QFont::Bold);
  int fontHeight = QFontMetrics(font).height();
  int vertWhiteBuffer = mmVerticalBuffer / mmPixelSize;
  int pixelPerMeter = mmPerMeter / mmPixelSize;
  int quietPixels = mmQuietZone / mmPixelSize;
  int barPixelHeight = mmBarHeight / mmPixelSize; 
  int textBoxTop =  (mmTextBarHeight / mmPixelSize); //  + vertWhiteBuffer;
  int textBoxBottom =  fontHeight  + textBoxTop;
  int extQuietPixels = mmExtQuietZone / mmPixelSize;
  eanImage.setDotsPerMeterX(pixelPerMeter);
  eanImage.setDotsPerMeterY(pixelPerMeter);

  // size in "bars"
  const int REGULAR_QUIET_ZONE = quietPixels; //upc_common::QUIET_ZONE.size();
  const int EXTENDED_QUIET_ZONE =  extQuietPixels; //8

  // insert "invalid" in text area when the barcode is empty
  QStringList formatedText(formatedSymbols());
  QString textSystemDigit(formatedText.value(0));
  QString textBlock1(formatedText.value(1));
  QString textBlock2(formatedText.value(2));
  QString textCheckDigit(formatedText.value(3));
  QString textExtBlock(formatedText.value(4));

  qDebug() << "textBlock1" << textBlock1;
  if (textSystemDigit.isEmpty()) {
    textSystemDigit = "<";
    formatedText[0] = textSystemDigit;
  }
  
  if (textBlock1.isEmpty()) {
    if (productCode() == upc_common::PS__EAN_8) {
      textBlock1 = "inva";
      textBlock2 = "lid ";
    }
    if (productCode() == upc_common::PS__UPC_A ||
      productCode() == upc_common::PS__EAN_13) {
      textBlock1 = " inva";
      textBlock2 = "lid  ";
    }
    if (productCode() == upc_common::PS__UPC_A) {
      textBlock1 = "invalid";
      textBlock2 = "";
    }
    formatedText[1] = textBlock1;
    formatedText[2] = textBlock2;
  }

  if (textCheckDigit.isEmpty()) {
    textCheckDigit = ">";
    formatedText[3] = textCheckDigit;
  }
  
  int textWidthSystemDigit = QFontMetrics(font).width(textSystemDigit);
  int textWidthBlock1 = QFontMetrics(font).width(textBlock1);
  int textWidthBlock2 = QFontMetrics(font).width(textBlock2);
  int textWidthCheckDigit = QFontMetrics(font).width(textCheckDigit);
  int textWidthExtBlock = QFontMetrics(font).width(textExtBlock);
  
  // image width = final barcode pixel width
  // ---------------------------------------------------------------------
  const int ONE_BLOCK_SIZE =	REGULAR_QUIET_ZONE +
				upc_common::LEFT_HAND_GUARD_BARS.size() + 
				upc_common::RIGHT_HAND_GUARD_BARS.size() +
				//upc_common::QUIET_ZONE.size();
				REGULAR_QUIET_ZONE;
				
  const int TWO_BLOCK_SIZE =	REGULAR_QUIET_ZONE +
				upc_common::LEFT_HAND_GUARD_BARS.size() + 
				upc_common::CENTER_GUARD_BARS.size() +
				upc_common::RIGHT_HAND_GUARD_BARS.size() +
				REGULAR_QUIET_ZONE;
				//upc_common::QUIET_ZONE.size();
				
  const int EXT_BLOCK_SIZE =	upc_common::ADD_ON_GUARD_BARS.size() +
				EXTENDED_QUIET_ZONE;
				
				
  const int ENCODE_SYMBOL_LEN = upc_common::ENCODE_LENGTH;	
  const int EXT_SEP_LEN = upc_common::ADD_ON_SEPERATOR.size();


  // ---------------------------- "bit" length --------------------------------
  QList<QStringList> encodedSymbols(encoded());
  // unpack encoding
  QStringList block1Encoding(encodedSymbols.at(0));
  QStringList block2Encoding(encodedSymbols.at(1));
  QStringList blockExtEncoding(encodedSymbols.at(2));
  
  // calculate actual bit Size
  int actualSize = 0;
  // block1  
  actualSize += block1Encoding.size() * ENCODE_SYMBOL_LEN;
  actualSize += block2Encoding.size() * ENCODE_SYMBOL_LEN;
  // check second block to determine number of additional bars
  if (block2Encoding.isEmpty() || productCode() == upc_common::PS__EAN_8) {
    actualSize += ONE_BLOCK_SIZE;
  } else {
    actualSize += TWO_BLOCK_SIZE;
  }
  // check extended block to determine number of additional bars
  actualSize += blockExtEncoding.size() * ENCODE_SYMBOL_LEN;
  if (!blockExtEncoding.isEmpty()) {
    actualSize += EXT_BLOCK_SIZE;
  }
  
  qDebug() << "size = " << actualSize << " mm = " << actualSize * mmPixelSize;
  // ------------------------------ image size -----------------------------
  // set minimum required by the barcode
  QSize defaultImageSize(actualSize + vertWhiteBuffer * 2,
			 barPixelHeight + vertWhiteBuffer * 4);
  minimumSize = QSizeF(defaultImageSize);
  eanImage = QImage(defaultImageSize,  QImage::Format_ARGB32);
  eanImage.setDotsPerMeterX(pixelPerMeter);
  eanImage.setDotsPerMeterY(pixelPerMeter);

  // -- set image "device" --
  QPainter * painter = new QPainter(&eanImage);
  QPen eanPen;
  eanPen.setWidth(1);
  eanPen.setColor(foreground);
  painter->setPen(eanPen);  
  painter->setFont(font);
  
  // paint background
  painter->fillRect(eanImage.rect(), background); // was Qt::white
  

  // ------------------------------ draw bars -----------------------------
  QList<QRect> textBlocks;
  int runningIndex = 0;
  
  // -- Encode main block (EAN-13/EAN-8/UPC-A/UPC-E) --
  // left "quiet zone"
  int systemDigit_start = runningIndex;
  runningIndex +=  REGULAR_QUIET_ZONE;
  int systemDigit_end = runningIndex;
  textBlocks.append(QRect(systemDigit_start, textBoxTop,
	systemDigit_end, fontHeight));
  
  runningIndex += drawBars(painter,
			  vertWhiteBuffer, barPixelHeight, runningIndex,
			  upc_common::LEFT_HAND_GUARD_BARS);
  
  // -- block: 1 --
  qDebug() << "block1" << block1Encoding;
  int block1_start = runningIndex;
  runningIndex += drawBars(painter,
			vertWhiteBuffer, barPixelHeight, runningIndex,
			block1Encoding);
  // ---------------------
  int block1_end = runningIndex;
  // -- block1 text --
  int block1Offset = block1_end - block1_start - textWidthBlock1;
  textBlocks.append(QRect(block1_start, textBoxTop,
	textWidthBlock1, fontHeight));

  qDebug() << "block2 start index " << runningIndex;
  qDebug() << "block2 empty " << block2Encoding.isEmpty();
  // -- section: block 2 --
  if (!block2Encoding.isEmpty()) {
    qDebug() << "block2" << block2Encoding;
    // ---------------------
    qDebug("center gauard");
    runningIndex += drawBars(painter,
			     vertWhiteBuffer, barPixelHeight, runningIndex,
			     upc_common::CENTER_GUARD_BARS);
    int block2_start = runningIndex;
    runningIndex += drawBars(painter,
			      vertWhiteBuffer, barPixelHeight, runningIndex,
			      block2Encoding);
    int block2_end = runningIndex;
    textBlocks.append(QRect(block2_start + 2, textBoxTop,
	textWidthBlock2, fontHeight));

    // ---------------------
  }
  
  // -- section: RIGHT_HAND_GUARD_BARS--
  runningIndex += drawBars(painter,
			   vertWhiteBuffer, barPixelHeight, runningIndex,
			   upc_common::RIGHT_HAND_GUARD_BARS);
  int checkDigit_start = runningIndex;
  runningIndex +=  REGULAR_QUIET_ZONE;
  int checkDigit_end = runningIndex;
  textBlocks.append(
      QRect(checkDigit_start, textBoxTop,
	    textWidthCheckDigit, fontHeight));

  // Draw text 
  // -------------------------------------------------------------
  int numTextBlocks = textBlocks.size();
  // start drawing, set forground = black, background = white

  int textBlockSize = textBlocks.size();
  for (int rText = 0; rText < textBlockSize; rText++) {
    painter->fillRect(textBlocks.at(rText), background);
    painter->setPen(QPen(foreground));
    painter->setFont(font);
    painter->drawText(textBlocks[rText],
	Qt::AlignCenter,
	formatedText.at(rText));
  }
  delete painter;
    
  // resize:
  // not sure how to indicate range of scaling to widget;
  // image can only scaled by the changing pixel size
  // according to GS1 specs or the barcode's readability suffers
  // i.e. pixel size = 0.33 * magnifacition (0.80 <-> 2.00 step 5)
  // 
  // information is lieted in figure 5.2.4.7-1  (p. 252 of GS1 general spec pdf)
  // -----------------------------------------------------------------
  minimumSize = QSizeF(eanImage.width()*0.80, eanImage.height()*0.80);

  return eanImage;
}

// ----------------------------------------------------------------------------
//                                    private
// ----------------------------------------------------------------------------
int ProductEngine::drawBars(QPainter* painter, int top, int bottom,
			    int startOffset, QStringList strLstBars) const
{
  int drawLength = 0;
  
  QStringList::const_iterator _first = strLstBars.begin();
  QStringList::const_iterator _last = strLstBars.end();
  
  while (_first != _last) {
    int wordLength = drawBars(painter, top, bottom, startOffset, *_first);
    drawLength += wordLength;
    startOffset += wordLength;
    _first++;
  }
  return drawLength;
}


int ProductEngine::drawBars(QPainter* painter, int top, int bottom,
			    int startOffset, QString strBars) const
{
  int index = 0;
  if (painter == 0) {
    qDebug("painter object missing");
    return index;
  }
  if (strBars.isEmpty()) {
    qDebug("string empty");
    return index;
  }
  QPoint pointTop(startOffset, top);
  QPoint pointBottom(startOffset, bottom);
  QLine bar(pointTop, pointBottom);
  
  int strBarsSize = strBars.size();
  while (index < strBarsSize) {
    if (strBars.at(index) == '1') {
      painter->drawLine(bar.translated(index + top, top));
    }
    index++;
  }
  return index;
}    

// void ProductEngine::fillExtendedEncodingList()
// {
//   if (d->m_parity2WidthEncoding.isEmpty()) {
//     d->m_parity2WidthEncoding.append(ean2::PARITY_2[0]);
//     d->m_parity2WidthEncoding.append(ean2::PARITY_2[1]);
//     d->m_parity2WidthEncoding.append(ean2::PARITY_2[2]);
//     d->m_parity2WidthEncoding.append(ean2::PARITY_2[3]);
// 
//     for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
//       d->m_parity5WidthEncoding.append(ean5::PARITY_5[i]);
//     }
//   }
// }


