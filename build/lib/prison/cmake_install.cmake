# Install script for directory: /home/ian/Projects/prison/lib/prison

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}/usr/local/lib/libprison.so.0.0.0"
      "$ENV{DESTDIR}/usr/local/lib/libprison.so.0"
      "$ENV{DESTDIR}/usr/local/lib/libprison.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libprison.so.0.0.0;/usr/local/lib/libprison.so.0;/usr/local/lib/libprison.so")
FILE(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES
    "/home/ian/Projects/prison/build/lib/prison/libprison.so.0.0.0"
    "/home/ian/Projects/prison/build/lib/prison/libprison.so.0"
    "/home/ian/Projects/prison/build/lib/prison/libprison.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/usr/local/lib/libprison.so.0.0.0"
      "$ENV{DESTDIR}/usr/local/lib/libprison.so.0"
      "$ENV{DESTDIR}/usr/local/lib/libprison.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_REMOVE
           FILE "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "devel")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/prison/abstractbarcode.h;/usr/local/include/prison/AbstractBarcode;/usr/local/include/prison/barcodeitem.h;/usr/local/include/prison/BarcodeItem;/usr/local/include/prison/barcodewidget.h;/usr/local/include/prison/BarcodeWidget;/usr/local/include/prison/code39barcode.h;/usr/local/include/prison/Code39Barcode;/usr/local/include/prison/code93barcode.h;/usr/local/include/prison/Code93Barcode;/usr/local/include/prison/datamatrixbarcode.h;/usr/local/include/prison/DataMatrixBarcode;/usr/local/include/prison/qrcodebarcode.h;/usr/local/include/prison/QRCodeBarcode;/usr/local/include/prison/Ean13Barcode;/usr/local/include/prison/ean13barcode.h;/usr/local/include/prison/Ean8Barcode;/usr/local/include/prison/ean8barcode.h;/usr/local/include/prison/UpcABarcode;/usr/local/include/prison/upcabarcode.h;/usr/local/include/prison/UpcEBarcode;/usr/local/include/prison/upcebarcode.h;/usr/local/include/prison/prison_export.h")
FILE(INSTALL DESTINATION "/usr/local/include/prison" TYPE FILE FILES
    "/home/ian/Projects/prison/lib/prison/abstractbarcode.h"
    "/home/ian/Projects/prison/lib/prison/AbstractBarcode"
    "/home/ian/Projects/prison/lib/prison/barcodeitem.h"
    "/home/ian/Projects/prison/lib/prison/BarcodeItem"
    "/home/ian/Projects/prison/lib/prison/barcodewidget.h"
    "/home/ian/Projects/prison/lib/prison/BarcodeWidget"
    "/home/ian/Projects/prison/lib/prison/code39barcode.h"
    "/home/ian/Projects/prison/lib/prison/Code39Barcode"
    "/home/ian/Projects/prison/lib/prison/code93barcode.h"
    "/home/ian/Projects/prison/lib/prison/Code93Barcode"
    "/home/ian/Projects/prison/lib/prison/datamatrixbarcode.h"
    "/home/ian/Projects/prison/lib/prison/DataMatrixBarcode"
    "/home/ian/Projects/prison/lib/prison/qrcodebarcode.h"
    "/home/ian/Projects/prison/lib/prison/QRCodeBarcode"
    "/home/ian/Projects/prison/lib/prison/Ean13Barcode"
    "/home/ian/Projects/prison/lib/prison/ean13barcode.h"
    "/home/ian/Projects/prison/lib/prison/Ean8Barcode"
    "/home/ian/Projects/prison/lib/prison/ean8barcode.h"
    "/home/ian/Projects/prison/lib/prison/UpcABarcode"
    "/home/ian/Projects/prison/lib/prison/upcabarcode.h"
    "/home/ian/Projects/prison/lib/prison/UpcEBarcode"
    "/home/ian/Projects/prison/lib/prison/upcebarcode.h"
    "/home/ian/Projects/prison/lib/prison/prison_export.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "devel")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/cmake/Prison/PrisonConfig.cmake")
FILE(INSTALL DESTINATION "/usr/local/lib/cmake/Prison" TYPE FILE FILES "/home/ian/Projects/prison/build/lib/prison/PrisonConfig.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/ian/Projects/prison/build/lib/prison/cellblock/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

