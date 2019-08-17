
#include <map>
#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>

#include "ResType.h"
#include "ResData.h"
#include "ResReader.h"

namespace resource
{

    ResReader::ResReader(const std::string& resName)
        : m_ResName(resName)
    {
        m_hFile = Load(m_ResName, m_FileSize, m_Version, m_ResMap);
        if (nullptr == m_hFile)
        {
            m_Version = 0;
            m_FileSize = 0;
            m_ResMap.clear();
        }
    }

    ResReader::~ResReader()
    {
        if (nullptr != m_hFile)
            fclose(m_hFile);
    }

    ResData ResReader::GetResource(const std::string& key) const
    {
        if (nullptr != m_hFile)
        {
            // 查找资源记录
            auto iter = m_ResMap.find(key);
            if (iter != m_ResMap.cend())
            {
                // 分配空间
                unsigned char* ptr = new unsigned char[iter->second._count];
                // 读取资源
                unsigned int n = _ReadData(ptr, iter->second._index, iter->second._count);

                // 构造返回资源信息
                return { ptr, n, iter->second._type, true };
            }
        }

        return ResData();
    }

    unsigned int ResReader::GetResource(const std::string& key, unsigned char* buffer, unsigned int start, unsigned int n) const
    {
        if (nullptr != buffer && 0 < n && nullptr != m_hFile)
        {
            // 查找资源记录
            auto iter = m_ResMap.find(key);
            if (iter != m_ResMap.cend())
            {
                if (start < iter->second._count)
                {
                    unsigned int nMax = iter->second._count - start;
                    start += iter->second._index;
                    if (nMax < n)
                        n = nMax;

                    return _ReadData(buffer, start, n);
                }
            }
        }

        return 0;
    }

    unsigned int ResReader::ReadData(unsigned char * buffer, unsigned int start, unsigned int n) const
    {
        if (nullptr != nullptr && 0 < n && start < m_FileSize)
        {
            if (nullptr != m_hFile)
            {
                return _ReadData(buffer, start, n);
            }
        }

        return 0;
    }

    bool ResReader::IsExisting(const std::string& key) const
    {
        return m_ResMap.find(key) != m_ResMap.cend();
    }

    bool ResReader::IsValid() const
    {
        return nullptr != m_hFile && 0 < m_FileSize && !m_ResMap.empty() && !m_ResName.empty();
    }

    unsigned int ResReader::_ReadData(unsigned char* buffer, unsigned int start, unsigned int n) const
    {
        // 数据读锁
        // std::lock_guard<std::mutex> _autolock(m_ReadMutex);
        // 移动读位置到指定值
        fseek(m_hFile, start, SEEK_SET);

        unsigned int nResult = 0;

        for (unsigned int i = 0; ; )
        {
            // 分批次读数据到分配的缓冲区
            if ((_OnceReadSize + i) < n)
            {
                nResult += fread(buffer + i, 1, _OnceReadSize, m_hFile);
                i += _OnceReadSize;
            }
            else
            {
                nResult += fread(buffer + i, 1, n - i, m_hFile);
                break;
            }
        }

        return nResult;
    }

    void ResReader::_ParseResMap(const char* start, unsigned int n, res_map_type& resMap)
    {
        for (const char* const endptr = start + n; start != endptr; )
        {
            // 资源名开始位置
            const char* ptr1 = std::find(start, endptr, 0);
            if (ptr1 == endptr)
                break;
            // 资源位置记号开始位置
            const char* ptr2 = std::find(++ptr1, endptr, 0);
            if (ptr2 == endptr)
                break;
            // 资源长度开始位置
            const char* ptr3 = std::find(++ptr2, endptr, 0);
            if (ptr3 == endptr)
                break;
            // 资源类型开始位置
            const char* ptr4 = std::find(++ptr3, endptr, 0);
            if (ptr4 == endptr)
                break;

            // 构造资源描述信息
            std::string name(start, ptr1 - start - 1);
            unsigned int index = strtoul(ptr1, nullptr, 10);
            unsigned int count = strtoul(ptr2, nullptr, 10);
            unsigned int type = strtoul(ptr3, nullptr, 10);

            if (!name.empty() && 0 < count)
                resMap.emplace(std::pair<std::string, _res_pos>{ name, { index, count, type }});

            start = ptr4 + 1;
        }
    }

    FILE * ResReader::Load(const std::string& resFile, unsigned int& fileSize, unsigned int& version, res_map_type & resMap)
    {
#ifdef _MSC_VER
        FILE* hFile = nullptr;
        errno_t result = fopen_s(&hFile, resFile.c_str(), "rb,ccs=UTF-8");
        if (nullptr == hFile || 0 != result)
#else
        FILE* hFile = fopen(resFile.c_str(), "rb");
        if (nullptr == hFile)
#endif
        {
            std::cout << "open file error !" << std::endl;
            return nullptr;
        }

        // 获取文件长度
        fseek(hFile, 0, SEEK_END);
        unsigned int endpos = ftell(hFile);
        if (endpos < 32)
        {
            std::cout << "file too small, less than 32 byte !" << std::endl;
            fclose(hFile);
            return nullptr;
        }
        
        char str[32]{ 0 };
        
        // 移动到开头读取版本号
        fseek(hFile, 0, SEEK_SET);
        fread(str, 1, 8, hFile);
        version = strtoul(str, nullptr, 16);
        
        // 移动到末尾前17个字节以读取总长度和描述格式长度
        fseek(hFile, -17, SEEK_END);
        fread(str, 1, 17, hFile);
        unsigned int nTotal = strtoul(str + 9, nullptr, 16);
        unsigned int nText = strtoul(str, nullptr, 16);
        
        if (nTotal != endpos)
        {
            std::cout << "file size error: " << nTotal << "/" << endpos << std::endl;
            fclose(hFile);
            return nullptr;
        }
        if (0 == nText)
        {
            std::cout << "don\'t found resource name list !" << std::endl;
            fclose(hFile);
            return nullptr;
        }

        fileSize = nTotal;
        
        // 将资源描述信息读出来
        std::string desctext(nText + 1, 0);
        
        unsigned int x = (nText + 26) % 8;
        fseek(hFile, endpos - 17 - (0 < x ? nText + (8 - x) : nText), SEEK_SET);
        fread(const_cast<char*>(desctext.data()), 1, nText, hFile);
        
        // 解析资源文件映射
        _ParseResMap(desctext.c_str(), nText, resMap);
        
        if (resMap.empty())
        {
            std::cout << "don\'t found any resources from: " << resFile.c_str() << std::endl;
            fclose(hFile);
            return nullptr;
        }

        return hFile;
    }

}
