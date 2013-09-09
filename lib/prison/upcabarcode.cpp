/*
    Copyright (c) 2011 Ian Hollander <ian.hollander@gmail.com>

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
#include "cellblock/product/upcaengine.h"
#include "upcabarcode.h"

using namespace prison;

/**
 * @cond PRIVATE
 */
class UpcABarcode::Private : public product::UpcAEngine {
 public:   
   Private();
   Private(const QString &data);
   ~Private();
};
/**
 * @endcond
 */

UpcABarcode::Private::Private() :  
  UpcAEngine()
{
}

UpcABarcode::Private::Private(const QString& data) :
  UpcAEngine(data)
{
}

UpcABarcode::Private::~Private()
{
}

// ------------------------------------------------------

UpcABarcode::UpcABarcode()  : AbstractBarcode(), d(0)
{    
  qDebug("UpcABarcode::UpcABarcode()");
  //d = new Private();
}

UpcABarcode::~UpcABarcode()
{   
  qDebug("UpcABarcode::~UpcABarcode()");
  //delete d;
  //if (d != 0) { delete d; }
}

QImage UpcABarcode::toImage(const QSizeF& size) 
{ 
//    qDebug() << "UpcABarcode::toImage() : data " << data();
//   qDebug() << "UpcABarcode::toImage() : upcA data " << d->userInput();
//   
//    product::UpcAEngine l_upca(data());
//   QSizeF currentMinimumSize(minimumSize());
//   QImage image(l_upca.image(size, currentMinimumSize, 
// 			foregroundColor(), backgroundColor()));
//   setMinimumSize(currentMinimumSize);
//   return image;
// }
  qDebug() << "UpcABarcode::toImage() : data " << data();
  //qDebug() << "UpcABarcode::toImage() : upcA data " << d->userInput();
  d = new Private();
    //qDebug() << "UpcABarcode : old data " << d->userInput();
    qDebug("UpcABarcode::toImage() : delete");
//     delete d;
//     d = new Private(data());
  
//   if (d->userInput() != data()) {
//     qDebug() << "UpcABarcode : old data " << d->userInput();
//     delete d;
//     d = new Private(data());
//   }
  
  QSizeF currentMinimumSize(minimumSize());
  QImage image(d->image(size, currentMinimumSize, 
		foregroundColor(), backgroundColor()));
  setMinimumSize(currentMinimumSize);
  //delete d;
  return image;
}
