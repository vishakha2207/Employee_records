#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <iomanip>  

using namespace std;

void analyze_file(const string& filename) {
    map<string, vector<string>> shifts;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string employee, shift_time;
        getline(ss, employee, ',');
        getline(ss, shift_time, ',');

        if (shifts.find(employee) == shifts.end()) {
            shifts[employee] = {shift_time};
        } else {
            vector<string> last_shifts = shifts[employee];
            tm last_shift_tm = {};
            istringstream last_shift_ss(last_shifts.back());
            last_shift_ss >> std::get_time(&last_shift_tm, "%Y-%m-%d %H:%M");  

            tm current_shift_tm = {};
            istringstream current_shift_ss(shift_time);
            current_shift_ss >> std::get_time(&current_shift_tm, "%Y-%m-%d %H:%M");

            int hours_between_shifts = difftime(mktime(&current_shift_tm), mktime(&last_shift_tm)) / 3600;

            shifts[employee].push_back(shift_time);

            if (hours_between_shifts < 1 || hours_between_shifts > 10) {
                cout << "Employee " << employee << " has more than 10 hours or less than 1 hour between shifts." << endl;
            }

            if (current_shift_tm.tm_hour > 14) {
                cout << "Employee " << employee << " has worked more than 14 hours in a single shift." << endl;
            }

            if (shifts[employee].size() > 7) {
                cout << "Employee " << employee << " has worked for 7 consecutive days." << endl;
            }
        }
    }
}

int main() {
    const string filename = "data.csv";
    analyze_file(filename);

    return 0;
}
