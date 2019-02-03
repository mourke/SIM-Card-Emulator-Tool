#include "AccentColor.h"
#include <QColor>
#include <winrt/Windows.UI.ViewManagement.h>
#pragma comment(lib, "windowsapp")


QColor accentColor() {
	winrt::Windows::UI::ViewManagement::UISettings settings;
	auto color = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
	return QColor(color.R, color.G, color.B, color.A);
}