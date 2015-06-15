#include "SettingsHelper.h"

#include <sstream>

#include "guid.h"

LONG GetDWORDRegKey(HKEY hKey, const std::wstring &strValueName, DWORD &nValue, DWORD nDefaultValue)
{
	nValue = nDefaultValue;
	DWORD dwBufferSize(sizeof(DWORD));
	DWORD nResult(0);
	LONG nError = ::RegQueryValueExW(hKey,
		strValueName.c_str(),
		0,
		NULL,
		reinterpret_cast<LPBYTE>(&nResult),
		&dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		nValue = nResult;
	}
	return nError;
}


LONG GetBoolRegKey(HKEY hKey, const std::wstring &strValueName, bool &bValue, bool bDefaultValue)
{
	DWORD nDefValue((bDefaultValue) ? 1 : 0);
	DWORD nResult(nDefValue);
	LONG nError = GetDWORDRegKey(hKey, strValueName.c_str(), nResult, nDefValue);
	if (ERROR_SUCCESS == nError)
	{
		bValue = (nResult != 0) ? true : false;
	}
	return nError;
}


LONG GetStringRegKey(HKEY &hKey, const std::wstring &strValueName, std::wstring &strValue)
{
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	LONG nError;
	nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		strValue.assign(szBuffer);
	}
	return nError;
}

LONG SetStringRegKey(HKEY &hKey, const std::wstring &strValueName, std::wstring &strValue)
{
	return RegSetValueEx(hKey, strValueName.c_str(), 0, REG_SZ, (const BYTE*)strValue.c_str(), (strValue.size() + 1) * sizeof(wchar_t));
}

std::wstring GuidToString(const GUID &guid)
{
	std::wstring result;

	WCHAR* bstrGuid;
	StringFromCLSID(guid, &bstrGuid);

	result.assign(bstrGuid);
	CoTaskMemFree(bstrGuid);

	return result;
}

SettingsHelper::SettingsHelper()
{
	std::wstringstream library_location;
	library_location << L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers\\" << GuidToString(CLSID_CSample);

	LONG lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, library_location.str().c_str(), 0, KEY_ALL_ACCESS, &this->library);

	if (lRes != ERROR_SUCCESS)
	{
		throw std::runtime_error("There's no settings library");
	}
}


SettingsHelper::~SettingsHelper()
{
	RegCloseKey(this->library);
}

SettingsHelper::Settings SettingsHelper::load()
{
	Settings settings;

	LONG lRes = GetStringRegKey(this->library, L"id", settings.id);

	if (lRes != ERROR_SUCCESS)
	{
		throw std::runtime_error("Unable to load id from library");
	}

	return settings;
}

SettingsHelper::Settings SettingsHelper::init()
{
	Settings settings;

	GUID guid;
	CoCreateGuid(&guid);

	settings.id = GuidToString(guid);

	LONG result = SetStringRegKey(this->library, L"id", settings.id);
	
	if (result != ERROR_SUCCESS)
	{
		throw std::runtime_error("Unable to store id");
	}

	return settings;
}



