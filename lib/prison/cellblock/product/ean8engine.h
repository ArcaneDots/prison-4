/*
    Copyright (C) 2011  Ian gmail <ianhollander at gmail.com>

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
   * default constructor
   */
  Ean8Engine();
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
  Ean8Engine(const QString &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * destructor
   */
  virtual ~Ean8Engine();  
  /**
   * Get symbol list
   * 
   * @returns list of valid symbols
   */
  QStringList getSymbolList() const;
};
};
#endif // EAN8ENGINE_H

