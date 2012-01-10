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

#include "ean2engine.h"

using namespace product;

Ean2Engine::Ean2Engine(const QString& defaultString,
		       int blockSize,
		       upc_common::PRODUCT_CODE_VALUES productCode) : 
		       UpcAEngine(defaultString,
					blockSize,
					blockSize,
					NOT_FOUND,
					blockSize,
					productCode)
{
  qDebug("UpcAEngine constructor()"); 
}

Ean2Engine::~Ean2Engine()
{
  // empty
}

QStringList Ean2Engine::processSymbolList(const QStringList &userSymbols)
{
  qDebug("Ean2Engine processSymbolList()"); 
  Q_ASSERT(upc_common::PS__EAN_2 == m_productCode);
  // don't bother processing two digits
  return userSymbols;
}


int Ean2Engine::calculateCheckDigit(const shared::LookupIndexArray& symbolArray) const
{
  qDebug("Ean2Engine calculateCheckDigit()"); 
  return UpcAEngine::calculateEan2CheckDigit(symbolArray);
}

void Ean2Engine::formatSymbols(const QStringList &symbolSrc, int splitIndex)
{
  qDebug("Ean2Engine formatSymbols()"); 
  Q_ASSERT(upc_common::PS__EAN_2 == m_productCode);
  UpcAEngine::formatExtendedBlock(symbolSrc);
  //m_formatedSymbols << m_userSymbols.join("");
}

void Ean2Engine::encodeSymbols(const QStringList &symbolSrc, int splitIndex)
{
  qDebug("Ean2Engine encodeSymbols()"); 
  Q_ASSERT(upc_common::PS__EAN_2 == m_productCode);
  UpcAEngine::encodeExtendedBlock(symbolSrc);
  //UpcAEngine::encodeExtendedDigits(convertSymbolsToIndexes(m_userSymbols));
}




