#include "Colors.h"

// This code is too complicated to maintain. WRL or WinRT is not available on older versions of
// Windows and weird colour blending using undocumented APIs has to be used to accuratly get the 
// user's colour on Windows Vista/7. Uncomment this if Windows 8 is the lowest supported operating
// system.

/*
#include <QOperatingSystemVersion>
#include <dwmapi.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <wrl.h>
#include <windows.ui.viewmanagement.h>
#pragma comment(lib, "windowsapp")

QColor accentColor() {
	if (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10) {
		winrt::Windows::UI::ViewManagement::UISettings settings;
		auto color = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
		return QColor(color.R, color.G, color.B, color.A);
	} else if (QOperatingSystemVersion::current() > QOperatingSystemVersion::Windows7) {
		Microsoft::WRL::ComPtr<ABI::Windows::UI::ViewManagement::IUISettings3> settings;
		Windows::Foundation::ActivateInstance(Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_UI_ViewManagement_UISettings).Get(), &settings);
		ABI::Windows::UI::Color color;
		settings->GetColorValue(ABI::Windows::UI::ViewManagement::UIColorType::UIColorType_Accent, &color);
		return QColor(color.R, color.G, color.B, color.A);
	} else {
		DWORD color = 0; // 0xAARRGGBB
		DwmGetColorizationColor(&color, nullptr); // use private API to get exact colour
		return QColor((color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff); //  from 0x00BBGGRR
	}
}
*/

QColor accentColor() {
	return QColor(0, 120, 215);
}

