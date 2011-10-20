#include "arrowpoint.h"
#include "alineitem.h"
#include "arrowsegment.h"

ArrowPoint::ArrowPoint(qreal x, qreal y): QPointF(x,y)
{
    m_block=NULL;
}


bool ArrowPoint::isBeginPoint()  const { return m_out.size()==0; }
bool ArrowPoint::isEndingPoint() const { return m_in.size()==0; }

bool ArrowPoint::mustBeDecomposed() const { return isEndingPoint() && m_out.size()>1; }
bool ArrowPoint::isSeparated()      const { return isBeginPoint() && isEndingPoint(); }

void ArrowPoint::update()
{
 for (int i=0;i<m_lines.size();i++)
    this->m_lines[i]->update();
 for (int i=0;i<m_in.size();i++)
    this->m_in[i]->update();
 for (int i=0;i<m_out.size();i++)
    this->m_out[i]->update();
}

ArrowPoint::~ArrowPoint()
{

}
