#pragma once

#include <iostream>

#include <Windows.h>

class SettingsHelper
{
public:
	struct Settings
	{
		std::wstring id;
	};

private:
	HKEY library;

public:
	SettingsHelper();
	~SettingsHelper();

	Settings load();
	Settings init();
};

