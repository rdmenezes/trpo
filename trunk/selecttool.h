/*! \file selecttool.h
    \author Mamontov
    Describes a selected tool data
*/
#ifndef SELECTTOOL_H
#define SELECTTOOL_H
#include "tool.h"
#include "box.h"
#include "freecomment.h"
#include "commentline.h"
/*! Select tool state, representing current user action
 */
enum SelectToolState
{
    STS_NONE,
    STS_MOVEFREECOMMENT
};

/*! Data about free comment move
 */
class FreeCommentMove
{
public:
    FreeComment * m_obj;                  //!< Moving object
    CommentLine * m_line;                 //!< Line
    QLineF        m_startlinepos;         //!< Starting line pos
    QPointF       m_startcommentpos;      //!< Starting commented pos
    QPointF       m_clickpos;             //!< Click position
    /*! Default constructor */
    inline FreeCommentMove() {}
    /*! Restores a positions of object */
    void restorePosition();
};


/*! \class SelectTool
    A selection tool, chosen by user
 */
class SelectTool : public Tool
{
private:
        SelectToolState  m_state; //!< State data
        FreeCommentMove  m_fcm;   //!< Free comment move data
public:
    /*! A selection tool
     */
    SelectTool();
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
    virtual ~SelectTool();
};

#endif // SELECTTOOL_H
