cmake_minimum_required(VERSION 3.22)

# Must be called before project call
set(VCPKG_DEFAULT_TRIPLET "x64-windows")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(hrs)

find_package(fmt CONFIG REQUIRED)
find_package(boost_beast CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(server)
add_subdirectory(client)
