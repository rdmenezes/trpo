/*! \file   diagramset.h
    \author Mamontov

    Describes a set of diagrams, used in program
*/
#include <QHash>
#include "diagram.h"
#include "serializable.h"
#include "saveload.h"
#ifndef DIAGRAMSET_H
#define DIAGRAMSET_H

class QDomDocument;

/*! \class DiagramSet
    Describes a set of diagrams
 */
class DiagramSet: public Serializable
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
        \param[in] l location
        \return new diagram
     */
    Diagram * create(const DiagramParent & l);
    /*! Gets a diagram by id
        \param[in] id
        \return pointer to diagram
     */
    Diagram * get(int id);
    /*! Returns diagram id by diagram
        \param[in] diagram diagram data
        \return id
     */
    int find(const Diagram * diagram) const;
    /*! Removes a diagram from set
        \param[in] id  diagram id
     */
    void remove(int id);
    /*! Tests, whether set is empty
     */
    bool empty() const;

    //Reimplemented from serializable
    /*!  Saves a data to document
         \param[in] doc     document data
         \param[in] element parent element data
     */
    virtual  void save(QDomDocument * doc,QDomElement * element);
    /*! Loads a default data from document populating address map
        \param[in] element element data
        \param[in] addressMap adressedMap
     */
    virtual  void load(QDomElement * element,
                       QMap<void *, Serializable *> & addressMap);
    /*! Resolves inner pointers, from stored in adress map
        \param[in] addressMap map of addresses
     */
    virtual  void resolvePointers(QMap<void *, Serializable *> & adressMap);

    /*! Destructor
     */
    ~DiagramSet();
};

#endif // DIAGRAMSET_H
