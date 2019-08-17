
#include "rt_ResTest.h"

int main(int argc, char *argv[])
{
    if (1 < argc)
    {
        test_call_t::instance()(argv[1]);
    }

    return 0;
}
