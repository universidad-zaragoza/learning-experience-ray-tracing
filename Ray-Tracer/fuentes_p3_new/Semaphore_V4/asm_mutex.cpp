#include "asm_mutex.hpp"

extern "C" void armv8_mutex_lock(int *x);
extern "C" void armv8_mutex_unlock(int *x);
extern "C" void armv8_mutex_sleep_lock(int *x);
extern "C" void armv8_mutex_sleep_unlock(int *x);

template<>
void asm_mutex<asm_mutex_impl::spin>::lock() { armv8_mutex_lock(&this->_acquired); }

template<>
void asm_mutex<asm_mutex_impl::spin>::unlock() { armv8_mutex_unlock(&this->_acquired); }

template<>
void asm_mutex<asm_mutex_impl::sleep>::lock() { armv8_mutex_sleep_lock(&this->_acquired); }

template<>
void asm_mutex<asm_mutex_impl::sleep>::unlock() { armv8_mutex_sleep_unlock(&this->_acquired); }

