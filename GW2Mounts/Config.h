#pragma once
#include "main.h"
#include "simpleini\SimpleIni.h"
#include <set>

class Config
{
public:
	Config();
	~Config();

	void Load();

	const auto& MountOverlayKeybind() { return _MountOverlayKeybind; }
	void MountOverlayKeybind(const std::set<uint>& val);

	const auto& MountOverlayLockedKeybind() { return _MountOverlayLockedKeybind; }
	void MountOverlayLockedKeybind(const std::set<uint>& val);

	const auto& MountKeybind(uint i) { return _MountKeybinds[i]; }
	void MountKeybind(uint i, const std::set<uint>& val);

	const auto& SettingsKeybind() { return _SettingsKeybind; }

	auto& ShowGriffon() { return _ShowGriffon; }
	void ShowGriffonSave();

	auto& ResetCursorOnLockedKeybind() { return _ResetCursorOnLockedKeybind; }
	void ResetCursorOnLockedKeybindSave();

	auto& LockCameraWhenOverlayed() { return _LockCameraWhenOverlayed; }
	void LockCameraWhenOverlayedSave();

	const auto& ImGuiConfigLocation() { return _ImGuiConfigLocation; }

protected:
	const TCHAR* ConfigName = TEXT("config.ini");
	const TCHAR* ImGuiConfigName = TEXT("imgui_config.ini");

	// Config file settings
	CSimpleIniA _INI;
	tstring _ConfigFolder;
	TCHAR _ConfigLocation[MAX_PATH];
	char _ImGuiConfigLocation[MAX_PATH];

	// Config data
	std::set<uint> _MountOverlayKeybind;
	std::set<uint> _MountOverlayLockedKeybind;
	std::set<uint> _MountKeybinds[5];
	std::set<uint> _SettingsKeybind;
	bool _ShowGriffon = false;
	bool _ResetCursorOnLockedKeybind = true;
	bool _LockCameraWhenOverlayed = true;
};

