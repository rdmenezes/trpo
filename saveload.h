#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <QString>
#include <QStringList>
#include <QRect>
#include <QRectF>
#include <QPointF>
#include <QPair>
#include <QHash>
#include "diagramdata.h"

//Общий класс, создающий единство интерфейса для сериализации и десериализации примитивов
template<typename T >
class SaveLoad
{
public:
  /*! Сохраняет объект в строку
           \param[in] object объект для сохранения
           \return строка с содержимым объекта
   */
  static QString save(const T & /*object*/)
  {
     return QString();
  }
  /*! Загружает объект из строки
           \param[in]  string строка с содержимым объекта
           \return  восстановленный объект
   */
  static T load(const QString & string)
  {
      return T();
  }
};

//Это инстанцирование шаблона определяет поведение для типа int
template<>
class SaveLoad<int>
{
public:
    /*! Сохраняет int в строку
           \param[in] object объект для сохранения
           \return строка с содержимым объекта
   */
  static QString save(const int & object);
  /*! Загружает int из строки
           \param[in]  string строка с содержимым объекта
           \return  восстановленный объект
   */
   static int load(const QString & string);
};

template<>
class SaveLoad<QString>
{
public:
    /*! Сохраняет int в строку
           \param[in] object объект для сохранения
           \return строка с содержимым объекта
   */
  static inline QString save(const QString & object)
  { return object; }
  /*! Загружает int из строки
           \param[in]  string строка с содержимым объекта
           \return  восстановленный объект
   */
  static inline QString load(const QString & string)
  { return string; }
};

template<>
class SaveLoad<bool>
{
public:
    /*! Сохраняет int в строку
           \param[in] object объект для сохранения
           \return строка с содержимым объекта
   */
  static inline QString save(bool  object)
  { int tmp=(object)?1:0; return SaveLoad<int>::save(tmp); }
  /*! Загружает int из строки
           \param[in]  string строка с содержимым объекта
           \return  восстановленный объект
   */
  static inline bool load(const QString & string)
  { return SaveLoad<int>::load(string)==1; }
};



template<>
class SaveLoad<double>
{
public:
    static QString save(const double & object);
    /*! Загружает int из строки
             \param[in]  string строка с содержимым объекта
             \return  восстановленный объект
     */

     static double load(const QString & string);
};

template<>
class SaveLoad<DiagramParent>
{
public:
    static QString save(const DiagramParent & loc);
    /*! Загружает int из строки
        \param[in]  string строка с содержимым объекта
        \return  восстановленный объект
     */
    static DiagramParent load(const QString & string);
};

template<>
class SaveLoad <void *>
{
public:
   static QString save( void * ptr);
   static void * load(const QString & string);
};

template<typename T>
class SaveLoad<T*>
{
public:
    static QString save( T * ptr)               { return SaveLoad<void*>::save(ptr);}
    static T * load(const QString & string)  { return (T*)(SaveLoad<void*>::load(string));}

};

template<>
class SaveLoad <QRect>
{
public:
  static QString save(const QRect & r);
  static QRect load(const QString & string);
};

template<>
class SaveLoad <QRectF>
{
public:
   static QString save(const QRectF & r);
   static QRectF load(const QString & string);
};



template<>
class SaveLoad <QPointF>
{
public:
   static QString save( const QPointF & p);
   static QPointF load( const QString & string);
};


template<>
class SaveLoad <QSize>
{
public:
   static QString save(const QSize & s);
   static QSize load(const QString & string);
};


template<>
class SaveLoad <QSizeF>
{
public:
   static QString save(const QSizeF & s);
   static QSizeF load(const QString & string);
};


template<typename T1, typename T2>
class SaveLoad <QPair <T1,T2> >
{
 public:
   static QString save( const QPair<T1,T2>  & p)
   {
     QString result1=SaveLoad<T1>::save(p.first) ;
     QString result2=SaveLoad<T2>::save(p.second) ;
     QString result;
     result.append(result1);
     result.append("@");
     result.append(result2);
     return result;
   }
   static QPair<T1,T2> load( const QString & string)
   {
       QPair<T1,T2> tmpPair;
       QString str1 =   string.section('@', 0, 0);
       QString str2 =   string.section('@', 1, 1);

       tmpPair.first=SaveLoad<T1>::load(str1);
       tmpPair.second=SaveLoad<T2>::load(str2);
       return tmpPair;
   }
};



template<typename T>
class SaveLoad <QVector <T> >
{
    public:


    static QString save( const QVector <T> & v)
    {
      QString tmpVec;
      for (int i=0;i<v.size();i++)
      {
            tmpVec.append(SaveLoad<T>::save(v[i]));
            if (i!=v.size()-1)
            tmpVec.append("^");
      };
      return tmpVec;
    }
    static QVector <T> load( const QString & string)
    {
      QVector<T> vecloadTmp;
      QStringList tmp=string.split("^");
       for (int i=0;i<tmp.size();i++)
       {
         vecloadTmp.append(SaveLoad<T>::load(tmp[i]));
       }
       return  vecloadTmp;
    }
};


template<>
class SaveLoad <SwapEntry>
{
public:
   static QString save( const SwapEntry & p);
   static SwapEntry load( const QString & string);
};


template<typename T1,typename T2>
class SaveLoad< QHash<T1,T2> >
{
public:
   static QString save( const QHash<T1,T2> & p)
   {
       QVector< QPair<T1,T2> > vector;
       typename QHash<T1,T2>::const_iterator it=p.begin();
       for (; it!=p.end();it++)
       {
           vector<<QPair<T1,T2>(it.key(),it.value());
       }
       return SaveLoad< QVector< QPair<T1,T2> > >::save(vector);
   }

   static QHash<T1,T2> load( const QString & string)
   {
         QVector< QPair<T1,T2> > vector=SaveLoad< QVector< QPair<T1,T2> > >::load(string);
         QHash<T1,T2> hash;
         for (int i=0;i<vector.size();i++)
         {
             hash.insert(vector[i].first,vector[i].second);
         }
         return hash;
   }
};

template<typename T>
QString save(const T & obj) { return SaveLoad<T>::save(obj); }

template<typename T>
T load(const QString & string) { return SaveLoad<T>::load(string); }


#endif // SAVELOAD_H
