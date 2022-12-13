#include <iostream>
#include <vector>
#include <thread>
#include "ThreadPool.h"
using namespace std;

int main()
{

    ThreadPool pool(100);
    thread thd1([&pool]()
                {
        for(int i = 0;i<10;i++)
        {
            auto thdID = this_thread::get_id();
            pool.addTask([thdID]{
                cout<<"thread1 ID: "<<thdID<<endl;
            });
        } });
    thread thd2([&pool]()
                {
        for(int i = 0;i<10;i++)
        {
            auto thdID = this_thread::get_id();
            pool.addTask([thdID]{
                cout<<"thread2 ID: "<<thdID<<endl;
            });
        } });
    this_thread::sleep_for(std::chrono::seconds(5));
    pool.stop();
    thd1.join();
    thd2.join();
    return 0;
}