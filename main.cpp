#include "main.h"
#include "LocalVar.hpp"

struct A : public GCObject {
private:
    int _value;

public:
    A* _llink = nullptr;
    A* _rlink = nullptr;

    virtual void markChildren() {
        if(_llink!=nullptr) _llink->mark();
        if(_rlink!=nullptr) _rlink->mark();
    }

    /// CTOR / ASSIGN / DTOR
    A() { // Default Construct
    }
    A(int v) {
        _value = v;
    }
    A(const A& a) { // Copy Construct
        _value = a._value;
    }

    A& operator = (const A& a) {
        _value = a._value;
        _llink = a._llink;
        _rlink = a._rlink;
        return *this;
    }

    virtual ~A() { // Destruct
        printf("A cleared, value=%d\n", _value);
    }
};

LocalVar<A> GetAValue() {
    LocalVar<A> a(new A(111));
    a->_llink = new A(222);
    a->_rlink = new A(333);

    LocalVar<A> b(new A(444));
    b->_llink = new A(555);

    return a;
}

int main() {
    LocalVar<A> a{GetAValue()};

    printf("gc collect...\n");
    GGC.collect(true);

    printf("over");
    return 0;
}
