#include "Colors.h"
#include <QColor>
#include <winrt/Windows.UI.ViewManagement.h>
#pragma comment(lib, "windowsapp")


QColor & accentColor() {
	winrt::Windows::UI::ViewManagement::UISettings settings;
	auto color = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
	return QColor(color.R, color.G, color.B, color.A);
}

QColor & headerColor() {
	return QColor(0x000080);
}

QColor & dataColor() {
	return QColor(0x8e05ff);
}

QColor & statusCodeColor() {
	return QColor(0x009933);
}

QColor & responseColor() {
	return QColor(0x7e8735);
}