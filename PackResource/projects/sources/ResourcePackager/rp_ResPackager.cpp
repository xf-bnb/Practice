#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "rp_ResPackager.h"

// 定义打包器版本号
static constexpr unsigned int _PackagerVersion(0x0101);

// 定义单次读取文件的长度
static constexpr unsigned int _OnceReadSize(0x0400);

// 定义文件字节对齐基数
static constexpr unsigned int _AlignNumber(1 << 3);

// 定义资源包头格式分隔符
static constexpr char _Separator('\0');

// 定义资源包文件名默认后缀
static const char* _ResSuffix(".respackage");

// 返回 x 按 n 对齐的值
template<unsigned int n>
inline unsigned int _AlignValue(unsigned int x) { return x + (n - (x % n)); }

template<>
inline unsigned int _AlignValue<0>(unsigned int x) { return x; }

template<>
inline unsigned int _AlignValue<1>(unsigned int x) { return x; }

// 定义文件信息
struct _file_info
{
    const std::filesystem::path _name;  // 文件路径
    const unsigned int _size;           // 文件大小
    const unsigned int _space;          // 占用空间(按8字节对齐)

    _file_info(const std::filesystem::path& name, unsigned int n)
        : _name(name), _size(n), _space(_AlignValue<_AlignNumber>(n))
    { }

    _file_info(const _file_info& fi)
        : _name(fi._name), _size(fi._size), _space(fi._space)
    { }

    // 多出来的空白字节数
    unsigned int SpaceCount() const { return _space - _size; }

    // 禁止赋值
    _file_info& operator=(const _file_info&) = delete;

};

// 生成一个资源包文件名
std::string _MakeResFileName(const std::filesystem::path& dir, const char* output, unsigned int n)
{
    if (nullptr == output)
    {
        time_t seconds = 0;
        if ((seconds = time(nullptr)) < 0)
            return std::string();

        tm t;

#ifdef _MSC_VER

        if (localtime_s(&t, &seconds) != 0)
            return std::string();

#else

        if (localtime_r(&seconds, &t) == nullptr)
            return std::string();

#endif  // _MSC_VER

        char buf[512]{ 0 };
        std::snprintf(buf, 512, "%s-%04d%02d%02d%02d%02d%02d%03u-%d%s"
            , dir.generic_string().c_str()
            , t.tm_year + 1900
            , t.tm_mon + 1
            , t.tm_mday
            , t.tm_hour
            , t.tm_min
            , t.tm_sec
            , _CurrentMillisecond() % 1000
            , n
            , _ResSuffix);

        // example: dir-20180717104132017-20.res
        return std::string(buf);
    }

    std::filesystem::path fn(output, std::filesystem::path::format::generic_format);
    // 如果没有指定文件扩展名则不上默认扩展名
    if (!fn.has_extension())
        fn += _ResSuffix;

    return (dir.parent_path() / fn).generic_string();
}

// 判定目录是否有效
bool IsValidDirectory(const char* dir, std::filesystem::path& outp)
{
    // 检查字符串有效性
    if (nullptr == dir)
        return false;

    std::string str(dir);
    if (str.empty())
        return false;

    if (str.back() == '/' || str.back() == '\\')
        str.pop_back();

    if (str.empty())
        return false;

    std::filesystem::path p(str, std::filesystem::path::format::generic_format);
    // 检查是否有效目录
    std::error_code ec;
    if (!std::filesystem::is_directory(p, ec))
        return false;

    // 绝对路径和父路径相同说明是顶层目录
    p = std::filesystem::absolute(p);
    if (std::filesystem::equivalent(p, p.parent_path()))
        return false;

    outp = p;

    return true;
}

// 将指定文件内容写入到输出流当中
unsigned int WriteResource(const _file_info& fi, FILE* hFout)
{
    unsigned int n = 0;

    // 以二进制只读方式打开文件
#ifdef _MSC_VER
    FILE* hFile = nullptr;
    errno_t result = fopen_s(&hFile, fi._name.generic_string().c_str(), "rb,ccs=UTF-8");
    if (nullptr != hFile && 0 == result)
#else
    FILE* hFile = fopen(fi._name.generic_string().c_str(), "rb");
    if (nullptr != hFile)
#endif
    {
        for ( ; ; )
        {
            unsigned char buf[_OnceReadSize]{ 0 };
            size_t len = fread(buf, 1, _OnceReadSize, hFile);
            if (0 < len)
            {
                // 将读到的字符写入输出流
                fwrite(buf, 1, len, hFout);
                n += len;
            }
            if (len < _OnceReadSize || feof(hFile))
            {
                if (n == fi._size)
                {
                    // 补齐空白
                    char space[_AlignNumber]{ 0 };
                    fwrite(space, 1, fi.SpaceCount(), hFout);
                    n += fi.SpaceCount();
                }
                else
                {
                    // 实际写入数量与应该写入数量不等
                    std::cout << "--> Warning: file \"" << fi._name << "\" write file error(" << n << "/" << fi._size << ") !!!" << std::endl;
                }

                break;
            }
        }

        fclose(hFile);
    }

    return n;
}

/*
 *  资源包文件格式：
 *  模板：版本号[\0][ ... 资源数据 ... ] 文件名[\0]起始位置[\0]文件长度[\0]文件类型[\0] ... [\0]包头长度[\0]总长度
 *  示例：00000101 [ ... resource ... ] filename1 240 1022 0 filename2 1264 2020 0 ... 0000080e 023f478d
 */
unsigned int PackResource(const char * dir, const char* output)
{
    // 判定路径是否有效
    std::filesystem::path p;
    if (!IsValidDirectory(dir, p))
    {
        std::cout << "the path \"" << p << "\" is invalid or is top directory !" << std::endl;
        return 0;
    }
    // 将目录中所有文件名提取到列表
    std::vector<_file_info> fileSet;
    for (auto& iter : std::filesystem::recursive_directory_iterator(p))
    {
        if (iter.is_regular_file() && 0 < iter.file_size())
        {
            fileSet.emplace_back(iter.path(), (unsigned int)(iter.file_size()));
        }
    }
    if (fileSet.empty())
    {
        std::cout << "Don\'t found any resource file from \"" << p << "\" !" << std::endl;
        return 0;
    }

    // 生成资源包输出文件名
    std::string strResFile = _MakeResFileName(p, output, fileSet.size());
    if (strResFile.empty())
    {
        std::cout << "Can\'t make the output resource file path !" << std::endl;
        return 0;
    }

    // 以二进制写入方式打开资源包文件
#ifdef _MSC_VER
    FILE* hFile = nullptr;
    errno_t result = fopen_s(&hFile, strResFile.c_str(), "wb,ccs=UTF-8");
    if (nullptr == hFile || 0 != result)
#else
    FILE* hFile = fopen(strResFile.c_str(), "wb");
    if (nullptr == hFile)
#endif
    {
        std::cout << "Open resource file failed: " << strResFile << std::endl;
        return 0;
    }

    // 先将版本号写入文件头
    char version[16]{ 0 };
    std::snprintf(version, 16, "%08x", _PackagerVersion);
    fwrite(version, 1, 9, hFile);

    unsigned int n = 0, total = 9;

    std::string listText;
    for (const auto& fi : fileSet)
    {
        // 将资源逐个写入资源包中
        unsigned int len = WriteResource(fi, hFile);
        if (0 < len)
        {
            // 实际写入长度和文件占用长度相等则为正常
            unsigned int nFile = (len == fi._space ? fi._size : len);
            // 截取文件相对路径为资源名
            std::string filename = std::filesystem::relative(fi._name, p).generic_string();

            // 组装资源列表描述信息：资源名[\0]开始位置[\0]长度[\0]类型[\0]
            listText.append(filename);
            listText.append(1, _Separator);
            listText.append(std::to_string(total));
            listText.append(1, _Separator);
            listText.append(std::to_string(nFile));
            listText.append(1, _Separator);
            listText.append(std::to_string(0));
            listText.append(1, _Separator);

            ++n;
            // std::cout << n << ": pack file: " << filename << ", pos: " << total << ", size: " << nFile << std::endl;

            total += len;
        }
    }

    if (0 < n)
    {
        // 将资源描述信息和信息长度写入资源包中
        fwrite(listText.c_str(), 1, listText.size(), hFile);

        total += listText.size();
        total += 17;
        unsigned int x = total % _AlignNumber;
        if (0 < x)
            total += (_AlignNumber - x);

        char strText[16]{ 0 };
        if (0 < x)
            fwrite(strText, 1, (_AlignNumber - x), hFile);

        std::snprintf(strText, 16, "%08x", listText.size());
        fwrite(strText, 1, 9, hFile);

        std::snprintf(strText, 16, "%08x", total);
        fwrite(strText, 1, 8, hFile);

        // std::cout << "list text: " << listText << ", size: " << listText.size() << std::endl;
        std::cout << "\nGeneral resource package file: " << strResFile << ", length: " << total << std::endl;
    }

    fclose(hFile);

    return n;
}

unsigned int _CurrentMillisecond()
{
    return (unsigned int)(((double)(clock()) / CLOCKS_PER_SEC) * 1000);
}
