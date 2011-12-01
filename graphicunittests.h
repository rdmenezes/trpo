/*! \file graphicunittests.h
    \author Mamontov
    This class performs a unit tests, used for graphics
 */
#ifndef GRAPHICUNITTESTS_H
#define GRAPHICUNITTESTS_H

class Diagram;

class GraphicUnitTests
{
private:
       Diagram * m_diag;
public:
        inline GraphicUnitTests(Diagram * d)  {m_diag=d;}
        void performBoxVerticalTextRegenTest();
        void performFreeCommentTest();
        void performArrowTest();
        void performVerticalTest();
};

#endif // GRAPHICUNITTESTS_H
