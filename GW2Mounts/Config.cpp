#include "Config.h"
#include <Shlobj.h>
#include "Utility.h"
#include <tchar.h>
#include <sstream>

Config::Config()
{
	_SettingsKeybind.insert(VK_SHIFT);
	_SettingsKeybind.insert(VK_MENU);
	_SettingsKeybind.insert('M');
}


Config::~Config()
{
}

void LoadKeybindString(const char* keys, std::set<uint>& out)
{
	if (strnlen_s(keys, 256) > 0)
	{
		std::stringstream ss(keys);
		std::vector<std::string> result;

		while (ss.good())
		{
			std::string substr;
			std::getline(ss, substr, ',');
			int val = std::stoi(substr);
			out.insert((uint)val);
		}
	}
}

void Config::Load()
{
	// Create folders
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(0, exeFullPath, MAX_PATH);
	tstring exeFolder;
	SplitFilename(exeFullPath, &exeFolder, nullptr);
	_ConfigFolder = exeFolder + TEXT("\\addons\\mounts\\");
	_tcscpy_s(_ConfigLocation, (_ConfigFolder + ConfigName).c_str());
#if _UNICODE
	strcpy_s(_ImGuiConfigLocation, ws2s(_ConfigFolder + ImGuiConfigName).c_str());
#else
	strcpy_s(_ImGuiConfigLocation, (_ConfigFolder + ImGuiConfigName).c_str());
#endif
	SHCreateDirectoryEx(nullptr, _ConfigFolder.c_str(), nullptr);

	// Load INI settings
	_INI.SetUnicode();
	_INI.LoadFile(_ConfigLocation);
	_ResetCursorOnLockedKeybind = _INI.GetBoolValue("General", "reset_cursor_on_locked_keybind", true);
	_LockCameraWhenOverlayed = _INI.GetBoolValue("General", "lock_camera_when_overlayed", true);
	_OverlayDelayMilliseconds = _INI.GetLongValue("General", "overlay_delay_ms", 0);

	const char* keys = _INI.GetValue("Keybinds", "mount_wheel", nullptr);
	LoadKeybindString(keys, _MountOverlayKeybind);
	const char* keys_locked = _INI.GetValue("Keybinds", "mount_wheel_locked", nullptr);
	LoadKeybindString(keys_locked, _MountOverlayLockedKeybind);

	for (uint i = 0; i < MountTypeCount; i++)
	{
		const char* keys_mount = _INI.GetValue("Keybinds", ("mount_" + std::to_string(i)).c_str(), nullptr);
		LoadKeybindString(keys_mount, _MountKeybinds[i]);
	}
}

void Config::MountOverlayKeybind(const std::set<uint>& val)
{
	_MountOverlayKeybind = val;
	std::string setting_value = "";
	for (const auto& k : _MountOverlayKeybind)
		setting_value += std::to_string(k) + ", ";

	_INI.SetValue("Keybinds", "mount_wheel", (setting_value.size() > 0 ? setting_value.substr(0, setting_value.size() - 2) : setting_value).c_str());
	_INI.SaveFile(_ConfigLocation);
}

void Config::MountOverlayLockedKeybind(const std::set<uint>& val)
{
	_MountOverlayLockedKeybind = val;
	std::string setting_value = "";
	for (const auto& k : _MountOverlayLockedKeybind)
		setting_value += std::to_string(k) + ", ";

	_INI.SetValue("Keybinds", "mount_wheel_locked", (setting_value.size() > 0 ? setting_value.substr(0, setting_value.size() - 2) : setting_value).c_str());
	_INI.SaveFile(_ConfigLocation);
}

void Config::MountKeybind(uint i, const std::set<uint>& val)
{
	_MountKeybinds[i] = val;
	std::string setting_value = "";
	for (const auto& k : _MountKeybinds[i])
		setting_value += std::to_string(k) + ", ";

	_INI.SetValue("Keybinds", ("mount_" + std::to_string(i)).c_str(), (setting_value.size() > 0 ? setting_value.substr(0, setting_value.size() - 2) : setting_value).c_str());
	_INI.SaveFile(_ConfigLocation);
}

void Config::ResetCursorOnLockedKeybindSave()
{
	_INI.SetBoolValue("General", "reset_cursor_on_locked_keybind", _ResetCursorOnLockedKeybind);
	_INI.SaveFile(_ConfigLocation);
}

void Config::LockCameraWhenOverlayedSave()
{
	_INI.SetBoolValue("General", "lock_camera_when_overlayed", _LockCameraWhenOverlayed);
	_INI.SaveFile(_ConfigLocation);
}

void Config::OverlayDelayMillisecondsSave()
{
	_INI.SetLongValue("General", "overlay_delay_ms", _OverlayDelayMilliseconds);
	_INI.SaveFile(_ConfigLocation);
}
