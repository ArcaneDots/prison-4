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

#ifndef PRISON_EAN8BARCODE_H
#define PRISON_EAN8BARCODE_H

//#include <prison/abstractbarcode.h>
#include <prison/ean13barcode.h>
#include <prison/prison_export.h>

namespace prison {

/**
 * Ean-8 barcode generator. 
 * 
 * Supports the compressed EAN product (EAN-8) 
 * based on seperate database of values.
 * As well as supplemental(EAN-2,EAN-5) code as defined in the EAN-13 standard.  
 */
class PRISON_EXPORT Ean8Barcode : public Ean13Barcode//public prison::AbstractBarcode
{
  public:
     /**
     * creates a EAN 8 barcode generator
     */
    Ean8Barcode();
    /**
     * destructor
     */
    virtual ~Ean8Barcode();
    /**
    * This is the function doing the actual work in drawing the barcode on a QImage object.
    * 
    * @param size The requested size of the barcode, 
    * @return QImage containing a scaled image of the EAN-8 barcode
    */
    virtual QImage toImage(const QSizeF& size);
    
  private:
    class Private;
    Private *d;    
};
};

#endif // PRISON_EAN8BARCODE_H
