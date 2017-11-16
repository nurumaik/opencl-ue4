#include "OCLUtility.h"
#include "CoreMinimal.h"
#include <string>

union FloatUnion
{
	float f;
	uint8 buff[sizeof(float)];
};

void FOCLUtility::FStringToCharByteArray(const FString& InString, TArray<uint8>& OutBytes)
{
	OutBytes.SetNum(InString.Len());

	for (int i = 0; i < OutBytes.Num(); i++)
	{
		OutBytes[i] = InString[i];
	}
}

void FOCLUtility::FloatToBytes(float InFloat, TArray<uint8>& OutBytes)
{
	//OutBytes
	FloatUnion ValueUnion;
	ValueUnion.f = InFloat;
	OutBytes[0] = ValueUnion.buff[0];
	OutBytes[1] = ValueUnion.buff[1];
	OutBytes[2] = ValueUnion.buff[2];
	OutBytes[3] = ValueUnion.buff[3];
}

void FOCLUtility::IntToBytes(int32 InInt, TArray<uint8>& OutBytes)
{
	OutBytes.SetNum(4);
	OutBytes[0] = (InInt >> 24);
	OutBytes[1] = (InInt >> 16);
	OutBytes[2] = (InInt >> 8);
	OutBytes[3] = InInt;
}

void FOCLUtility::IntBigEndianToBytes(int32 InInt, TArray<uint8>& OutBytes)
{
	OutBytes.SetNum(4);
	OutBytes[0] = InInt;
	OutBytes[1] = (InInt >> 8);
	OutBytes[2] = (InInt >> 16);
	OutBytes[3] = (InInt >> 24);
}

void FOCLUtility::VectorToBytes(const FVector& InVector, TArray<uint8>& OutBytes)
{
	TArray<uint8> FloatArray;
	FloatToBytes(InVector.X, FloatArray);
	OutBytes.Append(FloatArray);
	FloatToBytes(InVector.Y, FloatArray);
	OutBytes.Append(FloatArray);
	FloatToBytes(InVector.Z, FloatArray);
	OutBytes.Append(FloatArray);
}
