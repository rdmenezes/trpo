/*! \file   boxtool.h
    \author Mamontov
    A tool for box creation
*/
#ifndef BOXTOOL_H
#define BOXTOOL_H
#include "tool.h"
#include "box.h"

/*! \class BoxTool
    A tool for box creation
 */
class BoxTool : public Tool
{
private:
    Box * m_obj;
public:
    BoxTool();
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
    virtual ~BoxTool();
};

#endif // BOXTOOL_H
