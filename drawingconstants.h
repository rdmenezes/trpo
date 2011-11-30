/*! \file   drawingconstants.h
    \author Mamontov

    Describes a drawing constants data
*/
#ifndef DRAWINGCONSTANTS_H
#define DRAWINGCONSTANTS_H


/*! Default box text
*/
#define BOX_DEFAULT_TEXT "Default process"
/*! A space betweeen default label and default font
*/
#define BOX_DEFAULT_NUMBER_SPACE  3
/*! Box number font size
 */
#define BOX_NUMBER_FONT_SIZE 10
/*! Box number font size
 */
#define BOX_TEXT_FONT_SIZE   12
/*! Box border width
 */
#define BOX_BORDER_WIDTH 1
/*! Box border width
 */
#define BOX_BORDER_PADDING 2
/*! Common paddings
 */
#define BOX_LABEL_PADDING (BOX_BORDER_WIDTH+BOX_BORDER_PADDING)
/*! Box number text
 */
#define BOX_NUMBER_TEXT  QString("A")+QString::number(m_id)
/*! Box line color
 */
#define BOX_LINE_COLOR   QColor(0,0,0)
/*! Box text color
 */
#define BOX_TEXT_COLOR   QColor(0,0,0)

#endif // DRAWINGCONSTANTS_H
