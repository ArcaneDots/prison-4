/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Ian Hollander <ianhollander@gmail.com>

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


#ifndef EAN5ENGINE_H
#define EAN5ENGINE_H

#include "ean2engine.h"

namespace product
{
/**
 * 5 Digit Add-on symbol - often used for ISBN numbers
 *  
 * Can be appended to UPC A/E and EAN-13
 */ 
class Ean5Engine : public Ean2Engine
{

public:
    Ean5Engine(const QString& defaultString = ean5::DEFAULT_VALUE,
	       int blockSize = ean5::BLOCK_SIZE);
    virtual ~Ean5Engine();
    /**
   * Calculate EAN checksum digit used for parity encoding 
   * 
   * right to left, starting with  right-most value as odd, odd * 3, even * 1
   * 
   * @note Will not check whether supplied string has an invalid length. 
   * 
   * @param symbolArray array of symbol indexes
   * @return valid check digit
   */
  virtual int calculateCheckDigit(const shared::LookupIndexArray &symbolArray) const;
};
};
#endif // EAN5ENGINE_H
