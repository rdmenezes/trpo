/*! \file   commentlinetool.h
    \author Mamontov

    Describes a tool, for creation a comment lines
 */
#ifndef COMMENTLINETOOL_H
#define COMMENTLINETOOL_H
#include "tool.h"
#include "freecomment.h"
#include "commentline.h"
#include "attachedcomment.h"
#include "box.h"
#include "arrow.h"
#include "arrowtool.h"

/*! Describes a tool for state
 */
enum CommentLineToolState
{
  CLTS_START,         //!< Starting state
  CLTS_FIRSTPOINT     //!< First point
};




/*! Describes a tool for creation of comment line
 */
class CommentLineTool : public Tool
{
private:
        CommentLineToolState  m_state;  //!< State of comment line
        StartPointLocation    m_loc;    //!< Starting point location
        CommentLine         * m_line;   //!< Line data
        Box                 * m_box;    //!< Box data
        Arrow               * m_arrow;  //!< Arrow data
        QPointF               m_startpoint;  //!< Starting poiny
public:
        CommentLineTool();
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
        virtual ~CommentLineTool();
};

#endif // COMMENTLINETOOL_H
