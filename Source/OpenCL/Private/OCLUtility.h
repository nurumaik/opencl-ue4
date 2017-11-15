#pragma once

class FOCLUtility
{
public:
	/** Correctly convert FStrings to OpenCL char* */
	static TArray<uint8> FStringToStdChar(const FString& InString);
};