#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
#include "mainRandFunction.h"
using namespace std;
int countOrder = 0;
int countDelivery = 0;
mutex delivery_access;
vector<string> orderCourier;
mutex orderCourier_access;
map<int, string> turn;
mutex turn_access;
void courier(){
    this_thread::sleep_for(chrono::seconds(30));
    while(!orderCourier_access.try_lock());
    orderCourier.clear();
    orderCourier_access.unlock();
    while(!delivery_access.try_lock());
    countDelivery++;
    delivery_access.unlock();
    cout << "delivery arrived " << countDelivery << endl;
    if (countDelivery < 10) courier();
}
void kitchen(){
    while (!turn_access.try_lock());
    map<int, string>::iterator itTurn;
    if(!turn.empty()){
        itTurn = turn.begin();
        turn_access.unlock();
    }else {
        turn_access.unlock();
        this_thread::sleep_for(chrono::milliseconds(500));
        kitchen();
    }
#define NUM_ORDER itTurn->first
#define DISH itTurn->second
    cout << "kitchen: " << "accepted order " << NUM_ORDER << " " << DISH << endl;
    int delay = randMinMax(5, 15);
    //cout << "delay kitchen " << delay << endl;
    this_thread::sleep_for(chrono::seconds(delay));
    cout << "kitchen: " << "ready order " << NUM_ORDER << " " << DISH << endl;
    if (orderCourier_access.try_lock()) {
        cout << "kitchen: " << "order " << NUM_ORDER << " conveyed courier\n" << endl;
        orderCourier.push_back(DISH);
        turn.erase(NUM_ORDER);
        orderCourier_access.unlock();
    } else {
        cout << "kitchen: " << "order " << NUM_ORDER << " wait courier\n";
        while (!orderCourier_access.try_lock());
        cout << "kitchen: " << "order " << NUM_ORDER << " conveyed courier\n" << endl;
        orderCourier.push_back(DISH);
        turn.erase(NUM_ORDER);
        orderCourier_access.unlock();
    }
    while(!delivery_access.try_lock());
    if (countDelivery < 10) {
        delivery_access.unlock();
        kitchen();
    }else delivery_access.unlock();
}

void order(){
    while(!delivery_access.try_lock());
    if (countDelivery < 10) {
        delivery_access.unlock();
        int delay = randMinMax(5, 10);
        //cout << "delay order " << delay << endl;
        this_thread::sleep_for(chrono::seconds(delay));
        countOrder++;
        string menu[] = {"pizza", "salad", "soup", "steak", "sushi"};
        string dish = menu[randNum(5)];
        cout << "!!!NEW order " << countOrder << " : " << dish << endl;
        while (!turn_access.try_lock());
        turn[countOrder] = dish;
        turn_access.unlock();
        order();
    }else delivery_access.unlock();
}

int main(){
    thread th_Courier(courier);
    th_Courier.detach();
    thread th_Kitchen(kitchen);
    th_Kitchen.detach();
    order();
    return 0;
}

