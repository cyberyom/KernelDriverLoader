#include "NVDrv.h"
#include <fstream>
#include <windows.h>
#include <iostream>

bool LoadUnsignedDriver(NVDrv* NV, const char* driverPath)
{
    // Read driver file into memory
    DWORD driverSize;
    std::ifstream file(driverPath, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        driverSize = static_cast<DWORD>(file.tellg());
        file.seekg(0, std::ios::beg);
    }
    else {
        std::cerr << "Failed to open the driver file: " << driverPath << std::endl;
        return false;
    }

    // Allocate memory in the kernel space
    void* kernelMemory = NV->AllocateKernelMemory(driverSize);
    if (!kernelMemory) {
        std::cerr << "Failed to allocate kernel memory" << std::endl;
        return false;
    }

    // Read the driver into the allocated memory
    file.read(reinterpret_cast<char*>(kernelMemory), driverSize);
    file.close();

    // Ensure memory is executable and ready to run
    if (!NV->MakeMemoryExecutable(kernelMemory, driverSize)) {
        std::cerr << "Failed to make memory executable" << std::endl;
        return false;
    }

    std::cout << "Driver loaded into allocated memory" << std::endl;
    return true;
}

int main(int argc, char* argv[])
{
    const char* driverPath = nullptr;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-d" && i + 1 < argc) {
            driverPath = argv[++i];
        }
    }

    if (driverPath == nullptr) {
        std::cerr << "Driver path not specified. Usage: " << argv[0] << " -d <driver_path>" << std::endl;
        return 1;
    }

    NVDrv* NV = new NVDrv();

    // Get the base address of the process (heartbeat)
    uintptr_t ProcessBase = NV->GetProcessBase(L"explorer.exe");
    std::cout << "ProcessBase: " << (void*)ProcessBase << std::endl;

    if (LoadUnsignedDriver(NV, driverPath)) {
        std::cout << "Driver loaded successfully" << std::endl;
    }
    else {
        std::cerr << "Failed to load driver" << std::endl;
    }

    Sleep(-1);
    return 0;
}
