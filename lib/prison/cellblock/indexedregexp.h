/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Ian gmail <ianhollander@gmail.com>

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


#ifndef INDEXEDREGEXP_H
#define INDEXEDREGEXP_H

#include <vector>
#include <QtCore/QString>
#include <QtCore/QStringList>

/**
 * Default value for most class data members
 */
const int NOT_FOUND = -1;

namespace shared
{
  /**
   * Regular expression results storage object
   */
  class IndexedRegExp 
  {
  public:
    /**
     * Initialize the object with the regexp string
     * 
     * @param pattern string containing a regexp formated search pattern
     */
    IndexedRegExp(const QString &pattern);
    /**
     * Generate and store locations and text that is found in the source string
     * 
     * @param sourceString string to be searched
     * @return pattern was found in sourceString
     */
    bool setSourceString(const QString & sourceString) const;
    /**
     * Get the current index where the sourceString matches the pattern
     * 
     * @note "current" match is nth match based on a internal counter
     * @return current index
     */
    int getMatchingIndex() const;
    /**
     * Get the contents of the current match pattern in sourceString 
     * 
     * @note "current" match is nth match based on a internal counter
     * @return copy of matching string 
     */
    QString getMatchingString() const;
    /**
     * operator==
     * 
     * @param rh right-hand IndexedRegExp object argument
     * @returns true in case rh and current IndexedRegExp objects are "equal"
     */
    bool operator== (const IndexedRegExp& rh) const; 
    /**
     * operator()
     * 
     * @returns matching index
     */
    int operator() () const;
    /**
     * Advance object's "nextMatch" pointer
     */
    void nextMatch() const;
    /**
     * Get object's validity
     * 
     * A IndexedRegExp object is considered "valid" 
     * as long as there is current matching 
     * 
     * @returns true when matches remaining
     */
    bool isValid() const;
  private:
    mutable QString m_sourceString;
    mutable QStringList m_CapturedTexts;
    QString m_RegExpPattern;
    mutable int m_capCounter;  
    mutable std::vector<int> m_startingIndexArray;
  }; 
  
  /**
   * operator<
   * 
   * Define operator "<" so IndexedRegExp objects can be sort "correctly"
   * Move objects that empty or "exhusted" to the left and sort the remaining
   * object on the right.
   * 
   * @param lh left-hand IndexedRegExp object
   * @param rh right-hand IndexedRegExp object
   * @return is the left-hand IndexedRegExp object less than the on the right
   */
  static bool operator<(const IndexedRegExp& lh, const IndexedRegExp& rh) 
  {
    bool result = false;
    // sort normally
    if (rh.isValid() && lh.isValid()) {    
	result = (lh.getMatchingIndex() < rh.getMatchingIndex());
    // move valid objects to the left
    } else if (lh.isValid()) {
	result = true;
    }  
    return result;
  };
}
#endif // INDEXEDREGEXP_H
