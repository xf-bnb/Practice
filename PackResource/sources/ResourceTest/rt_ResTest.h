#ifndef _Resource_Test_H_
#define _Resource_Test_H_

// 包装测试
class test_call_t final
{
private:

    // 前置声明
    class _func_trait;

    static const _func_trait _tf_array[];

    test_call_t() = default;
    test_call_t(const test_call_t&) = delete;
    test_call_t& operator = (const test_call_t&) = delete;

public:

    static const test_call_t& instance() { static test_call_t t; return t; }

    bool operator () (const char* arg) const;
    bool operator () (unsigned int index) const;

private:

    // 函数对象
    class _func_trait
    {
    private:

        using _TestTraitFuncT = void(*)();

        _TestTraitFuncT _pfunc;
        const char* _name;

    public:

        _func_trait() : _func_trait(nullptr, nullptr) { }
        _func_trait(_TestTraitFuncT pf, const char* str) : _pfunc(pf), _name(str) { }

        void operator () () const;

        friend bool test_call_t::operator()(const char *) const;
    };

};  // class trait_call_t

#endif  // _Resource_Test_H_
