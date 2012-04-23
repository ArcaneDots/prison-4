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


#include "abstract2dbarcodeengine.h"




Abstract2DBarengineFlags::Abstract2DBarengineFlags(const QString& defaultString, 
						 int minLength, 
						 int maxLength, 
						 int checksumModulus, 
						 int checkDigits, 
						 int requiredCheckDigits, 
						 int checkDigitOffset, 
						 int maxSymbolLength,
						 int barStates) : 
						 AbstractBarengineFlags(defaultString,
								       minLength, 
								       maxLength, 
								       checksumModulus,
								       checkDigits, 
								       requiredCheckDigits,
								       checkDigitOffset, 
								       maxSymbolLength), 
						 m_barStates()
{

}

Abstract2DBarengineFlags::~Abstract2DBarengineFlags()
{

}

void Abstract2DBarengineFlags::setBarcodeString(const QString& userBarcode, AbstractBarengineFlags::Options flag)
{
    AbstractBarengineFlags::setBarcodeString(userBarcode, flag);
}

QStringList Abstract2DBarengineFlags::parseSymbolString(const QString& symbolString)
{
    return AbstractBarengineFlags::parseSymbolString(symbolString);
}

void Abstract2DBarengineFlags::formatSymbols()
{
    AbstractBarengineFlags::formatSymbols();
}

void Abstract2DBarengineFlags::encodeSymbols()
{
    AbstractBarengineFlags::encodeSymbols();
}



