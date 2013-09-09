 
#ifndef TEST_EAN13
#define TEST_EAN13

#include <QtCore/QObject>
#include <QtTest/QTest>

 #include "../ean13engine.h"
 
 
 class TestEan13: public QObject
 {
   Q_OBJECT
 
   
 private slots:
   void defaultConstructor();
 };
 
 #endif // TEST_EAN13