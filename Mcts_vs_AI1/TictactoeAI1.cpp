#include "TictactoeAI1.h"

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
	for (int i = 0; i < BOARDSIZE; ++i)
	{
		for (int j = 0; j < BOARDSIZE; ++j)
		{
			board[i][j] = 0;
		}
	}
}

// ó�� ������ ���� �ɶ���, �ٽ� �ô��� �� �ݵ�� �ʱ�ȭ�� �ʿ��� �����͵��� �ʱ�ȭ���ش�.
void CBoard::InitData()
{
	InitBoard();
	currentPosition = 0; //�ż� �� ������ �ٲ� ��ġ�� ����
	winner = 0; // ������ ���а� �����Ǹ� ������ ID�� ���
	checkedCell = 0; // ���� �� ������ �ξ��� ���
	bestPosition[1] = -1; //��ǻ�Ͱ� �ּ��� ���� ã�� �����ϴ� �迭 0���� ��ġ 1���� maxScore
}

// Root node���� ������ ���Ͽ� ������ ���� ���� ��ġ�� ������ �Բ� ����
void CBoard::SetBestPosition(int pos, int score)
{
	if (bestPosition[1] < score)
	{
		bestPosition[0] = pos;
		bestPosition[1] = score;
	}
}

// 1 ~ 9������ ��ġ�� ��ǥ�� ��ȯ�Ͽ� ��ȯ
Point CBoard::GetPoint(int position)
{
	Point p;
	p.x = position % BOARDSIZE;
	p.y = position / BOARDSIZE;
	return p;
}

// �� ���� �ξ��� �� ���忡 ���
void CBoard::SetCell(int position)
{
	player = 3 - player;
	Point p = GetPoint(position); //��ġ�� ��ǥ�� �޾ƿ´�.
	board[p.y][p.x] = player; //��ǥ�� ���� �� player�� ID�� ���
	checkedCell++; // �� �� �ξ����� �ϳ� ����
	currentPosition = position; // ���� �� ��ġ ����
}

int CBoard::GetMove()
{
	return currentPosition;
}

void CBoard::SetPlayer(int player)
{
	this->player = player;
}

int CBoard::GetTurn()
{
	return player;
}

// Minimax �˰��򿡼� �� ���� �ξ��, ���� ���� ���ư� ���� �ξ�Ҵ� ���� 0���� ���־�� �Ѵ�.
void CBoard::SetZero(int position)
{
	Point p = GetPoint(position);
	board[p.y][p.x] = 0;
	checkedCell--; // �ξ��� ���� �ŵξ����� ����
	player = 3 - player;
}

bool CBoard::isFill(int position)
{
	Point p = GetPoint(position);
	return board[p.y][p.x];
}

// �¸� �Ǵ��� �� ���� ���� ���������� �Ǵ��ϱ� ���Ͽ� �ʿ�
bool CBoard::isSamePlayer(Point p, int player)
{
	return board[p.y][p.x] == player;
}

// �� ��(�� �� �ִ� ��)�� ��ġ�� ã�Ƽ� �Ű������� �Ѿ�� �迭�� �����ϰ�, ������ �ε������� �� ���� �����Ͽ����� �� ���� ���
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
	emptyPosition[9] = index; //�迭�� �� ���� �� ������ �˷��ֱ� ���Ͽ� ���
}

// �� ��(�� �� �ִ� ��)�� ��ġ�� ã�Ƽ� �Ű������� �Ѿ�� �迭�� �����ϰ�, ������ �ε������� �� ���� �����Ͽ����� �� ���� ���
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

// ������ ���� ���� �� ������ �ο����ִ� �Լ�, ��ǻ�Ͱ� �̱�� 1�� ���� -1�� ���� 0��
int CBoard::Evaluation()
{
	int score = 0;
	if (winner == COMPUTER)
	{
		score = 1;
	}
	else if (winner == USER)
	{
		score = -1;
	}
	winner = 0;
	return score;
}

// �ξ��� ��ġ���� �� ��(����, ���� �Ǵ� �밢��)�� ��� ���� �������� �����ֳ� ���� �ؾƷ� ��ȯ
// �� ���� ������ �� �� ���̶� ������ ���� �������� ��, �� �Ǵ� ��, �ϵ� �� ���븦 üũ�غ��� �ϹǷ� 
// �� �ٿ� ��� ���ӵ� ���� �ִ��� �Ǵ��ϱ� ���ؼ��� �ι��� �˻簡 �ʿ�.
int CBoard::GetCount(const Point dir[2], int position, int player)
{
	int count = 1;// �� ���� �ξ��� ������ 1���� ����
	for (int i = 0; i < 2; ++i)
	{
		Point p = GetPoint(position);
		p.x = p.x + dir[i].x;
		p.y = p.y + dir[i].y;
		while (true)
		{
			// Range�� ���� ���� ��� 
			if (p.x < 0 or p.x >= BOARDSIZE or p.y < 0 or p.y >= BOARDSIZE)
			{
				break;
			}
			else if (!player || !isSamePlayer(p, player)) // ���� ���� �ƴ� ���
			{
				break;
			}
			else // �ٷ� ���� ���� ��ĭ ���� ���� ���� ��
			{
				count++; // ���ӵ� ���� ���� �ϳ� ����
				p.x = p.x + dir[i].x; // ���� ĭ���� �̵��Ͽ� ���� ������ �ݺ�
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
		{Left, Right}, // ���ι��� üũ 
		{Up, Down},  // ���ι��� üũ 
		{LeftDown, RightUp}, // �밢�� 1 
		{LeftUp, RightDown } }; // �밢�� 2 

	for (const auto& dir : direction) // C++11���� ����� ���� �ݺ����
	{
		// �� ��(��, �� �Ǵ� ��, �� ���)�� ����Ʈ �迭�� ������ ����� �޾��� �� 3�̸� �¸�
		if (GetCount(dir, currentPosition, player) == BOARDSIZE)
		{
			winner = player; // �¸��� player�� ���
			return true;
		}
	}
	return false;
}

double CBoard::GetResult(int player)
{
	if (isWin())
	{
		if (this->player == player)
		{
			//winner = 0;
			return 1.0;
		}
		else
		{
			//winner = 0;
			return 0.0;
		}
	}
	else if (isDraw())
	{
		return 0.5;
	}
}

int  CBoard::GetBestPosotion()
{
	bestPosition[1] = -1;
	return bestPosition[0];
}


int CBoard::MinimaxAlphaBeta(int depth, int player, int alpha, int beta)
{
	if (depth == 0 || isFinish())
	{
		return Evaluation();
	}

	int emptyPosition[10];
	GetEmptyPosition(emptyPosition);

	if (player == COMPUTER)
	{
		int score, maxScore = -INF;
		// �迭�� �� ĭ�� ������ ��ܿ� �� �Ź� �� ���� �ٸ��� ������ auto�� for���� ���� �� ����.
		for (int i = 0; i < emptyPosition[9]; ++i)
		{
			int pos = emptyPosition[i];
			SetCell(pos);
			score = MinimaxAlphaBeta(depth - 1, USER, alpha, beta);
			SetZero(pos);
			maxScore = max(score, maxScore);
			alpha = max(alpha, maxScore);
			if (depth == maxDepth)
			{
				SetBestPosition(pos, maxScore);
				//cout << pos + 1 << " " << score << " " << maxScore << endl;
			}
			if (beta <= alpha)
			{
				break;
			}
		}
		return maxScore;
	}
	else
	{
		int score, minScore = INF;
		for (int i = 0; i < emptyPosition[9]; ++i)
		{
			int pos = emptyPosition[i];
			SetCell(pos);
			score = MinimaxAlphaBeta(depth - 1, COMPUTER, alpha, beta);
			SetZero(pos);
			minScore = min(score, minScore);
			beta = min(minScore, beta);
			if (beta <= alpha)
			{
				break;
			}
		}
		return minScore;
	}
}

CNode::CNode(int move, CBoard* pBoard, CNode* parent)
{
	this->move = move;
	this->parent = parent;
	this->turn = pBoard->GetTurn();
	pBoard->GetEmptyCells(this->empty_cells);
}

CNode::~CNode()
{
	int cnt = 0;
	this->empty_cells.clear();
	if (this->child_nodes.size())
	{
		for (auto pNode : this->child_nodes)
		{
			//cout << cnt++ << endl;
			delete pNode;
			pNode = nullptr;
		}
	}
	this->child_nodes.clear();
}

CNode* CNode::AddChildNode(CBoard* pBoard)
{
	int move = this->empty_cells[empty_cells.size() - 1];
	this->empty_cells.pop_back();
	pBoard->SetCell(move);

	CNode* child = new CNode(move, pBoard, this);
	this->child_nodes.push_back(child);

	return child;
}

double CNode::GetUCB(CNode* c)
{
	return (c->wins / c->visits) + sqrt(2 * log(visits) / c->visits);
}

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

//static bool Compare(CNode* a, CNode* b)
//{
//	return a->GetVists() < b->GetVists();
//}
//
//int CNode::GetBestMove()
//{
//	vector<CNode*>::iterator it;
//
//	it = max_element(this->child_nodes.begin(), this->child_nodes.end(), Compare);
//	int best = distance(this->child_nodes.begin(), it);
//
//	//for (auto& c : this->child_nodes)
//	//{
//	//	cout << c->move << ", " << c->wins << ", " << c->visits << endl;
//	//}
//
//	return this->child_nodes[best]->move;
//}

int CNode::GetBestMove()
{
	double max = -100.0;
	int best = 0, idx = 0;
	for (auto& c : child_nodes)
	{
		if (max < c->visits)
		{
			max = c->visits;
			best = idx;
		}
		idx++;
	}

	//for (auto& c : this->child_nodes)
	//{
	//	cout << c->move << ", " << c->wins << ", " << c->visits << endl;
	//}

	return this->child_nodes[best]->move;
}

int CNode::GetTurn()
{
	return this->turn;
}

CMCTS::CMCTS()
{
	//pBoard = new CBoard;
}

CMCTS::~CMCTS()
{
	//if (pBoard) 
	//{
	//	delete pBoard;
	//	pBoard = nullptr;
	//}
}

int CMCTS::Mcts(CBoard* ptrBoard, int iter_num)
{
	pRoot = new CNode(-1, ptrBoard);
	for (int i = 0; i < iter_num; ++i)
	{
		pNode = pRoot;
		board = *ptrBoard;

		this->Selection();
		double result = Simulation();
		BackPropagation(result);
	}
	int move = pRoot->GetBestMove();
	delete pRoot;
	return move;
}

void CMCTS::Selection()
{
	while (!pNode->GetEmptyCellsSize() && pNode->GetChildNodesSize())
	{
		pNode = pNode->GetChildNode();
		pBoard->SetCell(pNode->GetMove());
	}
	if (pNode->GetEmptyCellsSize())
	{
		Expantion();
	}
}

void CMCTS::Expantion()
{
	if (!pBoard->isFinish())
	{
		pNode = pNode->AddChildNode(pBoard);
	}
}

double CMCTS::Simulation()
{
	while (!pBoard->isFinish())
	{
		pBoard->SetCell(pNode->GetNextMove(pBoard));
		//pBoard->PrintBoard();
	}
	return pBoard->GetResult(pNode->GetTurn());
}

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
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			cout << strNumber[i * BOARDSIZE + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


// ������ ����Ǵ� ���� ȭ�鿡 ǥ���� ���ڵ��� �̰��� ��� ��Ƽ� �䱸�ϴ� ���ڿ��� ������ش�.
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


// User�� Ű����κ��� �Է��� �޴´�.
int CTools::GetPosition()
{
	char position;
	while (true)
	{
		ShowMessage(INPUTPOSITION); // �Է��϶� �˸��� �޽����� ���
		cin >> position; // ��ġ�� �Է� �ް�
		cout << endl;
		if (position < '1' || position > '9') // ��Ȯ�ϰ� �Է��� �Ǿ��� �˻�
		{
			ShowMessage(OUTOFRANGE); //������ ����� �˸�
		}
		else if (isFill(position - '1')) // �̹� �ξ��� ���� �ٽ� �ξ��� üũ
		{
			ShowMessage(CHECKEDPOSITION); // �̹� �ξ��� ���� �ξ��ٴ� ������ ���
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

// User�� �� ���� �ǹٸ� �Է¸� �޾Ƽ� ��ġ�� �Ѱ��ݴϴ�.
int CUser::Action()
{
	int pos[10];
	pBoard->GetEmptyPosition(pos);
	return pMcts->Mcts(pBoard, pow((pos[9]), 4));
	//return pMcts->Mcts(pBoard, 40);
	//return pTools->GetPosition();
}

// ��ǻ�ʹ� �˰������κ��� ������ ��ġ�� Ž���Ͽ� ��ġ�� bestPosition�� ������ �ϰ�, �� ��ġ�� �޾ƿͼ� �Ѱ��ݴϴ�.
int CAI::Action()
{
	pBoard->MinimaxAlphaBeta(pBoard->GetMaxDepth(), this->ID, -INF, INF);
	int move = pBoard->GetBestPosotion();
	//cout << move << ", " << endl;
	//pBoard->PrintBoard();
	return move;
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

// User�� Computer���� ������ ���ؼ� �Ѱ��ݴϴ�.
CPlayer* CTictactoe::GetPlayer()
{
	return Player[rand() % 2];
}

void CTictactoe::InitGame()
{
	board.InitData();
	tools.InitStrNumber();
	//tools.PrintBoard();
}

// �� ���� �Ѷ����� ������ ���������� �˻��մϴ�.
bool CTictactoe::isFinish(int player)
{
	if (board.isWin())
	{
		count[player]++;
		//tools.ShowMessage(player); // �̰��� �� �¸� �޽����� ȭ�鿡 ������ݴϴ�.
	}
	else if (board.isDraw())
	{
		count[0]++;
		//tools.ShowMessage(DRAW); // ����� ��쵵 ���������� ȭ�鿡 ���
	}
	else
	{
		return false;
	}
	return true;
}

// Computer�� ���� ��� ù���� �����ϰ� �Ӵϴ�.
void CTictactoe::RandomPlay(int player)
{
	int position = rand() % 9;
	ShowBoard(position, player);
}

// Player�� ������ �ϰ� �Ǹ� ����� ȭ�鿡 ������ ���忡 ǥ�ø� �ϰ�, ȭ�鿡 �����ݴϴ�.
void CTictactoe::ShowBoard(int position, int player)
{
	board.SetCell(position);
	tools.SetPosition(position, player);
	//tools.PrintBoard();
}


void CTictactoe::PlayGame()
{
	int position;
	InitGame();
	CPlayer* pPlayer = GetPlayer();
	board.SetPlayer(3 - pPlayer->ID);
	if (pPlayer->ID == COMPUTER)
	{
		RandomPlay(pPlayer->ID); // �� ���� �ξ��� ������
		pPlayer = Player[2 - pPlayer->ID]; // Turn�� �ٲߴϴ�. ID�� 1�� 2�̰�, �迭�� 0, 1�� ���� �Ǿ��ֱ� ������
	}                                   // 2 - ID�� �ϸ�, 2 - 1�̳� 2 - 2�� �Ǿ ������ �ٲ�� �˴ϴ�.
	while (true)
	{
		//tools.ShowMessage(pPlayer->ID + 6); // Player�� ���ʸ� ȭ�鿡 ǥ���մϴ�.
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
	int iter = 100000, i = 0;
	clock_t t1, t2;
	srand((unsigned)time(NULL));

	CTictactoe ttt;
	t1 = clock();
	while (true)
	{
		//cout << "Tic tac toe AI v1.0" << endl;
		ttt.PlayGame();
		//if (!ttt.isContinue())
		if (++i >= iter)
		{
			break;
		}
	}
	t2 = clock();
	printf("Elapsed time of %d times = %.3lf\n", iter, (t2 - t1) / 1000.);
	printf("Draw count : %d, alpha beta wins : %d, mcts wins : %d", ttt.count[0], ttt.count[1], ttt.count[2]);
	system("pause");
	return 0;
}