#pragma once

#include "OCLBlueprintLibrary.h"
#include "OCLUtility.h"

UOCLBlueprintLibrary::UOCLBlueprintLibrary(const FObjectInitializer &init) : UBlueprintFunctionLibrary(init)
{

}

TArray<uint8> UOCLBlueprintLibrary::Conv_FloatToBytes(float InFloat)
{
	TArray<uint8> Result;
	FOCLUtility::FloatToBytes(InFloat, Result);
	return Result;
}

TArray<uint8> UOCLBlueprintLibrary::Conv_IntToBytes(int32 InInt)
{
	TArray<uint8> Result;
	FOCLUtility::Int32ToBytes(InInt, Result);
	return Result;
}

TArray<uint8> UOCLBlueprintLibrary::Conv_VectorToBytes(const FVector& InVector)
{
	TArray<uint8> Result;
	FOCLUtility::VectorToBytes(InVector, Result);
	return Result;
}

float UOCLBlueprintLibrary::Conv_BytesToFloat(const TArray<uint8>& InBytes)
{
	return FOCLUtility::FloatFromBytes(InBytes);
}

int32 UOCLBlueprintLibrary::Conv_BytesToInt(const TArray<uint8>& InBytes)
{
	return FOCLUtility::Int32FromBytes(InBytes);
}

FVector UOCLBlueprintLibrary::Conv_BytesToVector(const TArray<uint8>& InBytes)
{
	return FOCLUtility::VectorFromBytes(InBytes);
}

TArray<uint8> UOCLBlueprintLibrary::Conv_FloatArrayToBytes(const TArray<float>& InFloatArray)
{
	TArray<uint8> Result;
	FOCLUtility::ArrayFloatToBytes(InFloatArray, Result);
	return Result;
}

TArray<uint8> UOCLBlueprintLibrary::Conv_IntArrayToBytes(const TArray<int32>& InIntArray)
{
	TArray<uint8> Result;
	FOCLUtility::ArrayIntToBytes(InIntArray, Result);
	return Result;
}

TArray<uint8> UOCLBlueprintLibrary::Conv_VectorArrayToBytes(const TArray<FVector>& InVectorArray)
{
	TArray<uint8> Result;
	FOCLUtility::ArrayVectorToBytes(InVectorArray, Result);
	return Result;
}

TArray<float> UOCLBlueprintLibrary::Conv_BytesToFloatArray(const TArray<uint8>& InBytes)
{
	TArray<float> Result;
	FOCLUtility::ArrayFloatFromBytes(InBytes, Result);
	return Result;
}

TArray<int32> UOCLBlueprintLibrary::Conv_BytesToIntArray(const TArray<uint8>& InBytes)
{
	TArray<int32> Result;
	FOCLUtility::ArrayIntFromBytes(InBytes, Result);
	return Result;
}

TArray<FVector> UOCLBlueprintLibrary::Conv_BytesToArray(const TArray<uint8>& InBytes)
{
	TArray<FVector> Result;
	FOCLUtility::ArrayVectorFromBytes(InBytes, Result);
	return Result;
}

UTexture2D* UOCLBlueprintLibrary::Conv_BytesToTexture2D(const TArray<uint8>& InBytes, const FVector2D& InTextureSize)
{
	UTexture2D* Result = nullptr;
	FOCLUtility::Texture2DFromBytes(InBytes, InTextureSize, Result);
	return Result;
}

TArray<uint8> UOCLBlueprintLibrary::Conv_TextureToBytes(UTexture2D* InTexture)
{
	TArray<uint8> Result;
	FOCLUtility::Texture2DToBytes(InTexture, Result);
	return Result;
}