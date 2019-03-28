cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(imxusbloader_VERSION_MAJOR             1)
set(imxusbloader_VERSION_MINOR             0)
set(imxusbloader_VERSION_PATCH             3)
set(imxusbloader_VERSION_TWEAK             0)
set(imxusbloader_VERSION_FULL              "${imxusbloader_VERSION_MAJOR}.${imxusbloader_VERSION_MINOR}.${imxusbloader_VERSION_PATCH}.${imxusbloader_VERSION_TWEAK}")

project(imxusbloader VERSION ${imxusbloader_VERSION_FULL})

include(MiscHelpers)
vcpkg_setup_linking()

find_package(LibUsb10 REQUIRED)

set(IMX_COMMON_SOURCES
    image.h
    imx_loader_config.c
    imx_loader_config.h
    imx_loader.h
    imx_sdp.c
    imx_sdp.h
    osal.h
    osal.c
    portable.h
)

if(MSVC)
    set(IMX_GETOPT_SOURCES
        "msvc/getopt.h"
        "msvc/getopt.c"
    )
else()
    set(IMX_GETOPT_SOURCES "")
endif()

set(IMX_COMMON_PUBLIC_HEADERS
    osal.h
)

set(IMX_USB_CORE_SOURCES
    imx_sdp_simulation.c
    imx_sdp_simulation.h
    imx_usb.c
    imx_usb_lib.h
)

set(IMX_USB_CORE_PUBLIC_HEADERS
    imx_usb_lib.h
)

set(IMX_USB_SOURCES
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
set_target_properties(imx_common PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(imx_common PROPERTIES PUBLIC_HEADER "${IMX_COMMON_PUBLIC_HEADERS}")
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

add_library(imx_usb_core STATIC ${IMX_USB_CORE_SOURCES})
set_target_properties(imx_usb_core PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(imx_usb_core PROPERTIES PUBLIC_HEADER "${IMX_USB_CORE_PUBLIC_HEADERS}")
target_compile_definitions(imx_usb_core PRIVATE IMX_USB_LIB_ONLY)
target_link_libraries(imx_usb_core PUBLIC imx_common)
target_include_directories(imx_usb_core PRIVATE ${LIBUSB10_INCLUDE_DIRS})
target_link_libraries(imx_usb_core PUBLIC ${LIBUSB10_LIBRARIES})
target_compile_definitions(imx_usb_core PRIVATE ${LIBUSB10_DEFINITIONS})
if(MSVC)
    target_compile_definitions(imx_usb_core PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif()

add_executable(imx_usb ${IMX_USB_SOURCES} ${IMX_GETOPT_SOURCES})
set_target_properties(imx_usb PROPERTIES DEBUG_POSTFIX "d")
target_compile_definitions(imx_usb PRIVATE IMX_USB_APP_ONLY)
target_link_libraries(imx_usb PRIVATE imx_common)
target_link_libraries(imx_usb PRIVATE imx_usb_core)
target_include_directories(imx_usb PRIVATE ${LIBUSB10_INCLUDE_DIRS})
target_link_libraries(imx_usb PRIVATE ${LIBUSB10_LIBRARIES})
target_compile_definitions(imx_usb PRIVATE ${LIBUSB10_DEFINITIONS})
if(MSVC)
    target_include_directories(imx_usb PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_compile_definitions(imx_usb PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif()

add_executable(imx_uart ${IMX_UART_SOURCES} ${IMX_GETOPT_SOURCES})
set_target_properties(imx_uart PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(imx_uart PRIVATE imx_common)
if(MSVC)
    target_include_directories(imx_uart PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_compile_definitions(imx_uart PRIVATE _CRT_SECURE_NO_WARNINGS=1)
endif()

#---------- Installation -------------

install(TARGETS imx_common
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT "dev"
    PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imx_usb_loader
        COMPONENT "dev"
)
install(TARGETS imx_usb_core
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT "dev"
    PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imx_usb_loader
        COMPONENT "dev"
)

install(TARGETS imx_usb
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT "runtime"
)
install(TARGETS imx_uart
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT "runtime"
)
install(FILES ${CONFIGURATION_FILES}
    DESTINATION ${IMXRELCONFDIR}
    COMPONENT "config"
)
if(WIN32)
    if(NOT ${VCPKG_TARGET_TRIPLET} MATCHES "static")
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT "runtime")
        include (InstallRequiredSystemLibraries)
    endif()

    install(DIRECTORY $<TARGET_FILE_DIR:imx_uart>/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT "runtime"
        FILES_MATCHING PATTERN "*.dll"
    )
endif()

#---------- Testing -------------

enable_testing()

set(TESTS_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR})
if(WIN32)
    STRING(REPLACE "/" "\\\\" TESTS_CONFIG_DIR ${TESTS_CONFIG_DIR})
endif()

if(WIN32)
    set(TEST_RUNNER "runtest.bat")
else()
    set(TEST_RUNNER "runtest.sh")
endif()

add_test(
    NAME test.imx
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
    COMMAND ${TEST_RUNNER} $<TARGET_FILE:imx_usb> test.imx ${TESTS_CONFIG_DIR}
)
add_test(
    NAME test-dcd.imx
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
    COMMAND ${TEST_RUNNER} $<TARGET_FILE:imx_usb> test-dcd.imx ${TESTS_CONFIG_DIR}
)
add_test(
    NAME test-plugin.imx
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
    COMMAND ${TEST_RUNNER} $<TARGET_FILE:imx_usb> test-plugin.imx ${TESTS_CONFIG_DIR}
)

#---------- Packaging -------------

# Copy needed, WIX must have valid file extension
configure_file("COPYING" "COPYING.TXT" COPYONLY)

# CPack configuration
set(CPACK_PACKAGE_VERSION_MAJOR         ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR         ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH         ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_NAME                  "ImxUsbLoader")
set(CPACK_PACKAGE_VENDOR                "Octaedr Software")
#set(CPACK_PACKAGE_DESCRIPTION          "")
#set(CPACK_PACKAGE_DESCRIPTION_FILE     "")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   "i.MX/Vybrid recovery utility")
#set(CPACK_PACKAGE_ICON                 "")
set(CPACK_RESOURCE_FILE_LICENSE         "${CMAKE_CURRENT_BINARY_DIR}/COPYING.TXT")
#set(CPACK_RESOURCE_FILE_README         "")
#set(CPACK_RESOURCE_FILE_WELCOME        "")
#set(CPACK_PACKAGE_EXECUTABLES          "imx_usb;imx_uart")
#set(CPACK_CREATE_DESKTOP_LINKS         "imx_usb;imx_uart")
set(CPACK_PACKAGE_INSTALL_DIRECTORY     "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGE_CONTACT               "k.kapuscik@gmail.com")

if(WIN32)
    set(CPACK_GENERATOR                 "WIX;ZIP")

    set(CPACK_WIX_UPGRADE_GUID          "40239A0C-7E51-4E4A-AFA9-E74606C68A70")
    list(APPEND CPACK_WIX_EXTRA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/wix_extras.wxs")
    list(APPEND CPACK_WIX_PATCH_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/wix_patch.xml")
elseif(UNIX)
    set(CPACK_GENERATOR                 "DEB;ZIP")
    set(CPACK_DEB_COMPONENT_INSTALL     "1")

    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE   "amd64")
    else()
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE   "i386")
    endif()

    set(CPACK_DEBIAN_PACKAGE_NAME       "imx-usb-loader")
    set(CPACK_DEBIAN_PACKAGE_VERSION    "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS    "libusb-1.0-0 (>= 2:1.0.20-1)")
    set(CPACK_DEBIAN_PACKAGE_SECTION    "embedded")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE   "https://github.com/saveman/imx_usb_loader")

    set(CPACK_PACKAGE_FILE_NAME         "${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_DEBIAN_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
endif()

include(CPack)

cpack_add_component(runtime
    DISPLAY_NAME "Runtime"
    DESCRIPTION "Programs and required libraries"
    REQUIRED)
cpack_add_component(config
    DISPLAY_NAME "Configuration"
    DESCRIPTION "Set of default configuration files"
    DEPENDS runtime)
cpack_add_component(dev
    DISPLAY_NAME "Development"
    DESCRIPTION "Libraries and headers used for development")
