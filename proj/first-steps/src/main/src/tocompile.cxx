/************************************************************
 *
 * tocompile.cxx
 *
 * A test file to compile with llvm tools
 *
 * Copyright (c) 2017 Peter Saunderson <peteasa@gmail.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *************************************************************/

#include <iostream>
#include <typeinfo>

#define TESTRES(a, b)     \
    ({                    \
        int c;            \
        if ((a) < (b))    \
        {                 \
            c = a;        \
        } else {          \
            c = b;        \
        };                \
        c;                \
    })

namespace Compiler
{
    enum EnumA
        {
            A,
            B,
        };

    struct StructA
    {
        int a;
    };

    /**
       Used to demonstrate inheritance
    */
    class BaseClass
    {
    public:
        /**
           Base class constructor

           @param a  an integer stored in the base class
           @see Derived1 Derived2 HasA
        */
        BaseClass(int a);

        /**
           virtual to allow derived classes to do proper cleanup
           even when deleted via the BaseClass
        */
        virtual ~BaseClass();

        /**
           @return one of the values stored depending on the derived class
        */
        virtual int Store();

        /**
           @return the protected value stored
        */
        virtual int Storepb() const;

        /**
           Example of a pure virtual method
        */
        virtual int PVert() = 0;
    protected:
        /**
           protected member that can be accessed from the derived classes
        */
        int storepb;
    private:
        int storeb;
    };

    class Derived1: public BaseClass
    {
    public:
        /**
           Derived1 constructor

           @param a  an integer stored in a member of Derived1
           @param b  an integer stored in the BaseClass
        */
        Derived1(int a, int b);
        ~Derived1();

        /**
           @return the Derived1 member store1
        */
        virtual int Store();

        /**
           @return the value stored in the BaseClass
        */
        virtual int Storeb();

        /**
           @return BaseClass protected member
        */
        virtual int Storepb() const;
        virtual int PVert();
        virtual int RtnConst() const;
    private:
        int store1;
        int store22;
    };

    class Derived2: public BaseClass
    {
    public:
        Derived2(int a);
        ~Derived2();

        virtual int Store();
        virtual void Update2(int s2);
        virtual int Store2();
        virtual int PVert() { return 0; };
    private:
        int store2;
    };

    /**
       Adapter class for classes derived from BaseClass.  Provides access to
       common methods.
    */
    class HasA
    {
    public:
        HasA(BaseClass& b);
        ~HasA();

        /**
           @return one of the values stored see classes derived from BaseClass
        */
        int Store();

        /**
           @return the value stored in the BaseClass
        */
        int Storeb();
    private:
        BaseClass& base;
    };

    /**
       Generate Fibonacci numbers at compile time. Fibonacci numbers are define by
       @f[ f(n) = \begin{cases}
       0, \qquad\qquad\quad\quad n=0; \cr
       1, \qquad\qquad\quad\quad n=1; \cr
       f_{n-2} + f_{n-1}, \quad n \textgreater 2;
       \end{cases} @f]
       CppLanguage::Fib<1> and CppLanguage::Fib<2> provide the base for the
       compile time calculations. CppLanguage::Fib<N> extends these for larger @f$ N @f$.
    */
    template<int N> struct Fib
    {
        enum { val = Fib<N-1>::val + Fib<N-2>::val };
    };

    template<> struct Fib<1>
    {
        enum { val = 1 };
    };

    template<> struct Fib<2>
    {
        enum { val = 2 };
    };

}

namespace Compiler
{

    /**
       Prints the name of the class to demonstrate when the
       base class is created relative to the derived class.
       Stores the value provided in both protected and private member
       to demonstrate scope of members.
    */
    BaseClass::BaseClass(int a)
    {
        std::cout << "BaseClass()" << std::endl;
        storeb = a;
        storepb = a;
    }

    BaseClass::~BaseClass()
    {
        std::cout << "~BaseClass()" << std::endl;
    }

    int BaseClass::Store()
    {
        std::cout << "BaseClass::Store()" << std::endl;
        return storeb;
    }

    int BaseClass::Storepb() const
    {
        std::cout << "BaseClass::Storepb()" << std::endl;
        return storepb;
    }

    Derived1::Derived1(int a, int b)
        :BaseClass(b)
    {
        std::cout << "Derived1()" << std::endl;
        store1 = a;
        store22 = a;
    }

    Derived1::~Derived1()
    {
        std::cout << "~Derived1()" << std::endl;
    }

    /**
       Overrides the BaseClass method so that Derived1 member store1
       can be returned.
    */
    int Derived1::Store()
    {
        std::cout << "Derived1::Store()" << std::endl;
        return store1;
    }

    /**
       Overrides the BaseClass method so that BaseClass protected member storepb
       can be returned.
    */
    int Derived1::Storepb() const
    {
        std::cout << "Derived1::Storepb()" << std::endl;
        return storepb;
    }

    /**
       Method to return the BaseClass protected member store
    */
    int Derived1::Storeb()
    {
        std::cout << "Derived1::Storeb()" << std::endl;
        return BaseClass::Store();
    }

    /**
       Method to increment and return member store
       @returns store1 after increment
    */
    int Derived1::PVert()
    {
        std::cout << "Derived1::PVert()" << std::endl;
        store1++;
        return store1;
    }

    int Derived1::RtnConst() const
    {
        std::cout << "Derived1::RtnConst()" << std::endl;
        return store22;
    }

    Derived2::Derived2(int a)
        :BaseClass(a)
    {
        std::cout << "Derived2()" << std::endl;
        store2 = a;
    }

    Derived2::~Derived2()
    {
        std::cout << "~Derived2()" << std::endl;
    }

    int Derived2::Store()
    {
        std::cout << "Derived2::Store()" << std::endl;
        return BaseClass::Store();
    }

    void Derived2::Update2(int s2)
    {
        std::cout << "Derived2::Update2()" << std::endl;
        store2 = s2;
    }

    int Derived2::Store2()
    {
        std::cout << "Derived2::Store2()" << std::endl;
        return store2;
    }

    HasA::HasA(BaseClass& b)
        :base(b)
    {
        std::cout << "HasA(): is 1 " << (typeid(base) == typeid(Derived1)) << std::endl;        
    }

    HasA::~HasA()
    {
        std::cout << "~HasA()" << std::endl;
    }

    int HasA::Store()
    {
        std::cout << "HasA::Store()" << std::endl;
        return base.Store();
    }

    /**
       Note that not all the BaseClass derived classes have Storeb() defined.
       This method provides a way to demonstrate how to use run time type checking.
    */
    int HasA::Storeb()
    {
        std::cout << "HasA::Storeb()" << std::endl;

        int rtn = 0;
        if (typeid(base) == typeid(Derived1))
        {
            Derived1 *d1 = static_cast<Derived1 *>(&base);
            rtn = d1->Storeb();
        } else {
            rtn = base.Store();
        }

        return rtn;
    }

}

using namespace Compiler;

int main (void)
{
    StructA structA;
    structA.a = 22;
    int testmac = TESTRES(structA.a, 33);
    std::cout << "test macro output " << testmac << std::endl;
    std::cout << "test enum " << EnumA::A << std::endl;

    Derived1 da(1, 2);
    Derived2 db(3);
    HasA dc(da);
    HasA dd(db);

    std::cout << dc.Store() << std::endl;
    std::cout << dc.Storeb() << std::endl;
    std::cout << dd.Store() << std::endl;

    Derived1 *a = new Derived1(1, 2);

    const int i = 5;
    int *j = const_cast<int *>(&i);

    std::cout << "address of i 0x" << std::hex << &i << std::endl;
    std::cout << "j 0x" << std::hex << j << std::endl;

    *j = 6;

    BaseClass *b = a;
    Derived1 *c = dynamic_cast<Derived1 *>(b);

    std::cout << std::dec << c->Store() << std::endl;

    delete b;

    std::cout << Fib<5>::val << std::endl;

    std::cout << Fib<19>::val << std::endl;

    // Test c++11
    std::string v = "i is still: " + std::to_string(i);

    std::cout << v << std::endl;

    return 0;
}
