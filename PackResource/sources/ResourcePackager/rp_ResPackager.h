#ifndef _Resource_Packager_H_
#define _Resource_Packager_H_

/**
 *  将指定文件夹中的资源打包写入目标资源包文件中
 *  dir     - 指定文件夹路径
 *  output  - 指定输出资源包路径
 *  返回成功打包的资源文件数量
 */
unsigned int PackResource(const char* dir, const char* output);

// 获取当前毫秒数
unsigned int _CurrentMillisecond();

#endif  // _Resource_Packager_H_
