file(GLOB BC_SOURCES
    "*.cpp"
    "lock/*.cpp"
    "os/*.cpp"
    "file/*.cpp"
    "file/path/*.cpp"
    "time/*.cpp"
    "system/*.cpp"
    "system/file/*.cpp"
)

if(DEFINED WHOA_SYSTEM_WIN)
    file(GLOB BC_FILE_SYSTEM_SOURCES "system/file/win/*.cpp")
endif()

if(DEFINED WHOA_SYSTEM_LINUX OR WHOA_SYSTEM_MAC)
    file(GLOB BC_FILE_SYSTEM_SOURCES "system/file/posix/*.cpp")
endif()

list(APPEND BC_SOURCES ${BC_FILE_SYSTEM_SOURCES})

add_library(bc STATIC
    ${BC_SOURCES}
)

target_include_directories(bc
    PUBLIC
        ${PROJECT_SOURCE_DIR}
)

target_link_libraries(bc
    PUBLIC
        mem
)
