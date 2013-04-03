/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Ian gmail <ianhollander at gmail.com>

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


#include "indexedregexp.h"

using namespace shared;

IndexedRegExp::IndexedRegExp(const QString &pattern) :
    m_sourceString(),
    m_RegExpPattern(pattern),
    m_capCounter(0),
    m_CapturedTexts()
{    
  // empty
};

bool IndexedRegExp::setSourceString(const QString & sourceString) const {
  m_sourceString = sourceString;
  m_CapturedTexts.clear();
  m_startingIndexArray.clear();
  m_capCounter = 0;
  
  QRegExp tRegExp(m_RegExpPattern);
  int nextPos = tRegExp.indexIn(sourceString, 0);
  m_CapturedTexts = tRegExp.capturedTexts();
  while ((nextPos = tRegExp.indexIn(sourceString, nextPos)) != NOT_FOUND) {
    m_startingIndexArray.push_back(nextPos);
  }
  if (m_CapturedTexts.isEmpty()) {
    m_capCounter = NOT_FOUND;
  }
}; 

int IndexedRegExp::getMatchingIndex() const {
  return m_startingIndexArray.at(m_capCounter);
}

QString IndexedRegExp::getMatchingString() const {
  return m_CapturedTexts.at(m_capCounter);
}

bool IndexedRegExp::operator== (const IndexedRegExp& lh) const {
  return ((lh.m_RegExpPattern == m_RegExpPattern) && 
  (lh.m_sourceString == m_sourceString));
} 

int IndexedRegExp::operator() () const{
  return getMatchingIndex();
}

void IndexedRegExp::nextMatch() const{
  if (m_capCounter < m_startingIndexArray.size()) {
    m_capCounter++; 
  }
}

bool IndexedRegExp::isValid() const {
  return (getMatchingIndex() > NOT_FOUND);
};

