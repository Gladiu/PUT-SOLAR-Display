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
    dashicons/iso_grs_7000_4_0027.dat \
    dashicons/iso_grs_7000_4_0238.dat \
    dashicons/iso_grs_7000_4_0240.dat \
    dashicons/iso_grs_7000_4_0245.dat \
    dashicons/iso_grs_7000_4_0456.dat \
    dashicons/iso_grs_7000_4_0638.dat \
    dashicons/iso_grs_7000_4_0640.dat \
    dashicons/iso_grs_7000_4_1407.dat \
    dashicons/iso_grs_7000_4_1702.dat \
    dashicons/iso_grs_7000_4_2612.dat \
    dashpngs/backgrounds_195_610.png \
    dashpngs/gearbox_visual_195_150.png \
    dashpngs/gearbox_visual_1_195_187.png \
    dashpngs/gearbox_visual_2_195_185.png \
    dashpngs/gearbox_visual_3_195_181.png \
    dashpngs/gearbox_visual_4_195_179.png \
    dashpngs/gearbox_visual_5_195_162.png \
    dashpngs/iso_195_156.png \
    dashpngs/psd_logo_white.png \
    dashqmls/DashboardGaugeStyle.qml \
    dashqmls/IconGaugeStyle.qml \
    dashqmls/PowerGaugeStyle.qml \
    dashqmls/TachometerStyle.qml \
    dashqmls/TurnIndicator.qml \
    dashqmls/ValueSource.qml \
    imports/QtQuick/Studio/Components/ArcItem.qml \
    imports/QtQuick/Studio/Components/BorderItem.qml \
    imports/QtQuick/Studio/Components/FlipableItem.qml \
    imports/QtQuick/Studio/Components/IsoItem.qml \
    imports/QtQuick/Studio/Components/PieItem.qml \
    imports/QtQuick/Studio/Components/RectangleItem.qml \
    imports/QtQuick/Studio/Components/SvgPathItem.qml \
    imports/QtQuick/Studio/Components/TriangleItem.qml \
    imports/QtQuick/Studio/Components/designer/MyPlugin.metainfo \
    imports/QtQuick/Studio/Components/designer/images/custom-border-16px.png \
    imports/QtQuick/Studio/Components/designer/images/custom-border-24px.png \
    imports/QtQuick/Studio/Components/designer/images/custom-border-24px@2x.png \
    imports/QtQuick/Studio/Components/designer/images/custom-rectangle-16px.png \
    imports/QtQuick/Studio/Components/designer/images/custom-rectangle-24px.png \
    imports/QtQuick/Studio/Components/designer/images/custom-rectangle-24px@2x.png \
    imports/QtQuick/Studio/Components/designer/images/iso-icons-16px.png \
    imports/QtQuick/Studio/Components/designer/images/iso-icons-24px.png \
    imports/QtQuick/Studio/Components/designer/images/iso-icons-24px@2x.png \
    imports/QtQuick/Studio/Components/designer/images/item-arc-16px.png \
    imports/QtQuick/Studio/Components/designer/images/item-arc-24px.png \
    imports/QtQuick/Studio/Components/designer/images/item-arc-24px@2x.png \
    imports/QtQuick/Studio/Components/designer/images/item-flippable-16px.png \
    imports/QtQuick/Studio/Components/designer/images/item-flippable-24px.png \
    imports/QtQuick/Studio/Components/designer/images/item-flippable-24px@2x.png \
    imports/QtQuick/Studio/Components/designer/images/item-pie-16px.png \
    imports/QtQuick/Studio/Components/designer/images/item-pie-24px.png \
    imports/QtQuick/Studio/Components/designer/images/item-pie-24px@2x.png \
    imports/QtQuick/Studio/Components/designer/images/item-svg-16px.png \
    imports/QtQuick/Studio/Components/designer/images/item-svg-24px.png \
    imports/QtQuick/Studio/Components/designer/images/item-svg-24px@2x.png \
    imports/QtQuick/Studio/Components/designer/images/item-triangle-16px.png \
    imports/QtQuick/Studio/Components/designer/images/item-triangle-24px.png \
    imports/QtQuick/Studio/Components/designer/images/item-triangle-24px@2x.png \
    imports/QtQuick/Studio/Components/qmldir \


HEADERS += \
    serialport.h

