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
	pthread_mutex_t mWaitMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t mWaitCond = PTHREAD_COND_INITIALIZER;
	struct timespec timeToWait;
	struct timeval now;
	int rc;
	while(true)
	{
		pthread_mutex_lock(&mWaitMutex);
		gettimeofday(&now,NULL);
		timeToWait.tv_sec = now.tv_sec + 1;
		timeToWait.tv_nsec = now.tv_usec*1000UL;
		rc = pthread_cond_timedwait(&mWaitCond, &mWaitMutex, &timeToWait);
		if (rc == 0)
			break;
		cout << "****local thread 2****\n";
		a->f1();
		pthread_mutex_unlock(&mWaitMutex);
	}
	pthread_cond_signal(&mWaitCond);
}

void f3(shared_ptr<A> a)
{
	pthread_mutex_t mWaitMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t mWaitCond = PTHREAD_COND_INITIALIZER;
	struct timespec timeToWait;
	struct timeval now;
	int rc;
	while(true)
	{
		pthread_mutex_lock(&mWaitMutex);
		gettimeofday(&now,NULL);
		timeToWait.tv_sec = now.tv_sec + 1;
		timeToWait.tv_nsec = now.tv_usec*1000UL;
		rc = pthread_cond_timedwait(&mWaitCond, &mWaitMutex, &timeToWait);
		if (rc == 0)
			break;
		cout << "****local thread 3****\n";
		a->f1();
		pthread_mutex_unlock(&mWaitMutex);
	}
	pthread_cond_signal(&mWaitCond);
}

int main()
{
	shared_ptr<A> a(new A(10000000));
	std::thread t1(f2, a);
	std::thread t2(f3, a);
	t1.join();	
	t2.join();
	return 0;
}
