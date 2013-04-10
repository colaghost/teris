#ifndef DATA_H
#define DATA_H
#include <Windows.h>
#include <time.h>
#include <math.h>
#include <utility>
using namespace std;

struct AI_OFFSET
{
	int offsetX;
	int offsetY;
	int shape;
};
class Teris
{
public:
	class Shape
	{
	public:
		char pos[4][4];//��¼�������״
		char minR;//��¼������pos������С������
		char maxR;//��¼������pos������������
		char minC;//��¼������pos������С������
		char maxC;//��¼������pos������������
		char maxRow[4];
		char minRow[4];
	public:
		void InitShape(char[][4], char, char, char, char, char[], char[]);
	};
public:
	void InitGame();//��Ϸ���¿�ʼ
private:
	enum FORM
	{
		T,
		L,
		Z,
		I,
		O, 
		RL,
		RZ
	};
private:
	Shape sT[4];
	Shape sL[4];
	Shape sZ[4];
	Shape sI[4];
	Shape sO[4];
    Shape sRL[4];
	Shape sRZ[4];
private:
	void Init();//��Shape����ĳ�ʼ��
private:
	RECT rect;//��¼��Ϸ�����С����ʵ����Ļ��ʾ��ССʮ����
public:
	char **pDC;//��Ϸ��������
public:
	POINT nowPt;//��¼��ǰ����λ��
public:
	Shape *ptr;//ָ��ǰ����
	Shape *nextPtr;//ָ��һ�·���
private:
//	FORM nowForm;//��¼��ǰ����
	char nowShape;//��¼��ǰ������״
//	FORM nextForm;//��¼��һ����
	char nextShape;//��¼��һ��������״
	short *minRow;//��¼��Ϸ����ÿһ����ߵ�
	int clearLineCount;//�ϴ���������
private:
	BOOL fState;//�����Ƿ񴥵�
private:
	int ShapeCount;//��¼���·������
public:
	char** GetpDC();//
private:
	bool isTDLegal(POINT pt);//�жϴ�λ���ڴ�ֱ�������Ƿ�Ϸ�
	bool isLRLegal(POINT pt);//�жϴ�λ����ˮƽ�������Ƿ�Ϸ�
	void Configure();
public:
	Teris(RECT);
	Teris(Teris&);//�������캯��
	~Teris();
public:
	BOOL Up();//��ת����
	BOOL Down();//��������
	BOOL Left();//��������
	BOOL Right();//��������
	void Check();//�жϷ����Ƿ�����
private:
	void Change();//����ͣ��ʱ���Ľ�������״��
	BOOL isClearLine(int line);//�ж��Ƿ�����
	void CopyLine(int srLine, int descLine);
	void initminRow();//���к�����minRow�����ֵ
private:
	unsigned int score;//�������÷���
	BOOL fOver;//��Ϸ�Ƿ����
	int maxLine;//��¼�������
public:
	unsigned int GetScore()
	{
		return score;
	}
	BOOL GetGameState()//��֪��Ϸ�Ƿ����
	{
		return fOver;
	}
private:
	void ComputeScore(int clearLines)
	{
		score += (UINT)pow(2.0, clearLines);
	}
	void SetNextShape();
public:
	AI_OFFSET AI(Teris &teris);
	int getNowShape()
	{
		return nowShape;
	}
	int getNowPtX()
	{
		return nowPt.x;
	}
	int getNowPtY()
	{
		return nowPt.y;
	}
public:
	void ResetTeris(Teris &teris);
};
BOOL CompareAIScore(const pair<AI_OFFSET, int> &l, const pair<AI_OFFSET, int> &r);
#endif