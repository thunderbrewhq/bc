file(GLOB BC_SOURCES
    "*.cpp"
    "string/*.cpp"
    "file/*.cpp"
    "lock/*.cpp"
    "memory/*.cpp"
    "os/*.cpp"
    "os/file/*.cpp"
    "system/*.cpp"
    "system/file/*.cpp"
    "system/file/posix/*.cpp"
    "system/file/win/*.cpp"
    "time/*.cpp"
)

add_library(bc STATIC
    ${BC_SOURCES}
)

target_include_directories(bc
    PUBLIC
        ${PROJECT_SOURCE_DIR}
)
