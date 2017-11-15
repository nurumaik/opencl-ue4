#include "OCLUtility.h"
#include <string>

char* FOCLUtility::FStringToStdChar(const FString& InString)
{
	return (char*)std::string(TCHAR_TO_UTF8(*InString)).c_str();
}
