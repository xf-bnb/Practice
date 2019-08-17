#ifndef _Resource_Test_Log_H_
#define _Resource_Test_Log_H_

// 简单的日志类(单例模式)
class TestLog
{
private:

    constexpr static unsigned int _MaxLog{ 1024 };

    mutable std::mutex _LogMutex;

    // 默认构造
    TestLog() = default;

    // 禁止复制和移动
    TestLog(const TestLog&) = delete;
    TestLog(TestLog&&) = delete;
    TestLog& operator = (const TestLog&) = delete;
    TestLog& operator = (TestLog&&) = delete;

    // 输出日志
    bool _output(const char * extra, const char * text) const;

public:

    // 支持多线程的打印函数
    bool operator()(const char * format, ...) const;

    // 唯一公开的获取实例接口
    static const TestLog& GetInstance() { static TestLog _ResInst; return (_ResInst); }

};

// 获取当前毫秒数
inline unsigned int _CurrentMillisecond()
{
    return (unsigned int)(((double)(clock()) / CLOCKS_PER_SEC) * 1000);
}

#define _log    TestLog::GetInstance()

#endif  // _Resource_Test_Log_H_
