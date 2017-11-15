#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "OCLData.h"
#include "OpenCLComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOCLResultSignature, const FString& , Result);

UCLASS(ClassGroup = "Computing", meta = (BlueprintSpawnableComponent))
class OPENCL_API UOpenCLComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(BlueprintAssignable, Category = "OpenCL Events")
	FOCLResultSignature OnResult;

	/** Devices specified here will be the ones that will be used when running RunOpenCLKernel*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OpenCL Events")
	TArray<FOpenCLDeviceData> DeviceGroup;

	/** Did we enumerate at least one valid opencl device?*/
	UFUNCTION(BlueprintCallable, Category = "OpenCL Functions")
	bool HasValidHardware();

	/** Get a list of all the OpenCL device available and the compute capability */
	UFUNCTION(BlueprintCallable, Category = "OpenCL Functions")
	TArray<FOpenCLDeviceData> EnumerateDevices();

	UFUNCTION(BlueprintCallable, Category = "OpenCL Functions")
	FString ReadKernelFromFile(const FString& ProjectRelativePath);

	/** Run specified kernel on the current device group with passed in arguments */
	UFUNCTION(BlueprintCallable, Category = "OpenCL Functions")
	void RunOpenCLKernel(const FString& Kernel, const FString& InputArgs = TEXT(""));


protected:

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
};