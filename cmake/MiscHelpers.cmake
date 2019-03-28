macro(find_library_release)
    if(VCPKG_TARGET_TRIPLET)
        list(APPEND CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug)
        list(APPEND CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib)
        list(APPEND CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib/manual-link)
    endif()

    find_library(${ARGN})

    if(VCPKG_TARGET_TRIPLET)
        list(REMOVE_ITEM CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug)
        list(REMOVE_ITEM CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib)
        list(REMOVE_ITEM CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/lib/manual-link)
    endif()
endmacro()

macro(find_library_debug)
    find_library(${ARGN})
endmacro()

macro(configure_link_flags)
    if(MSVC AND VCPKG_TARGET_TRIPLET)
        set(MSVC_C_CXX_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
        )
        if(${VCPKG_TARGET_TRIPLET} MATCHES "static")
            message(STATUS "VCPKG: static link")
            foreach(flag ${MSVC_C_CXX_FLAGS})
                if(${flag} MATCHES "/MD")
                    string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
                endif()
            endforeach()
            set(VCPKG_CRT_LINKAGE "static")
            set(VCPKG_LIBRARY_LINKAGE "static")
        else()
            message(STATUS "VCPKG: dynamic link")
            foreach(flag ${MSVC_C_CXX_FLAGS})
                if(${flag} MATCHES "/MT")
                    string(REGEX REPLACE "/MT" "/MD" ${flag} "${${flag}}")
                endif()
            endforeach()
            set(VCPKG_CRT_LINKAGE "dynamic")
            set(VCPKG_LIBRARY_LINKAGE "dynamic")
        endif()
    endif()
endmacro()

macro(print_link_flags)
    if(MSVC)
        set(MSVC_C_CXX_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
        )
        message(STATUS "Build flags:")
        foreach(flag ${MSVC_C_CXX_FLAGS})
            message(STATUS " ${flag}: ${${flag}}")
        endforeach()
        message(STATUS "")
    endif()
endmacro()

macro(vcpkg_setup_linking)
    if(MSVC)
        configure_link_flags()
        print_link_flags()
        # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /VERBOSE:LIB")
    endif()
endmacro()
