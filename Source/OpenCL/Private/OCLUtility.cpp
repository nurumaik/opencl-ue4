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
	OutBytes.SetNum(4);
	FloatUnion ValueUnion;
	ValueUnion.f = InFloat;
	OutBytes[0] = ValueUnion.buff[0];
	OutBytes[1] = ValueUnion.buff[1];
	OutBytes[2] = ValueUnion.buff[2];
	OutBytes[3] = ValueUnion.buff[3];
}

void FOCLUtility::Int32ToBytes(int32 InInt, TArray<uint8>& OutBytes)
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
	OutBytes.Empty(12);
	TArray<uint8> FloatArray;
	FloatToBytes(InVector.X, FloatArray);
	OutBytes.Append(FloatArray);
	FloatToBytes(InVector.Y, FloatArray);
	OutBytes.Append(FloatArray);
	FloatToBytes(InVector.Z, FloatArray);
	OutBytes.Append(FloatArray);
}

void FOCLUtility::ArrayFloatToBytes(const TArray<float>& InFloatArray, TArray<uint8>& OutBytes)
{
	OutBytes.Reserve(InFloatArray.Num() * sizeof(float));

	for (float Value : InFloatArray)
	{
		TArray<uint8> FloatBytes;
		FloatToBytes(Value, FloatBytes);
		OutBytes.Append(FloatBytes);
	}
}

void FOCLUtility::ArrayIntToBytes(const TArray<int32>& InIntArray, TArray<uint8>& OutBytes)
{
	OutBytes.Reserve(InIntArray.Num() * sizeof(int32));

	for (int32 Value : InIntArray)
	{
		TArray<uint8> IntBytes;
		Int32ToBytes(Value, IntBytes);
		OutBytes.Append(IntBytes);
	}
}

void FOCLUtility::ArrayVectorToBytes(const TArray<FVector>& InVectorArray, TArray<uint8>& OutBytes)
{
	OutBytes.Reserve(InVectorArray.Num() * 12);	//3 floats of 4 bytes each

	for (const FVector& Value : InVectorArray)
	{
		TArray<uint8> IntBytes;
		VectorToBytes(Value, IntBytes);
		OutBytes.Append(IntBytes);
	}
}

float FOCLUtility::FloatFromBytes(const TArray<uint8>& InBytes)
{
	FloatUnion ValueUnion;
	ValueUnion.buff[0] = InBytes[0];
	ValueUnion.buff[1] = InBytes[1];
	ValueUnion.buff[2] = InBytes[2];
	ValueUnion.buff[3] = InBytes[3];
	return ValueUnion.f;
}

int32 FOCLUtility::Int32FromBytes(const TArray<uint8>& InBytes)
{
	return int32((uint8)(InBytes[0]) << 24 |
		(uint8)(InBytes[1]) << 16 |
		(uint8)(InBytes[2]) << 8 |
		(uint8)(InBytes[3]));
}

FVector FOCLUtility::VectorFromBytes(const TArray<uint8>& InBytes)
{
	//Todo: this can likely be optimized
	FVector Value;
	TArray<uint8> FloatBytes;
	FloatBytes.Append(&InBytes[0], 4);
	Value.X = FloatFromBytes(FloatBytes);
	FloatBytes.Empty(4);
	FloatBytes.Append(&InBytes[4], 4);
	Value.Y = FloatFromBytes(FloatBytes);
	FloatBytes.Empty(4);
	FloatBytes.Append(&InBytes[8], 4);
	Value.Z = FloatFromBytes(FloatBytes);

	return Value;
}

void FOCLUtility::ArrayFloatFromBytes(const TArray<uint8>& InBytes, TArray<float>& OutFloatArray)
{
	int32 ValueSize = sizeof(float);
	OutFloatArray.Reserve(InBytes.Num() / ValueSize);

	TArray<uint8> ValueBytes;
	for (int i=0; i<InBytes.Num(); i += ValueSize)	//we need to hop ValueSize bytes at a time
	{
		ValueBytes.Empty(ValueSize);
		ValueBytes.Append(InBytes.GetData(), ValueSize);
		OutFloatArray.Add(FloatFromBytes(ValueBytes));
	}
}

void FOCLUtility::ArrayIntFromBytes(const TArray<uint8>& InBytes, TArray<int32>& OutIntArray)
{
	int32 ValueSize = sizeof(int32);
	OutIntArray.Reserve(InBytes.Num() / ValueSize);

	TArray<uint8> ValueBytes;
	for (int i = 0; i < InBytes.Num(); i += ValueSize)	//we need to hop ValueSize bytes at a time
	{
		ValueBytes.Empty(ValueSize);
		ValueBytes.Append(InBytes.GetData(), ValueSize);
		OutIntArray.Add(Int32FromBytes(ValueBytes));
	}
}

void FOCLUtility::ArrayVectorFromBytes(const TArray<uint8>& InBytes, TArray<FVector>& OutVectorArray)
{
	int32 ValueSize = 12;	//todo: confirm sizeof(FVector) == 12
	OutVectorArray.Reserve(InBytes.Num() / ValueSize);

	TArray<uint8> ValueBytes;
	for (int i = 0; i < InBytes.Num(); i += ValueSize)	//we need to hop ValueSize bytes at a time
	{
		ValueBytes.Empty(ValueSize);
		ValueBytes.Append(InBytes.GetData(), ValueSize);
		OutVectorArray.Add(VectorFromBytes(ValueBytes));
	}
}
