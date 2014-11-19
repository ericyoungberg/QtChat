TEMPLATE=app
TARGET=QtChat

QT = gui widgets core dbus

HEADERS += \
  net/NetworkHandler.h \ 
  net/ChatAdaptor.h \
  net/ChatInterface.h \
  \
  gui/mainwindow.h \
  gui/newcontactdialog.h \
  gui/chatinput.h \
  gui/chatsettings.h \
  gui/conversationbox.h \
  gui/helpers.h 

SOURCES += \
  main.cpp \
  \
  net/NetworkHandler.cpp \
  net/ChatAdaptor.cpp \
  net/ChatInterface.cpp \
  \
  gui/mainwindow.cpp \
  gui/newcontactdialog.cpp \
  gui/chatinput.cpp \
  gui/chatsettings.cpp \
  gui/conversationbox.cpp \
  gui/helpers.cpp 
