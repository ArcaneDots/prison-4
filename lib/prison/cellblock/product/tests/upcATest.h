 
#ifndef TEST_UPCA
#define TEST_UPCA

#include <QtCore/QObject>
#include <QtTest/QTest>

#include "../upcaengine.h"
 
 Q_DECLARE_METATYPE(QList<shared::Symbol>);
 class TestUpcA: public QObject
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
 
 #endif // TEST_UPCA