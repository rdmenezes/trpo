/*!  \file     tool.h
     \author   MamontovDmitry
     Describes a selected tool
*/
#ifndef TOOL_H
#define TOOL_H
#include <QImage>
#include <QGraphicsItem>
#include <QPointF>
#include <QVector>

class DiagramScene;
class Diagram;
/*! \class Tool
    Describes a tool, selectable by user. A Tool manages all clicks, drag and drops
    instead of QGraphicsScene
*/
class Tool
{
protected:
        QImage         m_icon;    //!< Icon of tool, that can be shown in the tool panel
        DiagramScene*  m_scene;   //!< Diagram scene
        Diagram     *  m_diagram; //!< Diagram
public:
        /*! Sets a data for tool
            \param[in] scene scene
            \param[in] diagram diagram data
         */
        inline void  setDiagramData(DiagramScene * scene, Diagram * diagram)
        {
            if (!this) return;
             m_scene=scene;
             m_diagram=diagram;
        }
        /*! Clears an inner state of tool
         */
        virtual void   clearState();
        /*! An action, that is performed if user clicks on scene
            \param[in] p      point
            \param[in] item   item, located in the point (NULL can be passed too)
        */
        virtual void    onClick(const QPointF & p, QGraphicsItem * item)=0;
        /*! A vector of items, whick onClick can be performed
        */
        virtual QVector<int> getClickableItems()=0;
        /*! An action, that is performed if user releases a mouse button on scene
            \param[in] p      point
            \param[in] item   item, located in the point (NULL can be passed too)
        */
        virtual void    onRelease(const QPointF & p, QGraphicsItem * item)=0;
        /*! A vector of items, whick onRelease can be performed
         */
        virtual QVector<int> getReleaseableItems()=0;
        /*! An action, that is performed, when user moves mouse
            \param[in] lastpos  last position
            \param[in] pos      new position
         */
        virtual void onMove(const QPointF & lastpos, const QPointF & pos)=0;
        /*! This method is called, when user performs key down event
            \param[in] event event data
            \param[in] item  item
         */
        virtual void onKeyDown(QKeyEvent * event, QGraphicsItem * item)=0;
        /*! Returns an items,on which key down can be performed
         */
        virtual QVector<int> getKeyDownItems()=0;
        /*! Virtual destructor
        */
        virtual ~Tool();
};

#endif // TOOL_H
