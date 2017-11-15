// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ModuleManager.h"
#include "IOpenCLPlugin.h"
#include "CL/opencl.h"

class OpenCLPlugin : public IOpenCLPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void EnumerateDevices(TArray<FOpenCLDeviceData>& OutDevices, bool bForceRefresh = false) override;
	virtual void RunKernelOnDevices(const FString& KernelString, const FString& Args, TFunction<void(const FString&)> ResultCallback, const TArray<FOpenCLDeviceData>& OutDevices) override;

private:
	TArray<FOpenCLDeviceData> Devices;

	TArray<cl_device_id*> DeviceIdsMemoryList;
	bool bHasEnumeratedOnce;

	void FreeDeviceMemory();
};

IMPLEMENT_MODULE( OpenCLPlugin, OpenCL )
DEFINE_LOG_CATEGORY(LogOpenCL);

void OpenCLPlugin::StartupModule()
{
	//Enumerate once on startup
	bHasEnumeratedOnce = false;
	EnumerateDevices(Devices);

	//Log all devices
	UE_LOG(LogOpenCL, Log, TEXT("OpenCL Info:"));
	for (auto Device : Devices)
	{
		UE_LOG(LogOpenCL, Log, TEXT("%s"), *Device.ToPrintString());
	}
}

void OpenCLPlugin::ShutdownModule() 
{
	FreeDeviceMemory();
}


void OpenCLPlugin::EnumerateDevices(TArray<FOpenCLDeviceData>& OutDevices, bool bForceRefresh /*= false*/)
{
	if (bHasEnumeratedOnce && !bForceRefresh)
	{
		OutDevices = Devices;
		return;
	}

	FreeDeviceMemory();

	cl_uint i, j;
	char* Value;
	size_t ValueSize;
	cl_uint NumPlatforms;
	cl_platform_id* Platforms;
	cl_uint NumDevices;
	cl_device_id* Devices;
	cl_uint MaxComputeUnits;

	clGetPlatformIDs(0, NULL, &NumPlatforms);
	Platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * NumPlatforms);
	clGetPlatformIDs(NumPlatforms, Platforms, NULL);

	//For each platform
	for (i = 0; i < NumPlatforms; i++) {

		clGetPlatformInfo(Platforms[i], CL_PLATFORM_NAME, 0, NULL, &ValueSize);
		Value = (char*)malloc(ValueSize);
		clGetPlatformInfo(Platforms[i], CL_PLATFORM_NAME, ValueSize, Value, NULL);
		FString EnumeratedPlatform = FString(ANSI_TO_TCHAR(Value));
		free(Value);

		// get all Devices
		clGetDeviceIDs(Platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &NumDevices);
		Devices = (cl_device_id*)malloc(sizeof(cl_device_id) * NumDevices);
		clGetDeviceIDs(Platforms[i], CL_DEVICE_TYPE_ALL, NumDevices, Devices, NULL);

		DeviceIdsMemoryList.Add(Devices);

		//For each device
		for (j = 0; j < NumDevices; j++) {
			FOpenCLDeviceData Device;
			Device.Platform = EnumeratedPlatform;
			Device.DeviceId = FString::Printf(TEXT("%d"), ::PointerHash(Devices[j]));
			Device.RawDeviceId = Devices[j];

			clGetDeviceInfo(Devices[j], CL_DEVICE_NAME, 0, NULL, &ValueSize);
			Value = (char*)malloc(ValueSize);
			clGetDeviceInfo(Devices[j], CL_DEVICE_NAME, ValueSize, Value, NULL);
			Device.DeviceName = ANSI_TO_TCHAR(Value);
			free(Value);

			clGetDeviceInfo(Devices[j], CL_DEVICE_VERSION, 0, NULL, &ValueSize);
			Value = (char*)malloc(ValueSize);
			clGetDeviceInfo(Devices[j], CL_DEVICE_VERSION, ValueSize, Value, NULL);
			Device.HardwareVersion = ANSI_TO_TCHAR(Value);
			free(Value);

			clGetDeviceInfo(Devices[j], CL_DRIVER_VERSION, 0, NULL, &ValueSize);
			Value = (char*)malloc(ValueSize);
			clGetDeviceInfo(Devices[j], CL_DRIVER_VERSION, ValueSize, Value, NULL);
			Device.SoftwareVersion = ANSI_TO_TCHAR(Value);
			free(Value);

			clGetDeviceInfo(Devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &ValueSize);
			Value = (char*)malloc(ValueSize);
			clGetDeviceInfo(Devices[j], CL_DEVICE_OPENCL_C_VERSION, ValueSize, Value, NULL);
			Device.OpenCLVersion = ANSI_TO_TCHAR(Value);
			free(Value);

			clGetDeviceInfo(Devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(MaxComputeUnits), &MaxComputeUnits, NULL);
			Device.ParallelComputeUnits = MaxComputeUnits;
			OutDevices.Add(Device);
		}
		//free(Devices);
	}
	free(Platforms);

	bHasEnumeratedOnce = true;
}

void OpenCLPlugin::RunKernelOnDevices(const FString& KernelString, const FString& Args, TFunction<void(const FString&)> ResultCallback, const TArray<FOpenCLDeviceData>& DeviceGroup)
{
	//grab top level device for now

	/*for (auto& Device : DeviceGroup)
	{

	}*/
}

void OpenCLPlugin::FreeDeviceMemory()
{
	Devices.Empty();

	//Free each of our list of lists
	for (auto DeviceList : DeviceIdsMemoryList)
	{
		free(DeviceList);
	}
	DeviceIdsMemoryList.Empty();
}
