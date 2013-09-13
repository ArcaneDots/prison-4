/*
    Copyright (C) 2011  Ian Hollander <ianhollander at gmail.com>

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


#ifndef PRODUCTENGINE_H
#define PRODUCTENGINE_H

#include <QtGui/QImage>

#include "../abstractbarcodeengine.h"
#include "upceandefines_p.h"

namespace product 
{
/**
 * handles all operation common to all GS1 product bar code
 * 
 * Performs basic symbology and length validation based on values 
 * passed to member variables during child object construction.
 * Actual barcode construction is preformed by calls to virtual
 * functions overloaded in child classes. Except for extended codes
 * (EAN-2,EAN-5) because all product code support them,
 *
 * @note The seperate EAN-2 and EAN-5 classes exist only for the sake of  
 * @note completeness and for testing purposes. 
 */
class ProductEngine : public shared::AbstractBarcodeEngine
{
public:
  /**
   * constructor
   *
   * @param defaultString default input used when user input is invalid
   * @param minLength minimum length of user input 
   * @param maxLength maximum length of user input
   * @param checkDigitOffset offset of the internal check digit.
   * @param blockSize formated block size
   * @param productCode constant indicating the current product code
   **/
  ProductEngine(const QString &userBarcode, 
	      CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct,
	      const QString &defaultString = "",
	      int minLength = NOT_FOUND, 
	      int maxLength = NOT_FOUND,
	      int checkDigitOffset = NOT_FOUND,
	      int blockSize = NOT_FOUND,
	      int encodedBlockSize = NOT_FOUND,
	      upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__UNKNOWN);
  /**
   * destructor
   */
  virtual ~ProductEngine();
  
  virtual QString userInput() const;
  /**
   * 
   */
  upc_common::PRODUCT_CODE_VALUES productCode() const; 
  /**
   * Get QImage of barcode data
   *
   * @param requestedSize size of wigdet viewable area
   * @param minimumSize size reqired to correctly display the barcode information
   * @param foregroundColor text and bar color
   * @param background color of "white" space
   * @return QImage
   **/
  QImage image(const QSizeF &requestedSize, QSizeF &minimumSize, 
		  const QColor &foregroundColor, const QColor &backgroundColor);
protected:
  /**
   * Class specicfic initialization
   */
  virtual void initialize();
  /**
   * Set current barcode string
   *
   * @param userBarcode user string containing barcode symbols 
   * @param flag contruction hints; Defaults to "Auto".
   **/
  virtual void setBarcodeString();
  /**
   * Parse user input string into list of valid symbols (strings)
   *
   * UPCs only use digits
   * 
   * @param symbolString user input string
   * @returns similarly ordered list of single and multi-character symbols
   */
  virtual QStringList parseSymbolString(const QString &symbolString) const;  
  /**
   * Validate product codes and splits off the extended code (EAN-2/5)
   * 
   * Verifies the check digit value and the valid sizes with the range 
   * 
   * @param symbolSrc list containing all symbol
   * @param splitIndex[out] start of extended block 
   */
  virtual QStringList processSymbolList(const QStringList& userSymbols);
  /**
   * Calculate EAN checksum digit for a particular barcode
   *
   * Move right to left, starting with right-most value as odd, odd * 3, even * 1
   * @sa AbstractBarcodeEngine::CommonChecksumOddEven()
   *
   * @note (checksum value + check digit) & 10 == 0
   *
   * @param symbolArray array of symbol indexes
   * @return valid check digit
   */
  virtual int calculateCheckDigit(const shared::LookupIndexArray &symbolArray) const;
  /**
   * Calculate Ean-2 check digit
   * 
   * @return check digit 
   */
  int calculateEan2CheckDigit(const shared::LookupIndexArray &symbolArray) const;
  /**
   * Calculate Ean-5 check digit
   * 
   * @return check digit 
   */
  int calculateEan5CheckDigit(const shared::LookupIndexArray &symbolArray) const;
  /**
   * Seperate digits/symbols into logical blocks based on encoded layout
   *
   * format:
   * [leading digit|'<'] 
   * [first block] [second block] 
   * [trailing digit|'>'|' '(extended block)]
   * [extended block]
   * ['>'(extended block)]
   *
   * @param symbolSrc list containing all symbol
   * @param splitIndex 
   **/
  virtual void formatSymbols(const QStringList& symbolSrc);
  /**
   * Format digits in the main block
   * 
   * UPC-A  format: [0][1-5][6-(10)][12-13|15]
   * @param mainBlock list of symbols
   */
  virtual QStringList formatMainBlock(const QStringList &mainBlock) const;  
  /**
   * Format digits in the extended block (EAN-2/EAN-5)
   * 
   * @param extendedBlock list of symbols
   */
  QString formatExtendedBlock(const QStringList& extendedBlock) const;  
  /**
   * Encode complete number according to current barcode type
   *
   * UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param symbolSrc full list of symbols
   * @param splitIndex index of the "end" of the first half
   */
  virtual void encodeSymbols(const QStringList &symbolSrc); 
  /**
   * Encode complete number according to current barcode type
   * 
   * Overload UPC-A version because UPC-E only has 1 block of digits
   *  
   * @note UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  virtual QStringList encodeMainBlock(const QStringList &mainBlock) const; 
  /**
   * Attempt to encode any remaining symbols as UPC SUPPLEMENTAL digits
   * 
   * Encodes list of 2 or 5 symbols according to the related extended EAN rules.
   * EAN-2/EAN-5
   * 
   * @note tested but only EAN-5 verified
   * 
   * @param extendedBlock list of symbols
   * @returns encoded version of passed symbol list 
   */
  QStringList encodeExtendedBlock(const QStringList &extendedBlock) const;  
  /**
   * Get productCode specific encoding pattern for the first block of symbols
   * 
   * @param indexedPattern index of assiocated pattern
   */
  virtual QString getFirstBlockEncodePattern(int indexedPattern = 0) const;
  /**
   * Encode symbol based on "parity" pattern
   *
   * char * array version of encodeSymbolParity for QString
   * 
   * @param first1 start of symbols source
   * @param last1 end of symbols source
   * @param result start of destination
   * @param userParityPattern parity pattern used to encode the source
   */   
  template<class InIt, class OutIt> 
  OutIt encodeSymbolParity (InIt __first1, InIt __last1,OutIt __result,
			    const char *userParityPattern) const
  {
    return encodeSymbolParity(__first1, __last1, __result, 
			      QString(userParityPattern));
  }
  /**
   * Encode symbol based on "parity" pattern   
   *
   * @param first1 start of symbols source
   * @param last1 end of symbols source
   * @param result start of destination
   * @param userParityPattern parity pattern used to encode the source   
   */
  template<class InIt, class OutIt> 
  OutIt encodeSymbolParity (InIt __first1, InIt __last1,OutIt __result, 
			    const QString &parityPattern) const;      
  /**
   * Load all encoding patterns based on combo of system number (0-1) and check digit
   */
  virtual void fillWidthEncodingList();   
  /**
   * Zero-based index of the product code's check digit
   * 
   * @note "-1" == all check digit appear at end of barcode
   * @note only used for product codes (UPC/EAN-8/EAN-13)
   */
  int m_checkDigitOffset;     
  /**
   * Zero-based offset of the product code's check digit
   * 
   * @note "-1" == all check digit appear at end of barcode
   * @note only used for EAN-8/13
   */
  int m_mainBlockSize;
  /**
   * The product code number system digit is used by object
   */
  upc_common::PRODUCT_CODE_VALUES m_productCode;
  /**
   * starting index of first block
   */
  int m_firstBlockStartIndex;
  /**
   * starting index of second block
   * 
   * @note UPC-E does not contain a second block
   */
  int m_secondBlockStartIndex;
  /** 
   * block size
   */
  int m_blockSize;
  /**
   * encoded block size
   *
   * @note format blockSize may/not be equal to m_encodeBlockSize    
   */
  int m_encodeBlockSize;
  /**
   * encoding patterns for EAN-2 
   */
  QStringList m_parity2WidthEncoding;
  /**
   * encoding patterns for EAN-5
   */
  QStringList m_parity5WidthEncoding;    
  /**
   * "Left-hand Odd" symbol encoding used by all product codes
   */
  QStringList m_leftOddEncodingList;
  /**
   * "Left-hand Even" symbol encoding used by all product codes
   */
  QStringList m_leftEvenEncodingList;
  /**
   * "Right-hand" symbol encoding used by all product codes
   */
  QStringList m_rightEncodingList;
};
};
#endif // PRODUCTENGINE_H
