import os
import re
import sys
import subprocess
from setuptools import setup, find_packages
import shutil

try:
    subprocess.check_output(['cmake', '--version'])
    print("CMake found.")
except OSError:
    raise RuntimeError("CMake必须已安装并在PATH中可用")

PACKAGE_NAME = "pybadge"

PY_ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
PY_PKG_DIR = os.path.join(PY_ROOT_DIR, "pybadge")
PROJ_ROOT_DIR = os.path.dirname(PY_ROOT_DIR)
PY_BUILD_DIR = os.path.join(PY_ROOT_DIR, "_build")
PY_BUILD_CPP_DIR = os.path.join(PY_BUILD_DIR, "cpp")
PY_BUILD_LIB_DIR = os.path.join(PY_BUILD_DIR, "libs")

README_FILE = os.path.join(PY_ROOT_DIR, "README.md")

if os.path.exists(PY_BUILD_DIR):
    shutil.rmtree(PY_BUILD_DIR)
os.makedirs(PY_BUILD_DIR, exist_ok=True)
os.makedirs(PY_BUILD_CPP_DIR, exist_ok=True)
if os.path.exists(PY_BUILD_LIB_DIR):
    shutil.rmtree(PY_BUILD_LIB_DIR)
os.makedirs(PY_BUILD_LIB_DIR, exist_ok=True)


_NEED_REMOVE_FILES = []


def copy_file(src, dst):
    if os.path.exists(dst):
        os.remove(dst)
    print(f"Copying {src} to {dst}")
    shutil.copy(src, dst)
    _NEED_REMOVE_FILES.append(dst)


def clean_files():
    for file in _NEED_REMOVE_FILES:
        os.remove(file)


def build_cmake():

    CMAKE_BUILD_TYPE = "Release"

    subprocess.check_call([
        'cmake',
        f'-DCMAKE_BUILD_TYPE:STRING={CMAKE_BUILD_TYPE}',
        f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={PY_BUILD_LIB_DIR}',
        f'-DPYTHON_EXECUTABLE={sys.executable}',
        '-DBADGECPP_TESTS=OFF',
        '-DBADGECPP_PYTHON_BINDINGS=ON',
        f'-S{PROJ_ROOT_DIR}',
        f'-B{PY_BUILD_CPP_DIR}',
    ])
    subprocess.check_call([
        'cmake',
        '--build',
        PY_BUILD_CPP_DIR,
        '--config',
        CMAKE_BUILD_TYPE,
        '--target',
        'all',
        '-j',
        f'{os.cpu_count() or 1}',
    ])

    lib_files = [
        os.path.join(root, file)
        for root, _, files in os.walk(PY_BUILD_LIB_DIR)
        for file in files
        if re.match(r'^.*\.so(\.\d+)*$', file)
    ]
    print(f"Found {len(lib_files)} library files: ", [os.path.basename(f) for f in lib_files])

    for lib_file in lib_files:
        copy_file(lib_file, os.path.join(PY_PKG_DIR, os.path.basename(lib_file)))


def get_version():
    from pybadge import __version__
    return __version__


build_cmake()
version = get_version()
print(f"Package version: {version}")

setup(
    name=PACKAGE_NAME,
    version=version,
    author="MineYuanlu",
    description="Use Python code to generate SVG libraries for badges",
    long_description=open(README_FILE).read(),
    long_description_content_type="text/markdown",
    url="https://github.com/MineYuanlu/badgecpp",
    package_dir={PACKAGE_NAME: PACKAGE_NAME},
    packages=find_packages(),
    package_data={
        "pybadge": ["*.so*", "*.pyi"],
    },
    include_package_data=True,

)

clean_files()
