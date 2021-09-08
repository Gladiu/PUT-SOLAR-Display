QT += quick serialport core gui

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        serialport.cpp

RESOURCES += qml.qrc

INCLUDEPATH +=


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../../Qt/Examples/Qt-5.15.1/quickcontrols/extras/dashboard/qml/DashboardGaugeStyle.qml \
    DashboardGaugeStyle.qml \
    IconGaugeStyle.qml \
    IsoItem.qml \
    TurnIndicator.qml \
    dash_qmls/IconGaugeStyle.qml \
    dash_qmls/TachometerStyle.qml \
    dash_qmls/TurnIndicator.qml \
    dash_qmls/ValueSource.qml \
    dashpngs/backgrounds_195_610.png \
    dashqmls/DashboardGaugeStyle.qml \
    dashqmls/IconGaugeStyle.qml \
    dashqmls/PowerGaugeStyle.qml \
    dashqmls/TachometerStyle.qml \
    dashqmls/TurnIndicator.qml \
    dashqmls/ValueSource.qml \


HEADERS += \
    serialport.h

