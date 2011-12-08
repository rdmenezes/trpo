/*! \file selecttool.h
    \author Mamontov
    Describes a selected tool data
*/
#ifndef SELECTTOOL_H
#define SELECTTOOL_H
#include "tool.h"
#include "arrow.h"
#include "objectconnector.h"
#include "box.h"
#include "freecomment.h"
#include "commentline.h"

/*! \class DynamicEditState
    Represents a dynamic editing state of select tool
 */
class DynamicEditState
{
 protected:
            Diagram * m_diagram;   //!< Diagram testing tool
            QPointF   m_clickpos;  //!< Click position data
 public:
          /*! Constructs a dynamic editing state
              \param[in] diagram   diagram data
              \param[in] clickpos  click  position
           */
          inline DynamicEditState(Diagram * diagram, const QPointF & clickpos)
          { m_diagram=diagram; m_clickpos=clickpos; }
          /*! Clears an inner dynamic state
           */
          virtual void clearState()=0;
          /*! An action, which is performed, when releasing on action
              \param[in] p releasing point
           */
          virtual void onRelease(const QPointF & p)=0;
          /*! An action, which is performed, when moving
           */
          virtual void onMove(const QPointF & p)=0;
};



/*! Data about free comment move
 */
class FreeCommentMoving : public DynamicEditState
{
private:
    FreeComment * m_obj;                  //!< Moving object
    CommentLine * m_line;                 //!< Line
    QLineF        m_startlinepos;         //!< Starting line pos
    QPointF       m_startcommentpos;      //!< Starting commented pos
    /*! Restores a positions of object */
    void restorePosition();
public:
    /*! Default constructor
        \param[in] diagram   diagram data
        \param[in] clickpos  click  position
        \param[in] obj       object
     */
    FreeCommentMoving(Diagram * diagram,
                      const QPointF & clickpos,
                      FreeComment * obj);
    /*! Clears an inner dynamic state
     */
    virtual void clearState();
    /*! An action, which is performed, when releasing on action
        \param[in] p releasing point
     */
    virtual void onRelease(const QPointF & p);
    /*! An action, which is performed, when moving
     */
    virtual void onMove(const QPointF & p);

};

/*! Data for object changing
 */
class ObjectChangingData
{
  protected:
             QVector<CommentLine *> m_commentlines;            //!< Connected comment lines
             QVector<QLineF>        m_startingposition;        //!< Starting position

             QVector< QVector< ObjectConnector* > > m_connected_objects;  //!< Connected objects
             QVector< Connection >      m_connection_direction;           //!< Connected directions
             QVector< qreal >           m_poses;                          //!< Position
             QVector< ObjectConnector * > m_attachedconnector;            //!< Attached connectors
             QVector< QVector<Arrow*> > m_previews;                       //!< Previewed parts

             /*! Gets all connected data, filling object connector
              */
             void extractData(ObjectConnector * c);
             /*! Extracts a data from one vector
              */
             void extractData(ObjectConnector * c,Connection ct,const QVector< QPair<qreal,ObjectConnector *> > & from);
public:
             /*! Data for object changing
              */
             inline ObjectChangingData() {}
};
/*! Data for box changing
 */
class BoxChangingData: public ObjectChangingData
{
 protected:
            Box * m_box;           //!< Box
            QRectF m_rect;         //!< Rectangle
 public:
            /*! Data for object changing
                \param[in] box box data
             */
            inline BoxChangingData(Box * box) { m_box=box; m_rect=m_box->collisionRect(); }
};

/*! \class ArrowSegmentMoving
    A data abount arrow segment moving
 */
class ArrowSegmentMoving: public DynamicEditState,public ObjectChangingData
{
private:
    Arrow * m_obj;                              //!< Moving object
    QLineF  m_statingpos;                       //!< Starting object position
public:
    /*! Default constructor
        \param[in] diagram   diagram data
        \param[in] clickpos  click  position
        \param[in] obj       object
     */
    ArrowSegmentMoving(Diagram * diagram,
                      const QPointF & clickpos,
                      Arrow * obj);
    /*! Clears an inner dynamic state
     */
    virtual void clearState();
    /*! An action, which is performed, when releasing on action
        \param[in] p releasing point
     */
    virtual void onRelease(const QPointF & p);
    /*! An action, which is performed, when moving
     */
    virtual void onMove(const QPointF & p);
};

/*! \class BoxMoving
    A state,used when moving a box
 */
class BoxMoving: public DynamicEditState, public BoxChangingData
{
public:
    /*! Default constructor
        \param[in] diagram   diagram data
        \param[in] clickpos  click  position
        \param[in] obj       object
     */
    BoxMoving(Diagram * diagram,
              const QPointF & clickpos,
              Box * obj);
    /*! Clears an inner dynamic state
     */
    virtual void clearState();
    /*! An action, which is performed, when releasing on action
        \param[in] p releasing point
     */
    virtual void onRelease(const QPointF & p);
    /*! An action, which is performed, when moving
     */
    virtual void onMove(const QPointF & p);
};

/*! \enum BoxCorner
    BoxCorner, used for resizing
 */
enum BlockCorner
{
    BC_UPPERLEFT,
    BC_UPPERRIGHT,
    BC_LOWERLEFT,
    BC_LOWERRIGHT
};


/*! \class BoxMoving
    A state,used when moving a box
 */
class BoxResize: public DynamicEditState, public BoxChangingData
{
public:
    /*! Default constructor
        \param[in] diagram   diagram data
        \param[in] clickpos  click  position
        \param[in] obj       object
     */
    BoxResize(Diagram * diagram,
              const QPointF & clickpos,
              Box * obj);
    /*! Clears an inner dynamic state
     */
    virtual void clearState();
    /*! An action, which is performed, when releasing on action
        \param[in] p releasing point
     */
    virtual void onRelease(const QPointF & p);
    /*! An action, which is performed, when moving
     */
    virtual void onMove(const QPointF & p);
};


/*! \class SelectTool
    A selection tool, chosen by user
 */
class SelectTool : public Tool
{
private:
        DynamicEditState * m_state; //!< State data
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