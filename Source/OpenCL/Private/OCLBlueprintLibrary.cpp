#pragma once

#include "OCLBlueprintLibrary.h"

UOCLBlueprintLibrary::UOCLBlueprintLibrary(const FObjectInitializer &init) : UBlueprintFunctionLibrary(init)
{

}

TArray<uint8> UOCLBlueprintLibrary::Conv_FloatToJsonValue(const TArray<float>& InFloatArray)
{
	TArray<uint8> Result;
	//Todo: convert
	return Result;
}
