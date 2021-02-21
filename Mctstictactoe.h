#pragma once

#include <iostream>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#pragma warning(disable:4996)

using namespace std;

enum {
	EMPTY,
	COMPUTER,
	USER,
	BOARDSIZE,
	INF = 1000
};

enum {
	DRAW,
	COMPUTERWIN,
	USERWIN,
	INPUTPOSITION,
	OUTOFRANGE,
	CHECKEDPOSITION,
	CONTINUE,
	COMPUTERORDER,
	USERORDER,
};

static const char symbol[3] = { '_', 'X', 'O' };

struct Point {
	int x, y;

	Point() { ; }
	Point(int ax, int ay) {
		x = ax;
		y = ay;
	}
};

class CBoard {
private:
	int board[BOARDSIZE][BOARDSIZE];
	int currentPosition = -1;
	int player;
	int checkedCell = 0;

public:
	CBoard() { ; }
	~CBoard() {	}

	CBoard& operator = (const CBoard& obj);

	void InitBoard();
	void InitData();
	void SetCell(int position);
	void SetPlayer(int player);
	void GetEmptyPosition(int* emptyPosition);
	void GetEmptyCells(vector<int>&);
	void CopyBoard(const int b[][3]);
	void PrintBoard();

	bool isFill(int position);
	bool isWin();
	bool isSamePlayer(Point p, int player);
	bool isDraw();
	bool isSame(int player);
	bool isFinish();

	int  GetCount(const Point direction[2], int position, int player);
	int  GetTurn();
	int  GetCellCount();

	double  GetResult(int player);
	
	Point GetPoint(int position);
};


class CNode {
private:
	int turn = 0;
	int move = -1;

	double wins = 0;
	double visits = 0;

	CNode* parent = nullptr;
	vector<int> empty_cells;
	vector<CNode*> child_nodes;

public:
	CNode() { ; }
	CNode(int move, CBoard* board, CNode* parent = nullptr);
	~CNode();

	int GetBestMove();
	int GetMove();
	int GetNextMove(CBoard* pBoard);
	int GetEmptyCellsSize();
	int GetChildNodesSize();
	int GetTurn();

	void SetWins(double n);
	void SetVisits();

	double GetVists();
	double GetUCB(CNode*);

	CNode* GetParent();
	CNode* GetChildNode();
	CNode* AddChildNode(CBoard* pBoard);
};

class CMCTS {
private:
	CBoard  board;
	CBoard* pBoard = &board;
	CNode*  pNode;
	CNode*  pRoot;

public:
	CMCTS();
	~CMCTS();

	int  Mcts(CBoard* pBoard, int iter_num);

	void Selection();
	void Expantion();
	void BackPropagation(double result);

	double Simulation();
};

// 입출력 도구 
class CTools {
private:
	char strNumber[10];

public:
	CTools() { ; }
	~CTools() { ; }
	void InitStrNumber() { strncpy(strNumber, "123456789", 10); }
	void SetPosition(int position, int player) { strNumber[position] = symbol[player]; }
	void PrintBoard();
	void ShowMessage(int msgType);
	bool isFill(int position);
	int  GetPosition();
};

class CPlayer {
public:
	static CBoard* pBoard;
	static CTools* pTools;

	int ID;

public:
	CPlayer() {	}
	virtual ~CPlayer() {  }
	virtual int Action() = 0;

	void SetMember(CBoard* ptrBoard, CTools* ptrTools) {
		pBoard = ptrBoard;
		pTools = ptrTools;
	}
};
CBoard* CPlayer::pBoard = NULL; // class변수는 class외부에서 초기화를 해야한다.
CTools* CPlayer::pTools = NULL;

class CUser : public CPlayer {
public:
	CUser() {
		ID = USER;
	}
	virtual int Action();
};

class CAI : public CPlayer {
private:
	CMCTS* pMcts;
public:
	CAI(CMCTS* ptrMcts) 
	{
		ID = COMPUTER; 
		pMcts = ptrMcts;
	}
	virtual int Action();
};

class CTictactoe {
private:
	CBoard board;
	CTools tools;
	CMCTS mcts;
	CUser user;
	CAI AI = CAI(&mcts);
	CPlayer* Player[2] = { &AI, &user }; // 두 player를 배열에 넣어서 분기를 없앴다.

public:
	CTictactoe() {
		user.SetMember(&board, &tools);
	}
	~CTictactoe() { }
	bool isFinish(int player);
	bool isContinue();
	void RandomPlay(int player);
	void ShowBoard(int position, int player);
	void InitGame();
	void PlayGame();
	CPlayer* GetPlayer();
};
