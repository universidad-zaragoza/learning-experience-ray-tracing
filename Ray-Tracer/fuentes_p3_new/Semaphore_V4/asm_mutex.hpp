#pragma once

enum asm_mutex_impl {spin, sleep};

template<asm_mutex_impl impl>
class asm_mutex
{
    private:
        int _acquired; // 1 represents mutex hold
    public:
        asm_mutex() : _acquired(0) {};
        ~asm_mutex() {};
        void lock();
        void unlock();
};
