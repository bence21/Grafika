QT += core gui widgets opengl

win32 {
    CONFIG(release, debug|release): LIBS += -L"$$PWD/Dependencies/Lib/GL/" -lglew32 -lglu32 -lopengl32
    CONFIG(debug, debug|release): LIBS += -L"$$PWD/Dependencies/Lib/GL/" -lglew32 -lglu32 -lopengl32
    INCLUDEPATH += "$$PWD/Dependencies/Include"
    DEPENDPATH += "$$PWD/Dependencies/Lib/GL"
}

mac {
    # for GLEW installed into /usr/lib/libGLEW.so or /usr/lib/glew.lib
    # if libraries GLU or OpenGL are required uncomment the commented libraries
    LIBS += -lGLEW #-lGL
}

unix {
    # for GLEW installed into /usr/lib/libGLEW.so or /usr/lib/glew.lib
    # if libraries GLU or OpenGL are required uncomment the commented libraries
    LIBS += -lGLEW -lGL
}

FORMS += \
    GUI/MainWindow.ui \
    GUI/SideWidget.ui

HEADERS += \
    GUI/GLWidget.h \
    GUI/MainWindow.h \
    GUI/SideWidget.h \
    Core/Exceptions.h \
    test.h \
    Core/RealSquareMatrices.h \
    #Core/*.h\
    #Parametric/*.h
    Test/TestFunctions.h \
    Core/GenericCurves3.h \
    Core/LinearCombination3.h \
    Core/TriangulatedMeshes3.h \
    Core/TriangularFaces.h \
    Core/DCoordinates3.h \
    Core/Lights.h \
    Core/Materials.h \
    Parametric/ParametricCurves3.h \
    Parametric/ParametricSurfaces3.h \
    Cyclic/CyclicCurves3.h \
    Trigonometric/trigonometricbernsteinsurfaces3.h \
    Core/TensorProductSurfaces3.h \
    Bezier/BicubicBezierPatches.h \
    Bezier/BezierArc3.h \
    Core/GL/glew.h \
    Core/Colors4.h \
    Core/Constants.h \
    Core/HCoordinates3.h \
    Core/Matrices.h \
    Core/MatricesR.h \
    Core/ShaderPrograms.h \
    Core/TCoordinates4.h \
    Core/TensorProductSurfaces3R.h \
    CoreE/GL/glew.h \
    CoreE/Colors4.h \
    CoreE/Constants.h \
    CoreE/DCoordinates3.h \
    CoreE/Exceptions.h \
    CoreE/GenericCurves3.h \
    CoreE/HCoordinates3.h \
    CoreE/Lights.h \
    CoreE/LinearCombination3.h \
    CoreE/Materials.h \
    CoreE/Matrices.h \
    CoreE/RealSquareMatrices.h \
    CoreE/ShaderPrograms.h \
    CoreE/TCoordinates4.h \
    CoreE/TensorProductSurfaces3.h \
    CoreE/TriangularFaces.h \
    CoreE/TriangulatedMeshes3.h \
    Cyclic/CyclicCurves3E.h \
    Dependencies/Include/GL/glew.h

SOURCES += \
    GUI/GLWidget.cpp \
    GUI/MainWindow.cpp \
    GUI/SideWidget.cpp \
    main.cpp \
    #test.cpp
    Test/TestFunctions.cpp \
    Core/GenericCurves3.cpp \
    Core/LinearCombination3.cpp \
    Parametric/ParametricCurves3.cpp \
    Parametric/ParametricSurfaces3.cpp \
    Core/RealSquareMatrices.cpp \
    Core/TriangulatedMeshes3.cpp \
    Core/Lights.cpp \
    Core/Materials.cpp \
    Core/TensorProductSurfaces3.cpp \
    Cyclic/CyclicCurves3.cpp \
    Bezier/BicubicBezierPatches.cpp \
    Bezier/BezierArc3.cpp \
    Bezier/BicubicBezierPatchesP.cpp \
    Core/ShaderPrograms.cpp \
    CoreE/GenericCurves3.cpp \
    CoreE/Lights.cpp \
    CoreE/LinearCombination3.cpp \
    CoreE/Materials.cpp \
    CoreE/RealSquareMatrices.cpp \
    CoreE/ShaderPrograms.cpp \
    CoreE/TensorProductSurfaces3.cpp \
    CoreE/TriangulatedMeshes3.cpp \
    Cyclic/CyclicCurves3E.cpp \
    Trigonometric/trigonometricbernsteinsurfaces3.cpp \
    test.cpp
    #Core/*.cpp \
    #Parametric/*.cpp \
    #./*.cpp

