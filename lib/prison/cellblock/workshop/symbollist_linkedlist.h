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
 
 
#include "symbollist.h"
/**
 * doubly-linked list node
 */
struct SymbolList::list_node 
{
  friend list_node * cutNodes(SymbolList::iterator splitStart, 
			      SymbolList::iterator splitEnd);
  friend list_node * joinNodes(SymbolList::list_node * lhs,
			       SymbolList::list_node * rhs);
public:
  /**
   * default constructor
   */
  list_node() : 
    m_next(0), 
    m_previous(0),
    m_symbol(new Symbol),//),
    m_section(BlockNames::NOT_DEFINED)
  {
    qDebug("SymbolList list_node - default constructor");
  }
  /// copy constructor
  list_node(const list_node & other)  :      
    m_next(0),//other.m_next), 
    m_previous(0),//other.m_previous),
    m_symbol(other.m_symbol),
    m_section(other.m_section)
  {
    qDebug("SymbolList list_node - copy constructor");
  }
//     /**
//      * Symbol constructor
//      * 
//      * @param value Symbol 
//      */
//     list_node(const SymbolDatum * value, 
// 	      BlockNames::SubStrings subsection = BlockNames::NOT_DEFINED) :   
//       m_next(0), 
//       m_previous(0),
//       m_symbol(), 
//       m_section(subsection)
//     {
//       m_symbol =  new Symbol(value);      
//       //m_symbol =  new QSharedDataPointer<Symbol>(const_cast<Symbol *>(&value));      
//     }
  /**
   * Symbol constructor
   * 
   * @param value Symbol 
   */
  list_node(const Symbol & value, 
	    BlockNames::SubStrings subsection = BlockNames::NOT_DEFINED) : 
    m_next(0), 
    m_previous(0),
    m_symbol(new Symbol(value)), 
    m_section(subsection)
  {
    qDebug("SymbolList list_node(Symbol &, BlockNames::SubStrings)");
    //m_symbol->setSymbolDatum(value.getSymbolDatum());
    //m_symbol->operator=(value);      
    //m_symbol =  new QSharedDataPointer<Symbol>(const_cast<Symbol *>(&value));      
    //qDebug("SymbolList list_node(Symbol &, BlockNames::SubStrings) : end");
  }
  
  ~list_node()
  {
    qDebug("SymbolList list_node desructor() : start");
    // joinNodes(m_next, m_previous);
  }
  
  list_node & operator= (list_node * node)
  {
    qDebug("SymbolList list_node - operator= (list_node *) : start");
    m_symbol = node->m_symbol;
    qDebug("SymbolList list_node - operator= (list_node *) : end");
    return *this;
  }
//     /**
//      * operator= Symbol
//      * 
//      * @param s Symbol
//      */
//     Symbol & operator= (const Symbol & s)
//     {
//      m_symbol->operator=(const_cast<Symbol *>(&s));
//       return *m_symbol;
//     }
  /**
   * operator= Section
   *
   * @param subsection 
   */  
  list_node & operator= (BlockNames::SubStrings subsection)
  {
    qDebug("SymbolList list_node - operator= (BlockNames::SubStrings)");
    m_section = subsection;
    //qDebug("SymbolList list_node - operator= (BlockNames::SubStrings) : end");
  }
  // list is "terminated" with a empty node which is treated as the end()
  // parent begin(m_head), end(m_tail)
  // child  begin(m_begin()), end(m_end(0))
  //
  // all node edits happen at LinkedList object level
  // children are just sub ranges of the node in the LinkedList object
  /// previous node in the list
  list_node *  m_previous;
  /// next node in the list
  list_node *  m_next;
  /// current node's value
  QSharedDataPointer<Symbol> m_symbol;
  /// section
  BlockNames::SubStrings m_section;
};


// template<>
// SymbolList::LinkedList *QSharedDataPointer<SymbolList::LinkedList>::clone()
// {
//    return d->clone();
// }
  
class SymbolList::LinkedList : public QSharedData
{
public:
  /**
   * list's head node
   */
  list_node * m_head;
  /**
   * list's tail node
   */
  list_node * m_tail;
  /**
   * convenince flag indicating that SymbolList contains valid Symbol objects 
   */
  bool m_validSymbols;
  /**
   * Symbol object used to convert integers and strings into valid Symbol objects
   */
  SharedSymbol m_symbolParser;
  /**
   * size of containtor
   */
  int m_size;
  
  protected:
  /**
   * block name and starting iterator
   */
  class BlockStart {
  public:
    BlockStart(BlockNames::SubStrings _id, const iterator & _citr) : 
      m_blockId(_id),
      m_startNode(_citr._curpos)//(const_cast<list_node*>(start_node))
     {}
     BlockStart(BlockNames::SubStrings _id, list_node * _start) : 
      m_blockId(_id),
      m_startNode(_start)//(const_cast<list_node*>(start_node))
     {}
    /**
     * Search block_id constructor
     * 
     * @param block_id enum value of block to search for 
     */
    BlockStart(BlockNames::SubStrings block_id) : 
      m_blockId(block_id),
      m_startNode(0)
    {}
    /// Get block identifier
    inline BlockNames::SubStrings getBlockId() const { return m_blockId; };
    // Get iterator to begining of section 
    const_iterator getStartNode() const { return m_startNode; }
    /// operator< : compare section identifiers (enum)
    bool operator<(const BlockStart& b) const { return m_blockId < b.m_blockId; }  
    BlockNames::SubStrings 
      getCurrentSectionId(const_iterator itrPos, bool inserted = true); 

      
    BlockNames::SubStrings m_blockId;
    list_node * m_startNode;    
  };
  
public:
  typedef std::set<BlockStart> BlockStartSet;
  BlockStartSet m_BlockStarts;
 
public:
  // "get" is prepended to list's begin() and end() iterators to prevent confusion  
  /**
   * LinkedList: const_iterator begin()
   */
  SymbolList::const_iterator getBegin() const 
  { return SymbolList::const_iterator(m_head); } 
  /**
   * LinkedList: const_iterator end()
   */
  SymbolList::const_iterator getEnd() const 
  { return SymbolList::const_iterator(m_tail); } 
  /**
   * LinkedList: iterator begin()
   */
  SymbolList::iterator getBegin() 
  { return SymbolList::iterator(m_head); } 
  /**
   * LinkedList: iterator end()
   */
  SymbolList::iterator getEnd() 
  { return SymbolList::iterator(m_tail); }
  
  /**
   * Is LinkedList empty
   */
  bool empty() const
  {
    return (m_size == 0);
  }
  
  SubRange getNamedRange(
    BlockNames::SubStrings sectionName = BlockNames::NOT_DEFINED)
  {
    SubRange sectionRange(getEnd(), getEnd());
    if (sectionName == BlockNames::NONE) {    
      sectionRange.first = getBegin();
      if (m_BlockStarts.empty()) {
	sectionRange.second = getEnd();
      } else  {	
	sectionRange.second = 
	  const_iterator(m_BlockStarts.begin()->getStartNode());
      }
    } else { 
      BlockStartSet::iterator itrStart = m_BlockStarts.find(sectionName);
      if (itrStart != m_BlockStarts.end()) {
	sectionRange.first = const_iterator(itrStart->getStartNode());
	itrStart++;
	if (itrStart != m_BlockStarts.end()) {
	  sectionRange.second = const_iterator(itrStart->getStartNode());
	} else {
	  sectionRange.second = getEnd();
	}
      }
    }
    return sectionRange;
  }
  /**
   * Default constructor
   * 
   * Constructs empty and "invalid"  SymbolList object
   */
  LinkedList() :
    m_head(0),
    m_tail(0),
    m_validSymbols(false),
    m_symbolParser(new Symbol),
    m_size(0),
    m_BlockStarts()
  {
    m_head = m_tail = new list_node;
  }
  /**
   * "valid" constructor
   * 
   * Constructs empty but valid  SymbolList object  
   * @note validity based on BarcodeSetInfo object
   */
  LinkedList(const BarcodeSetInfo & barcodeInfo) :
    m_head(0),
    m_tail(0),
    m_validSymbols(barcodeInfo->isValid()),
    m_symbolParser(new Symbol(barcodeInfo)),
    m_size(0),
    m_BlockStarts()
  {
    m_head = m_tail = new list_node;
  }
  /**
   * "single symbol" constructor
   * 
   * Constructs a valid SymbolList object containing one element   
   * @note validity based on SymbolSymbol object
   */
  LinkedList(const Symbol & s) :
    m_head(0),
    m_tail(0),
    m_validSymbols(s.isValid()),
    m_symbolParser(new Symbol(s)),
    m_size(0),
    m_BlockStarts()
  {
    m_head = m_tail = new list_node;
    insert(getEnd(), s);
  }  
  /**
   * "single symbol" constructor
   * 
   * Constructs a possibly valid SymbolList object containing one element 
   * @note validity based on SharedSymbolSymbol object
   */
  LinkedList(const SharedSymbol & symbol) :
    m_head(0),
    m_tail(0),
    m_validSymbols(symbol->isValid()),
    m_symbolParser(symbol),
    m_size(0),
    m_BlockStarts()
  {
    m_head = m_tail = new list_node;
    insert(getEnd(), *(symbol.constData()));
  }
  /**
   * Copy constructor
   * 
   * 
   */
  LinkedList(const LinkedList & other) :
    QSharedData(other),
    m_head(0),
    m_tail(0),
    m_validSymbols(other.m_validSymbols),
    m_symbolParser(other.m_symbolParser),
    m_size(0),
    m_BlockStarts()
  {
    m_head = m_tail = new list_node;
    // copy "other" LinkedList object
    SymbolList::const_iterator first = other.getBegin();
    SymbolList::const_iterator last = other.getEnd();
    BlockNames::SubStrings curr_Section = BlockNames::NONE;
    
    while (first != last) {
      SymbolList::iterator node_copy = insert(getEnd(), *first);
      // update m_BlockStarts with the begining of each section 
      if (first.getSectionName() != curr_Section) {
	m_BlockStarts.insert(BlockStart(curr_Section, node_copy));
	curr_Section = first.getSectionName();
      }
      node_copy.setSectionName(curr_Section);
      first++;
    }
  }
  
  
  SymbolList::iterator insert(SymbolList::iterator position, 
			       SymbolList::list_node * nodes)
  {
    
  }
  
  /**
   * insert a Symbol
   */
  SymbolList::iterator insert(SymbolList::iterator position, const Symbol & x)
  {
    if (!m_validSymbols && x.isValid()) {
      setParser(x);
    }
    if (x.hasValue()) {
      if (position != getBegin()) {
	iterator previous(position);
	previous--;
	BlockNames::SubStrings curr_Section = previous.getSectionName();
	list_node * insert_node = new list_node(x, curr_Section);
	
	previous._curpos->m_next = insert_node;
	insert_node->m_previous = previous._curpos;
	insert_node->m_next = position._curpos; 
	position._curpos->m_previous = insert_node;
      } else {
	joinNodes(new list_node(x, BlockNames::NONE), m_head);
	m_head = m_head->m_previous;
      }    
      m_size++;
    }
    return position;
  }
 
  /**
   * insert a Symbol
   */
  SymbolList::iterator insert(SymbolList::iterator position, const QString & x,
    bool multiSymbol = false)
  {
    iterator result(getEnd());
    if (m_validSymbols) {
      QList<Symbol> symbols(getParseSymbol().parse(x));
      if (multiSymbol) {
	result = insert(position, symbols.begin(), symbols.end());
      } else {
	result = insert(position, *symbols.begin());
      }
    } else {
      result = insert(position, Symbol(x));
    }
    return result;
  }
 
  /**
   * insert a Symbol n times 
   */
  SymbolList::iterator insert(SymbolList::iterator position, 
			      SymbolList::size_type n, const Symbol& x)
  {
    iterator result(getEnd());
    for (int count = 0; count < n; count++) {
      result = insert(position, x);
    }
    return result;
  }
 
  /**
   * insert a range of Symbols
   */
  template <class InputIterator>
  SymbolList::iterator insert ( iterator position, InputIterator first, InputIterator last )
  {
    while (first != last) {
      if (!first->isValid() && m_validSymbols) {
	insert(position, getParseSymbol().parse(first->toString()));
      } else { 
	insert(position, *first++);
      }
    }
    return position;
  }
  
  /**
   * insert a list of Symbols
   */
  SymbolList::iterator insert(SymbolList::iterator position, const QList<Symbol> & x)
  {
    return insert(position, x.begin(), x.end());
  }
 
  /**
   * insert the contents of a SymbolList object
   */
  SymbolList::iterator insert(SymbolList::iterator position, const SymbolList & symbols)
  {
    BlockNames::SubStrings sectionId = BlockNames::NONE;
    if (position != getBegin()) {
      SymbolList::iterator previous(position);
      --previous;
      sectionId = previous.getSectionName();
    }
    SymbolList::list_node * segment = 
      buildSegment(symbols.begin(), symbols.end(), sectionId);
    qDebug("SymbolList _insert(iterator, T) : start");
    return insert(position, segment);
  }
    
  list_node * extractRange(SymbolList x) 
  {
    list_node * start = x.begin()._curpos;
    list_node * stop = x.end()._curpos->m_previous;
    if (x.begin() == getBegin()) {
      m_head = stop;
    } else {
      joinNodes(start->m_previous, stop);
    }
    return start;
  }
    
//   SymbolList::iterator move(SymbolList & src, SymbolList & dst)
//   {
//     iterator result(dst.end());
//     
//     if (src.d == dst.d) {
//       iterator src_first(src.begin());
//       iterator src_last(src.end());
//       iterator dst_first(dst.begin());
//       iterator dst_last(dst.end());
//       // special case: do nothing when inserting the idential range of nodes
//       if (src_first == dst_first && src_last == dst_last) {
// 	return result;
//       }
//       
//       while (src_first != src_last) {
// 	if (dst_first != dst_last) {
// 	  dst_first = (Symbol)*src_first++;
// 	  dst_first++;
// 	} else {
// 	  BlockNames::SubStrings sectionName = src_first.getSectionName();
// 	  insert(src_first, new list_node(*src_first++, sectionName));
// 	}	
//       }
//       // remove left over nodes in the dst range
//       while (dst_first != dst_last) {
// 	list_node * dnode = *dst_first++;
// 	delete dst_first->//._curpos->m_previous;
//       }
//     } else {
//       // replace dst range with src range 
//       //  then update each node's section name 
//       iterator position(dst.end());
//       list_node * src_start = extractRange(src);
//       list_node * dst_start = extractRange(dst);
//       dst.d->insert(position, src_start);
//       deleteSegment(dst_start);
//     }
//   }
  /**
   * Erase single node
   * 
   * @param position node to be deleted
   * @return position of next node 
   */
  iterator erase ( iterator position ) 
  {
    iterator result(getEnd());
    if (position == getBegin()) {
      m_head = m_head->m_next;
      delete m_head->m_previous;
      m_head->m_previous = 0;
      result = m_head->m_next;
    } else if (position != getEnd()) {
      list_node * previous = position._curpos->m_previous;
      list_node * next = position._curpos->m_next;
      joinNodes(previous, next);
      delete position._curpos;
      result = next;
    } 
    return result;
  }
  /**
   * Erase range of nodes
   * 
   * @param first start of range
   * @param last one node after range    
   * @return position of next node 
   */
  iterator erase ( iterator first, iterator last ) 
  {
    list_node * nodes = 0;
    while (first != last) {
      if (nodes != 0) {
	joinNodes(nodes, first._curpos);
      } else {
	nodes = first._curpos;
      }
    }
    if (nodes == getBegin()._curpos) {
      m_head = last._curpos;
      m_head->m_previous = 0;
    } else {
      joinNodes(nodes->m_previous, last._curpos);
    }
    deleteSegment(nodes);
  }
  
  /**
   * Set parsing object
   */
  bool setParser (const Symbol& s )
  {
    //qDebug("SymbolList setParser() : start");
    if (!getParseSymbol().isValid() || s.isValid()) {
      qDebug("SymbolList setParser() : m_symbolParser updated");
      m_symbolParser = new Symbol(s);//setParser(s);
    }
    //qDebug("SymbolList setParser() : end");
    return m_symbolParser->isValid();
  }
   /// Parse existing 
  bool parseSelf();
  //
  const Symbol& getParseSymbol() const { return *m_symbolParser; }
  /**
   * Update section ids of the nodes in a range
   */	 
  void updateRangeSectionId(iterator first, iterator last,
		   BlockNames::SubStrings sectionId);/**
   * Removes segment of nodes from source, assumes range is contigous
   * 
   * @param splitStart iterator pointing to the starting node
   * @param splitEnd  iterator pointing to one node past the end of the range
   * @return pointer to begin of the list segment on heap
   */
  list_node * cutNodes(SymbolList::iterator splitStart, SymbolList::iterator splitEnd);
protected:
  /**
   * Join two nodes
   * 
   * @param lhs left-hand side of join  
   * @param rhs right-hand side of join
   * 
   * @returns pointer to outer right-hand side node (range "end") 
   */
  SymbolList::list_node *  joinNodes(SymbolList::list_node * lhs, SymbolList::list_node * rhs)
  {
    qDebug("SymbolList joinNodes() : start");
    if (lhs != 0){
      lhs->m_next = rhs;
    }
    if (rhs != 0){
      rhs->m_previous = lhs;
    }
    qDebug("SymbolList joinNodes() : end");
    return (rhs != 0) ? rhs : lhs;
  }  
  
 /**
   * Build list_node segments
   * 
   * @param first range "begin"
   * @param last range "end"
   * @param sectionId section node will belong to 
   * 
   * @note increments current list size
   */  
  template<class InputIterator>
  list_node * buildSegment(InputIterator first, 
			   InputIterator last,
			   BlockNames::SubStrings sectionId = BlockNames::NONE)
  {
    qDebug("SymbolList buildSegment : start");
    list_node * segmentHead = 0;
    list_node * segmentTail = 0;
    while (first != last) {
      if (segmentHead != 0) {
	segmentTail = joinNodes(segmentTail, new SymbolList::list_node(*first++, sectionId));
      } else {
	segmentTail = segmentHead = new SymbolList::list_node(*first++, sectionId);
      }
      segmentTail->m_section = sectionId;
    }
    qDebug("SymbolList buildSegment : end");
    return segmentHead;
  }    
  /**
   * Splice terminated linked-node segment into another segment of linked-node
   * 
   * @param position insert position
   * @param startNodeList start of linked-list nodes to insert 
   */
  iterator attachSpaningNodes(iterator position, list_node * startNodeList);
  /**
   * Delete orphoned list of nodes
   * 
   * @param segmentHead pointer to first node of the segment
   * @returns number of nodes deleted
   */
  int deleteSegment(list_node * segmentHead); 
};

