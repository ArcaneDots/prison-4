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


#ifndef EAN8ENGINE_H
#define EAN8ENGINE_H

#include "productengine.h"

namespace product 
{

class Ean8Engine : public ProductEngine 
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
  Ean8Engine(const QString &defaultString = ean8::DEFAULT_VALUE,
	      int minLength = ean8::MIN_8, 
	      int maxLength = ean8::MAX_LEN,
	      int checkDigitOffset = ean8::CHECK_DIGIT_OFFSET,
	      int blockSize = ean8::BLOCK_SIZE,	     
	      int encodedBlockSize = ean8::ENCODE_BLOCK_SIZE,
	      upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__EAN_8);
  /**
   * destructor
   */
  virtual ~Ean8Engine();  
  /**
   * Set current barcode string
   *
   * @param userBarcode user string containing barcode symbols 
   * @param flag contruction hints; Defaults to "Auto".
   **/
  void setBarcodeString(const QString &userBarcode, 
		codeEngine::ConstructCodes flags = codeEngine::AutoProduct);
  /**
   * Set current barcode string
   * 
   * @note not tested
   *
   * @param ptrProductEngine pointer to product code engine
   **/
  virtual void setBarcodeString(ProductEngine *ptrProductEngine);
  /**
   * Get symbol list
   * 
   * @returns list of valid symbols
   */
  QStringList getSymbolList() const;
//   /**
//    * Attempt to get UPC-E version of the inputed product code
//    */
//   virtual QString toUpcE();
//   /**
//    * Attempt to get UPC-A version of the inputed product code
//    */
//   virtual QString toUpcA();
//   /**
//    * Attempt to get EAN-13 version of the inputed product code
//    */
//   virtual QString toEan13();
};
};
#endif // EAN8ENGINE_H

