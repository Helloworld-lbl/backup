### This file is automatically generated by Qt Design Studio.
### Do not change

add_subdirectory(Backup)
add_subdirectory(BackupContent)
add_subdirectory(App)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Backupplugin
    BackupContentplugin)