#ifndef LOCALVAR_H_INCLUDED
#define LOCALVAR_H_INCLUDED

#include "gc.h"

extern GarbageCollector& GGC;

template<class T>
class LocalVar {
    static_assert(std::is_base_of<GCObject, T>::value);
public:
    T* _pointer;
public:
    // Default Construct
    LocalVar() : _pointer(nullptr){

    }
    // Construct with a Pointer
    LocalVar(T* pT) : _pointer(pT) {
        GGC.addRoot(_pointer);
    }
    // Copy Construct
    LocalVar(const LocalVar<T>& obj) : _pointer(obj._pointer) {
        // _rvalueLocked = false;
    }
    // Destruct
    ~LocalVar() {
        GGC.removeRoot(_pointer);
    }
public:
    T* operator -> () {
        return _pointer;
    }

};

#endif // LOCALVAR_H_INCLUDED
