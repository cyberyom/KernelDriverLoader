#include <ntddk.h>


// Sample drivr used for testing.

VOID UnloadDriver(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject); 
    DbgPrint("SampleDriver: Unload routine called\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath); 

    DbgPrint("SampleDriver: DriverEntry called\n");

    DriverObject->DriverUnload = UnloadDriver;

    return STATUS_SUCCESS;
}
