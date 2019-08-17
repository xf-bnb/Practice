#ifndef _Resource_Loader_H_
#define _Resource_Loader_H_

#include <set>

#include "ResType.h"
#include "ResData.h"

namespace resource
{
    /*
     *  加载资源
     *  packNmae    - 指定要加载的资源包路径
     *
     *  return：若 packName 指定的资源包路径为有效的资源包文件则返回 true，否则返回 false。
     *
     *  Note：资源包路径支持绝对路径或相对路径，可以使用 '/' 或 '\' 作为路径分隔符，且不区分大小写。
     *        重复加载一个已经成功加载过的资源包将直接返回 true。
     *        在 Windows 系统下，该函数调用成功将导致参数 packName 指定的文件不可被修改和删除，直到调用 FreeResource。
     */
    bool LoadResource(const char* packName);

    /*
     *  释放资源
     *  packNmae    - 指定要加载的资源包路径
     *
     *  return：若 packName 指定的资源包已被加载则返回 true，否则返回 false。
     *
     *  Note：资源包路径支持绝对路径或相对路径，可以使用 '/' 或 '\' 作为路径分隔符，且不区分大小写。
     */
    bool FreeResource(const char* packName);

    /*
     *  获取资源数据
     *  resName     - 指定要获取的资源标识符(相对于原打包目录的路径)
     *  buffer      - 指定要接收资源数据的缓冲区
     *  start       - 指定要获取的资源数据的偏移位置
     *  n           - 指定要获取的资源数据长度
     *
     *  return：版本1：返回参数 resName 标识的资源数据对象，若资源不存在，则返回无效对象，返回值可用作 bool 判断。
     *          版本2：返回成功获取到的字节数，若资源数据不存在或指定的参数无效返回 0。
     *
     */
    ResData      GetResource(const char* resName);
    unsigned int GetResource(const char* resName, void* buffer, unsigned int start, unsigned int n);

    /*
     *  查找资源
     *  resName     - 指定要查找的资源标识符(相对于原打包目录的路径)
     *
     *  return：返回查询到的资源信息，若参数 resName 标识的资源不存在，则返回的对象中资源长度为 0。
     */
    ResInfo FindResource(const char* resName);

    /*
     *  获取资源长度资源
     *  resName     - 指定要查询的资源标识符(相对于原打包目录的路径)
     *
     *  return：返回查询到的资源长度以字节为单位，若参数 resName 标识的资源不存在，则返回 0。
     */
    unsigned int SizeOfResource(const char* resName);

    /*
     *  判定资源是否存在
     *  resName     - 指定要查询的资源标识符(相对于原打包目录的路径)
     *
     *  return：若参数 resName 标识的资源存在返回 true，否则返回 false。
     */
    bool IsExisting(const char* resName);

    /*
     *  获取所有的资源名
     *
     *  return：返回存在的所有资源名标识符集合
     */
    std::set<std::string> ResourceNameList();

}

#endif  // _Resource_Loader_H_
