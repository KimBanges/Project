#include<iostream>   // 입출력
#include<Windows.h>  // 윈도우 API를 사용
#include<conio.h>    // 키보드 입력을 처리 
#include<vector>	 // 동적 배열인 벡터 사용
#include<string>     // 문자열을 사용
#include<thread>     // 멀티스레딩을 지원
#include<ctime>		 // 시간과 관련된 함수를 사용
#include<iomanip>    // 입력과 출력의 형식을 지정
#pragma comment(lib, "winmm.lib")  // 라이브러리 파일을 링크하기 위한 pragma 지시자

using namespace std;

#define MAGIC_KEY 224
#define SPACE 32
#define KEY_NUM 9
#define LIFE 3
#define MAX_LEVEL 11

// enum은 열거형으로 특정한 값을 나타내기 위해 사용
// 게임 메뉴를 나타냄
enum MENU
{
	GAMESTART = 0,  // 게임 시작 메뉴를 나타내는 상수
	INFO,           // 게임 정보 메뉴
	QUIT            // 게임 종료 메뉴
};

// 키보드의 특정 키를 나타냄
enum KEYBOARD
{
	UP = 56,      // ↑ 위쪽 화살표 키를 나타내는 상수
	LEFT = 52,    // ← 왼쪽 
	RIGHT = 54,   // → 오른쪽
	DOWN = 50,    // ↓ 아래쪽
	L_UP = 55,    // ↖ 왼쪽 위 대각선
	R_UP = 57,    // ↗ 오른쪽 위 대각선
	L_DOWN = 49,  // ↙ 왼쪽 아래 대각선
	R_DOWN = 51,  // ↘ 오른쪽 아래 대각선
	SQUARE = 53   // □ 중앙 
};

// 콘솔 화면에서 커서의 위치를 이동시키는 함수
void gotoxy(int x, int y)
{
	COORD Pos;      // 2차원 구조체로, 2차원 좌표를 나타냄
	Pos.X = 2 * x;  // 한 글자의 너비가 두칸
	Pos.Y = y;      // 콘솔 화면에서의 세로 위치를 나타냄
	// 콘솔 화면의 커서 위치를 변경
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 콘솔 창의 크기와 제목을 설정하는 함수
void SetConsoleView()
{
	// 콘솔 창의 너비, 높이 설정
	system("mode con:cols=50 lines=20");
	// 콘솔 창의 제목 설정
	system("Guess arrow game");
}

// 게임 시작 화면을 그리는 함수
void DrawReadyGame()
{
	// 콘솔 화면 클리어
	system("cls");

	// 리듬 게임 타이틀 출력
	gotoxy(5, 2);
	cout << "******************************";
	gotoxy(5, 3);
	cout << "*      Guess arrow game      *";
	gotoxy(5, 4);
	cout << "******************************";

	// 게임 시작 메뉴 옵션 출력
	gotoxy(10, 8);
	cout << "GameStart";
	gotoxy(10, 9);
	cout << "GameInfo";
	gotoxy(10, 10);
	cout << "Quit" << endl;
}

// 게임 정보 화면을 그리는 함수
void DrawInfoGame()
{
	// 콘솔 화면 클리어
	system("cls");

	// 게임 개발자 정보와 블로그 주소를 출력
	gotoxy(1, 3);
	cout << "************************************************************";
	gotoxy(1, 5);
	cout << "                     게임 규칙                       ";
	gotoxy(1, 7);
	cout << "1. 넘버패드를 활용하여 전체 방향 화살표가 나오면 화살표 입력";

	gotoxy(1, 9);
	cout << "2. 입력 후 스페이스를 눌러서 정답 입력";

	// 하단 경계선과 추가 정보 출력
	gotoxy(1, 11);
	cout << "3. 정답 확인 후 다음 턴으로 넘어감";

	gotoxy(1, 13);
	cout << "4. Backspace 키를 통해 지우기 가능";

	gotoxy(1, 15);
	cout << "5. 제한 시간(1분)안에 많은 점수 얻기";

	gotoxy(1, 17);
	cout << "************************************************************";

}

clock_t startTime;

// 게임 플레이 중인 화면을 그리는 함수 
void DrawStartGame(const int life, const int score, const string questionStr, const string answerStr)
{
	// 콘솔 화면 클리어
	system("cls");

	// 현재 시간 계산
	clock_t currentTime = clock();
	double elapsedTime = (currentTime - startTime) / (double)CLOCKS_PER_SEC;

	// 상단 경계선 출력
	gotoxy(2, 1);
	cout << "*******************************************";

	// 생명과 점수, 현재 시간 정보 출력
	gotoxy(4, 3);
	cout << "Life : " << life << " / " << LIFE;  // LIFE는 최대 생명치를 나타내는 상수
	gotoxy(4, 4);
	cout << "Score : " << score;
	gotoxy(4, 5);
	cout << "Time : " << fixed << setprecision(0) << elapsedTime << " sec";

	// 현재 문제와 사용자의 답변 출력
	gotoxy(4, 7);
	cout << "--------------------------------" << endl;
	gotoxy(4, 8);
	cout << "Q : " << questionStr;
	gotoxy(4, 10);
	cout << "A : " << answerStr;
	gotoxy(4, 11);
	cout << "--------------------------------" << endl;

	// 하단 경계선 출력
	gotoxy(2, 17);
	cout << "*******************************************" << endl;
}

// 게임 오버 화면을 그리는 함수
void DrawGameOver(const int playTime)
{
	gotoxy(8, 13);
	cout << "-------------------";
	gotoxy(8, 14);
	cout << "| G A M E O V E R |";
	gotoxy(8, 15);
	cout << "-------------------";

	// 사용자가 아무 키나 누를 때까지 프로그램 실행을 일시 정지시키는 명령
	system("pause>null");
}

// 사용자 커서를 그리는 함수
void DrawUserCursor(int& y)
{
	// 커서 위치가 화면 상단을 넘어서지 않도록 조정
	if (y <= 0)
	{
		y = 0;
	}
	// 화면 하단
	else if (y >= 2)
	{
		y = 2;
	}

	// 지정된 위치에 커서('>')를 그림
	gotoxy(9, 8 + y);
	cout << ">";
}

// 게임 시작 전 메뉴를 보여주고 사용자 입력을 받는 함수
MENU ReadyGame()
{
	int y = 0;      // 사용자 커서의 위치
	int input = 0;  // 사용자로부터의 입력 값을 저장

	while (true)
	{
		DrawReadyGame();
		DrawUserCursor(y);
		input = _getch();

		// 방향키 입력 처리
		if (input != SPACE)  // MAGIC_KEY는 방향키 입력을 나타내는 특수한 값
		{
			switch (_getch())  // 실제 방향키 값 확인
			{
			case UP:    // 위 방향키
				--y;    // 커서 위치를 위로
				break;
			case DOWN:  // 아래 방향키
				++y;    // 커서 위치를 아래로
				break;
			}
		}
		else if (input == SPACE)   // 스페이스바 입력 처리
		{
			switch (y)
			{
			case 0:
				return GAMESTART;  // 게임 시작
			case 1:
				return INFO;       // 정보 보기
			case 2:
				return QUIT;       // 게임 종료
			}
		}
	}
}

// 게임 정보를 보여주는 함수
void InfoGame()
{
	DrawInfoGame();
	// 사용자의 입력을 기다림
	system("pause>null");
}

// 문제(화살표의 방향)를 설정하는 함수
void SetQuestion(vector<int>& questionVec, int level)
{
	if (level > MAX_LEVEL)
	{
		level = MAX_LEVEL;
	}

	int num = 0;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < level; ++i)	// 레벨에 따른 화살표의 개수만큼 반복
	{
		num = rand() % KEY_NUM;	// 0부터 KEY_NUM-1 사이의 난수 생성
		switch (num)
		{
		case 0:
			questionVec.push_back(UP);      // 위 방향 화살표 추가
			break;
		case 1:
			questionVec.push_back(RIGHT);   // 오른쪽
			break;
		case 2:
			questionVec.push_back(LEFT);    // 왼쪽
			break;
		case 3:
			questionVec.push_back(DOWN);    // 아래
			break;
		case 4:
			questionVec.push_back(R_UP);    // 오른쪽 위 대각선
			break;
		case 5:
			questionVec.push_back(L_UP);    // 왼쪽 위 대각선
			break;
		case 6:
			questionVec.push_back(R_DOWN);  // 오른쪽 아래 대각선
			break;
		case 7:
			questionVec.push_back(L_DOWN);  // 왼쪽 아래 대각선
			break;
		case 8:
			questionVec.push_back(SQUARE);  // 왼쪽 아래 대각선
			break;
		}
	}
}

// 벡터 내의 정수를 화살표 문자열로 변환하여 문자열에 누적하는 함수
void VectorToString(const vector<int> v, string& str)
{
	for (int i = 0; i < static_cast<int>(v.size()); ++i)  // 벡터 크기만큼 반복
	{
		switch (v[i])  // 벡터의 i번째 요소에 따라 분리
		{
		case UP:  // UP 상수와 일치하는 경우
			str += "↑ ";
			break;
		case DOWN:
			str += "↓ ";
			break;
		case LEFT:
			str += "← ";
			break;
		case RIGHT:
			str += "→ ";
			break;
		case R_UP:
			str += "↗ ";
			break;
		case L_UP:
			str += "↖ ";
			break;
		case R_DOWN:
			str += "↘ ";
			break;
		case L_DOWN:
			str += "↙ ";
			break;
		case SQUARE:
			str += "□ ";
			break;
		}
	}
}

// 두 벡터가 같은지 여부를 검사하는 함수
bool CheckAnswer(const vector<int> questionVec, const vector<int> answerVec)
{
	// 길이가 다르면 두 벡터는 다르다
	if (questionVec.size() != answerVec.size())
	{
		return false;
	}

	// 각 요소를 비교
	for (int i = 0; i < static_cast<int>(questionVec.size()); ++i)
	{
		// 요소가 다르면
		if (questionVec[i] != answerVec[i])
		{
			return false;
		}
	}
	return true;
}

const int GAME_DURATION_SECONDS = 60; // 게임 제한 시간(초)

void StartGame()
{
	startTime = clock();

	// PlaySound("HYP-Hit.wav", NULL, SND_NODEFAULT | SND_ASYNC | SND_LOOP);
	int life = LIFE;
	int score = 0;
	clock_t endTime;  // 게임의 시작과 끝 시간을 측정
	startTime = clock();         // 게임 시작 시간 기록

	// 문제
	vector<int> questionVec;  // 화살표 시퀀스를 저장할 벡터
	string questionStr = "";  // 화면에 표시될 화살표 문자열
	// 답안지
	vector<int> answerVec;    // 사용자가 입력할
	string answerStr = "";    // 사용자가 입력을 문자열로 표시

	int firstInput = 0;   // 사용자의 첫 번째 입력을 저장

	auto gameStartTime = chrono::steady_clock::now(); // 게임 시작 시간

	while (true)
	{
		// 점수에 따른 레벨 결정
		int level = (score / 30) + 1;

		// 문제 세팅
		SetQuestion(questionVec, level);
		// 생성된 문제를 문자열로 변환하여 표시
		VectorToString(questionVec, questionStr);

		while (true)
		{
			// 게임 진행 시간 체크
			auto gameCurrentTime = chrono::steady_clock::now();
			auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(gameCurrentTime - gameStartTime).count();

			if (elapsed_seconds >= GAME_DURATION_SECONDS)
			{
				// 게임 종료 시간 기록
				endTime = clock();
				// 게임 플레이 시간 계산
				int playTime = static_cast<int>((endTime - startTime) / CLOCKS_PER_SEC);
				// 게임 종료 화면 그리기
				DrawGameOver(playTime);
				// 게임 종료를 중지
				PlaySound(NULL, NULL, 0);
				return;
			}

			// 게임 화면 그리기
			DrawStartGame(life, score, questionStr, answerStr);

			if (life == 0)
			{
				// 게임 종료 시간 기록
				endTime = clock();
				// 게임 플레이 시간 계산
				int playTime = static_cast<int>((endTime - startTime) / CLOCKS_PER_SEC);
				// 게임 오버 화면 그리기
				DrawGameOver(playTime);
				// 게임 오버를 중지
				PlaySound(NULL, NULL, 0);
				return;
			}

			// 사용자 입력 받기
			firstInput = _getch();

			// 8은 백스페이스 키의 ASCII 코드입니다.
			if (firstInput != SPACE && firstInput != 8)
			{
				answerVec.push_back(firstInput);
				switch (firstInput)
				{
				case UP:
					answerStr += "↑ ";
					break;
				case DOWN:
					answerStr += "↓ ";
					break;
				case LEFT:
					answerStr += "← ";
					break;
				case RIGHT:
					answerStr += "→ ";
					break;
				case L_UP:
					answerStr += "↖ ";
					break;
				case R_UP:
					answerStr += "↗ ";
					break;
				case L_DOWN:
					answerStr += "↙ ";
					break;
				case R_DOWN:
					answerStr += "↘ ";
					break;
				case SQUARE:
					answerStr += "□ ";
					break;
				}
			}
			// 스페이스바를 눌러 답안 제출
			else if (firstInput == SPACE)
			{
				// 답안 확인
				if (CheckAnswer(questionVec, answerVec))
				{
					score += 10;  // 정답 시 점수 증가
				}
				else
				{
					--life;         // 오답시 생명 감소
					score -= 5;     // 점수 감소
					if (score < 0)  // 점수가 음수가 되지 않도록 보정
					{
						score = 0;
					}
				}

				// 다음 문제를 위해 벡터와 문자열 초기화
				questionVec.clear();
				questionStr = "";
				answerVec.clear();
				answerStr = "";
				break;
			}

			// 백스페이스 키를 눌렀을 때
			else if (firstInput == 8)
			{
				// 입력된 답안이 있는 경우에만 처리
				if (!answerVec.empty())
				{
					// 가장 최근에 입력된 답안을 제거
					answerVec.pop_back();
					// 이 부분에서 answerStr도 업데이트하여 마지막 입력된 화살표를 제거
					if (!answerStr.empty())
					{
						// 공백 하나와 그 이전 화살표를 제거
						answerStr.erase(answerStr.size() - 2, 2);
					}
					else
					{
						// answerStr이 비어있을 때 처리
						answerStr.clear(); // answerStr을 완전히 비웁니다.
					}
				}

				// 일단 answerStr을 비웁니다.
				answerStr = "";

				// answerVec에 있는 입력을 기반으로 answerStr을 다시 만듭니다.
				for (int arrow : answerVec)
				{
					switch (arrow)
					{
					case UP:
						answerStr += "↑ ";
						break;
					case DOWN:
						answerStr += "↓ ";
						break;
					case LEFT:
						answerStr += "← ";
						break;
					case RIGHT:
						answerStr += "→ ";
						break;
					case L_UP:
						answerStr += "↖ ";
						break;
					case R_UP:
						answerStr += "↗ ";
						break;
					case L_DOWN:
						answerStr += "↙ ";
						break;
					case R_DOWN:
						answerStr += "↘ ";
						break;
					case SQUARE:
						answerStr += "□ ";
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
