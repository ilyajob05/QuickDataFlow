import os
import sys
import distutils.ccompiler
import subprocess
import distutils.command.install
from setuptools import setup
import setuptools


class CustomInstall(setuptools.Command):
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        print('file', __file__)
        print('dirname', os.path.dirname(os.path.abspath(__file__)))
        print('listdir', os.listdir(os.path.dirname(os.path.abspath(__file__))))
        lib_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'QuickDataFlow/build')
        if os.path.exists(lib_path):
            print(f'\033[91m path:{lib_path} is exist \033[0m')
        else:
            os.mkdir(lib_path)
            print('cmake config')
            cmake_config = 'cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=RELEASE -S ../.. -B .'
            ret = subprocess.run(cmake_config, stderr=subprocess.STDOUT, shell=True, cwd=lib_path)

            print('cmake build')
            cmake_build = 'cmake --build . --clean-first --target QuickDataFlow'
            ret = subprocess.run(cmake_build, stderr=subprocess.STDOUT, shell=True, cwd=lib_path)


class install(distutils.command.install.install):
    _sub_command = ('CustomInstall', None,)
    _sub_commands = distutils.command.install.install.sub_commands
    sub_commands = [_sub_command] + _sub_commands


if __name__ == '__main__':
    setup(
        cmdclass={'CustomInstall': CustomInstall,
                  'install': install},
        # package_data={"QuickDataFlow": ["_precompiled_extension.pyd"]},
        has_ext_modules=lambda: True
)

