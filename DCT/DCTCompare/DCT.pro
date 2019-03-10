QT += core gui widgets

TARGET = DCTCompare
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        MainWindow.cpp \
        QCanvas.cpp

HEADERS += \
        MainWindow.hpp \
        QCanvas.hpp

FORMS += \
        MainWindow.ui

INCLUDEPATH += \
	../DCTLib

win32: {

	CONFIG += \
		c++11

	CONFIG(debug, debug|release): LIBS += \
		-L../../lib/ -lDCTLib

	CONFIG(release, debug|release): LIBS += \
		-L../../lib/ -lDCTLib

}

linux: {

	LIBS += \
		-L../../lib/ -lDCTLib

}

clang {

    CONFIG += \
		c++11

}

gcc:!clang {

    CONFIG += \
		c++11

}
