 
#include "symbolnode.h"
#include "abstractsymbology.h"
 
 using namespace linearSymbology;
 
SymbolNode::SymbolNode() : 
	m_symbology(),
	m_index(shared::NOT_FOUND),
	m_string(shared::EmptyString),
	m_isVisible(true), 
	m_namedSet(shared::DEFAULT_SET),
	m_expandedNodes(0),
	m_encoding(0),
	m_defaultEncoding(shared::EmptyString)
	{
	  
	}
	
SymbolNode::SymbolNode(const SymbolNode& other) : 
	m_symbology(other.m_symbology),
	m_index(other.m_index),
	m_string(other.m_string),
	m_isVisible(other.m_isVisible), 
	m_namedSet(other.m_namedSet),
	m_expandedNodes(other.m_expandedNodes.data()),
	m_encoding(other.m_encoding),
	m_defaultEncoding(other.m_defaultEncoding)/**/
	{
	  
	}

SymbolNode::SymbolNode(const AbstractSymbology* symbology) :
      m_symbology(symbology),      
      m_index(shared::NOT_FOUND),
      m_string(shared::EmptyString), 
      m_namedSet(shared::DEFAULT_SET),
      m_isVisible(true),
      m_expandedNodes(0),
      m_encoding(0),
      m_defaultEncoding(EmptyString)
      {
	//m_defaultEncoding = ;
      }
 const AbstractSymbology* SymbolNode::getSymbology() const
{
  return m_symbology;
}
      
QString SymbolNode::symbologyName() const
      {
	return !m_symbology ? EmptyString : m_symbology->getName();
	//return m_symbology.isNull() ? EmptyString : m_symbology->getName();
      }

QString SymbolNode::namedSet() const{
	return m_namedSet;
      }
      
int SymbolNode::index() const {return m_index;}

QStringList SymbolNode::encoding() const
      {
	return m_defaultEncoding;
      }

QStringList SymbolNode::encoding(QChar encodingSetName) const
      { 
	return encoding(QString(encodingSetName));
      }
      
QStringList SymbolNode::encoding(const QString& encodingSetName) const
      {	
	QStringList result;
	if (!m_expandedNodes.isNull()) {	
	  int count = m_expandedNodes->count();
	  for (int i = 0; i > count; ++i) {
	    result << m_expandedNodes->at(i).encoding(encodingSetName);
	  }
	} else if (m_encoding->isNull())  {
	    result <<  m_defaultEncoding;
	} else {
	  SymbolEncoding::const_iterator itEnc, itEnd = m_encoding->data()->constEnd();
	  itEnc = m_encoding->data()->find(encodingSetName);
	  if (itEnc != itEnd && itEnc.value().count() > m_index) {
	   result << itEnc->at(m_index);
	  }	
	}
	return result;
      }

// int SymbolNode::getIndex() const
// {
//   return m_index;
// //   return (!m_symbology) ? 0 : m_symbology->getIndex(index);
// }

bool SymbolNode::isValid() const
{
  return (!m_symbology) ? false : true;
}

bool SymbolNode::hasValue() const
{
  return (m_index != NOT_FOUND && !m_symbology) ? false : true;
}

bool SymbolNode::operator<(const SymbolNode& other) const
{
  return m_index < other.m_index;
}

bool SymbolNode::operator==(const SymbolNode& other) const
{
 return (m_index == other.m_index) ;
}


bool SymbolNode::operator==(const QString& string) const
{
  return (m_string == string);
}

      
bool SymbolNode::operator()(const QString& lhs, const QString& rhs) const
      {
	return lhs < rhs;
      }
      


SymbolNode::SymbolNode(const linearSymbology::AbstractSymbology& symbology, int index, const QString& string, const QString& namedSymbolSet, bool isVisible) :
      m_symbology(symbology),
      m_index(index),
      m_string(shared::EmptyString), 
      m_namedSet(namedSymbolSet),
      m_isVisible(isVisible),
      m_expandedNodes(0),
      m_encoding(0),
      m_defaultEncoding()
      {
	if (!m_symbology) { return; }
	
	m_defaultEncoding << shared::EmptyString;
// 	m_expandedNodes = m_symbology.constData()->expandedSymbols();
// 	m_encoding = m_symbology.constData()->encodings();
	
	SymbolEncoding::const_iterator itDefEncoding = m_encoding->data()->constEnd();
	SymbolEncoding::const_iterator itEncodingEnd = m_encoding->data()->constEnd();
// 	itDefEncoding = m_encoding->data()->find(m_symbology->defaultEncoding());	
	if (itDefEncoding != itEncodingEnd) {
	  m_defaultEncoding << itDefEncoding.value();
	}	  
      }  
      
      
bool linearSymbology::operator<=(const SymbolNode& left, const SymbolNode& right)
{
  return left.index() <= right.index();
}

bool linearSymbology::operator>=(const SymbolNode& left, const SymbolNode& right)
{
  return left.index() >= right.index();
}
