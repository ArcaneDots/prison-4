 
#ifndef TEST_EAN13
#define TEST_EAN13

#include <QtCore/QObject>
#include <QtTest/QTest>

 #include "../ean13engine.h"

 Q_DECLARE_METATYPE(QList<shared::Symbol>);
 class TestEan13: public QObject
 {
   Q_OBJECT
 private:
   QStringList encodedBlock(QList<shared::Symbol> symbolList, QString encoding);
   QMap<QString, QStringList> m_encodingMap;
   QStringList m_parity13WidthEncoding;
   
 private slots:
   /// test-wide "constructor"
   void initTestCase();
   void defaultConstructor();
   void defaultConstructor_data();
 };
 
 #endif // TEST_EAN13