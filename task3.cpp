#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
#include "mainRandFunction.h"
using namespace std;
int countOrder = 0;
vector<string> orderCareer;
mutex orderCareer_access;
map<int, string> turn;
mutex turn_access;
void kitchen(){
        while (!turn_access.try_lock());
        map<int, string>::iterator itTurn = turn.begin();
#define NUM_ORDER itTurn->first
#define DISH itTurn->second
        //turn[numberOrder] = dish;
        cout << "kitchen: " << "accepted order " << NUM_ORDER << " " << DISH << endl;
        timeSrand();
        this_thread::sleep_for(chrono::seconds(randMinMax(5, 15)));
        cout << "kitchen: " << "ready order " << NUM_ORDER << " " << DISH << endl;
        if (orderCareer_access.try_lock()) {
            cout << "kitchen: " << "order " << NUM_ORDER << " conveyed courier\n" << endl;
            orderCareer.push_back(DISH);
            turn.erase(NUM_ORDER);
            orderCareer_access.unlock();
            turn_access.unlock();
        } else {
            cout << "kitchen: " << "order " << NUM_ORDER << " wait courier\n";
            while (!orderCareer_access.try_lock());
            cout << "kitchen: " << "order " << NUM_ORDER << " conveyed courier\n" << endl;
            orderCareer.push_back(DISH);
            turn.erase(NUM_ORDER);
            orderCareer_access.unlock();
            turn_access.unlock();
        }
}
mutex kitchen_access;
/*
void order(){
    timeSrand();
    int numberOrder = countOrder;
    string menu[] = {"pizza", "salad", "soup", "steak", "sushi"};
    string dish = menu[randNum(5)];
    cout << "!!!NEW order " << numberOrder << " : " << dish << endl;
    while(!turn_access.try_lock());
    turn[numberOrder] = dish;
    turn_access.unlock();
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
*/
int main(){
    for (int i = 0; i < 10; i++){
        timeSrand();
        this_thread::sleep_for(chrono::seconds(randMinMax(5, 10)));
        countOrder++;
        string menu[] = {"pizza", "salad", "soup", "steak", "sushi"};
        string dish = menu[randNum(5)];
        cout << "!!!NEW order " << countOrder << " : " << dish << endl;
        while(!turn_access.try_lock());
        turn[countOrder] = dish;
        thread th_kitchen(kitchen);
        th_kitchen.detach();
        turn_access.unlock();
    }
    this_thread::sleep_for(chrono::seconds(100));
    return 0;
}

