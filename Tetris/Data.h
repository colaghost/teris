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
		char pos[4][4];//记录方块的形状
		char minR;//记录方块在pos里面最小的行数
		char maxR;//记录方块在pos里面最大的行数
		char minC;//记录方块在pos里面最小的列数
		char maxC;//记录方块在pos里面最大的列数
		char maxRow[4];
		char minRow[4];
	public:
		void InitShape(char[][4], char, char, char, char, char[], char[]);
	};
public:
	void InitGame();//游戏重新开始
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
	void Init();//对Shape对象的初始化
private:
	RECT rect;//记录游戏区域大小（比实际屏幕显示大小小十倍）
public:
	char **pDC;//游戏区域数组
public:
	POINT nowPt;//记录当前方块位置
public:
	Shape *ptr;//指向当前方块
	Shape *nextPtr;//指向一下方块
private:
//	FORM nowForm;//记录当前方块
	char nowShape;//记录当前方块形状
//	FORM nextForm;//记录下一方块
	char nextShape;//记录下一个方块形状
	short *minRow;//记录游戏区域每一列最高点
	int clearLineCount;//上次消掉行数
private:
	BOOL fState;//方块是否触底
private:
	int ShapeCount;//记录掉下方块个数
public:
	char** GetpDC();//
private:
	bool isTDLegal(POINT pt);//判断此位置在垂直方向上是否合法
	bool isLRLegal(POINT pt);//判断此位置在水平方向上是否合法
	void Configure();
public:
	Teris(RECT);
	Teris(Teris&);//拷贝构造函数
	~Teris();
public:
	BOOL Up();//翻转方块
	BOOL Down();//方块向下
	BOOL Left();//方块向左
	BOOL Right();//方块向右
	void Check();//判断方块是否依靠
private:
	void Change();//方块停靠时，改进方块形状等
	BOOL isClearLine(int line);//判断是否消行
	void CopyLine(int srLine, int descLine);
	void initminRow();//消行后重置minRow数组的值
private:
	unsigned int score;//保存所得分数
	BOOL fOver;//游戏是否结束
	int maxLine;//记录最高行数
public:
	unsigned int GetScore()
	{
		return score;
	}
	BOOL GetGameState()//得知游戏是否结束
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