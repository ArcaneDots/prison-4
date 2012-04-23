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


#ifndef SYMBOLLIST_H
#define SYMBOLLIST_H

#include <iterator>
#include <iostream>

#include <QList>
#include <QStringList>
#include <QtCore/QByteArray>

#include "shareddefines.h"
#include "barcodesymbolset.h"
#include "symbol.h"

using namespace barcodeEngine;

namespace BlockNames
{
  /**
   * 
   * @note any symbols in Check_Value(_$) must be treated a "base" symbols
   */
  enum SubStrings{
    NOT_DEFINED = -1, 
    NONE = 0,
    PREFIX,
    MAIN_CODE,
    MANUFACTOR,
    PRODUCT,
    CHECK_VALUE,
    CHECK_VALUE_C,
    CHECK_VALUE_K,
    EXTENDED_CODE
  };
};


  typedef QSharedDataPointer<Symbol> SharedSymbol;
/**
 * List of symbols with block start markers
 * 
 * will automaticly insert the parsed copy of any string only symbols inserted
 * 
 * insert: can not insert its own nodes
 * erase: blockstarts in range are deleted unless adjustment is posible
 * 
 * doubly-linked list with subsection pointers. 
 */
class SymbolList 
{
  friend bool hasSameSet(const SymbolList &lhs, const SymbolList &rhs);
  friend bool isRelated(const SymbolList &lhs, const SymbolList &rhs);
  friend bool operator == (const SymbolList &lhs, const SymbolList &rhs);
 
private:
  class LinkedList;
  struct list_node;
  typedef QSharedDataPointer<SymbolList::LinkedList> SharedList;
  SharedList d;
public:
  // iterator(s)
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef int size_type;
  typedef Symbol value_type;
  typedef std::ptrdiff_t       difference_type;
  typedef Symbol* pointer;
  typedef const Symbol* const_pointer;
  typedef Symbol& reference;
  typedef const Symbol& const_reference;
  /**
   * Common iterator base class
   * 
   * http://drdobbs.com/cpp/184401331
   */

public:
  class const_iterator : public  std::iterator<iterator_category, 
	  value_type, 
	  difference_type, 
	  pointer, 
	  reference>
  {
    friend class SymbolList;
    /**
     * node constructor
     * 
     * @param x current node
     */
    const_iterator (list_node * x) : _curpos(x) { }
  public:
    /// copy constructor
    const_iterator (const const_iterator& i) : _curpos(i._curpos) { }
    /// destructor
    virtual ~const_iterator() {};
    /**
     * *operator
     * 
     * @returns const reference to node's value
     */
    const_reference operator*() const; 
    /**
     * operator->
     * 
     * @returns const pointer to node's value
     */
    const_pointer operator->() const; 
    /// operator==
    bool operator==(const_iterator rhs) const { return _curpos == rhs._curpos; }
    /// operator!=
    bool operator!=(const_iterator rhs) const { return !operator==(rhs); }
    /// operator= : assignment operator
    virtual const_iterator operator= (const const_iterator & x) {
      _curpos = x._curpos;
      return *this;      
    }
    /**
     * operator()++ : move a step forward - postfix
     * 
     * @note does not allow pointer to fall off end of list
     */
    const_iterator & operator++();
    /// operator++() : move a step forward - prefix
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      tmp++;
      return tmp;
    }
    /// operator+= : Move x steps forward
    const_iterator & operator+=(int size) {
      if (size > 0) {
	++*this;
	return operator+=(--size);
      }
    }
    /**
     * operator()-- : move a step back - postfix
     * 
     * @note does not allow pointer to fall off front of list
     */
    const_iterator & operator--();
    /// operator--() : move a step forward - prefix
    const_iterator operator--(int) {
      const_iterator tmp(*this);
      tmp--;
      return tmp;
    }
    /// operator-= : Move x steps back
    const_iterator operator-=(int size) {
      if (size > 0){
	--*this;
	return this->operator-=(size--);
      }
    }
    BlockNames::SubStrings getSectionName() const;
    bool setSectionName(BlockNames::SubStrings sectionName);
    
  protected:
    /// pointer to current node
    list_node* _curpos;    
  };
  class iterator : public const_iterator
  {    
  public:        
    friend class SymbolList;
    friend SymbolList::list_node* cutNodes(iterator first, 
					   iterator last);
    /**
     * node constructor
     * 
     * @param x current node
     */
    iterator (list_node * x) : const_iterator(x) { }
    /// copy constructor
    iterator (const iterator& i) : const_iterator(i._curpos) { }
    /// copy const_iterator constructor 
    iterator (const const_iterator& i) : 
      const_iterator(const_cast<list_node *>(i._curpos)) { }
    /// destructor
    virtual ~iterator() {};
    /**
     * *operator
     * 
     * @returns const reference to node's value
     */
    reference operator*() const;
    /**
     * operator->
     * 
     * @returns const pointer to node's value
     */
    pointer operator->() const;
    /// operator= : iterator assignment operator
//     virtual iterator operator= (const iterator & x) {
//       std::swap<list_node *>(_curpos, const_cast<iterator *>(&x)->_curpos);
//       return *this;      
//     }
    bool operator==(iterator rhs) const { return (_curpos == rhs._curpos);  } 
    reference operator= (const iterator& i); 
    /// operator= : assignment operator "value"
    reference operator= (const Symbol & s);
    /// operator= : assignment operator "subsection"
    reference operator= (BlockNames::SubStrings section);
    /**
     * operator +=
     * 
     * explict "iterator" version reqiured by std::reverse_copy()
     */
    iterator & operator+= (int i) {return this->operator+=(i);}
    /**
     * operator -=
     */
    iterator & operator-= (int i) {return this->operator-=(i);}
  };
  
  /// iterator to list begin()
  iterator m_head;
  /// iterator to list end()
  iterator m_tail;
  /**
   * Named Section
   * 
   * all node in this subrange are part of the named section 
   */
  BlockNames::SubStrings m_currentBlock;
  protected:
  
 
public:
  /// begin() - const_iterator 
  const_iterator begin() const;
  /// end() - const_iterator 
  const_iterator end() const;
  /// begin() - iterator 
  iterator begin();
  /// end() - iterator 
  iterator end();
  
public:
  /**
   * default constructor
   */
  SymbolList();
  /**
   * copy constructor
   */
  SymbolList(const SymbolList& other);
  /**
   * Symbol copy constructor
   */
  SymbolList(const Symbol& symbol);  
  SymbolList(const BarcodeSetInfo &barcodeSymbol); 
  SymbolList(const BarcodeSetInfo &barcodeSymbol, const QString& symbols);
  
  typedef std::pair<SymbolList::const_iterator, SymbolList::const_iterator> SubRange;
protected:
  /**
   * Special explict sub-range SymbolList "copy" constructor
   * 
   * @note only usable by mid()
   * @sa mid()
   * 
   * @param parent "parent" SymbolList object
   * @param subRange pair of const_iterators denoting subrange
   */
  SymbolList(const SymbolList::SharedList& listData, 
	     SymbolList::SubRange subRange );
  /**
   * "Named section" sub-range SymbolList  "copy" constructor
   * 
   * @note only usable by SymbolList.[named section]()
   * @sa prefix(), main_code(), manf(), product(), check_value()
   * @sa extended_code(), check_value_C(), check_value_K();
   * 
   * @param parent "parent" SymbolList object
   * @param subRange pair of const_iterators denoting subrange
   */  
  SymbolList(const SymbolList::SharedList& listData, 
	     BlockNames::SubStrings sectionName = BlockNames::NONE);
public:
  /**
   * destructor
   */
  virtual ~SymbolList();
  
  //friend class SymbolList::iterator;

  /**
   * number of Symbol nodes
   */
  size_t size() const;
  /**
   * list does not contain any Symbols
   */
  bool empty() const;
  /**
   * Has a copy of the symbol lookup table
   */ 
  bool isValid() const;
  
  //const_iterator getSectionStart(BlockNames::SubStrings name);
  
  //bool setSectionStart(BlockNames::SubStrings name, const_iterator & start);
public: 
  /**
   * Move nodes all nodes from x -> current SymbolList
   */
  void splice ( iterator position, SymbolList& x );
  /**
   * Move single node from x -> current SymbolList
   */
  void splice ( iterator position, SymbolList& x, iterator i );
  /**
   * Move nodes a range of nodes from x -> current SymbolList
   */
  void splice ( iterator position, SymbolList& x, 
		iterator first, iterator last );

  /**
   * insert generic object 
   * @param position location to start insertion
   * @returns location of inserted node
   */
  template<class T>
  iterator insert ( iterator position, const T& t) {
    qDebug("SymbolList insert(single object)");
    return _insert(position, t);
  };  
  /**
   * insert multiple symbols
   */
  template<class T>
  void insert ( iterator position, int n, const T &t )
  {
    qDebug("SymbolList insert(multple objects)");
    for (int count = 0; count < n; count++) {
      _insert(position, t);
    }
  }
  /**
   * insert range of symbols
   */
  template <class InputIterator>
  void insert ( iterator position, InputIterator first, InputIterator last )
  {
    qDebug("SymbolList insert(range of objects)");
    _insert(position, first, last);
  };
  
  /**
   * Insert parsable data
   */
  template<class T>
  iterator insert(int position, const T& t)
  {
    qDebug("SymbolList insert(int, single object)");
    return insert(begin() += position, t);
  }
  /**
   * insert multiple symbols
   */  
  template<class T>
  void insert ( int position, int n, const T& t )
  {
    qDebug("SymbolList insert(int, multple objects)");
    return insert(begin() += position, n, t);
  }
  /**
   * insert range of symbols
   */
  template <class InputIterator>
  void insert ( int position, InputIterator first, InputIterator last )
  {
    qDebug("SymbolList insert(int, range of objects)");
    insert(begin() += position, first, last);
  };
  /// insert at front of the list
  template<class T>
  void push_front(T const& t)
  {
    qDebug("SymbolList push_front()");
    _insert(begin(), t);
  }
  /// insert at end of list
  template<class T>
  void push_back(T const& t)
  {
    qDebug("SymbolList push_back()");
    _insert(end(), t);
  }
  /// delete first node of list
  void pop_front();
  /// delete last node of list
  void pop_back();
  
  /**
   * Erase single node
   * 
   * @param position node to be deleted
   * @return position of next node 
   */
  iterator erase ( iterator position );
  /**
   * Erase range of nodes
   * 
   * @param first start of range
   * @param last one node after range    
   * @return position of next node 
   */
  iterator erase ( iterator first, iterator last );
  
  /// delete all nodes in list
  void clear();
protected:     
  /**
   * insert a single Symbol object in an empty list
   * 
   * @param position 
   * @param x symbol to be inserted
   * @returns
   */
  iterator _special_Insert( iterator position, const Symbol& x );
  /**
   * insert a single Symbol object
   * 
   * @param position 
   * @param x symbol to be inserted
   * @returns
   */
  iterator _insert( iterator position, const Symbol& x );
  /**
   * insert a QList<Symbol> object
   * 
   * @param position 
   * @param stringlist list of Symbols to be inserted
   * @returns
   */
  iterator _insert( iterator position, const QList<Symbol> & symbols); 
  /**
   * insert a QStringList object
   * 
   * @param position 
   * @param stringlist list of strings to be inserted
   * @returns
   */
  iterator _insert( iterator position, const QStringList& stringlist ); 
  /**
   * insert a SymbolList object
   * 
   * @param position 
   * @param x SymbolList to be inserted
   * @returns
   */
  iterator _insert( iterator position, const SymbolList& x ); 
  /**
   * Insert single object using integer position
   * 
   * @param position integer position
   * @param t object to be inserted
   */ 
  template<class T>
  iterator _insert ( iterator position, T &t )
  { 
    qDebug("SymbolList _insert(iterator, T) : start");
    iterator previous(position);
    previous--;
    BlockNames::SubStrings sectionId = previous.getSectionName();
    QList<Symbol> symbols(getParserSymbol().parse<T>(t));
    return insert<QList<Symbol> >(position, symbols);
  }  
  /**
   * insert range of symbols
   */
  template <class InputIterator>
  void _insert ( iterator position, InputIterator first, InputIterator last )
  {
    qDebug("SymbolList _insert(iterator, ranage) : start");
    while (first != last) {
      insert(position, first++);
    }
    qDebug("SymbolList _insert(iterator, range) : end");
  };
  
  iterator _insert ( iterator position, list_node * nodes);
public:  
  /// Get section starting and ending iterators
  SubRange getBlockRange( BlockNames::SubStrings id );
  
  SymbolList prefix();
  SymbolList main_code();
  SymbolList manf();
  SymbolList product();
  SymbolList check_value();
  SymbolList extended_code();
  SymbolList check_value_C();
  SymbolList check_value_K();
  

  /**
   * Get element at index
   * 
   * @param index position of element
   */
  const Symbol& at(int index) const;
  /**
   * Get subrange 
   * 
   * @param pos start of range
   * @param length size of range
   * @returns SymbolList of subrange 
   */
  const SymbolList mid(int pos, int length = NOT_FOUND) const;
  /**
   * Get first node in the current SymbolList's range
   */
  const Symbol& front() const;
  /**
   * Get last node in the current SymbolList's range
   */
  const Symbol& back() const;
  /**
   * Get first node in the current SymbolList's range
   */
  Symbol& front();
  /**
   * Get last node in the current SymbolList's range
   */
  Symbol& back();
  /**
   * Create an empty copy of the symbol list
   */
    SymbolList clone() const; 
  /**
   * Assgin range equal to a set of strings
   */
  void assign ( const QStringList & other );
  /**
   * Assgin range equal to the contents of a SymbolList
   */
  void assign ( const SymbolList & other );
  /**
   * Assgin range equal to number of symbols
   */
  void assign ( SymbolList::size_type n, const Symbol& x );
protected:
			 
  Symbol getParserSymbol() const;
  /**
   * Parsable data into a QList<Symbol>
   */
  template<class T>
  QList<Symbol> parse(const T &t) const
  {
    qDebug("SymbolList parse(T)");
    return getParserSymbol().parse<T>(t); //m_symbolParser->parse<T>(t);
  }
  
private:
//   /**
//    * Build list_node segments
//    * 
//    * @param first range "begin"
//    * @param last range "end"
//    * @param sectionId section node will belong to 
//    * 
//    * @note increments current list size
//    */  
//   template<class InputIterator>
//   list_node * buildSegment(InputIterator first, 
// 			   InputIterator last,
// 			   BlockNames::SubStrings sectionId = BlockNames::NONE)
//   {
//     qDebug("SymbolList buildSegment : start");
//     list_node * segmentHead = 0;
//     list_node * segmentTail = 0;
//     //count = 0;
//     while (first != last) {
//       if (segmentHead != 0) {
// 	segmentTail = joinNodes(segmentTail, new list_node(*first++, sectionId));
//       } else {
// 	segmentTail = segmentHead = new list_node(*first++, sectionId);
//       }
//       //m_size++;
//       segmentTail->m_section = sectionId;
//     }
//     qDebug("SymbolList buildSegment : end");
//     return segmentHead;
//   }
//   /**
//    * Join two nodes
//    * 
//    * @param rhs right-hand side of join
//    * @param lhs left-hand side of join  
//    * 
//    * @returns pointer to outer right-hand side node (range "end") 
//    */
//   list_node * joinNodes(list_node * lhs, list_node * rhs) const;
//   /**
//    * Removes segment of nodes from source, assumes range is contigous
//    * 
//    * @param splitStart iterator pointing to the starting node
//    * @param splitEnd  iterator pointing to one node past the end of the range
//    * @return pointer to begin of the list segment on heap
//    */
//   list_node * cutNodes(iterator splitStart, iterator splitEnd);
//   //list_node * cutNodes(const_iterator splitStart, const_iterator splitEnd);  
//   /**
//    * Splice terminated linked-node segment into another segment of linked-node
//    * 
//    * @param position insert position
//    * @param startNodeList start of linked-list nodes to insert 
//    */
//   iterator attachSpaningNodes(iterator position, list_node * startNodeList);
//   /**
//    * Update section ids of the nodes in a range
//    */	 
//   void updateRangeSectionId(iterator first, iterator last,
// 		   BlockNames::SubStrings sectionId);
//   /**
//    * Delete orphoned list of nodes
//    * 
//    * @param segmentHead pointer to first node of the segment
//    * @returns number of nodes deleted
//    */
//   int deleteSegment(list_node * segmentHead);
public:
  
  // operator(s)
  /**
   * Operator== SymbolList
   */
  bool operator==(const SymbolList & rhs) const;
  /**
   * operator= SymbolList
   */
  SymbolList& operator=(const SymbolList & rhs);  
  /**
   * operator= QStringList
   */
  SymbolList& operator=(const QStringList & rhs);    
  
  template<class T>
  SymbolList& operator=(const T & rhs){
    clear();
    insert(end(), parse<T>(rhs));
  }  
  
  template<class T>
  SymbolList& operator+= (const T &symbolinfo)
  {  
    insert(end(), symbolinfo);
    return *this;
  }
   
  template<class T>
  SymbolList operator+ (const T &t) const
  {
    SymbolList s(*this);
    s.insert(s.end(), t);
    return s;
  }
  /**
   * 
   */
  template<class T>
  SymbolList &operator<< (const T &t) 
  {
    push_back(parse<T>(t));
    return *this;
  }  
  /**
   * 
   */
  SymbolList &operator<< (const Symbol &symbol);
  /**
   * 
   */
  bool isSameSymbolSet(const SymbolList &s) const;
  /**
   * get list of values used to generate check symbols
   */
  barcodeEngine::LookupIndexArray getIndexValues() const;
  /**
   * convert symbols into a list of strings
   */
  QStringList toQStringList() const;
  /**
   * convert symbols into QList
   */
  QList<Symbol> toSymbols() const;

protected: 
  /**
   * Swap resources
   */
  void swap(SymbolList & x);
  /**
   * Get symbol set object
   */
  BarcodeSetInfo getFullSymbolSet() const;
};

/**
 * get range size
 */
int getSize(SymbolList::const_iterator begin, SymbolList::const_iterator end);
/**
 * find valid symbol(s) : std::find() helper function
 */
inline bool findValid(SymbolList s) { return s.isValid(); }

template<class T>
SymbolList operator+ (const T &t, const SymbolList &symbollist)  
{
  SymbolList s(symbollist);
  s.insert<T>(s.begin(), t);
  return s;
}
bool hasSameSet(const SymbolList &lhs, const SymbolList &rhs);
bool isRelated(const SymbolList &lhs, const SymbolList &rhs);
bool operator == (const SymbolList &lhs, const SymbolList &rhs);


QList<Symbol> operator<< (SymbolList &lh, const SymbolList &rh );

/**
 * SymbolList -> ostream
 */
std::ostream& operator<<(std::ostream& os, const SymbolList &s);
/**
 * SymbolList -> QDebug stream
 */
QDebug operator<<(QDebug debugString, const SymbolList &s);

/**
 * SubBlock -> QDebug stream
 */
//QDebug operator<<(QDebug debugString, const SymbolList::SubBlock &s);

#endif // SYMBOLLIST_H
