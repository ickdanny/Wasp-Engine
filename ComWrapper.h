#pragma once

namespace comwrapper {

    template <class T> 
    void safeRelease(T** ppT)
    {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }
}