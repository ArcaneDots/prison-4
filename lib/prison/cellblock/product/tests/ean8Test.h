 
#ifndef TEST_EAN8
#define TEST_EAN8

#include <QtCore/QObject>
#include <QtTest/QTest>

#include "../ean8engine.h"

 Q_DECLARE_METATYPE(QList<shared::Symbol>);
 class TestEan8: public QObject
  {
   Q_OBJECT
 private:
   QStringList encodedBlock(QList<shared::Symbol> symbolList, QString encoding);

   QMap<QString, QStringList> m_encodingMap;

 private slots:
   /// test "constructor"
   void initTestCase();
   void defaultConstructor();
   void defaultConstructor_data();
 };
 
 #endif // TEST_EAN8