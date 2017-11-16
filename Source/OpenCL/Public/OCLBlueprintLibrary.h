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
	static TArray<uint8> Conv_FloatArrayToBytes(const TArray<float>& InFloatArray);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Int Array)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_IntArrayToBytes(const TArray<int32>& InIntArray);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Vector Array)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_VectorArrayToBytes(const TArray<FVector>& InVectorArray);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Float Array (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<float> Conv_BytesFloatArray(const TArray<uint8>& InBytes);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Int32 Array (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<int32> Conv_BytesIntArray(const TArray<uint8>& InBytes);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Vector Array (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<FVector> Conv_BytesToArray(const TArray<uint8>& InBytes);
};