#include "Mctstictactoe.h"

void CBoard::CopyBoard(const int b[][3])
{
	for (int i = 0; i < BOARDSIZE; ++i)
	{
		for (int j = 0; j < BOARDSIZE; ++j)
		{
			this->board[i][j] = b[i][j];
		}
	}
}

void CBoard::PrintBoard()
{
	for (auto& row : board)
	{
		for (auto& col : row)
		{
			cout << col << " ";
		}
		cout << endl;
	}
	cout << endl;
}

CBoard& CBoard::operator = (const CBoard& obj)
{
	//this->checkedCell = obj.checkedCell;
	//this->currentPosition = obj.currentPosition;
	//this->winner = obj.winner;
	//this->player = obj.player;
	//this->CopyBoard(obj.board);
	memcpy(this, &obj, sizeof(CBoard));
	return *this;
}


void CBoard::InitBoard()
{
	for (auto& row : board)
	{
		for (auto& col : row)
		{
			col = 0;
		}
	}
}

// 처음 게임이 시작 될때나, 다시 시닥할 때 반드시 초기화가 필요한 데이터들을 초기화해준다.
void CBoard::InitData()
{
	InitBoard();
	currentPosition = 0; //매수 둘 때마다 바뀐 위치를 저장
	checkedCell = 0; // 현재 몇 수까지 두었나 기록
}

// 1 ~ 9까지의 위치를 좌표로 변환하여 반환
Point CBoard::GetPoint(int position)
{
	Point p;
	p.x = position % BOARDSIZE;
	p.y = position / BOARDSIZE;
	return p;
}

// 한 수를 두었을 때 보드에 기록
void CBoard::SetCell(int position)
{
	player = 3 - player;
	Point p = GetPoint(position); //위치를 좌표로 받아온다.
	board[p.y][p.x] = player; //좌표에 현재 둔 player의 ID를 기록
	checkedCell++; // 한 수 두었으니 하나 증가
	currentPosition = position; // 현재 둔 위치 저장
}

int CBoard::GetCellCount()
{
	return checkedCell;
}

void CBoard::SetPlayer(int player)
{
	this->player = player;
}

int CBoard::GetTurn()
{
	return player;
}

bool CBoard::isFill(int position)
{
	Point p = GetPoint(position);
	return board[p.y][p.x];
}

// 승리 판단할 때 같은 돌이 연속인지를 판단하기 위하여 필요
bool CBoard::isSamePlayer(Point p, int player)
{
	return board[p.y][p.x] == player;
}

// 빈 곳(둘 수 있는 곳)의 위치를 찾아서 매개변수로 넘어온 배열에 저장하고, 마지막 인덱스에는 몇 개를 저장하였는지 그 수를 기록
void CBoard::GetEmptyPosition(int* emptyPosition)
{
	int index = 0;
	for (int i = 0; i < BOARDSIZE; ++i)
	{
		for (int j = 0; j < BOARDSIZE; ++j)
		{
			if (board[i][j] == 0)
			{
				emptyPosition[index++] = i * BOARDSIZE + j;
			}
		}
	}
	emptyPosition[9] = index; //배열에 빈 곳이 몇 개인지 알려주기 위하여 사용
}

// 빈 곳(둘 수 있는 곳)의 위치를 찾아서 매개변수로 넘어온 배열에 저장하고, 마지막 인덱스에는 몇 개를 저장하였는지 그 수를 기록
void CBoard::GetEmptyCells(vector<int>& vi)
{
	for (int i = 0; i < BOARDSIZE; ++i)
	{
		for (int j = 0; j < BOARDSIZE; ++j)
		{
			if (board[i][j] == 0)
			{
				vi.push_back(i * BOARDSIZE + j);
			}
		}
	}
}

// 두어진 위치에서 한 줄(가로, 세로 또는 대각선)에 몇개의 돌이 연속으로 놓여있나 수를 해아려 반환
// 한 점이 놓였을 때 한 줄이란 놓여진 돌을 기준으로 좌, 우 또는 상, 하등 두 군대를 체크해봐야 하므로 
// 한 줄에 몇개의 연속된 돌이 있는지 판단하기 위해서는 두번의 검사가 필요.
int CBoard::GetCount(const Point dir[2], int position, int player)
{
	int count = 1;// 한 수를 두었기 때문에 1부터 시작
	for (int i = 0; i < 2; ++i)
	{
		Point p = GetPoint(position);
		p.x = p.x + dir[i].x;
		p.y = p.y + dir[i].y;
		while (true)
		{
			// Range를 벗어 났을 경우 
			if (p.x < 0 or p.x >= BOARDSIZE or p.y < 0 or p.y >= BOARDSIZE)
			{
				break;
			}
			else if (!player || !isSamePlayer(p, player)) // 같은 돌이 아닐 경우
			{
				break;
			}
			else // 바로 이전 수의 한칸 옆이 같은 돌일 때
			{
				count++; // 연속된 돌의 수를 하나 증가
				p.x = p.x + dir[i].x; // 다음 칸으로 이동하여 위의 과정을 반복
				p.y = p.y + dir[i].y;
			}
		}
	}
	return count;
}

bool CBoard::isSame(int player)
{
	return this->player == player;
}

bool CBoard::isDraw()
{
	return checkedCell == 9;
}

bool CBoard::isFinish()
{
	return isWin() || isDraw();
}

bool CBoard::isWin()
{
	Point Left(-1, 0);
	Point Right(1, 0);
	Point Up(0, 1);
	Point Down(0, -1);
	Point LeftDown(-1, -1);
	Point RightUp(1, 1);
	Point LeftUp(-1, 1);
	Point RightDown(1, -1);

	Point direction[4][2] = {
		{Left, Right}, // 가로방향 체크 
		{Up, Down},  // 세로방향 체크 
		{LeftDown, RightUp}, // 대각선 1 
		{LeftUp, RightDown } }; // 대각선 2 

	for (const auto& dir : direction) // C++11부터 적용된 편리한 반복기능
	{
		// 한 쌍(좌, 우 또는 상, 하 등등)의 포인트 배열을 보내어 결과를 받았을 때 3이면 승리
		if (GetCount(dir, currentPosition, player) == BOARDSIZE)
		{
			return true;
		}
	}
	return false;
}

//mcts에서 simulation 결과를 업데이트하기 위하여 점수를 계산
double CBoard::GetResult(int player)
{
	if (isWin())
	{
		if (this->player == player)
		{
			return 1.0;
		}
		else
		{
			return 0.0;
		}
	}
	else if (isDraw())
	{
		return 0.5;
	}
}

//노드를 생성할 때 대입이 필요한 변수들의 초기화
CNode::CNode(int move, CBoard* pBoard, CNode* parent)
{
	this->move = move;
	this->parent = parent;
	this->turn = pBoard->GetTurn();
	pBoard->GetEmptyCells(this->empty_cells);
}

//할당된 메모리 해제
CNode::~CNode()
{
	int cnt = 0;
	this->empty_cells.clear();
	if (this->child_nodes.size())
	{
		for (auto pNode : this->child_nodes)
		{
			delete pNode;
			pNode = nullptr;
		}
	}
	this->child_nodes.clear();
}


// 확장과정에서 child를 생성
CNode* CNode::AddChildNode(CBoard* pBoard)
{
	int move = this->empty_cells[empty_cells.size() - 1];
	this->empty_cells.pop_back();
	pBoard->SetCell(move);

	CNode* child = new CNode(move, pBoard, this);
	this->child_nodes.push_back(child);

	return child;
}

//UCB 값을 계산하는 공식
double CNode::GetUCB(CNode* c)
{
	return (c->wins / c->visits) + sqrt(2 * log(visits) / c->visits);
}

//다음과 같은 방법으로 UCB 값이 높은 child를 찾을 수 있지만 속도가 많이 느려서 아래의 방법 채택
//CNode* CNode::GetChildNode()
//{
//	vector<double> vd;
//	vector<double>::iterator it;
//
//	for (auto& c : this->child_nodes)
//	{
//		double ucb = GetUCB(c);
//		vd.push_back(ucb);
//	}
//
//	it = max_element(vd.begin(), vd.end());
//	int best = distance(vd.begin(), it);
//
//	return this->child_nodes[best];
//}

//UCB 값이 높은 child를 선택
CNode* CNode::GetChildNode()
{
	double ucb, max = -100;
	int idx = 0, index = 0;
	for (auto& c : this->child_nodes)
	{
		ucb = GetUCB(c);
		if (max < ucb)
		{
			max = ucb;
			idx = index;
		}
		index++;
	}

	return this->child_nodes[idx];
}

CNode* CNode::GetParent()
{
	return this->parent;
}

void CNode::SetWins(double n)
{
	wins += n;
}

void CNode::SetVisits()
{
	visits += 1.0;
}

double CNode::GetVists()
{
	return this->visits;
}

int CNode::GetMove()
{
	return this->move;
}

//simulation 과정에서 빈셀들을 가져와 무작위로 하나를 반환
int CNode::GetNextMove(CBoard* pBoard)
{
	int pos[10];
	pBoard->GetEmptyPosition(pos);
	int move = rand() % pos[9];

	return pos[move];
}

int CNode::GetEmptyCellsSize()
{
	return this->empty_cells.size();
}

int CNode::GetChildNodesSize()
{
	return this->child_nodes.size();
}

//child 노드의 최대 visits 값을 찾기위한 함수, 내장함수로 하는 것은 몰라서 외부함수로 만듬
static bool Compare(CNode* a, CNode* b)
{
	return a->GetVists() < b->GetVists();
}

//child 노드 중 visits 값이 최대인 child를 찾는 함수
int CNode::GetBestMove()
{
	vector<CNode*>::iterator it;

	it = max_element(this->child_nodes.begin(), this->child_nodes.end(), Compare);
	int best = distance(this->child_nodes.begin(), it);

	//child들의 정보를 출력
	for (auto& c : this->child_nodes)
	{
		cout << c->move + 1 << ", ";
		cout << setw(5) << c->wins << ", ";
		cout << setw(4) << c->visits << endl;
	}

	return this->child_nodes[best]->move;
}

int CNode::GetTurn()
{
	return this->turn;
}

CMCTS::CMCTS()
{
}

CMCTS::~CMCTS()
{
}

//MCTS에서 실제로 탐색을 위한 함수
int CMCTS::Mcts(CBoard* ptrBoard, int iter_num)
{
	pRoot = new CNode(-1, ptrBoard);
	for (int i = 0; i < iter_num; ++i)
	{
		pNode = pRoot;//노드가 확장하는 중에 child로 바뀌기 때문에 처음 시작할 때는 항상 Root로 복귀가 필요
		board = *ptrBoard;//simulation 과정에서 보드의 정보가 바뀌므로 처음 값으로 복귀가 필요

		this->Selection();
		double result = Simulation();
		BackPropagation(result);
	}
	int move = pRoot->GetBestMove();
	delete pRoot;//탐색이 끝나면 할당된 메모리를 해제해줘야한다.
	return move;
}

void CMCTS::Selection()
{
	//더이상 확장할 child가 없을 경우 child 중 UCB 값이 높은 노드를 선택한다.
	while (!pNode->GetEmptyCellsSize() && pNode->GetChildNodesSize())
	{
		pNode = pNode->GetChildNode();
		pBoard->SetCell(pNode->GetMove());//반복할 때 보드가 초기화 되므로 확장된 노드의 위치를 보드에 기록하여야 한다.
	}
	if (pNode->GetEmptyCellsSize())
	{
		Expantion();//빈셀이 있을 경우 확장한다.
	}
}

void CMCTS::Expantion()
{
	if (!pBoard->isFinish())//빈셀이 있더라도 게임이 끝난 상태이면 확장하지 않는다.
	{
		pNode = pNode->AddChildNode(pBoard);
	}
}

double CMCTS::Simulation()
{
	while (!pBoard->isFinish())
	{
		pBoard->SetCell(pNode->GetNextMove(pBoard));//게임이 끝날 때까지 무작위로 게임을 진행한다.
	}
	return pBoard->GetResult(pNode->GetTurn());
}

//simulatuon 결과를 상위노드에 적용한다.
//이때 상대(opponent) 노드엔 값을 적용하지 않는다.
void CMCTS::BackPropagation(double result)
{
	while (pNode)
	{
		pNode->SetWins(result);
		pNode->SetVisits();
		pNode = pNode->GetParent();
		if (result == 1.0) {
			result = 0.0;
		}
		else if (result == 0.0) {
			result = 1.0;
		}
	}
}

void CTools::PrintBoard()
{
	int i = 0;
	for (auto& n : strNumber)
	{
		cout << n;
		++i % 3 ? cout << " " : cout << endl;
	}
	cout << endl;
}

// 게임이 진행되는 동안 화면에 표시할 문자들을 이곳에 모두 모아서 요구하는 문자열을 출력해준다.
void CTools::ShowMessage(int msgType)
{
	const char* msg[] = {
		"Draw!\n",
		"Compter won!\n",
		"User won!\n",
		"Which position do you want? Input number(1 ~ 9) : ",
		"Please enter the correct number.\n",
		"Fill position, Please enter the other number\n",
		"One more game? y/n : ",
		"AI(Computer) order\n",
		"User order\n"
	};

	cout << msg[msgType];
}


// User가 키보드로부터 입력을 받는다.
int CTools::GetPosition()
{
	char position;
	while (true)
	{
		ShowMessage(INPUTPOSITION); // 입력하라 알리는 메시지를 출력
		cin >> position; // 위치를 입력 받고
		cout << endl;
		if (position < '1' || position > '9') // 정확하게 입력이 되었나 검사
		{
			ShowMessage(OUTOFRANGE); //범위를 벗어났다 알림
		}
		else if (isFill(position - '1')) // 이미 두어진 곳에 다시 두었나 체크
		{
			ShowMessage(CHECKEDPOSITION); // 이무 두어진 곳에 두었다는 메지지 출력
		}
		else
		{
			break;
		}
	}
	return position - '1';
}

bool CTools::isFill(int pos)
{
	return (strNumber[pos] == symbol[USER] || strNumber[pos] == symbol[COMPUTER]);
}

// User가 둘 때는 옳바른 입력만 받아서 위치를 넘겨줍니다.
int CUser::Action()
{
	return pTools->GetPosition();
}

// 컴퓨터는 알고리즘으로부터 최적의 위치를 탐색하여 위치를 bestPosition에 저장을 하고, 그 위치를 받아와서 넘겨줍니다.
int CAI::Action()
{
	return pMcts->Mcts(pBoard, pow((9 - pBoard->GetCellCount()), 4));
}

bool CTictactoe::isContinue()
{
	char ch;
	tools.ShowMessage(CONTINUE);
	cin >> ch;
	if (ch == 'y' || ch == 'Y')
	{
		return true;
	}
	return false;
}

// User와 Computer간에 선수를 정해서 넘겨줍니다.
CPlayer* CTictactoe::GetPlayer()
{
	return Player[rand() % 2];
}

void CTictactoe::InitGame()
{
	board.InitData();
	tools.InitStrNumber();
	tools.PrintBoard();
}

// 한 수씩 둘때마다 게임이 끝났는지를 검사합니다.
bool CTictactoe::isFinish(int player)
{
	if (board.isWin())
	{
		tools.ShowMessage(player); // 이겼을 때 승리 메시지를 화면에 출력해줍니다.
	}
	else if (board.isDraw())
	{
		tools.ShowMessage(DRAW); // 비겼을 경우도 마찬가지로 화면에 출력
	}
	else
	{
		return false;
	}
	return true;
}

// Computer가 선일 경우 첫수는 랜덤하게 둡니다.
void CTictactoe::RandomPlay(int player)
{
	int position = rand() % 9;
	ShowBoard(position, player);
}

// Player가 착수를 하게 되면 보드와 화면에 보여질 보드에 표시를 하고, 화면에 보여줍니다.
void CTictactoe::ShowBoard(int position, int player)
{
	board.SetCell(position);
	tools.SetPosition(position, player);
	tools.PrintBoard();
}


void CTictactoe::PlayGame()
{
	int position;
	InitGame();
	CPlayer* pPlayer = GetPlayer();
	board.SetPlayer(3 - pPlayer->ID);
	if (pPlayer->ID == COMPUTER)
	{
		RandomPlay(pPlayer->ID); // 한 수를 두었기 때문에
		pPlayer = Player[2 - pPlayer->ID]; // Turn을 바꿉니다. ID는 1과 2이고, 배열엔 0, 1에 저장 되어있기 때문에
	}                                   // 2 - ID를 하면, 2 - 1이나 2 - 2가 되어서 선수가 바뀌게 됩니다.
	while (true)
	{
		tools.ShowMessage(pPlayer->ID + 6); // Player의 차례를 화면에 표시합니다.
		position = pPlayer->Action();
		ShowBoard(position, pPlayer->ID);
		if (isFinish(pPlayer->ID))
		{
			break;
		}
		pPlayer = Player[2 - pPlayer->ID];
	}
}

int main(void)
{
	srand((unsigned)time(NULL));

	CTictactoe ttt;
	while (true)
	{
		cout << "Tic tac toe AI v1.0" << endl;
		ttt.PlayGame();
		if (!ttt.isContinue())
		{
			break;
		}
	}

	return 0;
}