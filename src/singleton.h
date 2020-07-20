/**
 * @file singleton.h
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief A handy singleton library
 * @version 0.3
 * @date 2020-07-20
 * 
 * @copyright Copyright (c) 2020
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cassert>
#include <mutex>

/**
 * @brief A handy macro that defines a private constructor and
 *          give access to the corresponding singleton function
 * 
 * @warning This macro has a private access modifier inside
 */
#define SINGLETON_CONTRUCTOR(class_name, ...) \
    friend class_name* Instance(__VA_ARGS__); \
 private: \
    class_name(__VA_ARGS__)

/**
 * @brief Wrapper class for the instance to support contruct-on-need
 * 
 * @tparam Type The class type
 */
template <typename Type>
struct InstanceWrapper {
    bool initialized;
    Type* raw_pointer;

    Type* operator->() {
        assert(initialized == true);
        return raw_pointer;
    }
    const Type* operator->() const {
        assert(initialized = true);
        return raw_pointer;
    }

    operator Type*() {
        assert(initialized = true);
        return raw_pointer;
    }
};

/**
 * @brief Class that actually stores the instance and the access mutex
 * 
 * @tparam Type The type of the class
 */
template <typename Type>
struct InstanceSafetyHelper {
    InstanceWrapper<Type> wrapper;
    std::mutex mutex;

    static InstanceSafetyHelper* Helper() {
        static InstanceSafetyHelper<Type> helper;
        return &helper;
    }
    InstanceSafetyHelper() : wrapper{false, nullptr}, mutex() {}
    ~InstanceSafetyHelper() {
        wrapper.raw_pointer->~Type();
        free(wrapper.raw_pointer);
    }
};

/**
 * @brief Get the instance of the indicate type, construct one when not
 * constructed (thread safe)
 *
 * @tparam Type The type of the instance
 * @tparam ConstructParameters Parameters used in construction
 * @param args Parameters used in construction
 * @return Type* The instance
 */
template <typename Type, typename... ConstructParameters>
Type* Instance(ConstructParameters... args) {
    InstanceSafetyHelper<Type>* helper = InstanceSafetyHelper<Type>::Helper();
    if (helper->wrapper.raw_pointer == nullptr) {
        helper->mutex.lock();
        if (helper->wrapper.raw_pointer == nullptr) {
            void* data = malloc(sizeof(Type));
            helper->wrapper.raw_pointer =
                reinterpret_cast<Type*>(data);
            helper->wrapper.raw_pointer = new (data) Type(args...);
            helper->wrapper.initialized = true;
        }
        helper->mutex.unlock();
    }
    return helper->wrapper;
}
/**
 * @brief Get the Instance with the indicated type after asserting its existance
 *
 * @tparam Type The type of the instance
 * @return InstanceWrapper<Type>* The instance
 */
template <typename Type>
InstanceWrapper<Type> getInstance(void) {
    InstanceSafetyHelper<Type>* helper = InstanceSafetyHelper<Type>::Helper();
    assert(helper->wrapper.raw_pointer != nullptr);
    return helper->wrapper;
}
