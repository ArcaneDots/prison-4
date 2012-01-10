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


#include "ean5engine.h"

using namespace product;

Ean5Engine::Ean5Engine(const QString& defaultString,
		       int blockSize): 
		       Ean2Engine(defaultString,
				  blockSize)
{
  //empty
}

Ean5Engine::~Ean5Engine()
{
  // empty
}

int Ean5Engine::calculateCheckDigit(const shared::LookupIndexArray& symbolArray) const
{
    return UpcAEngine::calculateEan5CheckDigit(symbolArray);
}

