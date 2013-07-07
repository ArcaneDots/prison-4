/*
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

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


#ifndef EAN13ENGINE_H
#define EAN13ENGINE_H

#include "upcaengine.h"

namespace product
{
class Ean13Engine :  public UpcAEngine 
{
public:
  Ean13Engine(const QString &userBarcode, 
	      CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  virtual ~Ean13Engine();   
  virtual QStringList toUpcE() const;
  /**
   * Attempt to get UPC-A version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual QStringList toUpcA() const;
  /**
   * Attempt to get EAN-13 version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual QStringList toEan13() const;
protected:
  /**
   * Class specicfic initialization
   */
  void initialize();
  /**
   * Attempt to get UPC-E version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  /**
   * Get productCode specific encoding pattern for the first block of symbols
   * 
   * @param indexedPattern index of assiocated pattern
   */
  QString getFirstBlockEncodePattern(int indexedPattern = 0) const;
    /**
   * Load all encoding patterns based on combo of system number (0-1) and check digit
   */
  void fillWidthEncodingList(); 
  /**
   * encoding patterns for EAN-13 first block
   */
  QStringList m_parity13WidthEncoding;
};
};
#endif // EAN13ENGINE_H
