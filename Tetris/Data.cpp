#include "Data.h"
#include <vector>
#include <algorithm>

using namespace std;

Teris::Teris(Teris &teris)
{
	fOver = FALSE;
	rect = teris.rect;
	pDC = NULL;
	minRow = NULL;
	Init();
	pDC = new char*[rect.bottom-rect.top];
	for (int loop=0; loop<rect.bottom-rect.top; ++loop)
	{
		pDC[loop] = new char[rect.right-rect.left];
	}
	minRow = new short[rect.right-rect.left];
	for (int loop1=0; loop1!=rect.bottom-rect.top; ++loop1)
	{
		for (int loop2=0; loop2!=rect.right-rect.left; ++loop2)
		{
			pDC[loop1][loop2] = teris.pDC[loop1][loop2];
		}
	}

	for (int loop=0; loop!=rect.right-rect.left; ++loop)
	{
		minRow[loop] = teris.minRow[loop];
	}
	maxLine = teris.maxLine;
	nowPt = teris.nowPt;
	nowShape = teris.nowShape;
	nextShape = teris.nextShape;
	ptr = teris.ptr;
	nextPtr = teris.nextPtr;
	score = teris.score;
}
void Teris::InitGame()
{
	maxLine = rect.bottom - rect.top;
	fOver = FALSE;
	score = 0;
	fState = FALSE;
	SetNextShape();
	ptr = nextPtr;
	nowShape = nextShape;
	SetNextShape();
	nowPt.x = (rect.right - rect.left) / 2 - ptr->maxC;
	nowPt.y = -(ptr->maxR+1);

	for (int loop1=0; loop1!=rect.bottom-rect.top; ++loop1)
	{
		for (int loop2=0; loop2!=rect.right-rect.left; ++loop2)
		{
			pDC[loop1][loop2] = 0;
		}
	}

	for (int loop=0; loop!=rect.right-rect.left; ++loop)
	{
		minRow[loop] = (short)(rect.bottom-rect.top);
	}

}
Teris::Teris(RECT rect):rect(rect)
{
	srand((unsigned short)time(NULL));
	pDC = NULL;
	Init();
	pDC = new char*[rect.bottom-rect.top];
	for (int loop=0; loop<rect.bottom-rect.top; ++loop)
	{
		pDC[loop] = new char[rect.right-rect.left];
	}
	minRow = new short[rect.right-rect.left];
	InitGame();
}

Teris::~Teris()
{
	if (pDC)
	{
		for (int loop=0; loop!=rect.bottom-rect.top; ++loop)
		{
			delete [] pDC[loop];
		}
		delete [] pDC;
	}
	if (minRow)
		delete [] minRow;
}

void Teris::Init()
{
	ShapeCount = 0;
	{
		char minRow[4][4] = {1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0};
		char maxRow[4][4] = {1, 2, 1, 0, 1, 2, 0, 0, 1, 1, 1, 0, 0, 2, 1, 0};
		char posT[4][4][4] = {{0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0}, 
		{0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, 
		{0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0}};
		sT[0].InitShape(posT[0], 1, 2, 0, 2, maxRow[0], minRow[0]);
		sT[1].InitShape(posT[1], 0, 2, 0, 1, maxRow[1], minRow[1]);
		sT[2].InitShape(posT[2], 0, 1, 0, 2, maxRow[2], minRow[2]);
		sT[3].InitShape(posT[3], 0, 2, 1, 2, maxRow[3], minRow[3]);
	}

	{
		char minRow[4][4] = {0, 1, 1, 0, 1, 1, 1, 0, 0, 2, 0, 0, 1, 2, 2, 0};
		char maxRow[4][4] = {0, 3, 1, 0, 1, 1, 2, 0, 0, 2, 2, 0, 2, 2, 2, 0};
		char posL[4][4][4] = {{0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0}, 
		{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, 
		{0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0}};
		sL[0].InitShape(posL[0], 1, 3, 1, 2, maxRow[0], minRow[0]);
		sL[1].InitShape(posL[1], 1, 2, 0, 2, maxRow[1], minRow[1]);
		sL[2].InitShape(posL[2], 0, 2, 1, 2, maxRow[2], minRow[2]);
		sL[3].InitShape(posL[3], 1, 2, 0, 2, maxRow[3], minRow[3]);
	}

	{
		char minRow[4][4] = {1, 1, 2, 0, 0, 1, 0, 0, 1, 1, 2, 0, 0, 1, 0, 0};
		char maxRow[4][4] = {1, 2, 2, 0, 0, 2, 1, 0, 1, 2, 2, 0, 0, 2, 1, 0};
		char posZ[4][4][4] = {{0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0}, 
		{0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0}, 
		{0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0}};
		sZ[0].InitShape(posZ[0], 1, 2, 0, 2, maxRow[0], minRow[0]);
		sZ[1].InitShape(posZ[1], 0, 2, 1, 2, maxRow[1], minRow[1]);
		sZ[2].InitShape(posZ[2], 1, 2, 0, 2, maxRow[2], minRow[2]);
		sZ[3].InitShape(posZ[3], 0, 2, 1, 2, maxRow[3], minRow[3]);
	}

	{
		char minRow[2][4] = {0, 0, 0, 0, 1, 1, 1, 1};
		char maxRow[2][4] = {0, 0, 3, 0, 1, 1, 1, 1};
		char posI[2][4][4] = {{0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
		{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};
		sI[0].InitShape(posI[0], 0, 3, 2, 2, maxRow[0], minRow[0]);
		sI[1].InitShape(posI[1], 1, 1, 0, 3, maxRow[1], minRow[1]);
		sI[2].InitShape(posI[0], 0, 3, 2, 2, maxRow[0], minRow[0]);
		sI[3].InitShape(posI[1], 1, 1, 0, 3, maxRow[1], minRow[1]);
	}

	{
		char minRow[4] = {0, 1, 1, 0};
		char maxRow[4] = {0, 2, 2, 0};
		char posO[4][4] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0};
		sO[0].InitShape(posO, 1, 2, 1, 2, maxRow, minRow);
		sO[1].InitShape(posO, 1, 2, 1, 2, maxRow, minRow);
		sO[2].InitShape(posO, 1, 2, 1, 2, maxRow, minRow);
		sO[3].InitShape(posO, 1, 2, 1, 2, maxRow, minRow);
	}
	{
		char minRow[2][4] = {2, 1, 1, 0, 0, 0, 1, 0};
		char maxRow[2][4] = {2, 2, 1, 0, 0, 1, 2, 0};
		char posRZ[2][4][4] = {{0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}};
		sRZ[0].InitShape(posRZ[0], 1, 2, 0, 2, maxRow[0], minRow[0]);
		sRZ[1].InitShape(posRZ[1], 0, 2, 1, 2, maxRow[1], minRow[1]);
		sRZ[2].InitShape(posRZ[0], 1, 2, 0, 2, maxRow[0], minRow[0]);
		sRZ[3].InitShape(posRZ[1], 0, 2, 1, 2, maxRow[1], minRow[1]);
	}
	{
		char minRow[4][4] = {0, 1, 1, 0, 2, 2, 1, 0, 0, 1, 3, 0, 1, 1, 1, 0};
		char maxRow[4][4] = {0, 1, 3, 0, 2, 2, 2, 0, 0, 3, 3, 0, 2, 1, 1, 0};
		char posRL[4][4][4] = {{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0},
		{0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0}};
		sRL[0].InitShape(posRL[0], 1, 3, 1, 2, maxRow[0], minRow[0]);
		sRL[1].InitShape(posRL[1], 1, 2, 0, 2, maxRow[1], minRow[1]);
		sRL[2].InitShape(posRL[2], 1, 3, 1, 2, maxRow[2], minRow[2]);
		sRL[3].InitShape(posRL[3], 1, 2, 0, 2, maxRow[3], minRow[3]);
	}
}

void Teris::Shape::InitShape(char pPos[][4], char min1, char max1, 
							 char min2, char max2, char maxRo[], char minRo[])
{
	minR = min1;
	maxR = max1;
	minC = min2;
	maxC = max2;
	for (int loop1=0; loop1!=4; ++loop1)
	{
		for (int loop2=0; loop2!=4; ++loop2)
		{
			pos[loop1][loop2] = pPos[loop1][loop2];
		}
		maxRow[loop1] = maxRo[loop1];
		minRow[loop1] = minRo[loop1];
	}
}

bool Teris::isTDLegal(POINT pt)
{
	if (ptr==NULL)
		return FALSE;

	if (nowPt.y+ptr->maxR>=rect.bottom-rect.top)
	{
		fState = FALSE;
		return FALSE;
	}
    for (char loop=ptr->minC; loop<=ptr->maxC; ++loop)
    {
		int temp = minRow[nowPt.x+loop] - (ptr->maxRow[loop]+nowPt.y);
		if (temp>1)
		{
			continue;
		}
		if (temp<0)
		{
			char index;
			for (index=ptr->minRow[loop]; 
				index<=ptr->maxRow[loop]; 
			    ++index)
			{
				if ((ptr->pos[index][loop]+pDC[nowPt.y+index][loop+nowPt.x])>1)
				{
					fState = FALSE;
					return FALSE;
				}	
			}
			if (ptr->maxR+nowPt.y==rect.bottom-rect.top-1)
			{
				fState = TRUE;
//				return TRUE;
			}
			if (nowPt.y+index>=rect.bottom-rect.top||pDC[nowPt.y+index][loop+nowPt.x])
			{
				fState = TRUE;
			}
		}
		else if (temp==0)
		{
			fState = FALSE;
			return FALSE;
		}
		else if (temp==1)
		{
			fState = TRUE;
		}
    }

	return TRUE;
}

bool Teris::isLRLegal(POINT pt)
{
	if (ptr==NULL)
		return FALSE;

	if (rect.left-nowPt.x>ptr->minC || nowPt.x+ptr->maxC>=rect.right)
	{
		return FALSE;
	}

	if (isTDLegal(pt))
	{
		return TRUE;
	}
	
	return FALSE;
}

void Teris::Configure()
{
// 	for (int loop1=ptr->minC; loop1<=ptr->maxC; ++loop1)
// 	{
// 		for (int loop2=ptr->minR; loop2<=ptr->maxR; ++loop2)
// 		{
// 			if (nowPt.y+loop2>=0)
// 			{
// 			//	pDC[nowPt.y+loop2][loop1+nowPt.x] |= ptr->pos[loop2][loop1];
// 				pDC[nowPt.y+loop2][loop1+now]
// 			}
// 		}
// 	}
	for (int loop1=ptr->minR; loop1<=ptr->maxR; ++loop1)
	{
		for (int loop2=ptr->minC; loop2<=ptr->maxC; ++loop2)
		{
			if (nowPt.y+loop1>=0&&ptr->pos[loop1][loop2])
			{
				pDC[nowPt.y+loop1][loop2+nowPt.x] = 1;
			}
		}
	}
}

char** Teris::GetpDC()
{
	return pDC;
}

BOOL Teris::Up()
{
	if (fOver)
	{
		return FALSE;
	}
	Shape *temp = ptr;
	ptr = (ptr-nowShape+(nowShape+1)%4);
    if (isLRLegal(nowPt))
    {
		nowShape = (nowShape+1) % 4;
// 		if (fState)
// 		{
// 			Change();
// 		}
		return TRUE;
    }
	ptr = temp;
	return FALSE;
}

BOOL Teris::Down()
{
	if (fOver)
	{
		return FALSE;
	}
	++nowPt.y;

	if (isTDLegal(nowPt))
	{
		return TRUE;
	}
	--nowPt.y;
	return FALSE;
}

BOOL Teris::Left()
{
	if (fOver)
	{
		return FALSE;
	}
	--nowPt.x;
	if (isLRLegal(nowPt))
	{
		return TRUE;
	}
	++nowPt.x;
	return FALSE;
}

BOOL Teris::Right()
{
	for (int loop=0; loop<rect.right; ++loop)
	{
		minRow[loop] = minRow[loop];
	}
	if (fOver)
	{
		return FALSE;
	}
	++nowPt.x;
	if (isLRLegal(nowPt))
	{
		return TRUE;
	}
	--nowPt.x;
	return FALSE;
}

void Teris::Change()
{
	Configure();
	for (int loop=ptr->minC; loop<=ptr->maxC; ++loop)
	{
		if (minRow[nowPt.x+loop]>(nowPt.y + ptr->minRow[loop]))
		{
			minRow[nowPt.x+loop] = (short)nowPt.y + ptr->minRow[loop];
			if (minRow[nowPt.x+loop]<maxLine)
			{
				maxLine = minRow[nowPt.x+loop];
			}
		}
	}
	for (int loop=0; loop<rect.right-rect.left; ++loop)
	{
		if (minRow[loop]<=0)
		{
			fOver = TRUE;
			return;
		}
	}
//	int clearLines = 0;//记录消除行数
	vector<int> clearLines;//记录要消除的行

	for (int loop=ptr->minR; loop<=ptr->maxR; ++loop)
	{
		if (isClearLine(loop+nowPt.y))
		{
		//	++clearLines;
		//	int line = loop + nowPt.y;
			clearLines.push_back(loop+nowPt.y);
// 			while (line>0)
// 			{
// 				CopyLine(line-1, line);
// 				--line;
// 			}
			//最后一行清零
// 			for (int loop=0; loop<rect.right-rect.left; ++loop)
// 			{
// 				pDC[line][loop] = 0;
// 			}
		}
	}
	clearLineCount = clearLines.size();
	if (clearLineCount>0)
	{
		int descCopyLine = clearLines[0];
		int srCopyLine = descCopyLine - 1;
		int endLine = maxLine;
		for (int loop=1; loop<clearLines.size(); ++loop)
		{
			int line = clearLines[loop];
			int count = line - descCopyLine;
			if (count==1)
			{
				descCopyLine = line;
				continue;
			}
			else
			{
				while (srCopyLine>=endLine)
				{
					CopyLine(srCopyLine--, descCopyLine--);
				}
				endLine = descCopyLine + 1;
				descCopyLine = line;
				srCopyLine = line - 1;
			}
		}

		while (srCopyLine>=endLine)
		{
			CopyLine(srCopyLine--, descCopyLine--);
		}
		for (int loop1=maxLine; loop1<clearLines.size()+maxLine; ++loop1)
		{
			for (int loop2=0; loop2<rect.right-rect.left; ++loop2)
			{
				pDC[loop1][loop2] = 0;
			}
		}
		initminRow();
		ComputeScore(clearLines.size());
	}
    
	nowShape = nextShape;
	ptr = nextPtr;
	
	nowPt.x = (rect.right - rect.left) / 2 - ptr->maxC;
	nowPt.y = -(ptr->maxR+1);
	fState = FALSE;
	SetNextShape();
}

BOOL Teris::isClearLine(int line)
{
	if (line<0)
	{
		return FALSE;
	}
	else
	{
		int lineSize = rect.right - rect.left;
		for (int loop=0; loop<lineSize; ++loop)
		{
			if (!pDC[line][loop])
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

void Teris::CopyLine(int srLine, int descLine)
{
	if (srLine<0||descLine<0)
	{
		return;
	}
	else
	{
		int lineSize = rect.right - rect.left;
		for (int loop=0; loop<lineSize; ++loop)
		{
			pDC[descLine][loop] = pDC[srLine][loop];
		}
	}
}

void Teris::initminRow()
{
	for (int loop1=0; loop1<rect.right-rect.left; ++loop1)
	{
		int loop2;
		for (loop2=0; loop2<rect.bottom-rect.top; ++loop2)
		{
			if (pDC[loop2][loop1])
			{
				minRow[loop1] = loop2;
				break;
			}
		}
		minRow[loop1] = loop2;
	}
}

void Teris::SetNextShape()
{
	++ShapeCount;
	nextShape = rand() % 4;
	switch(rand() % (RZ+1))
	{
	case T:
		nextPtr = sT + nextShape;
		break;
	case L:
		nextPtr = sL + nextShape;
		break;
	case Z:
		nextPtr = sZ + nextShape;
		break;
	case I:
		nextPtr = sI + nextShape;
		break;
	case O:
		nextPtr = sO + nextShape;
		break;
	case RL:
		nextPtr = sRL + nextShape;
		break;
	case RZ:
		nextPtr = sRZ + nextShape;
		break;
	default:
		nextPtr = NULL;
	}
}

void Teris::Check()
{
// 	if (fState)
// 	{
// 		Change();
// 	}
	Change();
}

AI_OFFSET Teris::AI(Teris &teris)
{
	typedef vector<pair<AI_OFFSET, int> > AI_SCORE;
	AI_OFFSET ao;
	AI_SCORE list;
//	temp->nowPt = 0;
	Teris temp(*this);
	for (int loop=0; loop<4; ++loop)
	{
	//	int count = rect.right - rect.left - (((ptr-(int)nowShape+loop)->maxC) - ((ptr-(int)nowShape+loop)->minC) + 1);
		int pos = -(ptr-(int)nowShape+loop)->minC;
		while (pos+(ptr-(int)nowShape+loop)->maxC<rect.right - rect.left)
		{
			int score = 0;		
			temp.ptr = temp.ptr - nowShape + loop;
			temp.nowPt.x = pos++;
			int nowX = temp.nowPt.x;
			int offset = temp.rect.bottom;
			for (int loop1=temp.ptr->minC; loop1<=temp.ptr->maxC; ++loop1)
			{
				if (temp.minRow[temp.nowPt.x+loop1]-temp.ptr->maxRow[loop1]-1<offset)
				{
					offset = temp.minRow[temp.nowPt.x+loop1]-temp.ptr->maxRow[loop1]-1;
				}
			}
			if (offset<(rect.bottom-rect.top)*3/5)
			{
				score -= ((rect.bottom-rect.top)*3/5 - offset) * 2;
			}
			int l = nowX + temp.ptr->minC;
			int r = nowX + temp.ptr->maxC;
			temp.nowPt.y = offset;
			temp.Change();
			if (!temp.fOver)
			{
				score += (pow(temp.clearLineCount, 2.0) - (temp.clearLineCount - 1));
			//	score += temp.clearLineCount * 2;
				for (int loop2=l; loop2<=r; ++loop2)
				{
					for (int loop3=temp.minRow[loop2]; loop3<rect.bottom-rect.top; ++loop3)
					{
						if (temp.pDC[loop3][loop2]==0)
						{
							score -= 4;
						}
					}
					for (int loop3=minRow[loop2]; loop3<rect.bottom-rect.top; ++loop3)
					{
						if (pDC[loop3][loop2]==0)
						{
							score += 4;
						}
					}
				}
				int count = 0;
				for (int loop2=temp.minRow[1]; loop2<=rect.bottom-rect.top; ++loop2)
				{
					if (loop2==rect.bottom-rect.top||temp.pDC[loop2][0]==1)
					{
						while (count>0)
						{
							score -= count--;
						}
						continue;
					}
					count++;
				}
				for (int loop2=0; loop2<rect.right-rect.left-2; ++loop2)
				{
					int maxR = temp.minRow[loop2]>=temp.minRow[loop2+2] ? 
						temp.minRow[loop2] : temp.minRow[loop2+2];
					for (int loop3=maxR; loop3<=rect.bottom-rect.top; ++loop3)
					{
						if (loop3==rect.bottom-rect.top||temp.pDC[loop3][loop2+1]==1)
						{
							while (count>0)
							{
								score -= count--;
							}
							continue;
						}
						++count;
					}
				}
				for (int loop2=temp.minRow[rect.right-rect.left-2]; 
					loop2<=rect.bottom-rect.top; ++loop2)
				{
					if (loop2==rect.bottom-rect.top ||
						temp.pDC[loop2][rect.right-rect.left-1]==1)
					{
						while (count>0)
						{
							score -= count--;
						}
						continue;
					}
					++count;
				}
				for (int loop2=temp.maxLine; loop2<rect.bottom-rect.top; ++loop2)
				{
					int flag = temp.pDC[loop2][0];
					for (int loop3=1; loop3<rect.right-rect.left; ++loop3)
					{
						if (flag!=temp.pDC[loop2][loop3])
						{
							score -= 1;
							flag = !flag;
						}
					}
				}
				for (int loop2=0; loop2<rect.right-rect.left; ++loop2)
				{
					if (temp.minRow[loop2]<18)
					{
						int flag = temp.pDC[temp.minRow[loop2]][loop2];
						for (int loop3=temp.minRow[loop2]+1; loop3<rect.bottom-rect.top; ++loop3)
						{
							if (flag!=temp.pDC[loop3][loop2])
							{
								score -= 1;
								flag = !flag;
							}
						}
					}
				}
			}
			else
				score = -10000;


			ao.offsetX = nowX;
			ao.shape = loop;
			ao.offsetY = offset;
			list.push_back(make_pair(ao, score));
			temp.ResetTeris(*this);
		}
	}
	vector<pair<AI_OFFSET, int> > listtemp;
	stable_sort(list.begin(), list.end(), CompareAIScore);
	int loop = list.size() - 1;
	int priority;
	while (loop>=0 && list[loop].second==list.at(list.size()-1).second)
	{
		if (list[loop].first.offsetX<=(rect.right - rect.left)/2-ptr->maxC)
		{
			priority = ((rect.right - rect.left)/2-ptr->maxC - list[loop].first.offsetX) * 100
				+ 10 + (list[loop].first.shape>=nowShape?(list[loop].first.shape-nowShape):(4-nowShape+list[loop].first.shape));
		}
		else
			priority = (list[loop].first.offsetX - (rect.right - rect.left)/2-ptr->maxC) * 100
			+ (list[loop].first.shape>=nowShape?(list[loop].first.shape-nowShape):(4-nowShape+list[loop].first.shape));
		listtemp.push_back(make_pair(list[loop].first, priority));
		--loop;
	}
	stable_sort(listtemp.begin(), listtemp.end(), CompareAIScore);
	return listtemp.at(listtemp.size()-1).first;
}

void Teris::ResetTeris(Teris &teris)
{
	fOver = teris.fOver;
	rect = teris.rect;

	for (int loop1=0; loop1!=rect.bottom-rect.top; ++loop1)
	{
		for (int loop2=0; loop2!=rect.right-rect.left; ++loop2)
		{
			pDC[loop1][loop2] = teris.pDC[loop1][loop2];
		}
	}

	for (int loop=0; loop!=rect.right-rect.left; ++loop)
	{
		minRow[loop] = teris.minRow[loop];
	}
	maxLine = teris.maxLine;
	nowPt = teris.nowPt;
	nowShape = teris.nowShape;
	nextShape = teris.nextShape;
	ptr = teris.ptr;
	nextPtr = teris.nextPtr;
	score = teris.score;
}

BOOL CompareAIScore(const pair<AI_OFFSET, int> &l, const pair<AI_OFFSET, int> &r)
{
	return l.second < r.second;
}