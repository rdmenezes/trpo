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
        void performArrowTest2();
        void performArrowTest3();
        void performArrowTest4();
        void performArrowTest5();
};

#endif // GRAPHICUNITTESTS_H
