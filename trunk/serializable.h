/*!  \file     serializable.h
     \author   MamontovDmitry
     Describes a serializable object, which can be restored from tags
*/
#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <QDomDocument>
#include <QDomElement>
#include <QMap>
#include <QString>
/*! \class Serializable
    Serializable interface, which describes an item data
 */
class Serializable
{
public:
      /*!  Saves a data to document
           \param[in] doc     document data
           \param[in] element parent element data
       */
      virtual  void save(QDomDocument * doc,QDomElement * element)=0;
      /*! Loads a default data from document populating address map
          \param[in] element element data
          \param[in] addressMap adressedMap
       */
      virtual  void load(QDomElement * element,
                         QMap<void *, Serializable *> & addressMap)=0;
      /*! Resolves inner pointers, from stored in adress map
          \param[in] addressMap map of addresses
       */
      virtual  void resolvePointers(QMap<void *, Serializable *> & adressMap)=0;
      /*! Serializable destructor data
       */
      virtual ~Serializable();
};


template<typename T>
class Creator
{
  public:
        static  Serializable * create() { return new T(); }
};

/*! Class, that creates object, corresponding tags
    He needs initialization on start, and any object, must have
    default constructor
 */
class SerializableFactory
{

  private:
         /*! Map String -> creator, used to create data.
          */
         static QMap<QString, Serializable * (*)()> m_tagmap;
  public:
         /*! Binds a new creator for serializable
          */
         template<typename T>
         static void bind(const QString & name)
         {
             m_tagmap.insert(name,&(Creator<T>::create));
         }
         /*! Creates an object, corresponding to tag
          */
         static inline Serializable * create(const QString & tag)
         {
            if (m_tagmap.contains(tag))
                return m_tagmap[tag]();
            else
                return NULL;
         }
};

#endif // SERIALIZABLE_H
