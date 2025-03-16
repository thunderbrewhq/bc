#ifndef BC_FILE_DEFINES_HPP
#define BC_FILE_DEFINES_HPP

#define BC_FILE_MAX_PATH 1024

// file attribute flags
#define BC_FILE_ATTRIBUTE_READONLY  0x01
#define BC_FILE_ATTRIBUTE_HIDDEN    0x02
#define BC_FILE_ATTRIBUTE_SYSTEM    0x04
#define BC_FILE_ATTRIBUTE_ARCHIVE   0x08
#define BC_FILE_ATTRIBUTE_TEMPORARY 0x10
#define BC_FILE_ATTRIBUTE_NORMAL    0x20
#define BC_FILE_ATTRIBUTE_DIRECTORY 0x40

// file error codes
#define BC_FILE_ERROR_GENERIC_FAILURE       0
#define BC_FILE_ERROR_ACCESS_DENIED         1
#define BC_FILE_ERROR_FILE_NOT_FOUND        2
#define BC_FILE_ERROR_BAD_FILE              3
#define BC_FILE_ERROR_BUSY                  4
#define BC_FILE_ERROR_READ                  5
#define BC_FILE_ERROR_WRITE                 6
#define BC_FILE_ERROR_END_OF_FILE           7
#define BC_FILE_ERROR_INVALID_ARGUMENT      8
#define BC_FILE_ERROR_UNIMPLEMENTED         9
#define BC_FILE_ERROR_NO_SPACE_ON_DEVICE   11

// file SetPos whence
#define BC_FILE_SEEK_START   0
#define BC_FILE_SEEK_CURRENT 1
#define BC_FILE_SEEK_END     2

// error handling utilities
#define BC_FILE_SET_ERROR(errorcode) ::Blizzard::File::SetLastError(static_cast<int32_t>(errorcode))
#define BC_FILE_SET_ERROR_MSG(errorcode, pfmt, ...) \
    ::Blizzard::File::SetLastError(errorcode); \
    ::Blizzard::File::AddToLastErrorStack(errorcode, ::Blizzard::String::QuickFormat<1024>(pfmt, ##__VA_ARGS__).ToString(), 1)

#endif
