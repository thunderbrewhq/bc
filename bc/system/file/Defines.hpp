#ifndef BC_SYSTEM_FILE_DEFINES_HPP
#define BC_SYSTEM_FILE_DEFINES_HPP

// how much data to buffer when copying with File::Copy
#define BC_SYSTEM_FILE_COPYBUFFER_SIZE 0xA00000LL

// this bit field controls which file information is set by SetFileInfo
#define BC_SYSTEM_FILE_INFO_UNK_0       0x01
#define BC_SYSTEM_FILE_INFO_UNK_1       0x02
#define BC_SYSTEM_FILE_INFO_ATTRIBUTES  0x04
#define BC_SYSTEM_FILE_INFO_UNK_3       0x08
#define BC_SYSTEM_FILE_INFO_TIMES       0x10

#endif
