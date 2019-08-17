#include <map>
#include <list>
#include <vector>
#include <string>
#include <random>
#include <mutex>
#include <thread>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <iostream>

#include "ResLoader.h"
#include "rt_TestLog.h"
#include "rt_ResTest.h"

// 定义一些常量路径用于测试目的
#ifdef WIN32

// 资源包列表
static const std::filesystem::path g_ResList[]{
    std::filesystem::path(R"(E:/Workspace/Practice/TestPack/res1.respackage)", std::filesystem::path::format::generic_format),
    std::filesystem::path(R"(E:/Workspace/Practice/TestPack/res2.respackage)", std::filesystem::path::format::generic_format),
    std::filesystem::path(R"(E:/Workspace/Practice/TestPack/res3.respackage)", std::filesystem::path::format::generic_format),
    std::filesystem::path(R"(E:/Workspace/Practice/TestPack/res4.respackage)", std::filesystem::path::format::generic_format)
};

// 还原目录位置
static const std::filesystem::path g_RecoverDir(R"(E:/Workspace/Practice/TestPack/recover)", std::filesystem::path::format::generic_format);

// 汇总目录
static const std::filesystem::path g_TotalDir(R"(E:/Workspace/Practice/TestPack/total)", std::filesystem::path::format::generic_format);

#else

static const std::filesystem::path g_ResList[]{
    std::filesystem::path(R"(/Users/Frank/Workspace/TestPack/res1.respackage)", std::filesystem::path::format::generic_format),
    std::filesystem::path(R"(/Users/Frank/Workspace/TestPack/res2.respackage)", std::filesystem::path::format::generic_format),
    std::filesystem::path(R"(/Users/Frank/Workspace/TestPack/res3.respackage)", std::filesystem::path::format::generic_format),
    std::filesystem::path(R"(/Users/Frank/Workspace/TestPack/res4.respackage)", std::filesystem::path::format::generic_format)
};

static const std::filesystem::path g_RecoverDir(R"(/Users/Frank/Workspace/TestPack/recover)", std::filesystem::path::format::generic_format);

static const std::filesystem::path g_TotalDir(R"(/Users/Frank/Workspace/TestPack/total)", std::filesystem::path::format::generic_format);

#endif

// 测试函数声明
//------------------------------------------------------------------------------

// 将资源包中的内容恢复到 g_RecoverDir 目录中
void test_recover();

// 加载所有的资源包，计算从包中读取资源的速度，以及从磁盘中读取资源的速度
void test_compare();

// 加载所有的资源包，逐个释放资源并查看存在的资源列表
void test_release();

// 启用多线程同时读取资源并同时释放资源
void test_thread();

// 测试类实现
void test_call_t::_func_trait::operator () () const
{
    std::cout << "call " << _name << " begin" << std::endl;
    std::cout << "------------------------------------------------" << std::endl << std::endl;

    _pfunc();

    std::cout << std::endl << "------------------------------------------------" << std::endl;
    std::cout << "call " << _name << " end" << std::endl << std::endl;
}

#define _AddTraitFunc(_func) { _func, #_func }

const test_call_t::_func_trait test_call_t::_tf_array[]{
    _AddTraitFunc(test_recover),
    _AddTraitFunc(test_release),
    _AddTraitFunc(test_compare),
    _AddTraitFunc(test_thread)
};

bool test_call_t::operator()(const char * arg) const
{
    for (auto& fx : _tf_array)
    {
        if (0 == strcmp(fx._name, arg))
        {
            fx();
            return true;
        }
    }

    if (0 < strlen(arg) && 0x2f < arg[0] && arg[0] < 0x3a)
    {
        operator()(strtoul(arg, nullptr, 10));
        return true;
    }

    std::cout << "You specified test parameter is invalid !" << std::endl;
    return false;
}

bool test_call_t::operator() (unsigned int index) const
{
    try
    {
        if (index < std::extent<decltype(_tf_array)>::value)
        {
            _tf_array[index]();
            return true;
        }
        
        throw std::out_of_range("not exist the index specified function!");
    }
    catch (std::out_of_range& str)
    {
        std::cout << str.what() << std::endl;
        exit(0);
    }

    return false;
}

/*
 *  一些辅助函数声明和定义
 */

// 加载指定数量的资源包
unsigned int LoadResourcePackage(unsigned int n);

// 加载所有的资源包
unsigned int LoadResourcePackage() { return LoadResourcePackage(std::extent<decltype(g_ResList)>::value); }

// 读取资源函数(用于多线程测试)
unsigned int ReadResourceThread(const std::vector<std::string>& names);

// 释放资源函数(用于多线程测试)
unsigned int FreeResourceThread();

// 打印存在的资源列表
void ShowResource();

// 将资源数据 rd 写入 dir 目录中的 fileName 文件中
bool WriteResource(const std::filesystem::path& dir, const std::string& fileName, const resource::ResData& rd);

// 从文件 fileName 中读取资源数据
resource::ResData ReadFile(const std::string& fileName);

// 遍历指定名字列表中的资源数据应用到 pfunc 行为
template<typename _Ty, typename _FuncT>
unsigned int ForeachResource(const _Ty& names, _FuncT pfunc)
{
    unsigned int n = 0;
    for (const auto& name : names)
    {
        if (pfunc(name, resource::GetResource(name.c_str())))
            ++n;
    }

    return n;
}

// 读取指定名字列表中的文件数据应用到 pfunc 行为
template<typename _Ty, typename _FuncT>
unsigned int ForeachFiles(const _Ty & names, _FuncT pfunc)
{
    unsigned int n = 0;
    for (const auto& name : names)
    {
        if (pfunc(name, ReadFile((g_TotalDir / name).generic_string())))
            ++n;
    }

    return n;
}

// 转换读取速度(单位: bytes/s)
inline unsigned int CalculateSpeed(unsigned int nByte, unsigned int nTime)
{
    if (nTime == 0)
        return 999999999;

    double s = double(nByte) / nTime;

    return (unsigned int)(s * 1000);
}

// 测试函数实现
//------------------------------------------------------------------------------

void test_recover()
{
    // 加载2个资源包
    LoadResourcePackage(2);

    // 删除 g_RecoverDir 目录中所有内容
    std::filesystem::remove_all(g_RecoverDir);

    unsigned int t1 = _CurrentMillisecond();

    // 遍历资源写入到恢复目录中
    unsigned int n = ForeachResource(resource::ResourceNameList(),
        [](const std::string& name, const resource::ResData& rd)
        {
            if (rd.IsValid())
                return WriteResource(g_RecoverDir, name, rd);

            std::cout << "--> Get resource from package: " << name << " failed !" << std::endl;
            return false;
        }
    );

    unsigned int t2 = _CurrentMillisecond();

    std::cout << "Successfully write " << n << " files, spend " << t2 - t1 << "ms." << std::endl;
}

void test_compare()
{
    // 加载所有资源包
    LoadResourcePackage();

    // 获取资源列表
    auto names = resource::ResourceNameList();

    // 标记读取的字节数
    unsigned int nByte1 = 0, nByte2 = 0;

    unsigned int t1 = _CurrentMillisecond();

    // 从包中遍历读取资源
    unsigned int nRes1 = ForeachResource(names,
        [&nByte1](const std::string& name, const resource::ResData& rd) mutable
        {
            if (rd.IsValid())
            {
                nByte1 += rd.Size();
                return true;
            }

            std::cout << "--> Get resource from package: " << name << " failed !" << std::endl;
            return false;
        }
    );

    unsigned int t2 = _CurrentMillisecond();

    // 从磁盘遍历读取资源
    unsigned int nRes2 = ForeachFiles(names,
        [&nByte2](const std::string& name, const resource::ResData& rd) mutable
        {
            if (rd.IsValid())
            {
                nByte2 += rd.Size();
                return true;
            }

            std::cout << "--> Get resource from device: " << name << " failed !" << std::endl;
            return false;
        }
    );

    unsigned int t3 = _CurrentMillisecond();

    // 计算用时和速度
    unsigned int nTime1 = t2 - t1, nTime2 = t3 - t2;
    unsigned int nSpeed1 = CalculateSpeed(nByte1, nTime1);
    unsigned int nSpeed2 = CalculateSpeed(nByte2, nTime2);

    std::cout << "From package get resource: " << nRes1 << ", bytes: " << nByte1 << ", time: " << nTime1 << " ms, speed: " << nSpeed1 << " bytes/s" << std::endl;
    std::cout << "From device get resource: "  << nRes2 << ", bytes: " << nByte2 << ", time: " << nTime2 << " ms, speed: " << nSpeed2 << " bytes/s" << std::endl;
}

void test_release()
{
    // 加载所有资源包
    for (auto res : g_ResList)
    {
        bool result = resource::LoadResource(res.generic_string().c_str());
        std::cout << "load resource: " << res << " " << (result ? "true" : "false") << std::endl;

        ShowResource();

        getchar();
    }

    std::cout << "load resource finished, begin to release resource, press any key continue ..." << std::endl;

    for (auto res : g_ResList)
    {
        getchar();

        // 逐个释放资源进行查看
        bool result = resource::FreeResource(res.generic_string().c_str());
        std::cout << "release resource: " << res << " " << (result ? "true" : "false") << std::endl;

        ShowResource();
    }
}

void test_thread()
{
    // 加载所有资源包
    LoadResourcePackage();

    // 获取资源列表
    auto names = resource::ResourceNameList();

    std::vector<std::string> vtrs1(names.begin(), names.end());
    std::vector<std::string> vtrs2(vtrs1.size(), vtrs1.front());
    std::vector<std::string> vtrs3(vtrs1.size(), vtrs1.back());
    std::vector<std::string> vtrs4(vtrs2);
    std::vector<std::string> vtrs5(vtrs3);
    std::vector<std::string> vtrs6(vtrs1);
    std::shuffle(vtrs6.begin(), vtrs6.end(), std::mt19937(std::random_device()()));

    std::vector<std::thread> vtrThread;

    // 6个读取资源的线程
    vtrThread.emplace_back(std::thread(ReadResourceThread, std::ref(vtrs1)));
    vtrThread.emplace_back(std::thread(ReadResourceThread, std::ref(vtrs2)));
    vtrThread.emplace_back(std::thread(ReadResourceThread, std::ref(vtrs3)));
    vtrThread.emplace_back(std::thread(ReadResourceThread, std::ref(vtrs4)));
    vtrThread.emplace_back(std::thread(ReadResourceThread, std::ref(vtrs5)));
    vtrThread.emplace_back(std::thread(ReadResourceThread, std::ref(vtrs6)));

    // 1个释放资源的线程
    vtrThread.emplace_back(std::thread(FreeResourceThread));

    // 依次调用 join 函数等待所有线程执行完成
    for (std::thread& t : vtrThread)
        t.join();
}

// 辅助函数实现
//------------------------------------------------------------------------------

unsigned int LoadResourcePackage(unsigned int n)
{
    unsigned int result = 0;

    if (0 < n && n <= std::extent<decltype(g_ResList)>::value)
    {
        // 按个数遍历加载资源包
        for (unsigned int i = 0; i < n; ++i)
        {
            if (resource::LoadResource(g_ResList[i].generic_string().c_str()))
            {
                std::cout << "Successfully load resource: " << g_ResList[i] << std::endl;
                ++result;
            }
            else
            {
                std::cout << "--> Failed to load resource: " << g_ResList[i] << std::endl;
            }
        }
    }

    return result;
}

unsigned int ReadResourceThread(const std::vector<std::string>& names)
{
    return ForeachResource(names,
        [](const std::string& name, const resource::ResData& rd)
        {
            bool bValid = (bool)rd;

            _log("Get resource: %s, result: %s", name.c_str(), (bValid ? "true" : "false"));

            return bValid;
        }
    );
}

unsigned int FreeResourceThread()
{
    unsigned int n = 0;

    for (auto res : g_ResList)
    {
        // 休眠 256ms
        std::this_thread::sleep_for(std::chrono::milliseconds(1 << 8));

        if (resource::FreeResource(res.generic_string().c_str()))
        {
            _log("Release resource: %s, result: true", res.generic_string().c_str());

            ++n;
            // ShowResource();
        }
        else
        {
            _log("Release resource: %s, result: false", res.c_str());
        }
    }

    return n;
}

resource::ResData ReadFile(const std::string& fileName)
{
    std::ifstream fin;
    fin.open(fileName, std::ios::binary | std::ios::in);
    if (fin.is_open())
    {
        fin.seekg(0, std::ios::end);
        unsigned int n = (unsigned int)(fin.tellg());
        fin.seekg(0, std::ios::beg);

        unsigned char* ptr = new unsigned char[n];

        for (unsigned int i = 0; ; )
        {
            // 分批次读数据到分配的缓冲区
            if ((4096 + i) < n)
            {
                fin.read((char*)ptr + i, 4096);
                i += 4096;
            }
            else
            {
                fin.read((char*)ptr + i, n - i);
                break;
            }
        }

        fin.close();

        return resource::ResData(ptr, n, 0, true);
    }

    return resource::ResData();
}

bool WriteResource(const std::filesystem::path& dir, const std::string& resName, const resource::ResData& rd)
{
    if (!rd.IsValid())
        return false;

    std::filesystem::path target(resName, std::filesystem::path::format::generic_format);

    // 递归创建目录
    if (target.has_parent_path())
        std::filesystem::create_directories(dir / target.parent_path());
    else
        std::filesystem::create_directories(dir);

    std::string filename((dir / target).generic_string());

    std::ofstream fout;
    fout.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if (fout.is_open())
    {
        fout.write(reinterpret_cast<const char*>(rd.Data()), rd.Size());

        fout.close();

        std::cout << "Successfully write file: " << filename << std::endl;
        return true;
    }

    std::cout << "Failed to write file: " << filename << std::endl;
    return false;
}

void ShowResource()
{
    std::cout << std::endl;

    auto names = resource::ResourceNameList();
    for (auto name : names)
    {
        resource::ResInfo ri = resource::FindResource(name.c_str());
        std::cout << "file: " << name << ", size: " << ri.Size() << std::endl;
    }

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "has resource: " << names.size() << std::endl;
}
