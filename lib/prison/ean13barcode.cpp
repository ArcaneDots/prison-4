/*
    Copyright (c) 2011 Ian Hollander <ian.hollander at gmail.com>

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

#include <QMap>
#include <QPainter>
#include "ean13barcode.h"
#include "cellblock/product/ean13engine.h"

using namespace prison;

/**
 * @cond PRIVATE
 */
class Ean13Barcode::Private : product::Ean13Engine{
public:
  Private();
  Private(const QString &data);
  ~Private();
};
/**
 * @endcond
 */

Ean13Barcode::Private::Private() :
  Ean13Engine()
{  
}

Ean13Barcode::Private::Private(const QString& data) :
  Ean13Engine(data)
{  
}

Ean13Barcode::Private::~Private()
{
}

// ------------------------------------------------------

Ean13Barcode::Ean13Barcode() : AbstractBarcode(), d(0)  
{ 
  qDebug("Ean13Barcode default constructor");
}

Ean13Barcode::~Ean13Barcode() {
  qDebug("Ean13Barcode destructor");
  delete d; 
}

QImage Ean13Barcode::toImage(const QSizeF& size) 
{  
  qDebug() << "Ean13Barcode::toImage() : data " << data();
  product::Ean13Engine ean13(data());

  QSizeF currentMinimumSize(minimumSize());
  QImage image(ean13.image(size, currentMinimumSize,
	     foregroundColor(), backgroundColor()));
  setMinimumSize(currentMinimumSize);
  return image;
}
//qDebug() << "Ean13Barcode::toImage() : ean13 data " << d->localEngine->userInput();
// QImage image;
//   qDebug() << "Ean13Barcode::toImage() : data " << data();
//   //qDebug() << "Ean13Barcode::toImage() : ean13 data " << d->localEngine->userInput();
//   
//   if (d != 0 && d->localEngine->userInput() != data()) {
//     qDebug() << "Ean13Barcode : old data " << d->localEngine->userInput();
//     delete d;
//     d = new Private(data());
//   }
//   
//   QSizeF currentMinimumSize(minimumSize());
//   QImage image(d->localEngine->image(size, currentMinimumSize, 
// 			   foregroundColor(), backgroundColor()));  
//   setMinimumSize(currentMinimumSize);
//   return image;
// }
