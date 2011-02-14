/*
    Copyright (c) 2010-2011 Sune Vuorela <sune@vuorela.dk>

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

#ifndef PRISON_DATAMATRIXBARCODE_H
#define PRISON_DATAMATRIXBARCODE_H

#include <prison/abstractbarcode.h>
#include <prison/prison_export.h>

namespace prison {

class PRISON_EXPORT DataMatrixBarcode : public prison::AbstractBarcode {
  public:
    /**
     * creates a datamatrixbarcode generator
     */
    DataMatrixBarcode();
    virtual ~DataMatrixBarcode();
    virtual QImage toImage(const QSizeF& size);
  private:
    class Private;
    Private *d;
};
};

#endif // PRISON_DATAMATRIXBARCODE_H
