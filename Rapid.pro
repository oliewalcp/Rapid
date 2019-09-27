QT += gui core widgets

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_LIB

LIBS += -lWS2_32

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/Memory.cpp\
    Core/Conver.cpp \
    Core/IO.cpp \
    Image/ImageBase.cpp \
    Test/TestAVLTree.cpp \
    Test/TestBinaryTree.cpp \
    Test/TestDoubleLinkedList.cpp \
    Test/TestGenetic.cpp \
    Test/TestMap.cpp \
    Test/TestMatrix.cpp \
    Test/TestMemory.cpp \
    Test/TestRedBlackTree.cpp \
    Test/TestSet.cpp \
    Test/TestSingleLinkedList.cpp \
    Test/TestSorter.cpp \
    Test/TestStack.cpp \
    Test/TestVector.cpp \
    Test/TreeTool.cpp \
        main.cpp

HEADERS += \
    Algorithm/Genetic.h \
    Algorithm/NeuralNetwork.h \
    Algorithm/Sorter.h \
    Core/AVLTree.h \
    Core/Atomic.h \
    Core/BinaryTree.h \
    Core/Compare.h \
    Core/Exception.h \
    Core/IO.h \
    Core/Map.h \
    Core/Matrix.h \
    Core/Memory.h \
    Core/Range.h \
    Core/RapidConfig.h \
    Core/RedBlackTree.h \
    Core/Set.h \
    Core/SharedPointer.h \
    Core/TLNode.h \
    Core/TypeTraits.h \
    Core/Vector.h \
    Core/Stack.h \
    Core/Conver.h \
    Core/SingleLinkedList.h \
    Core/DoubleLinkedList.h \
    Core/Preoperation.h \
    Core/Version.h \
    Image/ImageBase.h \
    Network/Socket.h \
    Test/TestAVLTree.h \
    Test/TestBinaryTree.h \
    Test/TestDoubleLinkedList.h \
    Test/TestGenetic.h \
    Test/TestMap.h \
    Test/TestMatrix.h \
    Test/TestMemory.h \
    Test/TestRedBlackTree.h \
    Test/TestSet.h \
    Test/TestSingleLinkedList.h \
    Test/TestSorter.h \
    Test/TestStack.h \
    Test/TestVector.h \
    Test/TreeTool.h
