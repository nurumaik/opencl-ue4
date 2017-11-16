#pragma once

class UTexture2D;

class FOCLUtility
{
public:
	//Conversion
	//To Bytes

	/** Correctly convert FStrings to OpenCL char* */
	static void FStringToCharByteArray(const FString& InString, TArray<uint8>& OutBytes);

	static void FloatToBytes(float InFloat, TArray<uint8>& OutBytes);
	static void Int32ToBytes(int32 InInt, TArray<uint8>& OutBytes);
	static void IntBigEndianToBytes(int32 InInt, TArray<uint8>& OutBytes);
	static void VectorToBytes(const FVector& InVector, TArray<uint8>& OutBytes);

	static void ArrayFloatToBytes(const TArray<float>& InFloatArray, TArray<uint8>& OutBytes);
	static void ArrayIntToBytes(const TArray<int32>& InIntArray, TArray<uint8>& OutBytes);
	static void ArrayVectorToBytes(const TArray<FVector>& InVectorArray, TArray<uint8>& OutBytes);
	static void Texture2DToBytes(const UTexture2D* InTexture, TArray<uint8>& OutBytes);

	//From Bytes
	static float FloatFromBytes(const TArray<uint8>& InBytes);
	static int32 Int32FromBytes(const TArray<uint8>& InBytes);
	static FVector VectorFromBytes(const TArray<uint8>& InBytes);

	static void ArrayFloatFromBytes(const TArray<uint8>& InBytes, TArray<float>& OutFloatArray);
	static void ArrayIntFromBytes(const TArray<uint8>& InBytes, TArray<int32>& OutIntArray);
	static void ArrayVectorFromBytes(const TArray<uint8>& InBytes, TArray<FVector>& OutVectorArray);
	static UTexture2D* Texture2DFromBytes(const TArray<uint8>& InBytes, const FVector2D& InSize);

	//todo: possible optimization: casting array data?
};