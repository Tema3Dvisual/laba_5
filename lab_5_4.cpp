#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;
    
    map<string, set<string>> friends; // хранит список друзей для каждого
    vector<string> output;
    
    for (int i = 0; i < n; i++) { // обработка программ
        string command;
        cin >> command;
        
        if (command == "FRIENDS") {
            string person1, person2;
            if (cin >> person1 >> person2) {
                friends[person1].insert(person2);
                friends[person2].insert(person1);
            } else {
                cerr << "ERROR: Invalid FRIENDS command format" << endl;
            }
        }
        else if (command == "COUNT") {
            string person;
            if (cin >> person) {
                output.push_back(to_string(friends[person].size()));
            } else {
                cerr << "ERROR: Invalid COUNT command format" << endl;
            }
        }
        else if (command == "QUESTION") {
            string person1, person2;
            if (cin >> person1 >> person2) {
                bool are_friends = friends.count(person1) && 
                                  friends[person1].find(person2) != friends[person1].end();
                output.push_back(are_friends ? "YES" : "NO");
            } else {
                cerr << "ERROR: Invalid QUESTION command format" << endl;
            }
        }
        else {
            cerr << "ERROR: Unknown command '" << command << "'" << endl;
            // Пропускаем оставшуюся часть строки
            string dummy;
            getline(cin, dummy);
        }
    }
    
    for (const auto& res : output) {
        cout << res << endl;
    }
    
    return 0;
}
