﻿//=============================================================================
// 文件：gm_Main.c
// 描述：定义主函数
//=============================================================================

#include "gm_Include.h"


//-----------------------------------------------------------------------------

int main( int _Argc, char ** _Argv )
{
    Init();     // 初始化

    Loop();     // 命令循环

    Exit();     // 退出

    return 0;
}
