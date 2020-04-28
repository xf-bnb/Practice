
#include <array>
#include <vector>
#include <string>
#include <algorithm>

#include "gm_resource.h"
#include "gm_manager.h"
#include "gm_view.h"

int main()
{
    // 初始化资源
    Resource::GetInstance().Init();

    // 初始化管理者
    Manager::GetInstance().Init();

    // 创建视图
    View view;
    view.Show();

    return 0;
}
