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

#ifndef PRISON_UPCABARCODE_H
#define PRISON_UPCABARCODE_H

#include <prison/abstractbarcode.h>

namespace prison {

/**
 * UPC-A barcode generator. 
 * 
 * Supports supplemental(EAN-2,EAN-5) code as defined in the EAN-13 standard.  
 */
class PRISON_EXPORT UpcABarcode : public prison::AbstractBarcode
{
  public:
     /**
     * creates a UPC-A barcode generator object
     * 
     * @todo 
     */
    UpcABarcode();
    /**
     * destructor
     */
    virtual ~UpcABarcode();
    /**
    * This is the function doing the actual work in drawing the barcode on a QImage object.
    * 
    * @param size The requested size of the barcode, 
    * @return QImage containing a scaled image of the UPC-A barcode
    */
    virtual QImage toImage(const QSizeF& size);
    
  private:
    class Private;
    Private *d;    
};
};

#endif // PRISON_UPCABARCODE_H
