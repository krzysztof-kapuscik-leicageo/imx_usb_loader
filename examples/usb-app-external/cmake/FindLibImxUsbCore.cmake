find_path(LIBIMXUSBCORE_INCLUDE_DIR
    NAMES
        imx_usb_loader/imx_usb_lib.h
    PATHS
        /usr/include
        /usr/local/include
        ${IMX_ROOT}/include
)

find_library(LIBIMXUSBCORE_LIBRARY
    NAMES
        imx_usb_core
    PATHS
        /usr/lib
        /usr/local/lib
        ${IMX_ROOT}/lib
)

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibImxUsbCore DEFAULT_MSG
    LIBIMXUSBCORE_LIBRARY LIBIMXUSBCORE_INCLUDE_DIR)

# show the variables only in the advanced view
mark_as_advanced(LIBIMXUSBCORE_INCLUDE_DIR LIBIMXUSBCORE_LIBRARY)

set(LIBIMXUSBCORE_INCLUDE_DIRS
    ${LIBIMXUSBCORE_INCLUDE_DIR}
)
set(LIBIMXUSBCORE_LIBRARIES
    ${LIBIMXUSBCORE_LIBRARY}
)
