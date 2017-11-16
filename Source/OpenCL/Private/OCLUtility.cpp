#include "OCLUtility.h"
#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "IOpenCLPlugin.h"
#include "RHI.h"
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
	int32 ByteCount = InFloatArray.Num() * sizeof(float);
	OutBytes.Empty(ByteCount);
	OutBytes.Append((uint8*)InFloatArray.GetData(), ByteCount);
}

void FOCLUtility::ArrayIntToBytes(const TArray<int32>& InIntArray, TArray<uint8>& OutBytes)
{
	//int32 have endian problems so must use this safer function
	OutBytes.Reserve(InIntArray.Num() * sizeof(int32));

	for (int32 Value : InIntArray)
	{
		TArray<uint8> ValueBytes;
		Int32ToBytes(Value, ValueBytes);
		OutBytes.Append(ValueBytes);
	}
}

void FOCLUtility::ArrayVectorToBytes(const TArray<FVector>& InVectorArray, TArray<uint8>& OutBytes)
{
	int32 ByteCount = InVectorArray.Num() * sizeof(FVector);
	OutBytes.Empty(ByteCount);
	OutBytes.Append((uint8*)InVectorArray.GetData(), ByteCount);
}

void FOCLUtility::Texture2DToBytes(const UTexture2D* InTexture, TArray<uint8>& OutBytes)
{
	//RGBA texture, todo: generalize
	int32 ByteCount = InTexture->GetSizeX()* InTexture->GetSizeY() * 4;
	OutBytes.Reserve(ByteCount);	//RGBA, 4 bytes per pixel

	//Lock the texture so it can be read
	uint8* MipData = static_cast<uint8*>(InTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));

	OutBytes.Append(MipData, ByteCount);

	InTexture->PlatformData->Mips[0].BulkData.Unlock();
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
		ValueBytes.Append(&InBytes[i], ValueSize);
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
		ValueBytes.Append(&InBytes[i], ValueSize);
		OutIntArray.Add(Int32FromBytes(ValueBytes));
	}
}

void FOCLUtility::ArrayVectorFromBytes(const TArray<uint8>& InBytes, TArray<FVector>& OutVectorArray)
{
	int32 ValueSize = sizeof(FVector);
	OutVectorArray.Reserve(InBytes.Num() / ValueSize);

	TArray<uint8> ValueBytes;
	for (int i = 0; i < InBytes.Num(); i += ValueSize)	//we need to hop ValueSize bytes at a time
	{
		ValueBytes.Empty(ValueSize);
		ValueBytes.Append(&InBytes[i], ValueSize);
		OutVectorArray.Add(VectorFromBytes(ValueBytes));
	}
}

UTexture2D* FOCLUtility::Texture2DFromBytes(const TArray<uint8>& InBytes, const FVector2D& InSize)
{
	UTexture2D* OutTexture;
	FVector2D Size;
	int32 Pixels = InBytes.Num() / 16;	//4 bytes per pixel

	//Create square image and lock for writing
	if (InSize == FVector2D(0, 0))
	{
		int32 Length = FMath::Pow(Pixels, 0.5);
		if (Length * Length != Pixels)
		{
			UE_LOG(LogOpenCL, Warning, TEXT("Texture2DFromBytes::Invalid bytes without specified size, needs to be square."));
		}
		Size = FVector2D(Length, Length);
	}
	else
	{
		Size = InSize;
	}

	OutTexture = UTexture2D::CreateTransient(Size.X, Size.Y, PF_R8G8B8A8);
	uint8* MipData = static_cast<uint8*>(OutTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	//Copy Data
	for (int i = 0; i < InBytes.Num(); i++)
	{
		MipData[i] = InBytes[i];
	}

	//Unlock and Return data
	OutTexture->PlatformData->Mips[0].BulkData.Unlock();
	OutTexture->UpdateResource();
	return OutTexture;
}
