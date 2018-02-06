//
// Created by chenchukun on 18/2/6.
//
#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <algorithm>
using namespace std;

class Observer
{
public:
    virtual void update() = 0;

    virtual ~Observer() {}
};

class Publisher
{
public:
    void add(Observer *observer) {
        lock_guard<mutex> guard(mutex_);
        observers_.push_back(observer);
    }

    void remove(Observer *observer) {
        lock_guard<mutex> guard(mutex_);
        auto it = find(observers_.begin(), observers_.end(), observer);
        // observers_不需要有序,将要删除的元素与最后一个元素交换位置,
        // 然后删除最后一个元素,可以避免后面的元素向前调整,提高效率
        if (it != observers_.end()) {
            swap(*it, observers_.back());
            observers_.pop_back();
        }
    }

    void nodify() {
        lock_guard<mutex> guard(mutex_);
        for (auto observer : observers_) {
            observer->update();
        }
    }

private:
    vector<Observer*> observers_;

    mutex mutex_;
};

class Test : public Observer
{
public:
    void update() {
        cout << "Test::update()" << endl;
    }
};

int main()
{
    Publisher publisher;
    Test *ptest = new Test;
    publisher.add(ptest);
    publisher.nodify();
    delete ptest;
    publisher.nodify();
    return 0;
}