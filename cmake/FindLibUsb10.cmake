# - Try to find libusb-1.0
# Once done this will define
#
# LIBUSB10_FOUND - system has libusb
# LIBUSB10_INCLUDE_DIRS - the libusb include directory
# LIBUSB10_LIBRARIES - Link these to use libusb
# LIBUSB10_DEFINITIONS - Compiler switches required for using libusb
#
# Adapted from cmake-modules Google Code project
#
# Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#
# (Changes for libusb) Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com>
#
# (Changes) Copyright (c) 2019 Krzysztof Kapuscik
#
# Redistribution and use is allowed according to the terms of the New BSD license.
#
# CMake-Modules Project New BSD License
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# * Neither the name of the CMake-Modules Project nor the names of its
# contributors may be used to endorse or promote products derived from this
# software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

find_path(LIBUSB10_INCLUDE_DIR
	NAMES
		libusb.h
	PATHS
		/usr/include
		/usr/local/include
		/sw/include
	PATH_SUFFIXES
		libusb-1.0
)

find_library(LIBUSB10_LIBRARY
	NAMES
		usb-1.0 usb libusb-1.0
	PATHS
		/usr/lib
		/usr/local/lib
		/sw/lib
)

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibUsb10 DEFAULT_MSG
								  LIBUSB10_LIBRARY LIBUSB10_INCLUDE_DIR)

# show the variables only in the advanced view
mark_as_advanced(LIBUSB10_INCLUDE_DIR LIBUSB10_LIBRARY)

set(LIBUSB10_INCLUDE_DIRS
	${LIBUSB10_INCLUDE_DIR}
)
set(LIBUSB10_LIBRARIES
	${LIBUSB10_LIBRARY}
)
