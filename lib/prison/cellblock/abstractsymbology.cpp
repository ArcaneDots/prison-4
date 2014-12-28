/*
 * Copyright (c) 2013 ian <email>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <algorithm>
#include <iterator>
#include <numeric>

#include "abstractsymbology_p.h"
#include "product/upceandefines_p.h"

#include <QRegExp>




namespace linearSymbology 
{

AbstractSymbology::AbstractSymbology() : 
  d_ptr(new AbstractSymbologyPrivate) 
{ /* Empty */ }

AbstractSymbology::AbstractSymbology(AbstractSymbologyPrivate& d) : d_ptr(&d)
{ /* Empty */ }

AbstractSymbology::~AbstractSymbology()
{ /* Empty */ }

QString AbstractSymbology::getName() const {return QString("Abstract");}

QString AbstractSymbology::errorEncoding() const { return "1010101"; }

Symbol AbstractSymbology::getSymbol(int userIndex, QString symbolSet)
{
  Q_D(const AbstractSymbology);
  return Symbol(d->findNode(userIndex, symbolSet));
}

Symbol AbstractSymbology::getSymbol(QString userString, QString symbolSet)
{
  Q_D(const AbstractSymbology);
  return Symbol(d->findNode(userString, symbolSet));
}

QList<Symbol> AbstractSymbology::parse(const QString& userInput) const
{
  Q_D(const AbstractSymbology);
  return d->parse(userInput); 
}

QList<Symbol> AbstractSymbology::parse(const QStringList& userInput) const
{
   Q_D(const AbstractSymbology);
   return d->parse(userInput);
}

SymbolNode const * AbstractSymbology::findNode(const QString& userSymbol,
					QString symbolSet) const
{  
  Q_D(const AbstractSymbology);
  return d->findNode(userSymbol, symbolSet);
}

SymbolNode const * AbstractSymbology::findNode(int index, 
					       QString symbolSet) const
{ 
  Q_D(const AbstractSymbology);
  return d->findNode(index, symbolSet);
}

QDebug operator<<(QDebug& dbg, const AbstractSymbology& s)
{ 
  dbg << s.getName(); 
  return dbg;
}

QList<Symbol> operator<<(const AbstractSymbology& symbols,
			 const QString& userInput)
{
  return symbols.parse(userInput);
}

QList<Symbol> operator<<(const AbstractSymbology& symbols, 
			 const QStringList& userInput)
{
  return symbols.parse(userInput);
}


}


  


