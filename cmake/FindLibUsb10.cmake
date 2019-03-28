include(MiscHelpers)

find_path(LIBUSB10_INCLUDE_DIR
	NAMES
		libusb-1.0/libusb.h
		libusb.h
	PATH_SUFFIXES
		libusb-1.0
)

find_library_release(LIBUSB10_LIBRARY_RELEASE
    NAMES
        usb-1.0
        usb
        libusb-1.0
)

find_library_debug(LIBUSB10_LIBRARY_DEBUG
    NAMES
        usb-1.0
        usb
        libusb-1.0
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibUsb10 DEFAULT_MSG
    LIBUSB10_INCLUDE_DIR
    LIBUSB10_LIBRARY_DEBUG
    LIBUSB10_LIBRARY_RELEASE
)

mark_as_advanced(
    LIBUSB10_INCLUDE_DIR
    LIBUSB10_LIBRARY_DEBUG
    LIBUSB10_LIBRARY_RELEASE
)

set(LIBUSB10_INCLUDE_DIRS
	${LIBUSB10_INCLUDE_DIR}
)
set(LIBUSB10_LIBRARIES
	debug     ${LIBUSB10_LIBRARY_DEBUG}
    optimized ${LIBUSB10_LIBRARY_RELEASE}
)
