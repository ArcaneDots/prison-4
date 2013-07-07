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


#ifndef UPCAENGINE_H
#define UPCAENGINE_H

#include <QtCore/QtDebug>
#include "productengine.h"

namespace product
{
/**
 * UPC-A barcode generator
 */
class UpcAEngine : public ProductEngine 
{

public:
  UpcAEngine();
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
  UpcAEngine(const QString &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct,
	     const QString &defaultString = upcA::DEFAULT_VALUE,
	     int minLength = upcA::MIN,
	     int maxLength = upcA::MAX_LEN,
	     int checkDigitOffset = upcA::CHECK_DIGIT_OFFSET,
	     int blockSize = upcA::BLOCK_SIZE,
	     upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__UPC_A);
 

  /**
   * destructor
   */
  virtual ~UpcAEngine();
    
  virtual QString userInput() const;  
  
  /**
   * Attempt to get UPC-E version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible 
   */
  virtual QStringList toUpcE();
  /**
   * Attempt to get UPC-A version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual QStringList toUpcA();
  /**
   * Attempt to get EAN-13 version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual QStringList toEan13();
  
protected:
  /**
   * Set current barcode string
   * 
   * Calls productengine version to 
   *
   * @param userBarcode user string containing barcode symbols 
   * @param flag contruction hints; Defaults to "Auto".
   */
  virtual void setBarcodeString();
  /**
   * Compress UPC-A barcode information into UPC-E format
   *
   *  		UPC-A				->  	UPC-E 			    
   * 1. manf dd[0-2]00, prod code 00ddd		->    	mmppp[0-2] 	
   * 2. manf dd[3-9]00,	prod code 000dd		->    	mmmpp3 		
   * 3. manf dddd0, prod code 0000d 		->    	mmmmp4 			 
   * 4. manf ddddd, prod code 0000[5-9] 	->	mmmmm[5-9] 	 
   * 
   * @param compressedUpc list of symbols, not including check digit
   * @param compressionMethod method used to compress the UPC-A code
   */
  QStringList compressUpc(const QStringList &symbolList) const;
  /**
   * Expand the UPC-E barcode information into UPC-A format 
   *
   * 	UPC-E 		->  		     UPC-A
   * 1. mmppp[0-2] 	-> 	manf dd[0-2]00, prod code 00ddd
   * 2. mmmpp3 		-> 	manf dd[3-9]00, prod code 000dd
   * 3. mmmmp4 		-> 	manf dddd0, prod code 0000d 	 
   * 4. mmmmm[5-9] 	-> 	manf ddddd, prod code 0000[5-9] 
   * 
   * @param compressedUpc list of symbols, not including check digit
   * @param compressionMethod method used to compress the UPC-A code
   */
  QStringList expandUpc(const QStringList &compressedUpc) const;
};
};
#endif // UPCAENGINE_H
