#pragma once

class FOCLUtility
{
public:
	/** Correctly convert FStrings to OpenCL char* */
	static void FStringToCharByteArray(const FString& InString, TArray<uint8>& OutBytes);

	//To Bytes
	static void FloatToBytes(float InFloat, TArray<uint8>& OutBytes);
	static void IntToBytes(int32 InInt, TArray<uint8>& OutBytes);
	static void IntBigEndianToBytes(int32 InInt, TArray<uint8>& OutBytes);
	static void VectorToBytes(const FVector& InVector, TArray<uint8>& OutBytes);
};