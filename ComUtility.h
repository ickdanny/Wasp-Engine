#pragma once

namespace comadapter {

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