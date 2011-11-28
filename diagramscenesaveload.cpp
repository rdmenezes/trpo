#include "diagramscene.h"
#include "diagramset.h"
#include "arrowsegment.h"
#include "alabelitem.h"
#include "alineitem.h"
#include <math.h>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>

bool DiagramScene::save(const QString & filename)
{
  QDomDocument  doc("IDEFML");
  //this->diagram()->set()->save(&doc);
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly))
      return false;
  QTextStream stream(&file);
  stream<<doc.toString();
  file.close();
  return true;
}

bool DiagramScene::load(const QString & filename)
{
  QDomDocument  doc("IDEFML");
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
      return false;
  if (!doc.setContent(&file))
  {
      file.close();
      return false;
  }
  QDomElement root=doc.documentElement();
  if (root.tagName()!="set")
      return false;
  this->clear();
  /*!< TODO: Replace with good code
  this->diagram()->set()->load(&root);
  for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
      if (this->diagram()->getBlockByID(i))
         this->addItem(this->diagram()->getBlockByID(i));
  for (int i=0;i<diagram()->annotationLabels().size();i++)
      this->addItem(this->diagram()->annotationLabels()[i]);
  for (int i=0;i<diagram()->annotationLines().size();i++)
      this->addItem(this->diagram()->annotationLines()[i]);
  for (int i=0;i<diagram()->arrowSegments().size();i++)
      this->addItem(this->diagram()->arrowSegments()[i]);
  this->update();
  */
  return true;
}

bool DiagramScene::exportTo(const QString & filename)
{
  //������ �������� ���������� ���������, �.�. ����� �� �������� ��������� �������
  this->hideUI();
  QImage image(this->width(), this->height(),QImage::Format_ARGB32);

  image.fill(0xFFFFFFFF);

  QPainter painter(&image);
  this->render(&painter);

  bool isSaved= image.save(filename, "png");
  return isSaved;
}
