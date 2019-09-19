#ifndef COLORS_H
#define COLORS_H

class QColor;

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
QColor brandColor();

/**
 * The background color for every second line in the text view.
 *
 * @retval	A greyey color.
 */
QColor textViewLineBackgroundColor();

/**
 * The color of the four APDU header bytes.
 *
 * @retval A bluey color.
 */
QColor headerColor();

/**
 * The color of the data bytes in the APDU command.
 *
 * @retval A pinky color.
 */
QColor dataColor();

/**
 * The color of the SW1 and SW2 bytes.
 *
 * @retval A greeny color.
 */
QColor statusCodeColor();

/**
 * The color of the data bytes in the APDU response.
 *
 * @retval A tanny color.
 */
QColor responseColor();

#endif // COLORS_H
