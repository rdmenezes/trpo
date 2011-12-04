/*! \file   arrowtool.h
    \author Mamontov
    Describes a tool, used to create arrows
 */
#ifndef ARROWTOOL_H
#define ARROWTOOL_H
#include "tool.h"
#include "box.h"
#include "arrow.h"

/*! State for arrow tool
 */
enum ArrowToolState
{
    ATS_START,             //!< A starting state for arrow tool
    ATS_FIRSTPOINT         //!< A state, when first point is specified
};

/*! Describes, where first point of tool is located
 */
enum ArrowToolPointLocation
{
    ATS_NONE,               //!< First point was found at no placed
    ATS_BOX,                //!< Box data
    ATS_ARROW               //!< Arrow data
};

/*! \class ArrowTool
    A tool, used to create arrows
 */
class ArrowTool : public Tool
{
private:
         ArrowToolState          m_state;        //!< State of arrow tool
         Arrow *                 m_preview[2];   //!< Previewed arrows

         ArrowToolPointLocation  m_loc[2];       //!< Where is starting point located
         Box *                   m_boxes[2];     //!< Boxes, where points are located
         Arrow *                 m_arrows[2];    //!< Arrows, where points are located
         qreal                   m_poses[2];     //!< Positions on points of data
         QPointF                 m_points[2];    //!< Specified positions of data
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
