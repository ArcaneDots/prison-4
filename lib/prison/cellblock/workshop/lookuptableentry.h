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


#ifndef LOOKUPTABLEENTRY_H
#define LOOKUPTABLEENTRY_H

#include <cstdio>
#include <cmath>
#include <iostream> 
#include <cstdlib>

#include <algorithm>
#include <map>
#include <QMap>
//#include <iterator>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "shareddefines.h"

using namespace std;
/**
 * One key -> value
 */
typedef map<const QString, const QString> String_Pairs;

/**
 * String_Pairs debug handler
 */
inline QDebug operator<<(QDebug debug, const String_Pairs &pair)
{
  String_Pairs::const_iterator itrPairs = pair.begin();
  while (itrPairs != pair.end()) {
    debug.space() << "String_Pair(" << itrPairs->first << ',' << itrPairs->second << ')';
    itrPairs++;
  }
  return debug.space();
}

/**
 * internal key name used when none specified
 * 
 * @note only use when storing one pair per map
 */
static const QString defaultSet("[_default_]");

/**
 * Collection of named object lists used to simulate a Multi-line table entry
 * 
 * @note stored in a map<const QString, Class T>
 */
template<typename T>
class LookupTableEntry 
{

public:
  typedef const QString                           	key_type;
  typedef const QList<T>                         	mapped_type;
  typedef std::pair<key_type, mapped_type>         	value_type;
  typedef std::map<key_type, mapped_type>   		_multiT;
  typedef mapped_type TList;
  typedef typename _multiT::iterator             iterator;
  typedef typename _multiT::const_iterator       const_iterator;
  typedef _multiT&             reference;
  typedef _multiT const&       const_reference;
  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;

private:
  _multiT m_storage;
  
public:
  // iterator support
  /// begin() - iterator
  iterator begin() { return m_storage.begin(); }
  /// end() - iterator
  iterator end() { return m_storage.end(); }
  /// begin() - const_iterator
  const_iterator begin() const { return m_storage.begin(); }
  /// end() - const_iterator
  const_iterator end() const { return m_storage.end(); }
  
  size_type size() const { return m_storage.size(); }
  bool empty() const {return m_storage.empty(); }
  /**
   * Default constructor
   */
  LookupTableEntry() :
    m_storage()
  {
    // empty
  }
  /**
   * Copy constructor
   * 
   * @param other LookupTableEntry<T> object to be copied
   */
  LookupTableEntry(const LookupTableEntry<T>& other) :
    m_storage(other.m_storage)
  {
    // empty
  }
  /**
   * Copy single named (defaultSet) set of data
   * 
   * @param source data set
   */
  LookupTableEntry (const QList<T>& source) : 
    m_storage()
  {
    addRow (source);
  }
  /**
   * Copy single named (defaultSet) set of objects
   * 
   * @param key string data set id 
   * @param source data set
   */
  LookupTableEntry (const QString& key, const QList< T >& source) :
    m_storage()
  {
    addRow (key, source);
  }
  /**
   * Destructor
   */
  ~LookupTableEntry(){}
  /**
   * Is empty
   */
  bool isEmpty() const {m_storage.empty();}  
  /**
   * Add list of data as default data set
   * 
   * @param source data set
   */
  bool addRow (const QList<T> & rowEntry)
  {
    return addRow (defaultSet, rowEntry);
  }
  /**
   * Add list of data at a specified key
   * 
   * @param key string data set id 
   * @param source data set
   */
  bool addRow (const QString & rowName, const QList<T> & rowEntry)
  {
    bool success = false;      
    if (!rowName.isEmpty() && !rowEntry.isEmpty()) {
      if (m_storage.find(rowName) == m_storage.end()) {
        //qDebug() << "inserting " << rowEntry << " at " << rowName << endl;
	m_storage.insert(std::pair<const QString, const QList<T> >(rowName, rowEntry));
        success = true;
      }    
    }
    return success;
  }
  /**
   * Remove named set 
   * 
   * @param key string data set id (or default data set if left blank)
   * @returns removed list
   */
  const QList< T > removeRow ( const QString& rowName = defaultSet )
  {
    QStringList returnValue;
    if (!this->empty() || !rowName.isEmpty()) {
      typename std::map<const QString, const QList<T> >::iterator itr = m_storage.find(rowName);
      if (itr != this->end()) {
        returnValue = itr->second;
        m_storage.erase (itr);
      }
    }
    return returnValue;
  }
  /**
   * Get a list of stored set names
   * 
   * @returns QList<QString> of data set keys
   */
  QList<QString> getRowNames() const
  {
    QList<QString> keys;
    const_iterator itr = m_storage.begin();
    while (itr != m_storage.end()) {
      keys.push_back(itr->first);
      itr++;
    }
    std::sort(keys.begin(), keys.end());
    return keys; 
  }
  /**
   * Get copy of the current set
   * 
   * @returns copy of list data
   */
  const QList< T > getRow( const QString& rowName = defaultSet) const
  {
    QList<T> returnValue;
    if (!empty() && !rowName.isEmpty()) {
      const_iterator itr = m_storage.find(rowName);
      if (itr != end()) {
        returnValue = itr->second;
      }
    }
    return returnValue;
  }
  /**
   * find element by matching associated string
   * 
   * @param searchString
   * @param key 
   */
  T findElement(const QString& searchString, 
		const QString& key = defaultSet ) const;
  /**
   * find element at specified index
   * 
   * @param searchString
   * @param key 
   */		
  T findElement(int index, const QString &key = defaultSet) const;
  /**
   * get matching pair <set name, T value> from named set at the same index
   * 
   * @param index
   */
  std::map<const QString, T> getMatchPairsAtIndex(int index) const
  {
    std::map<const QString, T> matchingPairs;
    
    const_iterator itrSet = m_storage.begin();
    while ( itrSet != m_storage.end()) {
      if ( itrSet->second.size() > index) {
        matchingPairs.insert(std::make_pair<const QString, T>(
          itrSet->first, 
          itrSet->second.at(index)));
      }
      itrSet++;
    }
    return matchingPairs;
  }
  /**
   * operator=
   * 
   * @param other compared LookupTableEntry<T> object
   */
  virtual LookupTableEntry& operator=(const LookupTableEntry& other)
  {
    m_storage = other.m_storage;
    return *this;
  }
  
  /**
   * all keys are matched and each key pair has the same number of elements 
   * 
   * @param other compared LookupTableEntry<T> object
   */
  bool matchingKeys (const LookupTableEntry& other) const
  {
    return (getRowNames() == other.getRowNames());
  }    
  /**
   * matching lengths
   * 
   * @param other compared LookupTableEntry<T> object
   */
  bool matchingContents(const LookupTableEntry& other, QString key) const
  {    
    return (getRow(key) == other.getRow(key));
  }    
  void swap (LookupTableEntry& lhs, LookupTableEntry& rhs)
  {
    if (rhs != &lhs) {
      _multiT::swap(rhs.m_storage, lhs.m_storage);          
    }
  }
  /**
   * Get string associated with an element
   * 
   * @note fuction must be specialized in order for a class functions to work 
   * 
   * @param t object to be processed
   * @returns string equivalent
   */
  QString getString(T t) const;
  /**
   * ostream iterator of type stored in lookuptableentry object 
   */
  typedef std::ostream_iterator<T> output_iterator_t;

private:
  /**
   * Actual search by string function
   * 
   * @returns empty object (or -1 for pointers) when element not found 
   */
  bool _findElement(T & result, const QString& searchString, const QString& setName = defaultSet ) const
  {
    bool foundElement = false;
    const_iterator itrSetList = m_storage.find(setName);
    if (itrSetList != m_storage.end()) {
      typename mapped_type::const_iterator itrResult = itrSetList->second.begin();
      while (itrResult != itrSetList->second.end()) {
	if (foundElement =(searchString == getString(*itrResult))) {
	  result = *itrResult;
	  break;
	}
	itrResult++;
      }
    }
    return foundElement;
  }
  /**
   *  Actual search by list index function 
   */
  bool _findElement(T & result, 
		    int index, 
		    const QString &setName = defaultSet) const
  { 
    bool foundElement = false;
    const_iterator itrSetList = m_storage.find(setName);
    if (itrSetList != m_storage.end() && 
    index > 0 && index < itrSetList->second.size()) {
      result = *(itrSetList->second.begin() + index);      
    }    
    return foundElement;
  }
};

 /**
   * Equals operator
   * 
   * Considered equal when the compared LookupTableEntry<T> objects
   * contains a complete set of matching keys;
   * and each list associated with the keys contain an 
   *  equal number of data elements 
   * each corresponding element must be  that is considered equal 
   *  by the data's class. 
   * the associated "value" contain the same number of entries,
   * otherwise all pair "value"s must have the same number of entries
   * @sa matchingKeys
   * @sa matchingLengths
   * 
   * @param other compared LookupTableEntry<T> object
   * @return return true if exact copy
   */
template<class T>
bool operator==(const LookupTableEntry<T>& lhs, const LookupTableEntry<T>& rhs)
{
  // match "set" keys 
  bool returnValue = lhs.matchingKeys(rhs);
  // match "set" contents 
  if (returnValue) {
    QList<QString> keys(rhs.getRowNames());
    for (int index = 0; returnValue && index < keys.size(); index++) {
      returnValue = rhs.matchingContents(rhs, keys.at(index));
    }
  }
  return returnValue;
}; 

/**
 * SymbolList -> ostream
 */
template<class T>
std::ostream& operator<<(std::ostream& os, 
                         const LookupTableEntry<T> &lt)
{
  typename LookupTableEntry<T>::output_iterator_t out_it (os,", ");
  QList<QString> keys(lt.getRowNames());
  QList<QString>::const_iterator itrSet = keys.begin();
  while(itrSet != keys.end()) {
    QList<T> contents(lt.getRowContents(*itrSet));
    os << " key : " << os << itrSet->toStdString() <<  os << " list : ";
    std::copy ( contents.begin(), contents.end(), out_it );
  }
}
/**
 * SymbolList -> QDebug stream
 */
template<class T>
QDebug operator<<(QDebug debugString, const LookupTableEntry<T> &lt)
{  
  typename LookupTableEntry<T>::_multiT::const_iterator itrSet = lt.begin();
  while(itrSet != lt.end()) {
    debugString.nospace() << " key : " << itrSet->first;
    if (itrSet->second.isEmpty()) {
      debugString.space() << "empty";
    } else {
      typename LookupTableEntry<T>::mapped_type::const_iterator 
	itrSetList = itrSet->second.begin();
      debugString.space() << "list : " << " (";
      while (itrSetList != itrSet->second.end()) {
	debugString.space() << *itrSetList++ << " ";
	if (itrSetList != itrSet->second.end()) { 
	  debugString.nospace() << ", "; 	  
	}
      }
      debugString.nospace() << " ) ";
    }
    itrSet++;
  }  
  return debugString.nospace();
}


#endif // LOOKUPTABLEENTRY_H
