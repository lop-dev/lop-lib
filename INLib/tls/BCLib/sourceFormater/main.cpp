#include "codeChecker.h"


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s config mode\n\n", argv[0]);
    }

    std::string cfg = argv[1];
    int mode = atoi(argv[2]);

    CCodeChecker ck;

    if(!ck.initCfg(cfg))
    {
        return -1;
    }

    if(!ck.check(mode))
    {
        return -1;
    }

    return 0;
}

