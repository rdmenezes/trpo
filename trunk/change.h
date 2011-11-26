/*!  \file     change.h
     \author   MamontovDmitry
     Describes a change, performed on diagram
*/
#ifndef CHANGE_H
#define CHANGE_H

class DiagramScene;
class Diagram;

extern  int ___change_type;
/*! \class Change
    Describes a change, performed on diagram
 */
class Change
{
protected:
         DiagramScene*  m_scene;   //!< Diagram scene
         Diagram     *  m_diagram; //!< Diagram
public:         
         /*! Sets a data for tool
             \param[in] scene scene
             \param[in] diagram diagram data
          */
         inline void  setDiagramData(DiagramScene * scene, Diagram * diagram)
         {
              m_scene=scene;
              m_diagram=diagram;
         }
         /*! Undo a change
          */
         virtual void undo()=0;
         /*! Redos a change
          */
         virtual void redo()=0;
         /*! Returns a type for change
          */ 
         virtual int type()=0;
         /*! Virtual destructor
          */
         virtual ~Change();
};

/*! Macro, that should be placed in beginning of Change subclasses to perform type checks
    If your need to know what class is Change, simply compare it to WhateverChange::Type
 */
#define CHANGE                                  \
public:                                         \
    static Type=___change_type++;               \
    int type() { return Type;}
#endif // CHANGE_H
