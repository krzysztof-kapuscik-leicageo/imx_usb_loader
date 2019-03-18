message(WARNING IMX_ROOT=${IMX_ROOT}/include)

find_path(LIBIMXCOMMON_INCLUDE_DIR
    NAMES
        imx_usb_loader/osal.h
    PATHS
        /usr/include
        /usr/local/include
        ${IMX_ROOT}/include
)

find_library(LIBIMXCOMMON_LIBRARY
    NAMES
        imx_common
    PATHS
        /usr/lib
        /usr/local/lib
        ${IMX_ROOT}/lib
)

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibImxCommon DEFAULT_MSG
    LIBIMXCOMMON_LIBRARY LIBIMXCOMMON_INCLUDE_DIR)

# show the variables only in the advanced view
mark_as_advanced(LIBIMXCOMMON_INCLUDE_DIR LIBIMXCOMMON_LIBRARY)

set(LIBIMXCOMMON_INCLUDE_DIRS
    ${LIBIMXCOMMON_INCLUDE_DIR}
)
set(LIBIMXCOMMON_LIBRARIES
    ${LIBIMXCOMMON_LIBRARY}
)
