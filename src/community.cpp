#include "community.h"

Community::Community():Community(vector<string>(), 0, 0){}
Community::Community(vector<string> student, int groupCount, int groupEntityCount):
    student(student), groupCount(groupCount), groupEntityCount(groupEntityCount){
    team = vector<vector<string>>(groupCount, vector<string>(groupEntityCount));
    for (int j = 0; j < groupEntityCount; j++) {
        for (int i = 0; i < groupCount; i++) {
            team[i][j] = student[i + j*groupCount];
        }
    }
}
Community::~Community(){}

void Community::shuffle() {
    int mod = team.size();
    int count = 0;
    vector<vector<string>> newTeam = team;
    for (int j = 0; j < team[0].size(); j++) {
        for (int i = 0; i < team.size(); i++) {
            newTeam[(i + (j + 1)) % mod][j] = team[i][j];
        }
    }

    if (count % (team.size() - 1) == team.size() - 2) {
        vector<vector<string>> newnewTeam = newTeam;
        mod = team[0].size();
        for (int i = 0; i < team.size(); i++) {
            for (int j = 0; j < team[0].size(); j++) {
                newnewTeam[i][(j + (i + 1)) % mod] = newTeam[i][j];

            }
        }
        team = newnewTeam;
    }
    else {
        team = newTeam;
    }

    count++;
}
