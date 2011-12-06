/*! \file   arrowtool.h
    \author Mamontov
    Describes a tool, used to create arrows
 */
#ifndef ARROWTOOL_H
#define ARROWTOOL_H
#include "tool.h"
#include "box.h"
#include "arrow.h"
#include <QVector>
/*! State for arrow tool
 */
enum ArrowToolState
{
    ATS_START,             //!< A starting state for arrow tool
    ATS_FIRSTPOINT         //!< A state, when first point is specified
};

/*! Describes, where first point of tool is located
 */
enum StartPointLocation
{
    ATS_NONE=0,              //!< First point was found at no placed
    ATS_ARROW=1,             //!< Arrow data
    ATS_BOX=2                //!< Box data
};
/*! Arrow sensivity on x
 */
#define ARROW_SENSIVITY_X 3
/*! Arrow  sensivity on y
 */
#define ARROW_SENSIVITY_Y 3

/*! maximum preview segments used
 */
#define MAX_PREVIEW_SEGMENTS 3
/*! \class ArrowTool
    A tool, used to create arrows
 */
class ArrowTool : public Tool
{
typedef  void (ArrowTool::*Callback)();
private:
         ArrowToolState          m_state;        //!< State of arrow tool
         Arrow *                 m_preview[MAX_PREVIEW_SEGMENTS];   //!< Previewed arrows
         int                     m_preview_amount;  //!< Current view amount
         void                   (ArrowTool::* m_drawarr[13])(const QPointF & ,
                                                             const QPointF & ); //!< Array of drawing functions
         void                   (ArrowTool::* m_drawllr[13])(const QPointF & ,
                                                             const QPointF & ); //!< Array of drawing functions for line to line case

         QVector< QVector<ArrowTool::Callback> >  m_connection_cbs; //!< Connection callbacks

         StartPointLocation      m_loc[2];       //!< Where is starting point located
         Box *                   m_boxes[2];     //!< Boxes, where points are located
         Arrow *                 m_arrows[2];    //!< Arrows, where points are located
         qreal                   m_poses[2];     //!< Positions on points of data
         QPointF                 m_points[2];    //!< Specified positions of data

         /*! Disconnect every preview
          */
         void disconnectAllPreviews();
         /*! Puts a previews arrows to diagram
          */
         void addPreviewsToDiagram();
         /*! Tests, whether previews can be placed
          */
         bool canPlacePreviews();
         /*! Removes odd segments
          */
         void removeOddSegments();
         /*! Generates a new preview arrow triplet
             \param[in] p position
          */
         void generatePreview(const QPointF & p);
         /*! Connects none to none
          */
         void connectNoneToNone();
         /*! Connect none to line
          */
         void connectNoneToLine();
         /*! Connect none to box
          */
         void connectNoneToBox();
         /*! Connect line to none
          */
         void connectLineToNone();
         /*! Connect line to  line
          */
         void connectLineToLine();
         /*! Connects box to box
          */
         void connectLineToBox();
         /*! Connects box to none
          */
         void connectBoxToNone();
         /*! Connects box to line
          */
         void connectBoxToLine();
         /*! Connects box to box
          */
         void connectBoxToBox();
         /*! Redraws a lines
             \param[in] pos        position
             \param[on] canBeZero  whether it can be zero
          */
         void redrawLines(const QPointF & pos,bool canBeZero);
         /*! Makes only first line of preview visible
          */
         void makeOneLineVisible();
         /*! Makes two lines of preview visible
          */
         void makeTwoLinesVisible();
         /*! Makes all lines of preview visible
          */
         void makeAllLinesVisible();
         /*! Draws zero length arrow
             \param[in] p1 first point
             \param[in] p2 second point
          */
         void    drawZeroDirected(const QPointF & p1, const QPointF & p2);
         /*! Draws a right directed arrow
             \param[in] p1 first point
             \param[in] p2 second point
          */
         void    drawRightLeftDirected(const QPointF & p1, const QPointF & p2);
         /*! Draws a top directed arrow
             \param[in] p1 first point
             \param[in] p2 second point
          */
         void    drawTopBottomDirected(const QPointF & p1, const QPointF & p2);
         /*! Draws a horizontal and after that vertical directed arrow
             \param[in] p1 first point
             \param[in] p2 second point
          */
         void    drawHVDirected(const QPointF & p1, const QPointF & p2);
         /*! Draws a vertical and after that horizontal directed arrow
             \param[in] p1 first point
             \param[in] p2 second point
          */
         void    drawVHDirected(const QPointF & p1, const QPointF & p2);
         /*! Draws a vertical three lines connections
             \param[in] p1 first point
             \param[in] p2 second point
          */
         void    drawVerticalDirected3(const QPointF & p1, const QPointF & p2);
         /*! Draws a horizontal three lines connections
             \param[in] p1 first point
             \param[in] p2 second point
          */
         void    drawHorizontalDirected3(const QPointF & p1, const QPointF & p2);
protected:
         /*! Returns a clockwise directions for arrow number - 0-12
             \param[in] pos               position
             \param[in] canReturnZero     whether it can return zero on some positions
          */
         int clockwiseDirection(const QPointF & pos, bool canReturnZero);
public:
         /*! Default constructor
          */
         ArrowTool();
         /*! Clears an inner state of tool
          */
         virtual void   clearState();
         /*! Inits an inner state of tool
          */
         virtual void   initState();
         /*! An action, that is performed if user clicks on scene
             \param[in] p      point
             \param[in] item   item, located in the point (NULL can be passed too)
         */
         virtual bool    onClick(const QPointF & p, QGraphicsItem * item);
         /*! A vector of items, whick onClick can be performed
         */
         virtual QVector<int> getClickableItems();
         /*! An action, that is performed if user releases a mouse button on scene
             \param[in] p      point
             \param[in] item   item, located in the point (NULL can be passed too)
         */
         virtual bool    onRelease(const QPointF & p, QGraphicsItem * item);
         /*! A vector of items, whick onRelease can be performed
          */
         virtual QVector<int> getReleaseableItems();
         /*! An action, that is performed, when user moves mouse
             \param[in] lastpos  last position
             \param[in] pos      new position
          */
         virtual void onMove(const QPointF & lastpos, const QPointF & pos);
         /*! This method is called, when user performs key down event
             \param[in] event event data
             \param[in] item  item
          */
         virtual bool onKeyDown(QKeyEvent * event, QGraphicsItem * item);
         /*! Returns an items,on which key down can be performed
          */
         virtual QVector<int> getKeyDownItems();
         /*! Virtual destructor
         */
         virtual ~ArrowTool();
};

#endif // ARROWTOOL_H
