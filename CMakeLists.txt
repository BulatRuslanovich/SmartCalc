cmake_minimum_required(VERSION 3.7)

project(SmartCalcV2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_compile_options(-Wall -Wextra -Werror -pedantic)

set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/SmartCalc_v2.0")
set(CMAKE_PREFIX_PATH "/home/bulatruslanovich/Desktop/Qt/6.5.3/gcc_64")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS PrintSupport REQUIRED)

set(PROJECT_SOURCES
        models/math/token.h
        models/math/token.cpp
        models/math/tokenHandle.h
        models/math/tokenHandle.cpp
        models/math/mathModel.h
        models/math/mathModel.cpp
        models/calcException.h
        models/financeException.h

        helpers/annunciator.h
        helpers/annunciator.cpp
        helpers/mathHelper.h
        helpers/mathHelper.cpp
        helpers/stringHelper.h
        helpers/stringHelper.cpp

        interfaces/observers/IObserver.h
        interfaces/observers/IObserverDeposit.h
        interfaces/observers/IObserverGraph.h
        interfaces/observers/IObserverMath.h
        interfaces/observers/IObserverCredit.h

        interfaces/ICreditModel.h
        interfaces/ICreditModelPublic.h
        interfaces/ICreditController.h
        interfaces/IDepositModel.h
        interfaces/IDepositModelPublic.h
        interfaces/IDepositController.h
        interfaces/IGraphModelPublic.h
        interfaces/IGraphModel.h
        interfaces/IGraphController.h
        interfaces/IMathModel.h
        interfaces/IMathModelPublic.h
        interfaces/IMathController.h
        models/graph/GraphModel.cpp
        models/graph/GraphModel.h
        models/credit/creditModel.cpp
        models/credit/creditModel.h
        models/deposit/DepositModel.cpp
        models/deposit/DepositModel.h
        controllers/MathController.cpp
        controllers/MathController.h
        controllers/GraphController.cpp
        controllers/GraphController.h
        controllers/CreditController.cpp
        controllers/CreditController.h
        controllers/DepositController.cpp
        controllers/DepositController.h
        views/ViewHelpers.cpp
        views/ViewHelpers.h
        views/CreditView.cpp
        views/CreditView.h
        views/GraphView.cpp
        views/GraphView.h

        views/widgets/qcustomplot.cc
        views/widgets/qcustomplot.h
        views/MathView.cpp
        views/MathView.h
        views/DepositView.cpp
        views/DepositView.h
        views/MainView.cpp
        views/MainView.h
        MasterModel.cpp
        MasterModel.h


)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SmartCalcV2
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${app_icon_macos}
    )
else()
    if(ANDROID)
        add_library(SmartCalcV2 SHARED
                ${PROJECT_SOURCES}
        )
    else()
        add_executable(SmartCalcV2
                ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SmartCalcV2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SmartCalcV2 PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)

set_target_properties(SmartCalcV2 PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

install(TARGETS SmartCalcV2
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SmartCalcV2)
endif()
