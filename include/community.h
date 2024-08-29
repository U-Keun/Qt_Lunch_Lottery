#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <string>
#include <vector>
using namespace std;

class Community {
    vector<string> student;
    vector<vector<string>> team;
    void update();
    int groupCount;
    int groupEntityCount;

    bool isValidTeam();
public:
    Community();
    Community(vector<string>, int, int);
    ~Community();

    void setStudent(vector<string>);
    void setGroupCount(int);
    void setGroupEntityCount(int);
    vector<vector<string>> getTeam() const;

    void shuffle();
};

#endif // COMMUNITY_H
