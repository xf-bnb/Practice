#ifndef _Resource_Reader_H_
#define _Resource_Reader_H_

namespace resource
{
    // 自定义的忽略大小写比较操作
    inline int _IgnoreCaseCompare(const char * a, const char * b)
    {
        int x, y;
        
        do {
            if ( ((x = (unsigned char)(*(a++))) >= 'A') && (x <= 'Z') )
                x -= ('A' - 'a');
            if ( ((y = (unsigned char)(*(b++))) >= 'A') && (y <= 'Z') )
                y -= ('A' - 'a');
        } while (x && (x == y));
        
        return(x - y);
    }
    
    inline bool _StringLessThan(const std::string& a, const std::string& b) { return (_IgnoreCaseCompare(a.c_str(), b.c_str()) < 0); }
    inline bool _StringEqual(const std::string& a, const std::string& b) { return (_IgnoreCaseCompare(a.c_str(), b.c_str()) == 0); }

    // 封装比较函数对象类型
    class _str_comp
    {
    public: inline bool operator()(const std::string& a, const std::string& b) const
        {
            return (_StringLessThan(a, b));
        }
    };

    // 资源包读取器
    class ResReader
    {
    private:

        // 单次读取长度
        static constexpr unsigned int _OnceReadSize{ 0x1000 };

        // 资源描述信息
        struct _res_pos
        {
            // 起始位置，长度，类型
            unsigned int _index{ 0 }, _count{ 0 }, _type{ ResType::RT_DATA };
        };

        // 定义资源描述信息映射类型
        using res_map_type = std::map<std::string, _res_pos, _str_comp>;

        FILE* m_hFile;                  // 文件读取流
        std::string m_ResName;          // 资源包名称
        unsigned int m_FileSize;        // 资源大小
        unsigned int m_Version;         // 版本号
        res_map_type m_ResMap;          // 资源文件位置映射
        // res_map_type m_StaticMap;       // 静态缓存位置映射
        // res_map_type m_DynamicMap;      // 动态缓存位置映射
        // mutable std::mutex m_ReadMutex; // 读取锁

        // 禁止复制和移动
        ResReader(const ResReader&) = delete;
        ResReader(ResReader&&) = delete;
        ResReader& operator = (const ResReader&) = delete;
        ResReader& operator = (ResReader&&) = delete;

        // 获取一段指定的资源数据放入缓冲区，返回实际读取的字节数
        unsigned int _ReadData(unsigned char* buffer, unsigned int start, unsigned int n) const;

        // 解析资源描述映射
        static void _ParseResMap(const char* desctext, unsigned int n, res_map_type& resMap);

    public:

        using map_type = res_map_type;

        // 解析资源包文件 resName 并与读取器绑定
        explicit ResReader(const std::string& resName);

        // 使用解析好的数据构造读取器
        ResReader(std::string & resName, FILE * hFile, unsigned int fileSize, unsigned int version, const res_map_type & resMap)
            : m_ResName(resName), m_hFile(hFile), m_FileSize(fileSize), m_Version(version), m_ResMap(resMap)
        { }

        ~ResReader();

        unsigned int GetVersion() const { return m_Version; }
        const std::string& GetFileName() const { return m_ResName; }
        const res_map_type& GetResMap() const { return m_ResMap; }

        // 根据资源标识获取资源
        ResData GetResource(const std::string& key) const;

        // 根据资源标识获取指定资源段的数据放入指定缓冲区，返回实际获取的字节数
        unsigned int GetResource(const std::string& key, unsigned char* buffer, unsigned int start, unsigned int n) const;

        // 获取一段资源数据放入缓冲区，返回实际获取的字节数
        unsigned int ReadData(unsigned char* buffer, unsigned int start, unsigned int n) const;

        // 判定是否存在指定名字的资源
        bool IsExisting(const std::string& key) const;

        // 判定读取器是否有效
        bool IsValid() const;
        inline operator bool() const { return IsValid(); }

        // 加载资源包
        static FILE* Load(const std::string& resFile, unsigned int& fileSize, unsigned int& version, res_map_type& resMap);

    };  // ResReader

}

#endif  // _Resource_Reader_H_
