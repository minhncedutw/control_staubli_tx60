QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=  \
            soapC.cpp stdsoap2.cpp \
            soapCS8ServerV0Proxy.cpp soapCS8ServerV1Proxy.cpp soapCS8ServerV3Proxy.cpp \
            TX60L.cpp \
            tcpserver.cpp \
#    main_old.cpp \
    main.cpp

HEADERS += soapH.h soapStub.h stdsoap2.h \
            soapCS8ServerV0Proxy.h soapCS8ServerV1Proxy.h soapCS8ServerV3Proxy.h \
            TX60L.h \
            tcpserver.h

RESOURCES += CS8ServerV0.nsmap CS8ServerV1.nsmap CS8ServerV3.nsmap
