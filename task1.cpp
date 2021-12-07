/*
 Тут я может чуть чуть перемудрил и можно было сделать проще,
 но мне было интересно сделать так.
 Я вот только не до конца понял как отследить что конкретный поток закончил выполнение.
 Ведь если что-то пойдет не так то программа не завершиться пока в map не будет 6 значений.
 Не понял как получать данные из потока. То есть если бы функция была возвращаемого типа например.
 */
#include <iostream>
#include <thread>
#include <fstream>
#include <mutex>
#include <map>
using namespace std;
multimap <int, string> result;
mutex result_access;
void swimmer(string name, int m_s){
    int distance = 100;
    int countTime = 0;
    while (distance > 0){
        this_thread::sleep_for(chrono::seconds(1));
        countTime++;
        if (distance - m_s < 0) {
            distance = 0;
        }else distance -= m_s;
        cout << name << "\t" << distance << endl;
    }
    result_access.lock();
    result.insert(pair<int, string>(countTime, name));
    result_access.unlock();
}
class Swimmer{
    static const int numberSwim = 6;
    int M_S[numberSwim] = {0};
    string name[numberSwim] = {"null"};
    thread** Swim = nullptr;
#define DELAY(sec) this_thread::sleep_for(chrono::seconds(sec));
public:
    void setSwim(){
        ifstream file ("swimmer.txt");
        for (int i = 0; i < numberSwim; i++){
            cout << "enter name and speed " << i+1 << " swimmer\n";
            //cin >> name[i] >> M_S[i];
            file >> name[i] >> M_S[i];
        }
        Swim = new thread*[numberSwim];
        for (int i = 0; i < numberSwim; i++){
            Swim[i] = new thread(swimmer,name[i],M_S[i]);
            Swim[i]->detach();
        }
    }
    int getNumberSwim(){
        return numberSwim;
    }
};
int main() {
    Swimmer swim = Swimmer();
    swim.setSwim();
    while (result.size() != swim.getNumberSwim());
    cout << "************\n";
    for (const auto& r : result){
        cout << r.first << " " << r.second << endl;
    }
    /*
    const int numberSwim = 6;
    string name[numberSwim] = {"null"};
    int M_S[numberSwim] = {0};
    for (int i = 0; i < numberSwim; i++){
        cout << "enter name and speed " << i+1 << " swimmer\n";
        cin >> name[i] >> M_S[i];
    }
    thread flow1(swimmer,name[0],M_S[0]);
    thread flow2(swimmer,name[1],M_S[1]);
    thread flow3(swimmer,name[2],M_S[2]);
    thread flow4(swimmer,name[3],M_S[3]);
    thread flow5(swimmer,name[4],M_S[4]);
    thread flow6(swimmer,name[5],M_S[5]);

    flow1.join();
    flow2.join();
    flow3.join();
    flow4.join();
    flow5.join();
    flow6.join();
     */
    return 0;
}
