cmake_minimum_required(VERSION 3.20)

set(VCPKG_DEFAULT_TRIPLET "x64-windows")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 20)

project(ws_server)


find_package(fmt CONFIG REQUIRED)
find_package(boost_beast CONFIG REQUIRED)

add_executable(uws_server src/main.cpp)
target_link_libraries(uws_server PRIVATE fmt::fmt Boost::beast)

# target_include_directories(uws_server PRIVATE ${UWEBSOCKETS_INCLUDE_DIRS})
# target_link_libraries(uws_server PRIVATE 
#     fmt::fmt 
#     ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/zlib.lib
#     ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/uv.lib
#     ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib/uSockets.lib
# )
# target_compile_definitions(uws_server PRIVATE LIBUS_NO_SSL)
# target_link_libraries(uws_server INTERFACE "-fPIC")
 