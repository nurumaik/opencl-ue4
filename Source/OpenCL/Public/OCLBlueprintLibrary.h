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

	//Basic Values
	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Float)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_FloatToBytes(float InFloat);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Int)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_IntToBytes(int32 InInt);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Vector)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_VectorToBytes(const FVector& InVector);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Float (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static float Conv_BytesToFloat(const TArray<uint8>& InBytes);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Int (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static int32 Conv_BytesToInt(const TArray<uint8>& InBytes);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Vector (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static FVector Conv_BytesToVector(const TArray<uint8>& InBytes);


	//Arrays

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Float Array)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_FloatArrayToBytes(const TArray<float>& InFloatArray);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Int Array)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_IntArrayToBytes(const TArray<int32>& InIntArray);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Vector Array)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_VectorArrayToBytes(const TArray<FVector>& InVectorArray);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Float Array (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<float> Conv_BytesToFloatArray(const TArray<uint8>& InBytes);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Int Array (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<int32> Conv_BytesToIntArray(const TArray<uint8>& InBytes);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Vector Array (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<FVector> Conv_BytesToArray(const TArray<uint8>& InBytes);

	//Textures
	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Texture2D (Bytes)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static UTexture2D* Conv_BytesToTexture2D(const TArray<uint8>& InBytes, const FVector2D& InTextureSize);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (Texture2D)", BlueprintAutocast), Category = "Utilities|OpenCL")
	static TArray<uint8> Conv_TextureToBytes(UTexture2D* InTexture);
};