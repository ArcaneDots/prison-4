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

#include <algorithm>
#include "ean13engine.h"

using namespace product;

Ean13Engine::Ean13Engine(const QString& defaultString, 
	 int minLength, 
	 int maxLength, 
	 int checkDigitOffset, 
	 int blockSize,
	 upc_common::PRODUCT_CODE_VALUES productCode): 
	 UpcAEngine(defaultString,
		 minLength,
		 maxLength,
		 checkDigitOffset,
		 blockSize,
		 productCode)
{
  qDebug("Ean13Engine constructor");
}

Ean13Engine::~Ean13Engine()
{
  qDebug("Ean13Engine destructor");
} 

void Ean13Engine::initialize()
{
  qDebug("Ean13Engine initialize");
  product::ProductEngine::initialize();
  fillWidthEncodingList();
}

QStringList Ean13Engine::toUpcE() const
{
  qDebug("Ean13Engine toUpcE");
  if (m_userSymbols.at(1) == "0" || m_userSymbols.at(1) == "1") {
    return compressUpc(m_userSymbols.mid(1));
  }
  return QStringList();
}

QStringList Ean13Engine::toUpcA() const
{  
  qDebug("Ean13Engine toUpcA");
  if (m_userSymbols.at(0) == "0") {
    return m_userSymbols.mid(1);
  }
  return QStringList();
}

QStringList Ean13Engine::toEan13() const
{
  qDebug("Ean13Engine toEan13");
  return m_userSymbols;
}

QString Ean13Engine::getFirstBlockEncodePattern(int indexedPattern) const
{
  qDebug("Ean13Engine encodeMainBlock() : EAN-13 pattern "); 
  Q_ASSERT(indexedPattern >= 0 && 
    indexedPattern <= m_parity13WidthEncoding.size()); 
  return m_parity13WidthEncoding.at(indexedPattern);
}

void Ean13Engine::fillWidthEncodingList()
{  
  qDebug("Ean13Engine fillWidthEncodingList() : start");
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    m_parity13WidthEncoding.append(ean13::PARITY_13[i]);
  }  
  qDebug("Ean13Engine fillWidthEncodingList() : end");
}

