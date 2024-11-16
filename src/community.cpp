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

double Community::calculateScore(vector<string>& a, vector<string>& b) {
    /*
    * 두 팀간 점수를 매깁니다.
    * 팀간 점수는 다음과 같이 매깁니다.
    * 1. 팀을 one-hot encoding으로 표현합니다.
    * 2. 두 팀간 차이를 xor로 연산합니다.
    * 3. 결과의 1을 1점으로 계산합니다.
    *   1. 만약 a팀에서 존재하는 인원이 b팀에서도 존재한다면 그 팀원의 수를 n이라고 합니다.
    *   2. n명이 존재한다면 1/n점을 부여합니다.
    */
    double score = 0;
    vector<int> aOneHot(student.size(), 0);
    vector<int> bOneHot(student.size(), 0);
    for (string& name : a) {
        aOneHot[nameToIndex[name]] = 1;
    }
    for (string& name : b) {
        bOneHot[nameToIndex[name]] = 1;
    }
    int n = 0;
    for (int i = 0; i < student.size(); i++) {
        score += aOneHot[i] ^ bOneHot[i];
        if (aOneHot[i] && bOneHot[i]) n++;
    }
    if (n != 0) score += 1.0 / n;

    return score;
}