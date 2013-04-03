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
#include "cellblock/product/ean8engine.h"
#include "ean8barcode.h"

using namespace prison;

/**
 * @cond PRIVATE
 */
class Ean8Barcode::Private : public product::Ean8Engine 
{
 public:
    Private();
    virtual ~Private(); 
    
    QStringList formatedText;
    QString oldData;
    QImage oldImage;  
};
/**
 * @endcond
 */

Ean8Barcode::Private::Private() :
  formatedText(),
  oldData(),
  oldImage()
{

}
Ean8Barcode::Private::~Private()
{
  // empty
}


Ean8Barcode::Ean8Barcode() : 
  d(new Ean8Barcode::Private())
{    
  // empty
}

Ean8Barcode::~Ean8Barcode() 
{
  delete d;
}

QImage Ean8Barcode::toImage(const QSizeF& size) 
{ 
  qDebug() << "Ean8Barcode::toImage() : data " << data();
  if (!data().isEmpty()) {
    d->setBarcodeString(data());
  }
  QSizeF currentMinimumSize(minimumSize());
  QImage image(d->getImage(size, currentMinimumSize, 
			   foregroundColor(), backgroundColor()));
  setMinimumSize(currentMinimumSize);
  return image;
}


