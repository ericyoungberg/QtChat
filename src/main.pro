TEMPLATE=app
TARGET=QtChat

QT = gui widgets core dbus

HEADERS += \
  gui/mainwindow.h \
  gui/newcontactdialog.h \
  gui/chatinput.h \
  gui/chatsettings.h \
  gui/conversationbox.h \
  gui/NetworkHandler.h \
  gui/helpers.h 

SOURCES += \
  main.cpp \
  gui/mainwindow.cpp \
  gui/newcontactdialog.cpp \
  gui/chatinput.cpp \
  gui/chatsettings.cpp \
  gui/conversationbox.cpp \
  gui/NetworkHandler.cpp \ 
  gui/helpers.cpp 
