cmake_minimum_required(VERSION 3.5)

project(imx_usb_loader)

if(NOT WIN32)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(LibUsb10 REQUIRED)
endif()

set(IMX_COMMON_SOURCES
    image.h
    imx_loader_config.c
    imx_loader_config.h
    imx_loader.h
    imx_sdp.c
    imx_sdp.h
    portable.h
)

set(IMX_USB_SOURCES
    imx_sdp_simulation.c
    imx_sdp_simulation.h
    imx_usb.c
)

set(IMX_UART_SOURCES
    imx_uart.c
)

if(NOT WIN32)
    include(GNUInstallDirs)
    set(IMX_SYSCONFDIR=${CMAKE_INSTALL_FULL_SYSCONFDIR})
else()
    message(FATAL_ERROR "Not working yet")
endif()

file(GLOB CONFIGURATION_FILES "*.conf")

add_executable(imx_usb ${IMX_COMMON_SOURCES} ${IMX_USB_SOURCES})
target_include_directories(imx_usb PRIVATE ${LIBUSB_1_INCLUDE_DIRS})
target_link_libraries(imx_usb PRIVATE ${LIBUSB_1_LIBRARIES})
target_compile_definitions(imx_usb PRIVATE ${LIBUSB_1_DEFINITIONS})
target_compile_definitions(imx_usb PRIVATE SYSCONFDIR="${IMX_SYSCONFDIR}")

add_executable(imx_uart ${IMX_COMMON_SOURCES} ${IMX_UART_SOURCES})
target_compile_definitions(imx_uart PRIVATE SYSCONFDIR="${IMX_SYSCONFDIR}")

install(TARGETS imx_usb
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS imx_uart
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CONFIGURATION_FILES}
    DESTINATION "${CMAKE_INSTALL_SYSCONFDIR}/imx-loader.d")
