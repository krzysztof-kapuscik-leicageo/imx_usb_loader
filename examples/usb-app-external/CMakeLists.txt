cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(LibUsb10 REQUIRED)
find_package(LibImxCommon REQUIRED)
find_package(LibImxUsbCore REQUIRED)

add_executable(usb-app-external main.c)

target_compile_definitions(usb-app-external PRIVATE ${LIBUSB10_DEFINITIONS})

target_include_directories(usb-app-external PRIVATE ${LIBIMXCOMMON_INCLUDE_DIRS})
target_include_directories(usb-app-external PRIVATE ${LIBIMXUSBCORE_INCLUDE_DIRS})
target_include_directories(usb-app-external PRIVATE ${LIBUSB10_INCLUDE_DIRS})

target_link_libraries(usb-app-external PRIVATE ${LIBIMXUSBCORE_LIBRARIES})
target_link_libraries(usb-app-external PRIVATE ${LIBIMXCOMMON_LIBRARIES})
target_link_libraries(usb-app-external PRIVATE ${LIBUSB10_LIBRARIES})
