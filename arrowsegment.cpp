#include "arrowsegment.h"
#include "arrowpoint.h"

ArrowSegment::ArrowSegment(ArrowPoint * in, ArrowPoint * out)
{
  m_in=in;
  m_out=out;
  m_in->addOutputSegment(this);
  m_out->addInputSegment(this);
}


ArrowSegment::~ArrowSegment()
{

}
