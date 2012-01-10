/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
  UpcAEngine(const QString &defaultString = upcA::DEFAULT_VALUE,
	     int minLength = upcA::MIN,
	     int maxLength = upcA::MAX_LEN,
	     int checkDigitOffset = upcA::CHECK_DIGIT_OFFSET,
	     int blockSize = upcA::BLOCK_SIZE,
	     upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__UPC_A);
 
  /**
   * destructor
   */
  virtual ~UpcAEngine();
  /**
   * Set current barcode string
   *
   * @param userBarcode user string containing barcode symbols 
   * @param flag contruction hints; Defaults to "Auto".
   **/
  void setBarcodeString(const QString &userBarcode, 
		CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * Set current barcode string
   * 
   * @note not tested
   *
   * @param ptrProductEngine pointer to product code engine
   **/
  void setBarcodeString(ProductEngine *ptrProductEngine);
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
