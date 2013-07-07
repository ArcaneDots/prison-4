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

#include <QtDebug>
#include "ean8engine.h"

using namespace product;
using namespace shared;

Ean8Engine::Ean8Engine(const QString &userBarcode, 
		       CodeEngine::ConstructCodes flags): 	
	ProductEngine(userBarcode, 
		flags,
		ean8::DEFAULT_VALUE,
		ean8::MIN_8,
		ean8::MAX_LEN,
		ean8::CHECK_DIGIT_OFFSET,
		ean8::BLOCK_SIZE,
		ean8::ENCODE_BLOCK_SIZE,
		upc_common::PS__EAN_8)
{
  qDebug("Ean8Engine constructor");
  ProductEngine::initialize();
  ProductEngine::setBarcodeString();
}

Ean8Engine::~Ean8Engine()
{
  qDebug("Ean8Engine destructor");
}

QStringList Ean8Engine::getSymbolList() const
{
  return m_userParsedSymbols;
}
