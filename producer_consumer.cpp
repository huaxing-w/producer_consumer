#include "bits/stdc++.h"

std::queue<int>q;
std::mutex mtx;
std::condition_variable cv;
bool done=false;

void producer(){
    for(int i=0;i<10;i++){
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i);
            std::cout<<"producer: processed: "<<i<<std::endl;
        }
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        done=true;
        std::cout<<"producer: all job produced"<<std::endl;
    }
    cv.notify_one();
    
};

void consumer(){
    while(1){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock,[]{
            return !q.empty() || done;
        });

        while(!q.empty()){
            int front=q.front();
            q.pop();
            std::cout<<"consumer: consumed: "<<front<<std::endl;
        }
        if(done && q.empty()){
            std::cout<<"consumer: all job finished"<<std::endl;
            break;
        }
    }
};


int main(){
    std::thread produceThread(producer);
    std::thread consumeThread(consumer);

    produceThread.join();
    consumeThread.join();


    return 0;
}
