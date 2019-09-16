#include "NET_Core.hpp"
#include "NET_Native.hpp"

int main(int argc, char const *argv[])
{
    NET::CLog::initLog(argv[0]);

    NET::CMain::getInstance()->start(); 

    return 0;
}
