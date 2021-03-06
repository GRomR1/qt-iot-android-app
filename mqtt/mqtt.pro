TARGET = QtMqtt

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/qmqttglobal.h \
    $$PWD/qmqttclient.h \
    $$PWD/qmqttmessage.h \
    $$PWD/qmqttsubscription.h \
    $$PWD/qmqtttopicfilter.h \
    $$PWD/qmqtttopicname.h

PRIVATE_HEADERS += \
    $$PWD/qmqttclient_p.h \
    $$PWD/qmqttconnection_p.h \
    $$PWD/qmqttcontrolpacket_p.h \
    $$PWD/qmqttsubscription_p.h

SOURCES += \
    $$PWD/qmqttclient.cpp \
    $$PWD/qmqttconnection.cpp \
    $$PWD/qmqttcontrolpacket.cpp \
    $$PWD/qmqttmessage.cpp \
    $$PWD/qmqttsubscription.cpp \
    $$PWD/qmqtttopicfilter.cpp \
    $$PWD/qmqtttopicname.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
