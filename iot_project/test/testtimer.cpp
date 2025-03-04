#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <atomic>
 
using namespace std;
class MyTimer {
	
public:
	MyTimer() {
		expired = true;
		tryToExpire = false;
	}
 
	//interval: ms
	void start(int interval, std::function<void()> task)
	{
		
		if (expired == false)//has already started, do not start again
			return;
 
		// start async timer, launch thread and wait in that thread
		expired = false;
		//将lambda函数传递给线程，做线程函数
		std::thread([this, interval, task]() {
 
			//小缺点：在task执行的过程中设置tryToExpire为true的话，需要等到本次task执行完毕后才能被while条件判断检测到。
			while (!tryToExpire)
			{
				// sleep every interval and do the task again and again until times up
				std::this_thread::sleep_for(std::chrono::milliseconds(interval));
				task(); //call this function every interval milliseconds.

			}
 
			// timer be stopped, update the condition variable expired and wake main thread
			std::lock_guard<std::mutex> locker(mut);
			expired = true;
			cv.notify_one();
 
		}).detach();
	}
 
	int stop()//该接口负责修改tryToExipired的值。
	{
		// do not stop again
		if (expired)
			return 0;
 
		if (tryToExpire)
			return 0;
 
		// wait until timer 
		tryToExpire = true; // change this bool value to make timer while loop stop. This val is atomic type.
 
		std::unique_lock<std::mutex> locker(mut);
		cv.wait(locker, [this] {return expired == true; });//不给lambda函数串this，不能使用成员变量expired!
 
		// reset the timer， 
		//成功停止后，设置tryToExpire为false. 其实，可以不使用wait动作，将tryToExpire=false的动作放到
		//start函数，当while循环被打断后，立即设置tryToExpire为false。
		//但是，这样单独放在这里设置也有好处吧，实现了tryToExipired的值仅仅由该stop函数负责。例如，任务线程成功结束后，这里可以进行一些额外的善后动作，
		
		if (expired == true)
			tryToExpire = false;
 
	}
 
private:
	condition_variable cv;
	mutex mut;//与cv配合
 
	atomic<bool> expired; //timer stop status
	atomic<bool> tryToExpire;//timer is in stop process.
 
 };
 
 void printNum(int num) {
	static int a = num;
	a++;
	cout << "a=" << a << endl;
}
int main() {
 
	MyTimer obj;
	obj.start(1000, []() { printNum(1); });
 
	getchar();//定时线程执行了detatch，这里需要等待才能看到起输出数字到屏幕上
	obj.stop();
	// getchar();
	return 0;
}
