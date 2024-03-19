if (EXISTS "${CURRENT_INSTALLED_DIR}/include/mysql/mysql.h")
    message(FATAL_ERROR "FATAL ERROR: ${PORT} and libmariadb are incompatible.")
endif()

set(PATCH_FILES
    ignore-boost-version.patch
    system-libs.patch
    export-cmake-targets.patch
    Add-target-include-directories.patch
    homebrew.patch
    fix_dup_symbols.patch
    scanner_changes.patch
)

if(NOT VCPKG_TARGET_IS_WINDOWS)
    # these changes are only needed for Linux/MacOS
    set(PATCH_FILES
        ${PATCH_FILES}
        posix_changes.patch
    )
else()
    set(PATCH_FILES
        ${PATCH_FILES}
        posix_win_changes.patch
    )
endif()

set(CROSS_COMPILING "")
set(STACK_DIRECTION "")
if(VCPKG_TARGET_ARCHITECTURE STREQUAL "x86" OR VCPKG_TARGET_ARCHITECTURE STREQUAL "x64")
    set(STACK_DIRECTION -DSTACK_DIRECTION=-1)
else()
    # ARM builds are always cross compiled
    # as such we build the executables (comp_sql, uca9dump, comp_client_err) separately
    set(PATCH_FILES
        ${PATCH_FILES}
        remove_executables.patch
    )
    if(VCPKG_TARGET_IS_LINUX)
        set(CROSS_COMPILING -DCMAKE_CROSSCOMPILING=1)
        set(STACK_DIRECTION -DSTACK_DIRECTION=-1)
    endif()
endif()


vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO mysql/mysql-server
    REF mysql-${VERSION}
    SHA512 8b9f15b301b158e6ffc99dd916b9062968d36f6bdd7b898636fa61badfbe68f7328d4a39fa3b8b3ebef180d3aec1aee353bd2dac9ef1594e5772291390e17ac0
    HEAD_REF master
    PATCHES
        ${PATCH_FILES}
)

file(REMOVE_RECURSE "${SOURCE_PATH}/include/boost_1_70_0")

#Skip the version check for Visual Studio
set(FORCE_UNSUPPORTED_COMPILER "")
if(VCPKG_TARGET_IS_WINDOWS)
    set(FORCE_UNSUPPORTED_COMPILER 1)
endif()

string(COMPARE EQUAL "${VCPKG_LIBRARY_LINKAGE}" "static"  BUILD_STATIC_LIBS)
string(COMPARE EQUAL "${VCPKG_CRT_LINKAGE}" "static"  STATIC_CRT_LINKAGE)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DWITHOUT_SERVER=ON
        -DWITH_BUILD_ID=OFF
        -DWITH_UNIT_TESTS=OFF
        -DENABLED_PROFILING=OFF
        -DENABLE_TESTING=OFF
        -DWIX_DIR=OFF
        ${STACK_DIRECTION}
        -DIGNORE_BOOST_VERSION=ON
        -DDOWNLOAD_BOOST=OFF
        -DWITH_SYSTEMD=OFF
        -DWITH_TEST_TRACE_PLUGIN=OFF
        -DMYSQL_MAINTAINER_MODE=OFF
        -DBUNDLE_RUNTIME_LIBRARIES=OFF
        -DWITH_SSL=system
        -DWITH_ICU=system
        -DWITH_LZ4=system
        -DWITH_ZLIB=system
        -DWITH_LTO=OFF
        -DHANDLE_FATAL_SIGNALS=OFF
        -DWITH_CLIENT_PROTOCOL_TRACING=OFF
        -DWITH_HYPERGRAPH_OPTIMIZER=OFF
        -DFORCE_UNSUPPORTED_COMPILER=ON
        -DINSTALL_STATIC_LIBRARIES=${BUILD_STATIC_LIBS}
        -DLINK_STATIC_RUNTIME_LIBRARIES=${STATIC_CRT_LINKAGE}
        ${CROSS_COMPILING}
    MAYBE_UNUSED_VARIABLES
        BUNDLE_RUNTIME_LIBRARIES # only on windows
        LINK_STATIC_RUNTIME_LIBRARIES # only on windows
        WIX_DIR # only on windows
        WITH_BUILD_ID # only on windows
        IGNORE_BOOST_VERSION # only on windows
        DOWNLOAD_BOOST # only on windows
)

vcpkg_cmake_install(ADD_BIN_TO_PATH)

file(RENAME "${CURRENT_PACKAGES_DIR}/share" "${CURRENT_PACKAGES_DIR}/${PORT}")
file(MAKE_DIRECTORY "${CURRENT_PACKAGES_DIR}/share")
file(RENAME "${CURRENT_PACKAGES_DIR}/${PORT}" "${CURRENT_PACKAGES_DIR}/share/${PORT}")

if(NOT VCPKG_BUILD_TYPE)
    file(RENAME "${CURRENT_PACKAGES_DIR}/debug/share" "${CURRENT_PACKAGES_DIR}/debug/${PORT}")
    file(MAKE_DIRECTORY "${CURRENT_PACKAGES_DIR}/debug/share")
    file(RENAME "${CURRENT_PACKAGES_DIR}/debug/${PORT}" "${CURRENT_PACKAGES_DIR}/debug/share/${PORT}")
endif()

vcpkg_cmake_config_fixup(PACKAGE_NAME unofficial-libmysql CONFIG_PATH share/${PORT}/unofficial-libmysql)

# switch mysql into /mysql
file(RENAME "${CURRENT_PACKAGES_DIR}/include" "${CURRENT_PACKAGES_DIR}/include2")
file(MAKE_DIRECTORY "${CURRENT_PACKAGES_DIR}/include")
file(RENAME "${CURRENT_PACKAGES_DIR}/include2" "${CURRENT_PACKAGES_DIR}/include/mysql")

## delete useless vcruntime/scripts/bin/msg file
file(REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
    "${CURRENT_PACKAGES_DIR}/debug/share"
    "${CURRENT_PACKAGES_DIR}/debug/man"
    "${CURRENT_PACKAGES_DIR}/docs"
    "${CURRENT_PACKAGES_DIR}/debug/docs"
    "${CURRENT_PACKAGES_DIR}/lib/debug"
    "${CURRENT_PACKAGES_DIR}/lib/plugin"
    "${CURRENT_PACKAGES_DIR}/debug/lib/plugin"
)

# delete dynamic dll on static build
if (BUILD_STATIC_LIBS)
    # libmysql.dll
    file(REMOVE_RECURSE 
        "${CURRENT_PACKAGES_DIR}/bin" 
        "${CURRENT_PACKAGES_DIR}/debug/bin"
    )
    file(REMOVE
        "${CURRENT_PACKAGES_DIR}/lib/libmysql.lib"
        "${CURRENT_PACKAGES_DIR}/debug/lib/libmysql.lib"
        "${CURRENT_PACKAGES_DIR}/lib/libmysql.pdb"
        "${CURRENT_PACKAGES_DIR}/debug/lib/libmysql.pdb"
    )
endif()

## remove misc files
file(REMOVE
    "${CURRENT_PACKAGES_DIR}/LICENSE"
    "${CURRENT_PACKAGES_DIR}/README"
    "${CURRENT_PACKAGES_DIR}/debug/LICENSE"
    "${CURRENT_PACKAGES_DIR}/debug/README"
)

vcpkg_replace_string("${CURRENT_PACKAGES_DIR}/include/mysql/mysql_com.h" "#include <mysql/udf_registration_types.h>" "#include \"mysql/udf_registration_types.h\"")

file(INSTALL "${CURRENT_PORT_DIR}/vcpkg-cmake-wrapper.cmake" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
file(INSTALL "${CURRENT_PORT_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

# copy license
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
