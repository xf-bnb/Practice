#ifndef _Resource_Data_H_
#define _Resource_Data_H_

namespace resource
{

    // 资源信息
    struct ResInfo
    {
    protected:

        const unsigned int resSize;         // 资源长度
        const unsigned int resType;         // 资源类型

    public:

        // 委托构造
        ResInfo() : ResInfo(0, ResType::RT_DATA) { }

        // 标准构造
        ResInfo(unsigned int n, unsigned int t) : resSize(n), resType(t) { }

        unsigned int Size() const { return resSize; }
        unsigned int Type() const { return resType; }

    };  // ResInfo

    // 资源数据
    class ResData : public ResInfo
    {
    private:

        const unsigned char* resData;       // 资源数据
        bool dynamicRes;                    // 是否需要释放

        // 禁用拷贝
        ResData(const ResData&) = delete;
        ResData& operator=(const ResData&) = delete;

    public:

        // 默认构造
        ResData() : ResInfo(), resData(nullptr), dynamicRes(false)
        { }

        // 标准构造
        ResData(const unsigned char* ptr, unsigned int n, unsigned int t, bool bDynamic)
            : ResInfo(n, t), resData(ptr), dynamicRes(bDynamic)
        { }

        // 移动构造
        ResData(ResData&& ri)
            : ResInfo(ri.resSize, ri.resType), resData(ri.resData), dynamicRes(ri.dynamicRes)
        {
            ri.resData = nullptr;
            ri.dynamicRes = false;
        }

        /*
        // 移动赋值
        ResData& operator=(ResData&& ri)
        {
            if (this != &ri)
            {
                if (dynamicRes && resData != nullptr)
                    delete[] resData;

                resData = ri.resData;
                resSize = ri.resSize;
                resType = ri.resType;
                dynamicRes = ri.dynamicRes;

                ri.resData = nullptr;
                ri.dynamicRes = false;
            }

            return *this;
        }
        */

        // 析构
        ~ResData()
        {
            if (dynamicRes && resData != nullptr)
                delete[] resData;
        }

        const unsigned char* const Data() const { return resData; }

        bool IsValid() const { return (resData != nullptr && 0 < resSize); }
        operator bool() const { return IsValid(); }

    };  // ResData

}

#endif  // _Resource_Data_H_
