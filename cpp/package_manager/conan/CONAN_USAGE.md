### Conan commands

1. Show conan home directory

    ```bash
    conan config home
    ```

1. Set default profile

    ```bash
    conan profile detect --force
    ```

1. Check default profile path

    ```bash
    conan profile path default
    # edit the default profile if needed
    vi ~/<default_profile_path>
    ```

1. List all profiles

    ```bash
    conan profile list
    ```

1. Show dependencies graph in html format

    ```bash
    conan graph info conanfile.py --format=html > graph.html
    ```

> `CMakeDeps` generates files so that CMake finds the downloaded library.
> `CMakeToolchain` generates a toolchain file for CMake so that we can transparently build our project with CMake using the same settings that we detected for our default profile.


### Native build

1. Install dependencies

    ```bash
    conan install . -of=build/x86_64 -b=missing
    ```

1. Build project

    Option 1: CMake
    ```bash
    cmake -B build/x86_64 -DCMAKE_TOOLCHAIN_FILE=./build/x86_64/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build build/x86_64
    ```

    Option 2: Conan
     ```bash
    conan build . -of=build/x86_64 -pr:b profiles/linux-arch-x86-64
    # or just directly build the project with installation
    conan build . -of=build/x86_64 -b=missing -pr:b profiles/linux-arch-x86-64
    ```

1. Create package

    ```bash
    conan create .
    # or just directly create the package with installation
    conan create . -b=missing
    ```

### Cross build

1. Install dependencies

    ```bash
    conan install . -s arch=armv8 -of=build/armv8 -b=missing
    ```

1. Build project

    Option 1: CMake
    ```bash
    cmake -B build/armv8 -DCMAKE_TOOLCHAIN_FILE=./build/armv8/build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build build/armv8
    ```

    Option 2: Conan
     ```bash
    conan build . -of=build/armv8 -pr:b profiles/linux-arch-x86-64 -pr:h profiles/linux-arch-armv8
    # or just directly build the project with installation
    conan build . -of=build/armv8 -b=missing -pr:b profiles/linux-arch-x86-64 -pr:h profiles/linux-arch-armv8
    ```

1. Create package

    ```bash
    conan create . -s arch=armv8
    # or just directly create the package with installation
    conan create . -s arch=armv8 -b=missing
    ```
