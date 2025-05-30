from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeDeps, CMakeToolchain, CMake
from conan.tools.build import can_run, cross_building

import os
import re


class TestPackageConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.21.3 <4.0.0]")
        self.tool_requires("ninja/[>=1.10.0 <2.0.0]")

    def generate(self):
        deps = CMakeDeps(self)
        deps.check_components_exist = True
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def _llvm_major_version(self):
        pattern = re.compile("^llvm/([0-9]+)")
        return int(re.findall(pattern, self.tested_reference_str)[0])

    def _ccpstd(self):
        cppstd = 14
        if self._llvm_major_version() >= 16:
            cppstd = 17
        return cppstd

    def build(self):
        cmake = CMake(self)
        cmake_definitions = {
            'CMAKE_CXX_STANDARD': self._ccpstd(),
            'llvm_build_llvm_dylib': False,
        }

        # XXX fine for v2 but service runs this test_package also for v1
        conan_v2 = hasattr(self, "dependencies")
        if conan_v2:
            # We could also add additional testing per project / runtime if needed
            cmake_definitions['llvm_build_llvm_dylib'] = self.dependencies[self.tested_reference_str].options.llvm_build_llvm_dylib

        cmake.configure(variables=cmake_definitions)
        cmake.build()

    def test(self):
        if can_run(self):
            bin_path = os.path.join(self.cpp.build.bindir, "test_package")
            self.run(bin_path, env="conanrun")
