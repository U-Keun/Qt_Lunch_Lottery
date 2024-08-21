#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <string>
#include <vector>
using namespace std;

class Community {
    vector<string> student;			// 입력 벡터
    vector<vector<string>> team;	// 초기 배치는 2차원 벡터로
    void update();                  // 2차원 벡터 업데이트
    int groupCount;					// 행
    int groupEntityCount;			// 열
    int count;                      // 랜밥 셔플 카운트

public:
    Community();
    Community(vector<string> student, int groupCount, int groupEntityCount, int count);
    ~Community();

    void setStudent(vector<string> s);
    void setGroupCount(int gc);
    void setGroupEntityCount(int gec);
    vector<string> getStudent();
    vector<vector<string>> getTeam();

    void shuffle();

};

#endif // COMMUNITY_H
