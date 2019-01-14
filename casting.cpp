#include <iostream>

using namespace std;


/*

http://www.cplusplus.com/doc/oldtutorial/typecasting/
https://en.cppreference.com/w/cpp/language/dynamic_cast
https://habr.com/post/106294/



https://stackoverflow.com/questions/8469900/cant-downcast-because-class-is-not-polymorphic
The reason why:  static_cast basically has the compiler perform a check at compile time "Could the input be cast to the output?" This is can be used for cases 
where you are casting up or down an inheritance hierarchy of pointers (or references). But the check is only at compile time, and the compiler assumes you know 
what you are doing.

dynamic_cast can only be used in the case of a pointer or reference cast, and in addition to the compile time check, it does an additional run time check that the 
cast is legal. It requires that the class in question have at least 1 virtual method, which allows the compiler (if it supports RTTI) to perform this additional check. 
However, if the type in question does not have any virtual methods, then it cannot be used.

The simplest case, and probably worthwhile if you're passing pointers around like this, is to consider making the base class's destructor virtual. 
In addition to allowing you to use dynamic cast, it also allows the proper destructors to be called when a base class pointer is deleted.




*/


class Base 
{
public:
	Base()
	{
		A = 1;
		B = 2;
	}

	int A;
	int B;
	
	virtual void foo()
	{

	}
};

class Derrived : public Base
{
public:
	Derrived()
	{
		C = 3;
	}

	int C;
	char buffer[10];
};




class CDummy {
	float i, j;
	public:
		CDummy()
		{
			i = 1;
			j = 2;
		}
};

class CAddition {
	int x, y;
public:
	CAddition(int a, int b) { x = a; y = b; }
	int result() { return x + y; }
};





struct V {
	virtual void f() {};  // must be polymorphic to use runtime-checked dynamic_cast
};
struct A : virtual V {};
struct B : virtual V {
	B(V* v, A* a) {
		// casts during construction (see the call in the constructor of D below)
		dynamic_cast<B*>(v); // well-defined: v of type V*, V base of B, results in B*
		dynamic_cast<B*>(a); // undefined behavior: a has type A*, A not a base of B
	}
};
struct D : A, B {
 	D() : B(static_cast<A*>(this), this) { }
};

struct BaseClass {
	virtual ~BaseClass() {}
};

struct DerivedClass : BaseClass {
	virtual void name() {}
};

class Equal1 
{
	public:
		int A=1;

};
class Equal2 
{
	public:
		int B=2;


};




/*
	The cast is syntactically corrects but leads 
	to undefined beahviour
*/
void ErrorCStyleConversion()
{
	CDummy d;

	CAddition * padd;
	padd = (CAddition*)&d;

	cout << padd->result();

}

/*
    1. Not checks type cast correctness at compile time.	
	2. Check correctness of conversion at runtime (slower than staic_cast 
	    at runtime)
	  On conversion problem:
	   1) If pointer - returns null
	   2) If refference throws exception
	3. Must be "polymorphic"  (must be derrived from class which
	   has at least one virtual method)
	4. Could type cast only pointers and refferences
	5. Is faster than static cast
*/

void DynamicCast()
{
	
	
	Base baseObj;
	Derrived derrivedObj;


	//Note: must be pointer or refference.
	//Compile error
	//derrivedObj = dynamic_cast<Base> (baseObj);

	//Reffernces Derrivde => base - valid conversion
	baseObj = dynamic_cast<Base&> (derrivedObj);
	


	Base * pBase;
	Derrived *pDerrived;

	//Upcast Pointer Derrived => base - valid conversion
	pDerrived = new Derrived();
	pBase = dynamic_cast<Base*> (pDerrived);

	Base * pBase2;
	Derrived *pDerrived2;
	
	//Downcast base => derrived - invalid conversion 
	pBase2 = new Base();
	pDerrived = dynamic_cast<Derrived*> (pBase2);//result is null-pointer
	if (!pDerrived)
	{
		cout << "invalid conversion";
	}


	Base pBase3;	
	//Downcast => this throws "Bad cast" exception
	//Derrived & pDerrived3 =  dynamic_cast<Derrived &>(pBase3);


	

	//pDerrived3Derrived *pValidDerrived = dynamic_cast<Derrived*>(pDerrived3);
	

	//from
	//https://en.cppreference.com/w/cpp/language/dynamic_cast
	//A:V, B:V
	//struct D : A, B 

	D d; // the most derived object
	A& a = d; // upcast, dynamic_cast may be used, but unnecessary
	D& new_d = dynamic_cast<D&>(a); // downcast
	B& new_b = dynamic_cast<B&>(a); // sidecast

	

	//This doesn't work
	BaseClass* b1 = new BaseClass;
	if (DerivedClass* d = dynamic_cast<DerivedClass*>(b1))
	{
		std::cout << "downcast from b1 to d successful\n";
		d->name(); // safe to call
	}
	//This works as b2 is realy DerivedClass
	BaseClass* b2 = new DerivedClass;
	if (DerivedClass* d = dynamic_cast<DerivedClass*>(b2))
	{
		std::cout << "downcast from b2 to d successful\n";
		d->name(); // safe to call
	}

	delete b1;
	delete b2;

}

/*
	1. Checks at compile time
	2. Not checks on runtime (faster than dynamic_cast at runtime)

*/

void StaticCast()
{

	//normal casting
 	Base base;
   	Derrived derrived = static_cast<Derrived &>(base);
	
	//The object was not fully initialized - garbage in
	//fields
	cout << derrived.C;
	
	Derrived derrived2;
	Base base2 = static_cast<Base &> (derrived2);

	CAddition addition();
	//not possible
	//CDummy & dummy = static_cast<CDummy&> (addition);

}


/*
  Reinterpret_cast converts any pointer type to any other pointer type, even of unrelated classes. 
  The operation result is a simple binary copy of the value from one pointer to the other.
  The conversions that can be performed by reinterpret_cast but not by static_cast are low-level operations, 
   whose interpretation results in code which is generally system-specific, and thus non-portable
*/

void ReinterpretCast()
{
	Equal1 * equal1 = new Equal1();
	Equal2 * equal2 = reinterpret_cast<Equal2 *>(equal1);

}
/*
	Cast "const" or "volatile" qualifier

*/
void ConstCast()
{
	const char *const_string = "const string";
	char * non_const_string = const_cast<char*>(const_string);	
}




void casting()
{

	ErrorCStyleConversion();
	DynamicCast();
	StaticCast();
	ReinterpretCast();
	ConstCast();
}
