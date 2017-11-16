#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "OCLBlueprintLibrary.generated.h"

/**
* Useful global functions for data format conversion
*/
UCLASS()
class UOCLBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Float Array)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_FloatToJsonValue(const TArray<float>& InFloatArray);


private:

	//Utility internal functions



	//From Bytes
};