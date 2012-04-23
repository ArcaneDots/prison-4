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


#include <algorithm>
#include <iterator>


#include "symbollist_linkedlist.h"

SymbolList::SymbolList() :
  d(new SymbolList::LinkedList()),
  m_head(d.constData()->getBegin()),
  m_tail(d.constData()->getEnd()),
  m_currentBlock()
{  
  qDebug("SymbolList - default constructor()");  
}

SymbolList::SymbolList(const Symbol& symbol):
  d(new SymbolList::LinkedList(symbol)),
  m_head(d.constData()->getBegin()),
  m_tail(d.constData()->getEnd()),
  m_currentBlock()
{
  qDebug("SymbolList - constructor(const Symbol&)");
}

SymbolList::SymbolList (const BarcodeSetInfo& barcodeSymbol) :
  d(new SymbolList::LinkedList(barcodeSymbol)),
  m_head(d.constData()->getBegin()),
  m_tail(d.constData()->getEnd()),
  m_currentBlock()
{
  //qDebug("SymbolList constructor(const BarcodeSetInfo&) ");  
}

SymbolList::SymbolList(const BarcodeSetInfo& barcodeSymbol, 
		       const QString& userString) :
  d(new SymbolList::LinkedList(barcodeSymbol)),
  m_head(d.constData()->getBegin()),
  m_tail(d.constData()->getEnd()),
  m_currentBlock()
{
  //qDebug("SymbolList constructor(const BarcodeSetInfo&, const QString&)");
  insert(begin(), userString);
}

SymbolList::SymbolList(const SymbolList& other) :
  d(other.d),
  m_head(d.constData()->getBegin()),
  m_tail(d.constData()->getEnd()),
  m_currentBlock()
{  
  qDebug("SymbolList copy constructor");
  //qDebug("SymbolList copy constructor : end");
}

SymbolList::SymbolList(const SharedList& listData,
                       SubRange subRange) :
  d(listData),
  m_head(subRange.first),
  m_tail(subRange.second),
  m_currentBlock()
{
  qDebug("SymbolList sub-constructor");
}

SymbolList::SymbolList(const SharedList& listData, 
		       BlockNames::SubStrings sectionName) :
  d(listData),
  m_head(0),
  m_tail(0),
  m_currentBlock(sectionName) 
{
  SubRange namedRange = d->getNamedRange(sectionName);
  m_head = namedRange.first;
  m_tail = namedRange.second;
}


SymbolList::~SymbolList()
{  
  qDebug("SymbolList destructor");
}

SymbolList::const_reference SymbolList::const_iterator::operator*() const
{
  return *(_curpos->m_symbol.constData()); 
}

SymbolList::const_pointer SymbolList::const_iterator::operator->() const
{
  return _curpos->m_symbol.constData();
}


SymbolList::const_iterator & SymbolList::const_iterator::operator++()
{
  if (_curpos->m_next != 0) {
    _curpos = _curpos->m_next;
  }
  return *this;
}


SymbolList::const_iterator & SymbolList::const_iterator::operator--()
{
  if (_curpos->m_previous != 0) {
    _curpos = _curpos->m_previous;
  }
  return *this;
}

BlockNames::SubStrings SymbolList::const_iterator::getSectionName() const
{
  return _curpos->m_section;
}

bool SymbolList::const_iterator::setSectionName(BlockNames::SubStrings sectionName)
{
  bool result = false;
  if (_curpos->m_symbol->hasValue() && _curpos->m_section != sectionName) {
    list_node * previous = _curpos->m_previous;
    if (previous != 0 && previous->m_section < sectionName) {
      _curpos->m_section = sectionName;
    }
  }
  return _curpos->m_section == sectionName;
}


SymbolList::reference SymbolList::iterator::operator*() const
{
  return *(_curpos->m_symbol.data());
}

SymbolList::pointer SymbolList::iterator::operator->() const
{
  return _curpos->m_symbol.data();
}

SymbolList::reference SymbolList::iterator::operator= (const iterator& i) 
{ 
  _curpos = i._curpos;
  return *(_curpos->m_symbol.data()); 
}    

SymbolList::reference SymbolList::iterator::operator=(const Symbol& s)
{
  if (s.hasValue()) {    
    _curpos->m_symbol = new Symbol(s);
  }
  return *(_curpos->m_symbol.data()); 
}

SymbolList::reference SymbolList::iterator::operator=(BlockNames::SubStrings section)
{
  _curpos->operator=(section);
  return *(_curpos->m_symbol.data()); 
}


SymbolList::const_iterator SymbolList::begin() const 
{ return const_iterator(m_head); };
SymbolList::const_iterator SymbolList::end() const
{ return const_iterator(m_tail); };
SymbolList::iterator SymbolList::begin() 
{ return iterator(m_head); };
SymbolList::iterator SymbolList::end() 
{ return iterator(m_tail); };

size_t SymbolList::size() const
{
  size_t curr_size = d->m_size;
  if (m_head != d->getBegin() || m_tail != d->getEnd()) {
    curr_size = getSize(m_head, m_tail);
  }
  return curr_size;
}

bool SymbolList::empty() const
{
  bool isEmpty = d->empty();
  if (m_head != d->getBegin() || m_tail != d->getEnd()) {
    isEmpty = (size() == 0);
  }
  return isEmpty; 
}

// SymbolList::const_iterator SymbolList::getSectionStart(BlockNames::SubStrings name)
// {
//   return (m_BlockStarts.find(BlockStart(name)))->getStartNode();
// }

// bool SymbolList::setSectionStart(BlockNames::SubStrings name, SymbolList::const_iterator& start)
// {
//   std::set<SymbolList::BlockStart>::iterator blockInfo = 
//     m_BlockStarts.find(BlockStart(name));
//   iterator initalStart = (blockInfo == m_BlockStarts.end()) ? end() : blockInfo->getStartNode();
//   iterator userStart(start);
//   
//   // inside named section : shrink range
//   if (userStart._curpos->m_section == name) {
//     BlockStartSet::iterator prevBlockInfo(blockInfo);
//     prevBlockInfo--;
//     BlockNames::SubStrings prevName = prevBlockInfo->getBlockId();
//     if (prevName == name) {
//       prevName = BlockNames::NONE;
//     }
//     d->updateRangeSectionId(initalStart, userStart, prevName);
//   }
//   
//   // to the right of section : expand range to right
//   if (userStart._curpos->m_section < name) {
//     BlockStartSet::iterator nextBlockInfo(blockInfo);
//     nextBlockInfo++;
//     iterator sectionEnd = 
//       (blockInfo != nextBlockInfo) ? nextBlockInfo->getStartNode() : end();
//     BlockNames::SubStrings curr_name = BlockNames::NOT_DEFINED;
//     BlockStartSet::iterator curBlock = m_BlockStarts.end();
//     while (userStart != sectionEnd) {  
//       if (curBlock == m_BlockStarts.end()) {
//         curBlock = 
// 	  m_BlockStarts.find(BlockStart(userStart._curpos->m_section));
//       } else { curBlock++; }
//       BlockStartSet::iterator nextBlock(curBlock);
//       nextBlock++;
//       iterator currSectionEnd = sectionEnd;
//       if (nextBlock->getBlockId() != name){
// 	currSectionEnd = nextBlock->getStartNode();
//       } 
//       d->updateRangeSectionId(userStart,currSectionEnd, name);
//       // on the start iterator of a previous section 
//       if (userStart == curBlock->getStartNode()) {
//       // delete block
// 	m_BlockStarts.erase(curBlock);
//       }
//       userStart = currSectionEnd;
//     }
//   }
// }

// list manipulation fuctions


SymbolList::list_node* SymbolList::LinkedList::cutNodes(SymbolList::iterator first, 
					    SymbolList::iterator last)
{    
  //Debug("SymbolList cutNodes(first, last) : start");
  if (empty()) { 
    qDebug("SymbolList cutNodes(first, last) : empty list");
    return 0;     
  }
  // remove range from list
  list_node * start_node = first._curpos;
  list_node * stop_node = last._curpos->m_previous;
  if (first != getBegin()) {
    qDebug("SymbolList cutNodes(first, last) : normal join");
    joinNodes(first._curpos->m_previous, last._curpos);
  } else {
    qDebug("SymbolList cutNodes(first, last) : adjust m_head");
    m_head = stop_node; 
    
  }
  // fully disconnect range
  start_node->m_previous = 0;
  stop_node->m_next = 0;
  //qDebug("SymbolList cutNodes(first, last) : end");
  return start_node; 
}
SymbolList::iterator SymbolList::LinkedList::attachSpaningNodes(SymbolList::iterator position, 
					     SymbolList::list_node * spaningNodes)
{
  qDebug("SymbolList spliceNodes() : start");
  Q_ASSERT(spaningNodes != 0);
  int count = 0;
    
  list_node * rightAbutmentNode = position._curpos;
  list_node * leftAbutmentNode = rightAbutmentNode->m_previous;
  
  // -- attach to left side of 
  // special case: shift m_head if inserting at begin() or list empty
  if (rightAbutmentNode != m_head && !empty()) {
    joinNodes(leftAbutmentNode, spaningNodes);
  } else  {
    m_head = spaningNodes;
  } 
  list_node * endOfSpaningNodes = spaningNodes;
  count++;
  while (endOfSpaningNodes->m_next != 0) {  
    count++;
    endOfSpaningNodes = endOfSpaningNodes->m_next;
  }  
  // attach segment to previous node
  qDebug("SymbolList spliceNodes : attach to righthand side (position_node)");
  joinNodes(endOfSpaningNodes, rightAbutmentNode);

  m_size += count;
  qDebug("SymbolList spliceNodes() : end");
  return rightAbutmentNode;
}

// SymbolList::list_node * SymbolList::cutNodes(SymbolList::const_iterator splitStart, SymbolList::const_iterator splitEnd)
// {
//   return cutNodes(iterator(const_cast<list_node *>(splitStart._curpos)), iterator(splitEnd._curpos));
// }
 


void SymbolList::LinkedList::updateRangeSectionId(SymbolList::iterator first, 
			     SymbolList::iterator last,
			     BlockNames::SubStrings sectionId)
{
  while (first != last) {
    first = sectionId;
    first++;
  }
}

int SymbolList::LinkedList::deleteSegment(SymbolList::list_node* segmentHead)
{
  //qDebug("SymbolList deleteSegment() : start");
  int nodesDeleted = (segmentHead != 0) ? 1 : 0;
  if (nodesDeleted > 0) {
    qDebug("SymbolList deleteSegment() : node found");
    list_node * ptr = segmentHead;
    while (ptr->m_next != 0) {
      ptr = ptr->m_next;
      delete ptr->m_previous;
      nodesDeleted++;
    }
    delete ptr;    
  }
  //qDebug("SymbolList deleteSegment() : end");
  return nodesDeleted;
}
// bool SymbolList::parseSelf()
// {
//   qDebug("SymbolList parseSelf() : start");
//   Q_ASSERT(d == 0);
//   if (m_symbolParser->isValid() && !d->m_validSymbols) {
//     iterator first(begin());
//     iterator last(end());
//     qDebug("SymbolList parseSelf() : checking nodes ");
//     while (first != last) {
//       if (!first->isValid()) {
// 	qDebug("SymbolList parseSelf() : parse node %s", first->toString().data());
// 	iterator local_end(first);
// 	local_end++;
// 	BlockNames::SubStrings sectionName = first._curpos->m_section;
// 	QList<Symbol> parsedSymbols(parse(*first));	
// 	delete cutNodes(first, local_end);
// 	m_size--;
// 	//int count = 0;
// 	attachSpaningNodes(++first, 
// 	       buildSegment(parsedSymbols.begin(), parsedSymbols.end(),
// 			    sectionName));
// 	//m_size += count;
//       }
//       first++;
//     }    
//     m_validSymbols = true;
//   }
//   qDebug("SymbolList parseSelf() : end");
//   return m_validSymbols;
// }
void SymbolList::splice(SymbolList::iterator position, SymbolList& x)
{
  d->insert(position, x);//cutNodes(x.begin(), x.end()));
  //d->attachSpaningNodes(position, cutNodes(x.begin(), x.end()));
  x.clear();
}

void SymbolList::splice ( iterator position, SymbolList& x, iterator i )
{
  d->insert(position, *i); 
  x.erase(i, ++i);
}

void SymbolList::splice(SymbolList::iterator position, SymbolList& x, 
			SymbolList::iterator first, SymbolList::iterator last)
{  
  qDebug("SymbolList splice(position, SymbolList, first, last) : start");
  d->insert(position, first, last); //cutNodes(first, last));
  x.erase(first, last);
  //d->attachSpaningNodes(position, cutNodes(first, last));
  qDebug("SymbolList splice(position, SymbolList, first, last) : end");
}




SymbolList::iterator SymbolList::_insert( SymbolList::iterator position,
					  const Symbol& x )
{ 
  return d->insert(position, x);
}

SymbolList::iterator SymbolList::_insert(SymbolList::iterator position,
					const QList<Symbol>& symbols)
{ 
  qDebug("SymbolList _insert(QList<Symbol>) ");
  return d->insert(position, symbols);
}

SymbolList::iterator SymbolList::_insert ( iterator position,  const QStringList& stringlist ) 
{
  //qDebug("SymbolList _insert(QStringList) : start");
  QList<Symbol> symbols;
  QStringList::const_iterator itrString = stringlist.begin();
  while (itrString != stringlist.end()) {
    qDebug("SymbolList _insert(QStringList) : parse string");
    symbols.append(d->getParseSymbol().parse(*itrString++));
  }
  //qDebug("SymbolList _insert(QStringList) : end");
  return d->insert(position, symbols);
}

SymbolList::iterator SymbolList::_insert ( SymbolList::iterator position, 
					  const SymbolList& x )
{
  //qDebug("SymbolList _insert(SymbolList)");
  return d->insert(position, x);
}

Symbol SymbolList::getParserSymbol() const
{
  return d->getParseSymbol();
}

SymbolList::iterator SymbolList::erase(SymbolList::iterator position)
{  
  return d->erase(position);
}

SymbolList::iterator SymbolList::erase(iterator first, iterator last)
{   
  d->erase(first, last);
}

void SymbolList::pop_front()
{
  erase(begin());
}

void SymbolList::pop_back()
{  
  erase(iterator(m_tail));
}

void SymbolList::clear()
{
  erase(begin(), end());
}

SymbolList SymbolList::clone() const
{
  return SymbolList(getFullSymbolSet());
}

void SymbolList::swap(SymbolList& x)
{
  std::swap(d, x.d);
  std::swap(m_head, x.m_head);
  std::swap(m_tail, x.m_tail);
  std::swap(m_currentBlock, x.m_currentBlock);
}


BarcodeSetInfo SymbolList::getFullSymbolSet() const
{
  d->getParseSymbol();
}

bool SymbolList::isValid() const
{
  return d->getParseSymbol().isValid();
}

const SymbolList SymbolList::mid( int pos, int length ) const
{  
  const_iterator start(begin() += pos);
  const_iterator stop(end());
  /// doesn't prevent pos value that overrun segment length
  if (length != NOT_FOUND) {
    int remaining_size = getSize(start, stop);
    if (remaining_size > 0 && (pos + length) < remaining_size) {
      stop = begin() += pos + length;
    }  
  }
  SubRange subRange(start, stop);
  return SymbolList(this->d, subRange);
}

barcodeEngine::LookupIndexArray SymbolList::getIndexValues() const
{ 
  barcodeEngine::LookupIndexArray indexArray;
  const_iterator itrSymbol = begin();
  while (itrSymbol != end()) {
    LookupIndexArray indexes(itrSymbol->getIndexes());
    std::copy(indexes.begin(), indexes.end(), std::back_inserter(indexArray)); 
  }
}

QStringList SymbolList::toQStringList() const 
{
  QStringList outputList;
  const_iterator itrSymbol = begin();
  while (itrSymbol != end()) {
    outputList << itrSymbol->toString();
    itrSymbol++;
  }  
  return outputList;
}

QList< Symbol > SymbolList::toSymbols() const
{
  QList<Symbol> symbols;
  iterator first(begin());
  iterator last(end());
  while (first != last) {
    symbols.append(*first++);
  }
  return symbols; 
}

/**
 * special header symbols 
 * 
 * i.e. EAN-128 header
 */
SymbolList SymbolList::prefix()
{
  return SymbolList(this->d, BlockNames::PREFIX);
}

/**
 * main portion of barcode (not including check symbols)
 */
SymbolList SymbolList::main_code()
{
  return SymbolList(this->d, BlockNames::MAIN_CODE);
}

/**
 * Product code : manufactor code
 */
SymbolList SymbolList::manf()
{
  return SymbolList(this->d, BlockNames::MANUFACTOR);
}
/**
 * Product code : product code
 */
SymbolList SymbolList::product()
{
  return SymbolList(this->d, BlockNames::PRODUCT);
}

/**
 * check value/symbol(s)
 */
SymbolList SymbolList::check_value()
{
  return SymbolList(this->d, BlockNames::CHECK_VALUE);
}

SymbolList SymbolList::extended_code()
{
  return SymbolList(this->d, BlockNames::EXTENDED_CODE);
}

SymbolList SymbolList::check_value_C()
{
  return SymbolList(this->d, BlockNames::CHECK_VALUE_C);
}

SymbolList SymbolList::check_value_K()
{
  return SymbolList(this->d, BlockNames::CHECK_VALUE_K);
}

const Symbol& SymbolList::at(int index) const
{
  return *const_iterator(begin() += index);
}

const Symbol& SymbolList::front() const
{ 
  return *m_head;   
}
const Symbol& SymbolList::back() const
{ 
  iterator back(m_tail);
  back--;
  return *back; 
}
Symbol& SymbolList::front()
{ 
  return *m_head;   
}
Symbol& SymbolList::back()
{ 
  iterator back(m_tail);
  back--;
  return *back; 
}
// operator(s)

void SymbolList::assign(SymbolList::size_type n, const Symbol& x)
{
  d = new LinkedList(d->getParseSymbol());
  // insert "source" node range into destintion
  d->insert(d->getEnd(), n, x);
}

void SymbolList::assign(const QStringList & other)
{
  d = new LinkedList(d->getParseSymbol());
  // insert "source" node range into destintion
  QStringList::const_iterator first(other.begin());
  QStringList::const_iterator last(other.end());
  while (first != last) {
    d->insert(d->getEnd(), *first++, true);
  }
}

void SymbolList::assign ( const SymbolList& other )
{
    // FIXME add block support
  SharedSymbol parserSymbol(new Symbol(d->getParseSymbol()));
  SharedList tempD(new LinkedList(parserSymbol));
  SharedList oldD(d);
  d = new LinkedList(parserSymbol);
  
  iterator first(oldD->getBegin());
  iterator last(oldD->getEnd());
  
  iterator position(0);
  
  iterator src_first(other.begin());
  iterator src_last(other.end());
  iterator dst_first(this->begin());
  iterator dst_last(this->end());
  
  // copy everything except source and destintion node ranges to new "d" 
  d = new LinkedList(parserSymbol);
  while (first != last) {
    if (first == src_first || first == dst_first) {
      if (first == src_first && src_first != src_last) {
	qDebug("SymbolList assign() : save dst node ");
	tempD->insert(tempD->getBegin(), new list_node(*first));
	src_first++;
      }
      if (first == dst_first && dst_first != dst_last) {
	position = d->getEnd();
	dst_first++;
      }
    } else {
      qDebug("SymbolList assign() : save dst node ");
      d->insert(d->getBegin(), new list_node(*first));
    }
    first++;
  }
  if (d != other.d) {
    tempD->insert(tempD->getEnd(), other.begin(), other.end());
  }
  // insert "source" node range into destintion
  d->insert(position, tempD->getBegin(), tempD->getEnd());
}

SymbolList& SymbolList::operator= ( const SymbolList& rhs )
{
  assign(rhs);
  return *this;
}

/**
 * Get number of nodes in a range
 */
int getSize(SymbolList::const_iterator begin, SymbolList::const_iterator end)
{
  int count = 0;
  while (begin != end) {
    begin++;
    count++;
  }
  return count;
}

SymbolList& SymbolList::operator= ( const QStringList& rhs )
{
  assign(rhs);
  return *this;
}

SymbolList& SymbolList::operator<<(const Symbol& symbol)
{       
  push_back<Symbol>(symbol);
  return *this;
}

bool hasSameSet(const SymbolList& lhs, const SymbolList& rhs)
{
  return (lhs.getFullSymbolSet() == rhs.getFullSymbolSet());
}

bool operator==(const SymbolList& lhs, const SymbolList& rhs)
{
  return (&lhs == &rhs);
}

QList<Symbol> operator<< (QList<Symbol> lh, QList<Symbol> rh )
{
  std::copy( rh.begin(), rh.end(), std::back_inserter( lh ));
  return lh;    
}

QList<Symbol> operator<<(SymbolList& lh, const SymbolList& rh)
{
  QList<Symbol> s(lh.toSymbols());
  s.append(rh.toSymbols());
  return s;
}

SymbolList operator<< ( const SymbolList& symbolList, const QString& stringSymbols )
{
  SymbolList nextList(symbolList);
  return nextList;
}

std::ostream& operator<<( std::ostream& os, const SymbolList &s) 
{
  QStringList::const_iterator itrStrings = s.toQStringList().begin();
  while (itrStrings != s.toQStringList().end()) {
    os << (os, itrStrings->toStdString());
  }
  return os; 
}
QDebug operator<< ( QDebug debugString, const SymbolList& s )
{
  return debugString << s.toQStringList();   
}

bool SymbolList::LinkedList::parseSelf()
{
  //qDebug("SymbolList::LinkedList parseSelf() : start");
  if (m_symbolParser->isValid() && !m_validSymbols) {
    iterator first(getBegin());
    iterator last(getEnd());
    qDebug("SymbolList parseSelf() : checking nodes ");
    while (first != last) {
      if (!first->isValid()) {
	qDebug() << "SymbolList parseSelf() : parse node " << first->toString();
	iterator local_end(first);
	local_end++;
	BlockNames::SubStrings sectionName = first._curpos->m_section;
	QList<Symbol> parsedSymbols(m_symbolParser->parse(Symbol(*first)));	
	delete cutNodes(first, local_end);
	m_size--;
	attachSpaningNodes(++first, 
	       buildSegment(parsedSymbols.begin(), parsedSymbols.end(),
			    sectionName));
      }
      first++;
    }    
    m_validSymbols = true;
  }
  //qDebug("SymbolList::LinkedList parseSelf() : end");
  return m_validSymbols;
}

