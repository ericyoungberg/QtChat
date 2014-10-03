TEMPLATE=app
TARGET=QtChat

QT = gui widgets core

HEADERS += \
  mainwindow.h \
  newcontactdialog.h \
  chatinput.h \
  chatsettings.h \
  conversationbox.h \
  SocketHandler.h \
  helpers.h

SOURCES += \
  main.cpp \
  mainwindow.cpp \
  newcontactdialog.cpp \
  chatinput.cpp \
  chatsettings.cpp \
  conversationbox.cpp \
  SocketHandler.cpp \
  helpers.cpp
