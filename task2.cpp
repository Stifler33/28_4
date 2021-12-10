#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;
vector<char> terminal;
mutex terminal_access;
void Train (char trainName, int travelTime){
    cout << "Train " << trainName << " left\n";
    this_thread::sleep_for(chrono::seconds(travelTime));
    string action;
    bool flag = false;
    while (!terminal_access.try_lock()){
        if (!flag){
            flag = true;
            cout << trainName << " expects\n";
        }
    }
    while (action != "depart"){
        cout << "Train " << trainName << " Enter action\n";
        cin >> action;
        if (action != "depart") cout << "filet action\n";
    }
    cout << "Train " << trainName << " depart\n";
    terminal.push_back(trainName);
    terminal_access.unlock();
}
int main() {
    int seconds;
    char nameTrain;
    cout << "Enter time travel A\n";
    cin >> seconds;
    thread A(Train, 'A', seconds);
    cout << "Enter time travel B\n";
    cin >> seconds;
    thread B(Train, 'B', seconds);
    cout << "Enter time travel C\n";
    cin >> seconds;
    thread C(Train, 'C', seconds);
    while(A.joinable() && B.joinable() && C.joinable()){
        A.join();
        B.join();
        C.join();
    }
    if (terminal.size() < 3) {
        cout << "not everyone key\n";
    }else cout << "all arrived\n";
    return 0;
}