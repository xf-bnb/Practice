#ifndef _Resource_Type_H_
#define _Resource_Type_H_

namespace resource
{

    // 定义资源类型
    enum ResType
    {
        RT_DATA,        // 二进制数据
        RT_TEXT,        // 文本
        RT_IMAGE,       // 图片
        RT_AUDIO,       // 音频
        RT_VIDEO,       // 视频

        RT_MAX
    };  // enum ResType

}

#endif  // _Resource_Type_H_
