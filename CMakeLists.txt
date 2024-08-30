cmake_minimum_required(VERSION 3.5)

project(QuotationAnalysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QuotationAnalysis main.cpp
    quotesmanager.h quotesmanager.cpp)

include(GNUInstallDirs)
install(TARGETS QuotationAnalysis
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
