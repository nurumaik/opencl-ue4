#pragma once

class FOCLUtility
{
public:
	/** Correctly convert FStrings to OpenCL char* */
	static char* FStringToStdChar(const FString& InString);
};