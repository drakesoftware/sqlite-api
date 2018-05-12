/// \file RegressionTests.h
/// \brief  <Description>

#ifndef __regressiontests__h
#define __regressiontests__h


#include <time.h>
#include <math.h>

//*********************************************8
//Structure with test function pointer and test name
//with verbosity option
struct TestEntry
{
    bool (*func)(bool verbose);
    const char* name;
};


class RegressionTests
{
    
    
    public:
    
    RegressionTests(void)
    {
    }

    ~RegressionTests(void)
    {
    }


    static bool run(bool verbose=false);

};

bool RunTestList(TestEntry testlist[], bool verbose);

#endif

