
#include <map>
#include <list>
#include <mutex>
#include <algorithm>
#include <filesystem>

#include "ResLoader.h"
#include "ResReader.h"

namespace resource
{

    // 资源加载器(单例模式)
    class ResLoader
    {
    private:

        std::list<ResReader*> _ResList;     // 资源读取器列表
        mutable std::mutex _ResMutex;       // 线程锁

        // 默认构造
        ResLoader() = default;

        // 禁止复制和移动
        ResLoader(const ResLoader&) = delete;
        ResLoader(ResLoader&&) = delete;
        ResLoader& operator = (const ResLoader&) = delete;
        ResLoader& operator = (ResLoader&&) = delete;

        static std::string _MakeKey(const char* key)
        {
            std::string str(key);

            std::replace(str.begin(), str.end(), '\\', '/');

            return str;
        }

        // 查找读取器
        const ResReader* FindReader(const std::string& key) const
        {
            // 反向查找
            for (auto iter = _ResList.rbegin(); iter != _ResList.rend(); ++iter)
                if (_StringEqual((*iter)->GetFileName(), key))
                    return (*iter);

            return nullptr;
        }

    public:

        // 唯一公开的获取实例接口
        static ResLoader& GetInstance() { static ResLoader _ResInst; return (_ResInst); }

        // 析构
        ~ResLoader()
        {
            for (const auto& rr : _ResList)
                delete rr;
        }

        // 加载资源
        bool LoadResource(const char* packName)
        {
            if (nullptr != packName)
            {
                std::filesystem::path pfile(packName);
                // 判定是否常规文件
                if (std::filesystem::is_regular_file(pfile))
                {
                    std::string key = std::filesystem::absolute(pfile).generic_string();

                    std::lock_guard<std::mutex> _autoloack(_ResMutex);

                    // 先检查该资源包是否已经加载
                    if (nullptr != FindReader(key))
                        return true;

                    ResReader::map_type resMap;
                    unsigned int fileSize = 0, version = 0;

                    // 加载资源包
                    FILE* hFile = ResReader::Load(key, fileSize, version, resMap);

                    if (nullptr != hFile)
                    {
                        // 使用成功加载的数据构造读取器
                        _ResList.push_back(new ResReader(key, hFile, fileSize, version, resMap));

                        return true;
                    }
                }
            }

            return false;
        }

        // 释放资源
        bool FreeResource(const char* packName)
        {
            if (nullptr != packName)
            {
                std::filesystem::path pfile(packName);
                if (std::filesystem::is_regular_file(pfile))
                {
                    std::string key = std::filesystem::absolute(pfile).generic_string();

                    std::lock_guard<std::mutex> _autoloack(_ResMutex);

                    for (auto iter = _ResList.begin(); iter != _ResList.end(); ++iter)
                    {
                        // 遍历查找路径名匹配的资源位置
                        if (_StringEqual((*iter)->GetFileName(), key))
                        {
                            // 释放该资源
                            delete *iter;
                            // 从列表中剔除
                            _ResList.erase(iter);

                            return true;
                        }
                    }
                }
            }

            return false;
        }

        ResData GetResource(const char* resName) const
        {
            if (nullptr != resName)
            {
                std::string key = _MakeKey(resName);

                std::lock_guard<std::mutex> _autoloack(_ResMutex);

                for (auto iter = _ResList.rbegin(); iter != _ResList.rend(); ++iter)
                {
                    ResData rd = (*iter)->GetResource(key);
                    if (rd.IsValid())
                        return rd;
                }
            }

            return ResData();
        }

        unsigned int GetResource(const char* resName, void* buffer, unsigned int start, unsigned int n) const
        {
            if (nullptr != resName && nullptr != buffer && 0 < n)
            {
                std::string key = _MakeKey(resName);

                std::lock_guard<std::mutex> _autoloack(_ResMutex);

                for (auto iter = _ResList.rbegin(); iter != _ResList.rend(); ++iter)
                {
                    // 找到目标资源位置
                    auto value = (*iter)->GetResMap().find(key);
                    if (value != (*iter)->GetResMap().cend())
                    {
                        if (start < value->second._count)
                        {
                            unsigned int nMax = value->second._count - start;
                            start += value->second._index;
                            if (nMax < n)
                                n = nMax;

                            return (*iter)->ReadData(static_cast<unsigned char*>(buffer), start, n);
                        }

                        return 0;
                    }
                }
            }

            return 0;
        }

        ResInfo FindResource(const char* resName) const
        {
            if (nullptr != resName)
            {
                std::string key = _MakeKey(resName);

                std::lock_guard<std::mutex> _autoloack(_ResMutex);

                // 反向遍历
                for (auto iter = _ResList.rbegin(); iter != _ResList.rend(); ++iter)
                {
                    auto value = (*iter)->GetResMap().find(key);
                    if (value != (*iter)->GetResMap().cend())
                    {
                        return { value->second._count, value->second._type };
                    }
                }
            }

            return ResInfo();
        }

        std::set<std::string> NameList() const
        {
            std::set<std::string> keys;

            std::lock_guard<std::mutex> _autoloack(_ResMutex);

            for (const auto& rr : _ResList)
                for (const auto& value : rr->GetResMap())
                    keys.insert(value.first);

            return keys;
        }

        bool IsExisting(const char* resName) const
        {
            if (nullptr != resName)
            {
                std::string key = _MakeKey(resName);

                std::lock_guard<std::mutex> _autoloack(_ResMutex);

                for (const auto& rr : _ResList)
                    if (rr->IsExisting(key))
                        return true;
            }

            return false;
        }

    };  // class ResLoader

    bool LoadResource(const char * packName)
    {
        return ResLoader::GetInstance().LoadResource(packName);
    }

    bool FreeResource(const char * packName)
    {
        return ResLoader::GetInstance().FreeResource(packName);
    }

    unsigned int GetResource(const char * resName, void * buffer, unsigned int start, unsigned int n)
    {
        return ResLoader::GetInstance().GetResource(resName, buffer, start, n);
    }

    ResData GetResource(const char * resName)
    {
        return ResLoader::GetInstance().GetResource(resName);
    }

    ResInfo FindResource(const char * resName)
    {
        return ResLoader::GetInstance().FindResource(resName);
    }

    unsigned int SizeOfResource(const char * resName)
    {
        ResInfo ri = FindResource(resName);
        return ri.Size();
    }

    std::set<std::string> ResourceNameList()
    {
        return ResLoader::GetInstance().NameList();
    }

    bool IsExisting(const char * resName)
    {
        return ResLoader::GetInstance().IsExisting(resName);
    }

}   // namespace resource
