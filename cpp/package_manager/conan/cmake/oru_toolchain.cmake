### toolchain variables
set(arch      aarch64)
set(bin       ${arch}-linux-gnu)

### cmake system environments
set(CMAKE_SYSTEM_NAME      Linux)
set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_PROCESSOR ${arch})

### compiler
set(CMAKE_C_COMPILER   ${bin}-gcc)
set(CMAKE_CXX_COMPILER ${bin}-g++)
set(CMAKE_AR           ${bin}-ar)
set(CMAKE_STRIP        ${bin}-strip)
set(CMAKE_LINKER       ${bin}-ld)
set(CMAKE_NM           ${bin}-nm)
set(CMAKE_OBJCOPY      ${bin}-objcopy)
set(CMAKE_OBJDUMP      ${bin}-objdump)
set(CMAKE_RANLIB       ${bin}-ranlib)
