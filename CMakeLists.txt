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
#        interfaces/observer/observer_credit_interfaces.h
#        interfaces/observer/observer_deposit_interfaces.h
#        interfaces/observer/observer_graph_interfaces.h
#        interfaces/observer/observer_interface.h
#        interfaces/observer/observer_math_interfaces.h
#        interfaces/credit_controller_interface.h
#        interfaces/credit_model_interface.h
#        interfaces/credit_model_public_interface.h
#        interfaces/deposit_controller_interface.h
#        interfaces/deposit_model_interface.h
#        interfaces/deposit_model_public_interface.h
#        interfaces/graph_controller_interface.h
#        interfaces/graph_model_interface.h
#        interfaces/graph_model_public_interface.h
#        interfaces/math_controller_interface.h
#        interfaces/math_model_interface.h
#        interfaces/math_model_public_interface.h
#
#        controller/math_controller.cc
#        controller/math_controller.h
#        controller/graph_controller.cc
#        controller/graph_controller.h
#        controller/credit_controller.cc
#        controller/credit_controller.h
#        controller/deposit_controller.cc
#        controller/deposit_controller.h
#
#        models/math/math_model.cc
#        models/math/math_model.h
#        models/math/lexeme.cc
#        models/math/lexeme.h
#        models/math/lexer.cc
#        models/math/lexer.h
#        models/graph/graph_model.cc
#        models/graph/graph_model.h
#        models/credit/credit_model.cc
#        models/credit/credit_model.h
#        models/deposit/deposit_model.cc
#        models/deposit/deposit_model.h
#        models/calc_exception.h
#        models/finance_exception.h
#
#        ui/main_view.ui
#        ui/math_view.ui
#        ui/graph_view.ui
#        ui/deposit_view.ui
#        ui/credit_view.ui
#        ui/qrc.qrc
#
#        view/main_view.cc
#        view/main_view.h
#        view/math_view.cc
#        view/math_view.h
#        view/graph_view.cc
#        view/graph_view.h
#        view/deposit_view.cc
#        view/deposit_view.h
#        view/credit_view.cc
#        view/credit_view.h
#        view/view_helpers.cc
#        view/view_helpers.h
#        view/widgets/qcustomplot.cc
#        view/widgets/qcustomplot.h
#
#        helpers/notifier.cc
#        helpers/notifier.h
#        helpers/math_helpers.cc
#        helpers/math_helpers.h
#        helpers/string_helpers.cc
#        helpers/string_helpers.h
#
#        calc_model_facade.cc
#        calc_model_facade.h
#        container_calc_app.cc
#        container_calc_app.h
#        main.cc
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
