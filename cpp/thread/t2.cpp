#include <thread>
#include <mutex>
#include <memory>
#include <chrono>
#include <iostream>
#include <sys/time.h>

using namespace std;

class A {
public:
	A(int _x):x(_x){};
	~A(){};
	void f1()
	{
		lock_guard<std::mutex> guard(mMutex);
		--x;
		cout << "x = " << x << endl;
	}
private:
	int x;
	mutex mMutex;		
};

void f2(shared_ptr<A> a)
{
	cout << "****local thread 2****\n";
	a->f1();
}

void f3(shared_ptr<A> a)
{
	cout << "****local thread 3****\n";
	a->f1();
}

int main()
{
	shared_ptr<A> a(new A(10));
	for(int i = 0; i < 100; i++)
	{
		std::thread t1(f2, a);
		std::thread t2(f3, a);
		t1.join();	
		t2.join();
	}
	return 0;
}
