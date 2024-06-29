set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_SYSROOT  $ENV{NEUBOAT_TOOLCHAIN_PATH}/${TARGET_BOARD}.rootfs)
set(TOOLCHAIN_ROOT $ENV{NEUBOAT_TOOLCHAIN_PATH}/bootlin-toolchain-9.3.0)

set(CMAKE_C_COMPILER   ${TOOLCHAIN_ROOT}/bin/aarch64-linux-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_ROOT}/bin/aarch64-linux-g++)

set(CMAKE_C_FLAGS_INIT             "-I${CMAKE_SYSROOT}/usr/include/aarch64-linux-gnu")
set(CMAKE_CXX_FLAGS_INIT           "-I${CMAKE_SYSROOT}/usr/include/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS_INIT    "-L${CMAKE_SYSROOT}/usr/lib/aarch64-linux-gnu -Wl,-rpath,/usr/lib/aarch64-linux-gnu --sysroot=${CMAKE_SYSROOT}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/usr/lib/aarch64-linux-gnu -Wl,-rpath,/usr/lib/aarch64-linux-gnu --sysroot=${CMAKE_SYSROOT}")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-L${CMAKE_SYSROOT}/usr/lib/aarch64-linux-gnu -Wl,-rpath,/usr/lib/aarch64-linux-gnu --sysroot=${CMAKE_SYSROOT}")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)