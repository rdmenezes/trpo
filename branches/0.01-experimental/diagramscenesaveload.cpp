#include "diagramscene.h"
#include "diagramset.h"
#include <math.h>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

bool DiagramScene::save(const QString & filename)
{
  QDomDocument  doc("IDEFML");
  if (!(this->diagram()->set()->areDiagramsCorrect()))
      return false;
  this->diagram()->set()->save(&doc);
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
  (void)filename;
  return rand()<RAND_MAX/2;
}

bool DiagramScene::exportTo(const QString & filename)
{
  //Данное действие необходимо выполнить, т.к. иначе на картинке останутся виджеты
  this->hideUI();
  (void)filename;
  return rand()<RAND_MAX/2;
}
