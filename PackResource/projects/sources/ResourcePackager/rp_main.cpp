
#include <iostream>

#include "rp_ResPackager.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Hasn\'t specified the resource directory !" << std::endl;
        return 0;
    }

    std::cout << "pack dir: " << argv[1] << std::endl;

    unsigned int t1 = _CurrentMillisecond();

    // 将第一个参数当作资源目录进行打包
    unsigned int n = PackResource(argv[1], 2 < argc ? argv[2] : nullptr);

    unsigned int t2 = _CurrentMillisecond();

    std::cout << "From \"" << argv[1] << "\" packed " << n << " resource files. spend time: " << t2 - t1 << "ms" << std::endl;
    
    return 0;
}
