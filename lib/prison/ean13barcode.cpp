/*
    Copyright (c) 2011 Ian Hollander <ian.hollander@gmail.com>

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

#include <QMap>
#include <QPainter>
#include "ean/ean13defines.h"
#include "ean13barcode.h"

using namespace prison;

/**
 * @cond PRIVATE
 */
class Ean13Barcode::Private {
  public:
    Private() : fontSize(7.0), fontFamily("Lucida Console"){ } 
    /**
     * Set current barcode string
     */
    void setBarcodeString(const QString &userBarcode);    
    /**
     * Validate a string of charactors used for a EAN-13 barcode
     * 
     * Individual boolean reference arguments represent a particular reason for failure. 
     * 
     * @param userDigits input barcode string
     * @param invalidLength number of digits does not match alloawables length(s)
     * @param invalidChars contains one or more non-digit charactors
     * @param hasChecksum includes a possible check digit
     * @param invalidChecksum check digit does not match checksum of other digits
     * @return true; string is a valid ean-13 barcode
     */
    bool isValid(const QString &barcodeString, 
				     bool &invalidLength, 
				     bool &invalidChars, 
				     bool &hasChecksum, 
				     bool &invalidChecksum);    
    /**
     * Calculate EAN checksum digit for a particular barcode
     * 
     * @note Will not check whether supplied string has an invalid length. 
     * @note (checksum value + check digit) & 10 == 0  
     * @see isValidateBarcodeNumber()
     * 
     * @param suppliedBarcodeString user supplied barcode string
     * @return valid check digit
     */
    const QChar CalculateCheckDigit(const QString &suppliedBarcodeString);    
    /**
     * Seperate digits into logical blocks based on encoded layout
     *
     */
    QStringList getSeperatedDigits();    
    /**
     * Encode complete number according to EAN-13 standard
     * 
     * @note 8 or 12 digits + 2 or 3 digits (optional) + checksum   
     * 
     * @param fullCode string of digits
     * @return digits seperated into blocks based on final encoding
     */ 
    QStringList getEncodedDigits(const QStringList &seperatedDigits);
    /**
     * Get font object
     */
    QFont getFont() {
      return QFont(fontFamily, fontSize);
    }
private:
        
    /**
     * Encoding string of digits using "parity" string
     * 
     * @param parity string containing the encoding pattern
     * @param digits digits to be encoded
     * @param supplementalDigits add special separator bars to supplemental portion of the barcode (EAN-2, EAN-5)
     */
    QString encodeDigitParity(const char *parity, 
			      const QString &digits,
                              bool supplementalDigits = false);
    /**
     * encode Ean-13 extended digits (EAN-2)
     * 
     * @param extendedDigits supplemental digits to be encoded
     */
    QString encode2Block(const QString &extendedDigits);
    /**
     * encode Ean-13 extended digits (EAN-5)
     * 
     * @param extendedDigits supplemental digits to be encoded
     */
    QString encode5Block(const QString &extendedDigits);
    /**
     * Calculate special encoding check digit for supplemental block
     * 
     * @param userCode valid EAN-5 barcode digits 
     * @return check digit
     * 
     * @see CalculateExtendedChecksum
     */
    QChar CalculateEan5Checksum(const QString &userCode);
        
    // font size constants
    qreal fontSize;
    QString fontFamily;
    
    QString currrentBarcodeData;
};
/**
 * @endcond
 */

enum barPositions {
  barStart = 0,
  block1start,
  block1end,
  block2start,
  block2end,
  mainblockend,
  blockextstart,
  blockextend,
  barEnd
};

Ean13Barcode::Ean13Barcode() : 
  d(new Ean13Barcode::Private())
{    
}

Ean13Barcode::~Ean13Barcode() 
{
  delete d;
}

QImage Ean13Barcode::toImage(const QSizeF& size) 
{ 
  if (!size.isValid() || data().isEmpty()) {
    return QImage();
  }  
  
  // font size ,
  QFont font(d->getFont()); 
  int fontHeight = QFontMetrics(font).height();
  int fontWidth = QFontMetrics(font).width("<");
  int yFontMultiplier = 3;
  int fullHeight = fontHeight * yFontMultiplier;
  
  // encode digit
  d->setBarcodeString(data());
  QStringList seperatedDigitBlocks(d->getSeperatedDigits());
  QStringList encodedDigitBlocks(d->getEncodedDigits(seperatedDigitBlocks));
  
  int textLowerTopY = fontHeight * (yFontMultiplier - 1);
  int imageHeight = fontHeight * yFontMultiplier;
  // size of a single charactor
  QSize sizeChar(ean13::quietZone.size(), fontHeight);
  // size of a block of digits
  QSize sizeBlock((encodedDigitBlocks[1].size()), fontHeight);
  
  if (seperatedDigitBlocks[0].isEmpty()) {
    seperatedDigitBlocks[0] = "<";    
  }
 
  QMap<int, int> barPosition;  
  
  QList<QRect> textBlocks;  
  // -- draw the barcode lines --
  // parity digit (EAN 13) or "<" for EAN 8
  if (seperatedDigitBlocks[0] == "<") {
    textBlocks.append(QRect(QPoint(0, textLowerTopY),  sizeChar));
  }
  else {
    textBlocks.append(QRect(QPoint(0, textLowerTopY - (fontHeight / 3)), sizeChar));
  }
  Q_ASSERT(textBlocks.size() == 1);
  
  // split up data into logical blocks
  // 0: first digit, used to encode first block (EAN-13 only)
  // 1: first block of "readable" digits
  // 2: second block of "readable" digits
  // 3: supplemental block of "readable" digits
  QString encodedBars;
  encodedBars.clear();
  encodedBars.append(ean13::quietZone);
  barPosition[barStart] = encodedBars.size();
  encodedBars.append(ean13::guardBar);
  barPosition[block1start] = encodedBars.size();
  encodedBars.append(encodedDigitBlocks[0]);  
  barPosition[block1end] = encodedBars.size();
  encodedBars.append(ean13::separator);  
  barPosition[block2start] = encodedBars.size();
  encodedBars.append(encodedDigitBlocks[1]);
  barPosition[block2end] = encodedBars.size();
  encodedBars.append(ean13::guardBar);
  barPosition[mainblockend] = encodedBars.size();
  encodedBars.append(ean13::quietZone);
  
  textBlocks.append(QRect(QPoint(barPosition[block1start], textLowerTopY), sizeBlock));
  textBlocks.append(QRect(QPoint(barPosition[block2start], textLowerTopY), sizeBlock));  
  
  Q_ASSERT(textBlocks.size() == 3);
  // ean-2, ean-5; extended code 
  if (encodedDigitBlocks.size() == 3) {   
    barPosition[blockextstart] = encodedBars.size();
    encodedBars.append(ean13::lhGuardBar); // left-hand guard
    encodedBars.append(encodedDigitBlocks[2]);  
    QSize sizeExtendedBlock(encodedDigitBlocks[2].size() + ean13::lhGuardBar.size(), fontHeight);
    textBlocks.append(QRect(QPoint(barPosition[blockextstart], 0), sizeExtendedBlock));
    barPosition[blockextend] = encodedBars.size();
    encodedBars.append(ean13::quietZone);
  } 
  
  Q_ASSERT(textBlocks.size() >= seperatedDigitBlocks.size());
  
  // create QImage (attempted to make eanImage a pointer to reduce redrawing the image)
  QSize defaultImageSize(encodedBars.size(), fullHeight);
  QImage eanImage(defaultImageSize,  QImage::Format_Mono);
  QLine drawBar(0, 0, 0, fullHeight); 
  
  // start drawing, set background to white
  QPainter * painter = new QPainter(&eanImage);   
  painter->setPen(QPen(foregroundColor()));
  painter->setFont(font);
  painter->fillRect(eanImage.rect(), backgroundColor()); // was Qt::white	
  
  // -- draw the barcode lines --
  for (int index = 0; index < encodedBars.size(); index++) {
    if (encodedBars[index] == '1') {
      painter->drawLine(drawBar.translated(index, 0));	
    }
  }
  
   // -- add human readable digits to barcode image --
  for (int index = 0; index < seperatedDigitBlocks.size(); index++) {      
    painter->fillRect(textBlocks[index], backgroundColor()); //normally Qt::white
    painter->drawText(textBlocks[index], Qt::AlignCenter, seperatedDigitBlocks[index]);
  } 
  
  // draw additional text areas for specific product info i.e. ISBN #, price
  
  // -->
  // ----
  
   
  delete painter;
  setMinimumSize(QSize(eanImage.width(), eanImage.height()*1.5));
  return eanImage.scaled(qMax<int>(minimumSize().width(),size.width()),
			 qMax<int>(minimumSize().height(), size.height()),
			 Qt::KeepAspectRatio);
}

void Ean13Barcode::Private::setBarcodeString(const QString& userBarcode)
{
  currrentBarcodeData = userBarcode;
  
  bool badLen;
  bool badChars;
  bool checksum;
  bool badChecksum;
  
  if (!isValid(currrentBarcodeData, badLen, badChars, checksum, badChecksum)) {
    currrentBarcodeData = QString("123456789012");
    checksum = false;
  }
  if (!checksum) {
    int checkDigitIndex = (currrentBarcodeData.size() == ean13::MIN_8) ? 
						ean13::MIN_8 : ean13::MIN_13;
    currrentBarcodeData.insert(checkDigitIndex, CalculateCheckDigit(currrentBarcodeData));
  }
}
  
QStringList Ean13Barcode::Private::getSeperatedDigits()
{
  QString digits(currrentBarcodeData);
  
  QStringList blocks;
  int remainder = 0;
  //  EAN-8
  if (digits.size() == ean13::MAX_8) {
    blocks << QString() << digits.left(4) << digits.mid(4, 4); // << digits.mid(9);
    remainder = 9;
  }
  // EAN-13
  else if ((digits.size() >= ean13::MAX_13) && (digits.size() <= ean13::MAX_LEN)) {
    blocks << digits.left(1) << digits.mid(1,6) << digits.mid(7, 6);// << digits.mid(13);
    remainder = 13;
  }  
  // supplemental digits - EAN-2, EAN-5
  if (!digits.mid(remainder).isEmpty()) {
    blocks << digits.mid(remainder);   
  }
  return blocks;
}

bool Ean13Barcode::Private::isValid(const QString &barcodeString, bool &invalidLength, bool &invalidChars, bool &hasChecksum, bool &invalidChecksum) 
{
  invalidLength = true;
  invalidChars = true;
  hasChecksum = false;
  invalidChecksum = true;
  
  for (int i = 0; i < barcodeString.size(); i++) {
    if (!barcodeString[i].isDigit()){
      return false;
    }
  }
  
  // min/max length
  if (barcodeString.size() >= ean13::MIN_8 && 
    barcodeString.size() <= ean13::MAX_LEN) {
    if (barcodeString.size() == ean13::MIN_8 || 
	barcodeString.size() == ean13::MIN_13 ||
	barcodeString.size() == ean13::MIN_2 || 
	barcodeString.size() == ean13::MIN_5) {
      invalidLength = false;
      hasChecksum = false;	
    } 
    else if (barcodeString.size() == ean13::MAX_8 || 
	barcodeString.size() == ean13::MAX_13 ||
	barcodeString.size() == ean13::MAX_2 || 
	barcodeString.size() == ean13::MAX_5) {
      invalidLength = false;
      hasChecksum = true;    
      // find check digit index
      int checkDigitIndex = ((barcodeString.size() < ean13::MAX_13) ? ean13::MAX_8 : ean13::MAX_13) - 1;     
      invalidChecksum = (CalculateCheckDigit(barcodeString) != barcodeString.at(checkDigitIndex));      
    }
  }
  if (!invalidLength) {
    // all digits 
    int isNumberDigits = 0;
    for (int index = 0; index < barcodeString.size(); index++) {
      if (barcodeString[index].isDigit()) {
	isNumberDigits++;
      }      
    }
    invalidChars = (barcodeString.size() != isNumberDigits);    
  }
  return (!invalidLength && !invalidChars && !(hasChecksum && !invalidChecksum));
}
 

QStringList Ean13Barcode::Private::getEncodedDigits(const QStringList &seperatedDigits)
{
  // encode blocks of digits; first, second, extended(optional)
  if (seperatedDigits.size() < 3) {
    return QStringList();
  }
  QString workingBlock;
  // --_ Encode first block of digits: ---
  QString firstBlock(seperatedDigits[1]);
  // EAN-8 : encode left "odd"
  if (seperatedDigits[0].isNull()) {    
    workingBlock = encodeDigitParity("OOOO", firstBlock);
  } 
  // EAN-13 : encode based on first digit
  else {
    workingBlock = encodeDigitParity(ean13::Parity13[seperatedDigits[0].toInt()], firstBlock);
  }
  QStringList encodedBlocks(workingBlock);
  workingBlock.clear();
  // --- Encode second block: encode all using "righthand" parity --
  QString secondBlock(seperatedDigits[2]);
  for (int index = 0; index < secondBlock.size(); index++) {
    int digit = secondBlock[index].digitValue();
    workingBlock.append(ean13::RCode[digit]);
  }
  encodedBlocks << workingBlock;
  workingBlock.clear();
  // -- Encode supplemental digits if present (2 or 5 digits)
  if ((seperatedDigits.size() == 4) && !seperatedDigits[3].isEmpty()) {
    QString extendedBlock(seperatedDigits[3]);
    // EAN-2
    if (extendedBlock.size() == 2) {
      encodedBlocks << encode2Block(extendedBlock);
    }
    // EAN-5
    else if (extendedBlock.size() == 5) {
      encodedBlocks << encode5Block(extendedBlock);
    }
  }
  return encodedBlocks;
}

const QChar Ean13Barcode::Private::CalculateCheckDigit(const QString& userCode)
{
  int checksumTotal = 0;  
  // calculate barcode total
  int length = userCode.size() < ean13::MIN_13 ? ean13::MIN_8 : ean13::MIN_13;
  for (int index = 0; index < length; index++) {
    // odd (base 1): value * 3
    if ((index % 2) == 0) {
      checksumTotal += (userCode[index].digitValue() * 3);
    }
    // even: value
    else {
      checksumTotal += userCode[index].digitValue();
    }
  }
  // multiple of 10 == total + check digit
  int checksum = (10 - checksumTotal % 10) % 10;
  return QChar(checksum + 48); // 48 == '0'
}

QString Ean13Barcode::Private::encode2Block(const QString &extendedDigits)
{
 int encodeValue = extendedDigits.toInt() % 4;
 return encodeDigitParity(ean13::Parity2[encodeValue], extendedDigits, true); 
}

QString Ean13Barcode::Private::encode5Block(const QString &extendedDigits)
{
 int encodeValue = CalculateEan5Checksum(extendedDigits).digitValue();
 return encodeDigitParity(ean13::Parity5[encodeValue], extendedDigits, true);
}

QChar Ean13Barcode::Private::CalculateEan5Checksum(const QString& userCode)
{
  int checksum = 0;  
  for (int index = 0; (index < userCode.size() ); index++){
    // odd (base 1): value * 3
    if ((index % 2) == 0) {
      checksum += (userCode[index].digitValue() * 3);
    }
    // even: value
    else {
      checksum += (userCode[index].digitValue() * 9);
    }
  }
  checksum %=  10;
  return QChar(checksum + 48); // ASCII 48 == '0'
}

QString Ean13Barcode::Private::encodeDigitParity(const char * parity, 
					      const QString &digits,
					      bool supplementalDigits)
{
  QString parityPattern(parity);
  // sainty check
  if (parityPattern.size() != digits.size()) {
    return digits;
  }
  // encode each digit based on matching parity type
  QString encodedPattern;
  for (int index = 0; index < parityPattern.size(); index++) {
    if('O' == parityPattern[index]){
      encodedPattern.append(ean13::LOCode[digits[index].digitValue()]);
    }
    else if('E' == parityPattern[index]){
      encodedPattern.append(ean13::LECode[digits[index].digitValue()]);
    }
    else if('R' == parityPattern[index]){
      encodedPattern.append(ean13::RCode[digits[index].digitValue()]);
    }
    // append a Left-hand guard bars after each supplemental digit
    if (supplementalDigits) {
      encodedPattern.append(ean13::cSeparator);
    }
  }
  // return encoded digits
  return encodedPattern;
}
