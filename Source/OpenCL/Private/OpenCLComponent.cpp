#include "OpenCLComponent.h"
#include "IOpenCLPlugin.h"

UOpenCLComponent::UOpenCLComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;

	DeviceGroup = EnumerateDevices();
}

bool UOpenCLComponent::HasValidHardware()
{
	TArray<FOpenCLDeviceData> Devices = EnumerateDevices();
	IOpenCLPlugin::Get().EnumerateDevices(Devices);
	return Devices.Num() > 0;
}

TArray<FOpenCLDeviceData> UOpenCLComponent::EnumerateDevices()
{
	TArray<FOpenCLDeviceData> Devices;
	if (IOpenCLPlugin::Get().IsAvailable())
	{
		IOpenCLPlugin::Get().EnumerateDevices(Devices);
	}
	return Devices;
}

FString UOpenCLComponent::ReadKernelFromFile(const FString& ProjectRelativePath)
{
	FString AbsolutePath = FPaths::ProjectContentDir() + ProjectRelativePath;
	
	FString ResultString;
	FFileHelper::LoadFileToString(ResultString, *AbsolutePath);
	return ResultString;
}

void UOpenCLComponent::RunOpenCLKernel(const FString& Kernel, const FString& KernelName /*= TEXT("main")*/, const FString& InputArgs /*= TEXT("")*/)
{
	if (IOpenCLPlugin::Get().IsAvailable())
	{
		IOpenCLPlugin::Get().RunKernelOnDevices(Kernel, KernelName, InputArgs, [this](const FString& Result)
		{
			OnResult.Broadcast(Result);
		}, DeviceGroup);
	}
}

void UOpenCLComponent::InitializeComponent()
{
	Super::InitializeComponent();
	DeviceGroup = EnumerateDevices();
}

void UOpenCLComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}
