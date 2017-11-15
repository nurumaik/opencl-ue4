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

void UOpenCLComponent::RunOpenCLKernel(const FString& Kernel, const FString& InputArgs /*= TEXT("")*/)
{
	if (IOpenCLPlugin::Get().IsAvailable())
	{
		IOpenCLPlugin::Get().RunKernelOnDevices(Kernel, InputArgs, [this](const FString& Result)
		{
			OnResult.Broadcast(Result);
		}, DeviceGroup);
	}
}

void UOpenCLComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UOpenCLComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
}
