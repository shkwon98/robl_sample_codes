from conan import ConanFile

from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy

class HelloRecipe(ConanFile):
    name = "hello"
    version = "1.0.0"

    # Optional metadata
    # license = "<Put the package license here>"
    # author = "<Put your name here> <And your email here>"
    # url = "<Package recipe repository url here, for issues about the package>"
    # description = "<Description of hello package here>"
    # topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    # options = {"shared": [True, False], "fPIC": [True, False]}
    # default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "protos/*"

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.os != "Linux":
            raise ConanInvalidConfiguration("Only Linux supported")
        if self.settings.arch not in ("x86_64", "armv8"):
            raise ConanInvalidConfiguration("Only x86_64 and armv8 supported")

    def requirements(self):
        self.requires("fmt/10.2.1")
        self.requires("opencv/4.5.5")
        self.requires("sqlite_orm/1.6")
        self.requires("boost/1.83.0")
        self.requires("pcl/1.13.1")
        self.requires("protobuf/3.21.12")
        self.requires("grpc/1.54.3")

    def build_requirements(self):
        self.tool_requires("protobuf/<host_version>")

    def configure(self):
        self.options["opencv/*"].with_webp = False
        self.options["opencv/*"].with_ffmpeg = False
        self.options["opencv/*"].with_qt = False
        self.options["opencv/*"].with_gtk = False
        self.options["opencv/*"].with_tiff = False
        self.options["pcl/*"].with_libusb = False

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        # Cross-compilation setup for armv8
        if self.settings.arch == "armv8":
            tc.variables["CMAKE_SYSTEM_NAME"] = "Linux"
            tc.variables["CMAKE_SYSTEM_PROCESSOR"] = "aarch64"
            tc.variables["CMAKE_C_COMPILER"] = "aarch64-linux-gnu-gcc"
            tc.variables["CMAKE_CXX_COMPILER"] = "aarch64-linux-gnu-g++"
            tc.variables["CMAKE_LINKER"] = "aarch64-linux-gnu-ld"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        # Copying dependencies to the package
        for dep in self.dependencies.values():
            for libdir in dep.cpp_info.libdirs:
                copy(self, "*", src=libdir, dst=self.package_folder + "/lib")

    def package_info(self):
        self.output.info("Setting package environment variables.")
        self.cpp_info.libs = ["example_project_lib"]  # Replace with your actual library names, if applicable.
        self.cpp_info.requires = ["protobuf::libprotobuf"]
