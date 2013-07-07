 
#ifndef TEST_UPCA
#define TEST_UPCA

#include <QtCore/QObject>
#include <QtTest/QTest>

 #include "../ean13engine.h"
 
 
 class TestEan13: public QObject
 {
   Q_OBJECT
 
   
 private slots:
   void defaultConstructor();
 };
 
 #endif // TEST_UPCA