#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <string>
#include <vector>
using namespace std;

class Community {
    vector<string> student;			// 입력 벡터
    vector<vector<string>> team;	// 2차원 벡터로 초기 배치
    int groupCount;					// 행
    int groupEntityCount;			// 열
public:
    Community();
    Community(vector<string> student, int groupCount, int groupEntityCount);
    ~Community();

    void shuffle();

};

#endif // COMMUNITY_H
