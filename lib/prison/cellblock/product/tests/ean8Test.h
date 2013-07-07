 
#ifndef TEST_UPCA
#define TEST_UPCA

#include <QtCore/QObject>
#include <QtTest/QTest>

 #include "../ean8engine.h"
 
 
 class TestUpcA: public QObject
 {
   Q_OBJECT
 
   
 private slots:
   void defaultConstructor();
 };
 
 #endif // TEST_UPCA