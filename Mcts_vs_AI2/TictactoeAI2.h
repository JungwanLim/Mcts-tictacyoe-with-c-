#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
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
	int winner = 0;
	int checkedCell = 0;
	int bestPosition[2];
	const int maxDepth = 9; // c++ 11부터는 class안에서 초기화가 가능하다.

public:
	CBoard() { ; }
	~CBoard() {	}

	CBoard& operator = (const CBoard& obj);

	void InitBoard();
	void InitData();
	void SetCell(int position);
	void SetZero(int position);
	void SetPlayer(int player);
	void GetEmptyPosition(int* emptyPosition);
	void SetBestPosition(int pos, int score);
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
	int  Evaluation();
	int  MinimaxAlphaBeta(int depth, int player, int alpha, int beta);
	int  GetBestPosotion();
	int  GetMaxDepth() { return maxDepth; }
	int  GetTurn();
	int  GetMove();

	double  GetResult(int player);
	
	Point GetPoint(int position);
};


class CNode {
private:
	int turn = 0;
	int move = -1;
	int child_size = 0;
	int empty_cells[10];

	double wins = 0;
	double visits = 0;

	CNode*  parent = nullptr;
	CNode** child_nodes = nullptr;

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

	//bool Compare(CNode*, CNode*);

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
	CNode* pNode;
	CNode* pRoot;

public:
	CMCTS();
	~CMCTS();

	int  Mcts(CBoard* pBoard, int iter_num);

	double  Simulation();

	void Selection();
	void Expantion();
	void BackPropagation(double result);
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
private:
	CMCTS* pMcts;
public:
	CUser(CMCTS* ptrMcts) {
		ID = USER;
		pMcts = ptrMcts;
	}
	virtual int Action();
};

class CAI : public CPlayer {
public:
	CAI() { ID = COMPUTER; }
	virtual int Action();
};

class CTictactoe {
private:
	CBoard board;
	CTools tools;
	CMCTS mcts;
	CUser user = CUser(&mcts);
	CAI AI;
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

	int count[3] = { 0, 0, 0 };
};
