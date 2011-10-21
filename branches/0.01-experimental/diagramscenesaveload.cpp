#include "diagramscene.h"
#include <math.h>

bool DiagramScene::save(const QString & filename)
{
  (void)filename;
  return rand()<RAND_MAX/2;
}

bool DiagramScene::load(const QString & filename)
{
  (void)filename;
  return rand()<RAND_MAX/2;
}

bool DiagramScene::exportTo(const QString & filename)
{
  (void)filename;
  return rand()<RAND_MAX/2;
}
