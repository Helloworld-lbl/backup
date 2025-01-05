# 添加包含目录
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    include
)

# 定义 App 目录下的源文件和头文件
file(GLOB APP_HEADERS "include/*.h")
file(GLOB APP_AES_HEADERS "include/aes/*.h")
file(GLOB APP_DES_HEADERS "include/des/*.h")
file(GLOB APP_Huffman_HEADERS "include/huffman/*.h")
file(GLOB APP_LZ77_HEADERS "include/lz77/*.h")

file(GLOB APP_SOURCES "src/*.cpp")

# 将获取到的文件添加到变量中
set(APP_ALL_HEADERS
    ${APP_HEADERS}
    ${APP_AES_HEADERS}
    ${APP_DES_HEADERS}
    ${APP_Huffman_HEADERS}
    ${APP_LZ77_HEADERS}
)

set(APP_ALL_SOURCES
    ${APP_SOURCES}
)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
    ${APP_ALL_HEADERS}
    ${APP_ALL_SOURCES}
)

# 编译选项
add_compile_options(-w -O2 -std=c++17 -lcrypto)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)
