//
// Created by Peter Xi on 2022/5/14.
//

#ifndef ANDROIDTEST_VARSTR_H
#define ANDROIDTEST_VARSTR_H
#pragma once
#include <iostream>

using namespace std;

namespace JustPackage
{

    struct Varstr
    {
        const char* m_data;
        size_t m_size;

        explicit Varstr(const char* data = "", size_t size = 0) { set(data, size); }
        void set(const char* data, size_t size)
        {
            m_data = data;
            m_size = size;
        }
        bool empty() const { return m_size == 0; }

        const char* data() const { return m_data; }
        size_t size() const { return m_size; }

        template <class T> // std::string cstr blockbuffer
        explicit Varstr(T& s)
        {
            *this = s;
        }

        template <class T> Varstr& operator=(T& s)
        {
            m_data = s.data();
            m_size = s.size();
            return *this;
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const Varstr& vs)
    {
        return os.write(vs.data(), std::streamsize(vs.size()));
    }

} // namespace JustPackage

#endif //ANDROIDTEST_VARSTR_H
