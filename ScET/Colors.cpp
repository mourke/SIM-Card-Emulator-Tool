#include "Colors.h"
#include <QColor>
#include <winrt/Windows.UI.ViewManagement.h>
#pragma comment(lib, "windowsapp")


QColor accentColor() {
	winrt::Windows::UI::ViewManagement::UISettings settings;
	auto color = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
	return QColor(color.R, color.G, color.B, color.A);
}

QColor brandColor() {
	return QColor(250, 67, 67);
}

QColor textViewLineBackgroundColor() {
	return QColor(243, 246, 250);
}

QColor headerColor() {
	return QColor(0, 0, 128);
}

QColor dataColor() {
	return QColor(142, 5, 255);
}

QColor statusCodeColor() {
	return QColor(0, 153, 51);
}

QColor responseColor() {
	return QColor(126, 135, 53);
}