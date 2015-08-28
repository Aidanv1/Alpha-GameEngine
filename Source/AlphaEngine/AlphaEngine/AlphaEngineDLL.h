#pragma once
//DLL export for C++ applications. Should be used for classes.
#define DLLExport __declspec(dllexport)
//DLL export for C/C# applications. Should only be used for C functions.
#define DLLExportC extern "C" __declspec(dllexport)