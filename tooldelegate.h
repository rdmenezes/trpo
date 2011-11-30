/*!  \file     tooldelegate.h
     \author   MamontovDmitry
     Describes a selected tool delegate for QTableView
*/
#ifndef TOOLDELEGATE_H
#define TOOLDELEGATE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QImage>
#include "tool.h"
#include "mainwindow.h"

class ToolScene;

/*! \class ToolSceneData
    Class of tool scene data, that includes an icon,  tool for data
    and pointer to vector of other scenes to provide selections
 */
class ToolSceneData
{
 protected:
      QImage *     m_icon;     //!< Icon of tool scene
      MainWindow * m_window;   //!< Main window
      Tool       * m_tool;     //!< Tool
      ToolScene  * m_scene;    //!< Scene
 public:
      /*!  Creates a tool data
           \param[in] icon   icon data
           \param[in] tool   tool data
           \param[in] window window
       */
      inline ToolSceneData(QImage * icon, MainWindow * window, Tool * tool)
      { m_icon=icon; m_window=window; m_tool=tool; }
      /*! Returns a tool of data
          \return tool
       */
      inline Tool * tool() const { return m_tool; }
      /*! Returns an icon for data
       */
      inline QImage * icon() const { return m_icon; }
      /*! Sets a scene for data
          \param[in] data tool data
       */
      inline void setScene(ToolScene * scene) { m_scene=scene; }
      /*! Deselects a tool by calling ToolScene::deselect
       */
      void deselect();
      /*! Selects a tool by calling MainWindow::selectTool.
       */
      void select();
      /*! Destructor
       */
      inline ~ToolSceneData() { delete m_icon; delete m_tool; }
};

class ToolScene  : public QGraphicsScene
{
Q_OBJECT
protected:
        bool                      m_init;  //!< Init data
        ToolSceneData *           m_data;  //!< Tool scene data
        QVector<QGraphicsItem *>  m_frame; //!< Red frame border
        /*! Draws a selection frame
         */
        virtual void drawSelectionFrame();
        /*! Performs item selection
         */
        void 	mousePressEvent ( QGraphicsSceneMouseEvent * /* mouseEvent */ );
public:
        /*! Empty constructor
         */
        inline ToolScene() { m_init=false; };
        /*! Sets a data for scene
         */
        void setData(ToolSceneData * data);
        /*! Deselects a toolscene
         */
        void deselect();
        /*! Selects   a scene
         */
        void select();
        /*! Deletes a data
         */
        inline ~ToolScene() { delete m_data; }
        /*! Draws a background
            \param[in] painter painter data
            \param[in] rect    rectangle
         */
        void drawBackground ( QPainter * painter, const QRectF & rect );
};



#endif // TOOLDELEGATE_H
