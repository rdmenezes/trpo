/*! \file   diagramset.h
    \author Mamontov

    Describes a set of diagrams, used in program
*/
#include <QHash>
#include "diagram.h"
#ifndef DIAGRAMSET_H
#define DIAGRAMSET_H

class QDomDocument;

/*! \class DiagramSet
    Describes a set of diagrams
 */
class DiagramSet
{
private:
    /*! Max diagrams in set
     */
    int                   m_max_id;
    /*! Set of diagram
     */
    QHash<int,Diagram *>  m_set;
public:
    /*! Constructs a diagram with one diagram. Created diagram has an id - 0
     */
    DiagramSet();
    /*! Creates a new diagram with specified location
        \param[in] loc location
        \return diagram id
     */
    int create(const ParentLocation & loc);
    /*! Gets a diagram by id
        \param[in] id
        \return pointer to diagram
     */
    Diagram * get(int id);
    /*! Saves itself to a document
        \param[in] document to save to
     */
    void save(QDomDocument * doc);
    /*! Tests,whether diagrams are correct
     */
    bool areDiagramsCorrect();
    /*! Destructor
     */
    ~DiagramSet();
};

#endif // DIAGRAMSET_H
