#ifndef COLORS_H
#define COLORS_H

#include <QColor>

/** 
 * The accent colour that the user has selected.
 *
 * @retval The QColor representation of the system's accentColor.
 */
QColor accentColor();

/**
 * The colour of the Card Centric Ltd. brand.
 *
 * @retval A redish colour.
 */
inline QColor brandColor() { return QColor(250, 67, 67); }

/**
 * The background color for every second line in the text view.
 *
 * @retval	A greyey color.
 */
inline QColor textViewLineBackgroundColor() { return QColor(243, 246, 250); }

/**
 * The color of the four APDU header bytes.
 *
 * @retval A bluey color.
 */
inline QColor headerColor() { return QColor(0, 0, 128); }

/**
 * The color of the data bytes in the APDU command.
 *
 * @retval A pinky color.
 */
inline QColor dataColor() { return QColor(142, 5, 255); }

/**
 * The color of the SW1 and SW2 bytes.
 *
 * @retval A greeny color.
 */
inline QColor statusCodeColor() { return QColor(0, 153, 51); }

/**
 * The color of the data bytes in the APDU response.
 *
 * @retval A tanny color.
 */
inline QColor responseColor() { return QColor(126, 135, 53); }

#endif // COLORS_H
