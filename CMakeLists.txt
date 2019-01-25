cmake_minimum_required(VERSION 3.5)

project(imx_usb_loader)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(LibUsb10 REQUIRED)
if(WIN32)
	find_package(GetOpt REQUIRED)
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

file(GLOB CONFIGURATION_FILES "*.conf")

include(GNUInstallDirs)
set(IMXRELCONFDIR "${CMAKE_INSTALL_SYSCONFDIR}/imx-loader.d/")
set(IMXABSCONFDIR "${CMAKE_INSTALL_FULL_SYSCONFDIR}/imx-loader.d/")
if(WIN32)
	STRING(REPLACE "/" "\\\\" IMXRELCONFDIR ${IMXRELCONFDIR})
	STRING(REPLACE "/" "\\\\" IMXABSCONFDIR ${IMXABSCONFDIR})
endif()

add_library(imx_common STATIC ${IMX_COMMON_SOURCES})
target_compile_definitions(imx_common PRIVATE IMXRELCONFDIR="${IMXRELCONFDIR}")
target_compile_definitions(imx_common PRIVATE IMXABSCONFDIR="${IMXABSCONFDIR}")
if(MSVC)
	target_compile_definitions(imx_common PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif()
add_custom_command(TARGET imx_common
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CONFIGURATION_FILES}
		$<TARGET_FILE_DIR:imx_common>)

add_executable(imx_usb ${IMX_USB_SOURCES})
target_include_directories(imx_usb PRIVATE ${LIBUSB10_INCLUDE_DIRS})
target_link_libraries(imx_usb PRIVATE imx_common)
target_link_libraries(imx_usb PRIVATE ${LIBUSB10_LIBRARIES})
target_compile_definitions(imx_usb PRIVATE ${LIBUSB10_DEFINITIONS})
if(WIN32)
	target_include_directories(imx_usb PRIVATE ${GETOPT_INCLUDE_DIRS})
	target_link_libraries(imx_usb PRIVATE ${GETOPT_LIBRARIES})
endif()
if(MSVC)
	target_compile_definitions(imx_usb PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif()

add_executable(imx_uart ${IMX_UART_SOURCES})
target_link_libraries(imx_uart PRIVATE imx_common)
if(WIN32)
	target_include_directories(imx_uart PRIVATE ${GETOPT_INCLUDE_DIRS})
	target_link_libraries(imx_uart PRIVATE ${GETOPT_LIBRARIES})
endif()
if(MSVC)
	target_compile_definitions(imx_uart PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif()

install(TARGETS imx_usb
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS imx_uart
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CONFIGURATION_FILES}
    DESTINATION ${IMXRELCONFDIR})
if(WIN32)
	include (InstallRequiredSystemLibraries)

    install(
        DIRECTORY $<TARGET_FILE_DIR:imx_uart>/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILES_MATCHING PATTERN "*.dll")
endif()
