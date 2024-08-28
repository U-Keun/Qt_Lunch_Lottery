#include "community.h"

Community::Community():Community(vector<string>(), 0, 0, 0){}
Community::Community(vector<string> student, int groupCount, int groupEntityCount, int count):
    student(student), groupCount(groupCount), groupEntityCount(groupEntityCount), count(0){
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

vector<string> Community::getStudent(){
    return student;
}

vector<vector<string>> Community::getTeam(){
    return team;
}

bool Community::isValidTeam() {
    int standard = team[0].size() / 2;
    for (const auto& row : team) {
        int count = 0;
        for (string name : row) {
            if (name == "none") count++;
        }

        if (count > standard) return false;
    }

    return true;
}

void Community::shuffle() {
    int mod = team.size();
    do {
        vector<vector<string>> newTeam = team;
        for (int j = 0; j < team[0].size(); j++) {
            for (int i = 0; i < team.size(); i++) {
                newTeam[(i + (j + 1)) % mod][j] = team[i][j];
            }
        }
        team = newTeam;

        if (count % (team.size() - 1) == team.size() - 2) {
            mod = team[0].size();
            for (int i = 0; i < team.size(); i++) {
                for (int j = 0; j < team[0].size(); j++) {
                    newTeam[i][(j + (i + 1)) % mod] = team[i][j];
                }
            }
            team = newTeam;
        }

        count++;
    } while(!isValidTeam());
}
