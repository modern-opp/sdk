from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout


class OPPFrontendConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("llvm/17.0.2")
        self.requires("gtest/1.16.0")

    def build_requirements(self):
        self.tool_requires("bison/3.8.2")

    def configure(self):
        self.options["llvm"].conan_center_index_limits = False

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
