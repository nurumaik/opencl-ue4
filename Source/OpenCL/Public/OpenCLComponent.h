#pragma once

#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "OpenCLComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOCLResultSignature, const FString& , Result);

UCLASS(ClassGroup = "Computing", meta = (BlueprintSpawnableComponent))
class OPENCL_API UOpenCLComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(BlueprintAssignable, Category = "OpenCL Events")
	FOCLResultSignature OnResult;

	UFUNCTION(BlueprintCallable, Category = "OpenCL Functions")
	bool HasValidHardware();

	UFUNCTION(BlueprintCallable, Category = "OpenCL Functions")
	void RunOpenCLKernel(const FString& Kernel, const FString& InputArgs = TEXT(""));
};