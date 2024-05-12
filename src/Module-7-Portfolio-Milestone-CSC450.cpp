#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


int count_up = 0;
int count_down = 20;


bool counting_up_completed = false;


std::mutex mtx;
std::condition_variable cv;


void countUp() {
    while (count_up < 20) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            std::cout << "Count Up: " << ++count_up << std::endl;
            counting_up_completed = (count_up == 20);
        }
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


void countDown() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return counting_up_completed; });
    while (count_down >= 0) {
        std::cout << "Count Down: " << count_down-- << std::endl;
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {

    std::thread t1(countUp);
    std::thread t2(countDown);


    t1.join();
    t2.join();

    return 0;
}

