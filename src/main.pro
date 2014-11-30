TEMPLATE=app
TARGET=QtChat

QT = gui widgets core dbus

HEADERS += \
  net/NetworkHandler.h \ 
  net/ChatAdaptor.h \
  net/ChatInterface.h \
  \
  gui/MainWindow.h \
  gui/NewContactDialog.h \
  gui/ChatInput.h \
  gui/ChatSettings.h \
  gui/ConversationBox.h \
  gui/Helpers.h 

SOURCES += \
  main.cpp \
  \
  net/NetworkHandler.cpp \
  net/ChatAdaptor.cpp \
  net/ChatInterface.cpp \
  \
  gui/MainWindow.cpp \
  gui/NewContactDialog.cpp \
  gui/ChatInput.cpp \
  gui/ChatSettings.cpp \
  gui/ConversationBox.cpp \
  gui/Helpers.cpp 
