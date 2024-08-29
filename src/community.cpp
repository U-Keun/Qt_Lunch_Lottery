#include <random>
#include <chrono>

#include "community.h"

using namespace std;

Community::Community():Community(vector<string>(), 0, 0){}
Community::Community(vector<string> student, int groupCount, int groupEntityCount):
    student(student), groupCount(groupCount), groupEntityCount(groupEntityCount) {
    update();
}
Community::~Community(){}

void Community::update(){
    team.clear();
    team.resize(groupCount, vector<string>(groupEntityCount));

    for (int j = 0; j < groupEntityCount; j++) {
        for (int i = 0; i < groupCount; i++) {
            team[i][j] = student[i + j*groupCount];
        }
    }
}

void Community::setStudent(vector<string> student){
    this->student = student;
    update();
}

void Community::setGroupCount(int groupCount){
    this->groupCount = groupCount;
}

void Community::setGroupEntityCount(int groupEntityCount){
    this->groupEntityCount = groupEntityCount;
}

vector<vector<string>> Community::getTeam() const {
    return team;
}

bool Community::isValidTeam() {
    int standard = team[0].size() / 2 - 1;
    for (const auto& row : team) {
        int count = 0;
        for (const string& name : row) {
            if (name == "none") count++;
        }

        if (count > standard) return false;
    }

    return true;
}

mt19937 initialize_random_engine() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    return mt19937(seed);
}

mt19937 g = initialize_random_engine();

void Community::shuffle() {
    do {
        vector<string> flatTeam;
        for (const auto& row : team) {
            flatTeam.insert(flatTeam.end(), row.begin(), row.end());
        }

        std::shuffle(flatTeam.begin(), flatTeam.end(), g);

        int index = 0;
        for (int i = 0; i < groupCount; i++) {
            for (int j = 0; j < groupEntityCount; j++) {
                team[i][j] = flatTeam[index++];
            }
        }
    } while(!isValidTeam());
}
