#pragma once

#include "OCLBlueprintLibrary.h"
#include "OCLUtility.h"

UOCLBlueprintLibrary::UOCLBlueprintLibrary(const FObjectInitializer &init) : UBlueprintFunctionLibrary(init)
{

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

TArray<float> UOCLBlueprintLibrary::Conv_BytesFloatArray(const TArray<uint8>& InBytes)
{
	TArray<float> Result;
	FOCLUtility::ArrayFloatFromBytes(InBytes, Result);
	return Result;
}

TArray<int32> UOCLBlueprintLibrary::Conv_BytesIntArray(const TArray<uint8>& InBytes)
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
