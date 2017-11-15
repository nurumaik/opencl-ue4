#include "OpenCLComponent.h"

UOpenCLComponent::UOpenCLComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
}

bool UOpenCLComponent::HasValidHardware()
{
	return false;
}

void UOpenCLComponent::RunOpenCLKernel(const FString& Kernel, const FString& InputArgs /*= TEXT("")*/)
{

}
