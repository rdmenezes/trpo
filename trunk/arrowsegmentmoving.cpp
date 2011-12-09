#include "selecttool.h"



ArrowSegmentMoving::ArrowSegmentMoving(Diagram * diagram,
                                       const QPointF & clickpos,
                                       Arrow * obj)
                   :DynamicEditState(diagram,clickpos),
                    ObjectChangingData()

{
    m_obj=obj;
    m_startingpos=*(obj->model());
}


void ArrowSegmentMoving::clearState()
{

}


void ArrowSegmentMoving::onRelease(const QPointF &p)
{

}

void ArrowSegmentMoving::onMove(const QPointF &p)
{

}
