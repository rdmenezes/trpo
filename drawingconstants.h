/*! \file   drawingconstants.h
    \author Mamontov

    Describes a drawing constants data
*/
#ifndef DRAWINGCONSTANTS_H
#define DRAWINGCONSTANTS_H
#include <QtGlobal>
#include <QPointF>
#include <QRectF>

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
#define FC_PADDING 0.5
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
/*! Arrow angle
 */
#define A_ARROW_ANGLE 30
/*! Horizontal length of arrow
 */
#define A_ARROW_HORIZ_LEN 5
/*! Vertical length of arrow
 */
#define A_ARROW_VERT_LEN 3


/*! Comment line width
 */
#define CL_WIDTH 1
/*! Comment line color
 */
#define CL_COLOR QColor(0,0,0)

inline QRectF bound(QPointF  ps[], int n)
{
    qreal minx=ps[0].x(),miny=ps[0].y(),maxx=ps[0].x(),maxy=ps[0].y();
    for (int i=1;i<n;i++)
    {
        if (ps[i].x()<minx) minx=ps[i].x();
        if (ps[i].y()<miny) miny=ps[i].y();
        if (ps[i].x()>maxx) maxx=ps[i].x();
        if (ps[i].y()>maxy) maxy=ps[i].y();
    }
    return QRectF(minx,miny,maxx-minx,maxy-miny);
}


#endif // DRAWINGCONSTANTS_H
