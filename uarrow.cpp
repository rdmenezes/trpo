#include "uarrow.h"

#include "uarrowsegment.h"
#include "uarrowshaft.h"
#include "uarrowhead.h"
#include "uarrowlink.h"

UArrow::UArrow(QWidget *parent, QWidget *slot) : QObject() {
    
    superView = (MainWindow *)parent;
    
    segments = new QVector<UArrowSegment *>();
    
    UArrowShaft *shaft = new UArrowShaft(parent, slot);
    segments->append(shaft);
}
