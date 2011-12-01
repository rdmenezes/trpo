/*! \file   drawingconstants.h
    \author Mamontov

    Describes a drawing constants data
*/
#ifndef DRAWINGCONSTANTS_H
#define DRAWINGCONSTANTS_H
#include <QtGlobal>

/*! Rectangle for testing metrics
 */
#define METRICS_TEST_RECT  QRectF(0,0,4.0E+9,4.0E+9)

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


/*! Free comment default label
 */
#define FC_TEXT   "Comment"
/*! Default font size
 */
#define FC_FONT_SIZE  12
/*! Default rectangle padding between text
 */
#define FC_PADDING 1
/*! Default font color
 */
#define FC_FONT_COLOR  QColor(0,0,0)


/*! Rounding radius with padding
 */
#define A_ROUNDING_PADDING 6
/*! Rounding radius
 */
#define A_ROUNDING_RADIUS  5
/*! Pen width
 */
#define A_PEN_WIDTH 1
/*! Arrow color
 */
#define A_COLOR  QColor(0,0,0)

class QPainter;
class QPointF;
/*! Draws a brackets
    \param[in] p painter
    \param[in] point point
    \param[in] bracket bracket
 */
void drawBracket(QPainter * p,const QPointF & point, qreal angle);

#endif // DRAWINGCONSTANTS_H
