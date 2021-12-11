#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "mainRandFunction.h"
using namespace std;
int countOrder = 0;
vector<string> orderCareer;
mutex orderCareer_access;
void kitchen(string& dish, int numberOrder){
    cout << "kitchen: " << "accepted order " << numberOrder << " " << dish << endl;
    timeSrand();
    this_thread::sleep_for(chrono::seconds(randMinMax(5, 15)));
    cout << "kitchen: " << "ready order " << numberOrder << " " << dish << endl;
    if (orderCareer_access.try_lock()){
        cout << "kitchen: " << "order " << numberOrder << " conveyed courier\n" << endl;
        orderCareer.push_back(dish);
        orderCareer_access.unlock();
    }else{
        cout << "kitchen: " << "order " << numberOrder << " wait courier\n";
        while (!orderCareer_access.try_lock());
        cout << "kitchen: " << "order " << numberOrder << " conveyed courier\n" << endl;
        orderCareer.push_back(dish);
        orderCareer_access.unlock();
    }
}
mutex kitchen_access;
void order(){
    timeSrand();
    int numberOrder = countOrder;
    string menu[] = {"pizza", "salad", "soup", "steak", "sushi"};
    string dish = menu[randNum(5)];
    cout << "!!!NEW order " << numberOrder << " : " << dish << endl;
    if (kitchen_access.try_lock()){//если кухня свободна
        cout << "order " << numberOrder << " : " << dish << " to kitchen\n";
        kitchen(dish, numberOrder);//передаем заказ на кухню
        kitchen_access.unlock();
    }else{
        cout << "order " << numberOrder << " wait\n";
        while (!kitchen_access.try_lock());//ждем когда кухня освободиться
        cout << "order " << numberOrder << " to kitchen\n";
        kitchen(dish, numberOrder);//передаем заказ на кухню
        kitchen_access.unlock();
    }
}

int main(){
    for (int i = 0; i < 10; i++){
        timeSrand();
        this_thread::sleep_for(chrono::seconds(randMinMax(5, 10)));
        countOrder++;
        thread *ptrOrder = new thread(order);
        ptrOrder->detach();
    }
    this_thread::sleep_for(chrono::seconds(50));
    return 0;
}

