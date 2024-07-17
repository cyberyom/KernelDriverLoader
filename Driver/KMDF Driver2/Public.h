/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KMDFDriver2,
    0x41815605,0xb981,0x4cac,0xb0,0x78,0x81,0x11,0xa8,0xe1,0x28,0xb3);
// {41815605-b981-4cac-b078-8111a8e128b3}
