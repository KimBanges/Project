#include<iostream>   // �����
#include<Windows.h>  // ������ API�� ���
#include<conio.h>    // Ű���� �Է��� ó�� 
#include<vector>	 // ���� �迭�� ���� ���
#include<string>     // ���ڿ��� ���
#include<thread>     // ��Ƽ�������� ����
#include<ctime>		 // �ð��� ���õ� �Լ��� ���
#include<iomanip>    // �Է°� ����� ������ ����
#pragma comment(lib, "winmm.lib")  // ���̺귯�� ������ ��ũ�ϱ� ���� pragma ������

using namespace std;

#define MAGIC_KEY 224
#define SPACE 32
#define KEY_NUM 9
#define LIFE 3
#define MAX_LEVEL 11

// enum�� ���������� Ư���� ���� ��Ÿ���� ���� ���
// ���� �޴��� ��Ÿ��
enum MENU
{
	GAMESTART = 0,  // ���� ���� �޴��� ��Ÿ���� ���
	INFO,           // ���� ���� �޴�
	QUIT            // ���� ���� �޴�
};

// Ű������ Ư�� Ű�� ��Ÿ��
enum KEYBOARD
{
	UP = 56,      // �� ���� ȭ��ǥ Ű�� ��Ÿ���� ���
	LEFT = 52,    // �� ���� 
	RIGHT = 54,   // �� ������
	DOWN = 50,    // �� �Ʒ���
	L_UP = 55,    // �� ���� �� �밢��
	R_UP = 57,    // �� ������ �� �밢��
	L_DOWN = 49,  // �� ���� �Ʒ� �밢��
	R_DOWN = 51,  // �� ������ �Ʒ� �밢��
	SQUARE = 53   // �� �߾� 
};

// �ܼ� ȭ�鿡�� Ŀ���� ��ġ�� �̵���Ű�� �Լ�
void gotoxy(int x, int y)
{
	COORD Pos;      // 2���� ����ü��, 2���� ��ǥ�� ��Ÿ��
	Pos.X = 2 * x;  // �� ������ �ʺ� ��ĭ
	Pos.Y = y;      // �ܼ� ȭ�鿡���� ���� ��ġ�� ��Ÿ��
	// �ܼ� ȭ���� Ŀ�� ��ġ�� ����
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// �ܼ� â�� ũ��� ������ �����ϴ� �Լ�
void SetConsoleView()
{
	// �ܼ� â�� �ʺ�, ���� ����
	system("mode con:cols=50 lines=20");
	// �ܼ� â�� ���� ����
	system("Guess arrow game");
}

// ���� ���� ȭ���� �׸��� �Լ�
void DrawReadyGame()
{
	// �ܼ� ȭ�� Ŭ����
	system("cls");

	// ���� ���� Ÿ��Ʋ ���
	gotoxy(5, 2);
	cout << "******************************";
	gotoxy(5, 3);
	cout << "*      Guess arrow game      *";
	gotoxy(5, 4);
	cout << "******************************";

	// ���� ���� �޴� �ɼ� ���
	gotoxy(10, 8);
	cout << "GameStart";
	gotoxy(10, 9);
	cout << "GameInfo";
	gotoxy(10, 10);
	cout << "Quit" << endl;
}

// ���� ���� ȭ���� �׸��� �Լ�
void DrawInfoGame()
{
	// �ܼ� ȭ�� Ŭ����
	system("cls");

	// ���� ������ ������ ��α� �ּҸ� ���
	gotoxy(1, 3);
	cout << "************************************************************";
	gotoxy(1, 5);
	cout << "                     ���� ��Ģ                       ";
	gotoxy(1, 7);
	cout << "1. �ѹ��е带 Ȱ���Ͽ� ��ü ���� ȭ��ǥ�� ������ ȭ��ǥ �Է�";

	gotoxy(1, 9);
	cout << "2. �Է� �� �����̽��� ������ ���� �Է�";

	// �ϴ� ��輱�� �߰� ���� ���
	gotoxy(1, 11);
	cout << "3. ���� Ȯ�� �� ���� ������ �Ѿ";

	gotoxy(1, 13);
	cout << "4. Backspace Ű�� ���� ����� ����";

	gotoxy(1, 15);
	cout << "5. ���� �ð�(1��)�ȿ� ���� ���� ���";

	gotoxy(1, 17);
	cout << "************************************************************";

}

clock_t startTime;

// ���� �÷��� ���� ȭ���� �׸��� �Լ� 
void DrawStartGame(const int life, const int score, const string questionStr, const string answerStr)
{
	// �ܼ� ȭ�� Ŭ����
	system("cls");

	// ���� �ð� ���
	clock_t currentTime = clock();
	double elapsedTime = (currentTime - startTime) / (double)CLOCKS_PER_SEC;

	// ��� ��輱 ���
	gotoxy(2, 1);
	cout << "*******************************************";

	// ����� ����, ���� �ð� ���� ���
	gotoxy(4, 3);
	cout << "Life : " << life << " / " << LIFE;  // LIFE�� �ִ� ����ġ�� ��Ÿ���� ���
	gotoxy(4, 4);
	cout << "Score : " << score;
	gotoxy(4, 5);
	cout << "Time : " << fixed << setprecision(0) << elapsedTime << " sec";

	// ���� ������ ������� �亯 ���
	gotoxy(4, 7);
	cout << "--------------------------------" << endl;
	gotoxy(4, 8);
	cout << "Q : " << questionStr;
	gotoxy(4, 10);
	cout << "A : " << answerStr;
	gotoxy(4, 11);
	cout << "--------------------------------" << endl;

	// �ϴ� ��輱 ���
	gotoxy(2, 17);
	cout << "*******************************************" << endl;
}

// ���� ���� ȭ���� �׸��� �Լ�
void DrawGameOver(const int playTime)
{
	gotoxy(8, 13);
	cout << "-------------------";
	gotoxy(8, 14);
	cout << "| G A M E O V E R |";
	gotoxy(8, 15);
	cout << "-------------------";

	// ����ڰ� �ƹ� Ű�� ���� ������ ���α׷� ������ �Ͻ� ������Ű�� ���
	system("pause>null");
}

// ����� Ŀ���� �׸��� �Լ�
void DrawUserCursor(int& y)
{
	// Ŀ�� ��ġ�� ȭ�� ����� �Ѿ�� �ʵ��� ����
	if (y <= 0)
	{
		y = 0;
	}
	// ȭ�� �ϴ�
	else if (y >= 2)
	{
		y = 2;
	}

	// ������ ��ġ�� Ŀ��('>')�� �׸�
	gotoxy(9, 8 + y);
	cout << ">";
}

// ���� ���� �� �޴��� �����ְ� ����� �Է��� �޴� �Լ�
MENU ReadyGame()
{
	int y = 0;      // ����� Ŀ���� ��ġ
	int input = 0;  // ����ڷκ����� �Է� ���� ����

	while (true)
	{
		DrawReadyGame();
		DrawUserCursor(y);
		input = _getch();

		// ����Ű �Է� ó��
		if (input != SPACE)  // MAGIC_KEY�� ����Ű �Է��� ��Ÿ���� Ư���� ��
		{
			switch (_getch())  // ���� ����Ű �� Ȯ��
			{
			case UP:    // �� ����Ű
				--y;    // Ŀ�� ��ġ�� ����
				break;
			case DOWN:  // �Ʒ� ����Ű
				++y;    // Ŀ�� ��ġ�� �Ʒ���
				break;
			}
		}
		else if (input == SPACE)   // �����̽��� �Է� ó��
		{
			switch (y)
			{
			case 0:
				return GAMESTART;  // ���� ����
			case 1:
				return INFO;       // ���� ����
			case 2:
				return QUIT;       // ���� ����
			}
		}
	}
}

// ���� ������ �����ִ� �Լ�
void InfoGame()
{
	DrawInfoGame();
	// ������� �Է��� ��ٸ�
	system("pause>null");
}

// ����(ȭ��ǥ�� ����)�� �����ϴ� �Լ�
void SetQuestion(vector<int>& questionVec, int level)
{
	if (level > MAX_LEVEL)
	{
		level = MAX_LEVEL;
	}

	int num = 0;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < level; ++i)	// ������ ���� ȭ��ǥ�� ������ŭ �ݺ�
	{
		num = rand() % KEY_NUM;	// 0���� KEY_NUM-1 ������ ���� ����
		switch (num)
		{
		case 0:
			questionVec.push_back(UP);      // �� ���� ȭ��ǥ �߰�
			break;
		case 1:
			questionVec.push_back(RIGHT);   // ������
			break;
		case 2:
			questionVec.push_back(LEFT);    // ����
			break;
		case 3:
			questionVec.push_back(DOWN);    // �Ʒ�
			break;
		case 4:
			questionVec.push_back(R_UP);    // ������ �� �밢��
			break;
		case 5:
			questionVec.push_back(L_UP);    // ���� �� �밢��
			break;
		case 6:
			questionVec.push_back(R_DOWN);  // ������ �Ʒ� �밢��
			break;
		case 7:
			questionVec.push_back(L_DOWN);  // ���� �Ʒ� �밢��
			break;
		case 8:
			questionVec.push_back(SQUARE);  // ���� �Ʒ� �밢��
			break;
		}
	}
}

// ���� ���� ������ ȭ��ǥ ���ڿ��� ��ȯ�Ͽ� ���ڿ��� �����ϴ� �Լ�
void VectorToString(const vector<int> v, string& str)
{
	for (int i = 0; i < static_cast<int>(v.size()); ++i)  // ���� ũ�⸸ŭ �ݺ�
	{
		switch (v[i])  // ������ i��° ��ҿ� ���� �и�
		{
		case UP:  // UP ����� ��ġ�ϴ� ���
			str += "�� ";
			break;
		case DOWN:
			str += "�� ";
			break;
		case LEFT:
			str += "�� ";
			break;
		case RIGHT:
			str += "�� ";
			break;
		case R_UP:
			str += "�� ";
			break;
		case L_UP:
			str += "�� ";
			break;
		case R_DOWN:
			str += "�� ";
			break;
		case L_DOWN:
			str += "�� ";
			break;
		case SQUARE:
			str += "�� ";
			break;
		}
	}
}

// �� ���Ͱ� ������ ���θ� �˻��ϴ� �Լ�
bool CheckAnswer(const vector<int> questionVec, const vector<int> answerVec)
{
	// ���̰� �ٸ��� �� ���ʹ� �ٸ���
	if (questionVec.size() != answerVec.size())
	{
		return false;
	}

	// �� ��Ҹ� ��
	for (int i = 0; i < static_cast<int>(questionVec.size()); ++i)
	{
		// ��Ұ� �ٸ���
		if (questionVec[i] != answerVec[i])
		{
			return false;
		}
	}
	return true;
}

const int GAME_DURATION_SECONDS = 60; // ���� ���� �ð�(��)

void StartGame()
{
	startTime = clock();

	// PlaySound("HYP-Hit.wav", NULL, SND_NODEFAULT | SND_ASYNC | SND_LOOP);
	int life = LIFE;
	int score = 0;
	clock_t endTime;  // ������ ���۰� �� �ð��� ����
	startTime = clock();         // ���� ���� �ð� ���

	// ����
	vector<int> questionVec;  // ȭ��ǥ �������� ������ ����
	string questionStr = "";  // ȭ�鿡 ǥ�õ� ȭ��ǥ ���ڿ�
	// �����
	vector<int> answerVec;    // ����ڰ� �Է���
	string answerStr = "";    // ����ڰ� �Է��� ���ڿ��� ǥ��

	int firstInput = 0;   // ������� ù ��° �Է��� ����

	auto gameStartTime = chrono::steady_clock::now(); // ���� ���� �ð�

	while (true)
	{
		// ������ ���� ���� ����
		int level = (score / 30) + 1;

		// ���� ����
		SetQuestion(questionVec, level);
		// ������ ������ ���ڿ��� ��ȯ�Ͽ� ǥ��
		VectorToString(questionVec, questionStr);

		while (true)
		{
			// ���� ���� �ð� üũ
			auto gameCurrentTime = chrono::steady_clock::now();
			auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(gameCurrentTime - gameStartTime).count();

			if (elapsed_seconds >= GAME_DURATION_SECONDS)
			{
				// ���� ���� �ð� ���
				endTime = clock();
				// ���� �÷��� �ð� ���
				int playTime = static_cast<int>((endTime - startTime) / CLOCKS_PER_SEC);
				// ���� ���� ȭ�� �׸���
				DrawGameOver(playTime);
				// ���� ���Ḧ ����
				PlaySound(NULL, NULL, 0);
				return;
			}

			// ���� ȭ�� �׸���
			DrawStartGame(life, score, questionStr, answerStr);

			if (life == 0)
			{
				// ���� ���� �ð� ���
				endTime = clock();
				// ���� �÷��� �ð� ���
				int playTime = static_cast<int>((endTime - startTime) / CLOCKS_PER_SEC);
				// ���� ���� ȭ�� �׸���
				DrawGameOver(playTime);
				// ���� ������ ����
				PlaySound(NULL, NULL, 0);
				return;
			}

			// ����� �Է� �ޱ�
			firstInput = _getch();

			// 8�� �齺���̽� Ű�� ASCII �ڵ��Դϴ�.
			if (firstInput != SPACE && firstInput != 8)
			{
				answerVec.push_back(firstInput);
				switch (firstInput)
				{
				case UP:
					answerStr += "�� ";
					break;
				case DOWN:
					answerStr += "�� ";
					break;
				case LEFT:
					answerStr += "�� ";
					break;
				case RIGHT:
					answerStr += "�� ";
					break;
				case L_UP:
					answerStr += "�� ";
					break;
				case R_UP:
					answerStr += "�� ";
					break;
				case L_DOWN:
					answerStr += "�� ";
					break;
				case R_DOWN:
					answerStr += "�� ";
					break;
				case SQUARE:
					answerStr += "�� ";
					break;
				}
			}
			// �����̽��ٸ� ���� ��� ����
			else if (firstInput == SPACE)
			{
				// ��� Ȯ��
				if (CheckAnswer(questionVec, answerVec))
				{
					score += 10;  // ���� �� ���� ����
				}
				else
				{
					--life;         // ����� ���� ����
					score -= 5;     // ���� ����
					if (score < 0)  // ������ ������ ���� �ʵ��� ����
					{
						score = 0;
					}
				}

				// ���� ������ ���� ���Ϳ� ���ڿ� �ʱ�ȭ
				questionVec.clear();
				questionStr = "";
				answerVec.clear();
				answerStr = "";
				break;
			}

			// �齺���̽� Ű�� ������ ��
			else if (firstInput == 8)
			{
				// �Էµ� ����� �ִ� ��쿡�� ó��
				if (!answerVec.empty())
				{
					// ���� �ֱٿ� �Էµ� ����� ����
					answerVec.pop_back();
					// �� �κп��� answerStr�� ������Ʈ�Ͽ� ������ �Էµ� ȭ��ǥ�� ����
					if (!answerStr.empty())
					{
						// ���� �ϳ��� �� ���� ȭ��ǥ�� ����
						answerStr.erase(answerStr.size() - 2, 2);
					}
					else
					{
						// answerStr�� ������� �� ó��
						answerStr.clear(); // answerStr�� ������ ���ϴ�.
					}
				}

				// �ϴ� answerStr�� ���ϴ�.
				answerStr = "";

				// answerVec�� �ִ� �Է��� ������� answerStr�� �ٽ� ����ϴ�.
				for (int arrow : answerVec)
				{
					switch (arrow)
					{
					case UP:
						answerStr += "�� ";
						break;
					case DOWN:
						answerStr += "�� ";
						break;
					case LEFT:
						answerStr += "�� ";
						break;
					case RIGHT:
						answerStr += "�� ";
						break;
					case L_UP:
						answerStr += "�� ";
						break;
					case R_UP:
						answerStr += "�� ";
						break;
					case L_DOWN:
						answerStr += "�� ";
						break;
					case R_DOWN:
						answerStr += "�� ";
						break;
					case SQUARE:
						answerStr += "�� ";
						break;
					}
				}
			}
		}
	}
}

int main(void)
{
	SetConsoleView();
	while (true)
	{
		switch (ReadyGame())
		{
		case GAMESTART:
			StartGame();
			break;
		case INFO:
			InfoGame();
			break;
		case QUIT:
			return 0;
		}
	}
	return 0;
}
