#include "console/console.h"
#include "game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <random>




void Game::win() // win 메시지 출력
{
    if(linecount == 0)
    {
    std::string message = "You Win";
    std::string time = Game::timelog();
    console::draw(5-message.length()/2+1, 10, message);
    console::draw(5-message.length()/2, 11, time);
    finish = 1;
    }
}
void Game::lost() //lost메시지 출력
{
    std::string message = "You Lost";
    console::draw(5-message.length()/2+1, 10, message);
}

std::string timelog()//시간계산
{
    using namespace std::chrono;

    // 현재 시간을 밀리초까지 얻기
    auto now = system_clock::now();
    auto now_as_time_t = system_clock::to_time_t(now);

    // 밀리초 계산
    auto milliseconds = duration_cast<milliseconds>(now.time_since_epoch()) % 1000; //왜 계속 오류뜨는지 모르겠습니다

    // 밀리초를 10으로 나누어 두 자리로 만들기
    int Milliseconds = milliseconds.count() / 10;

    // 현재 시간을 분과 초로 변환
    std::tm now_tm = *std::localtime(&now_as_time_t);

    // 문자열로 바꾸기
    std::ostringstream otime;
    otime << std::put_time(&now_tm, "%M:%S.") << std::setfill('0') << std::setw(2) << Milliseconds;
    return otime.str();
}

void Game:: printtime() //시간출력
{
    std::string result = Game::timelog();
    console::draw(3, 24, result);
}   


void Game::lineCount() //라인수 출력
{
    std::string result = std::to_string(linecount) + " lines left";
    console::draw(0, 23, result);
}

Tetromino Game::randomTetromino() //랜덤 테트로미노 생성
{
    std::random_device rd;
    std::minstd_rand gen(rd());
    std::uniform_int_distribution<int> dis(0, 6);
    return rTetromino[dis(gen)];
}

void Game::nextTetromino() //다음 테트로미노
{
    next = Game::randomTetromino();
    now = next;
    wh[0] = (5-now.size()/2)+1;//중앙
    wh[1] = 1;
    
}

void Game::printTetromino() //테트로미노 출력
{
    now.drawAt(BLOCK_STRING, wh[0], wh[1]);
}
void Game::printNext() //next테트로미노 출력
{
    next.drawAt(BLOCK_STRING, 14, 1);
}

/*
void Game::printHold() //hold출력
{
    if(hold->original == nullptr) {
        return;
    }
    
    hold->drawAt(BLOCK_STRING, 21, 1);
}
*/

void Game::Ground() //바닥에 떨어진 고정된 테트로미노 출력
{
    for(int i = 0; i < BOARD_WIDTH; i++) 
    {
        for(int j = 0; j < BOARD_HEIGHT; j++) 
        {
            if(board_[i][j]) 
            {
                console::draw(i+1, j+1, BLOCK_STRING);
            }
        }
    }
}


bool Game::check(Tetromino now, int x, int y) // 벽과 아래에 있는 테트로미노와 닿는지 체크한다
{
    for(int i = 0; i < now.size(); i++) 
    {
        for(int j = 0; j < now.size(); j++) 
        {
            if(now.check(i, j)) 
            {
                if(!(x+i >= 1 && x+i <= 10 && y+j >= 1 && y+j <= 20 )) //벽과닿는지
                {
                    if(board_[x+i-1][y+j-1])//아래나 옆의 테트로미노와 닿는지
                    return false;
                }
            }
        }
    }
    return true;
}
bool Game::isGround() //바닥에 도달했는지 확인
{
    return !check(now, wh[0], wh[1]+1);
}


void Game::fix() //바닥에 고정
{
    for(int i = 0; i < now.size(); i++) 
    {
        for(int j = 0; j < now.size(); j++) 
        {
            if(now.check(i, j)) 
            {
                board_[wh[0]+i-1][wh[1]+j-1] = true;
            }
        }
    }
}

void Game::shadow() // 새도우 출력
{
    shadowwh[0] = wh[0];
    shadowwh[1] = wh[1];

    while(check(now, shadowwh[0], shadowwh[1]+1)) //y좌표가 닿기전위치에 새도우
    {
        shadowwh[1]++;
    }

    now.drawAt(SHADOW_STRING, shadowwh[0], shadowwh[1]);
}
void Game::remove() // 다찬라인제거
{
    int x = 0;
    for(int r = 0; r < BOARD_HEIGHT; r++) //행
    {
        for(int w = 0; w < BOARD_WIDTH; w++) //열
        {
            if(!board_[w][r]) 
            {
                
                return;
            }
            int x = 1;
        }
        if(x)
        {
            for(int i = r; i > 0; i--) //다 찬 라인 발견
            {
                for(int j = 0; j < BOARD_WIDTH; j++) 
                {
                    board_[j][i] = board_[j][i-1]; //한칸씩 아래로 내리기
                }
            }
        }
       
    
        linecount--;
    }
}


void Game::keyborad() //누르면 실행가능한지 체크후 실행
{
    if(console::key(console::K_LEFT)) //왼쪽키
    {
        if(Game::check(now, wh[0]-1, wh[1])) 
        {
            wh[0]--;
        }
    } 
    else if(console::key(console::K_RIGHT)) //오른쪽키
    {
        if(Game::check(now, wh[0]+1, wh[1])) 
        {
           wh[0]++;
        }
    } 
    else if(console::key(console::K_UP)) //하드드롭 새도우좌표로 즉시 내림
    {
        wh[0] = shadowwh[0];
        wh[1] = shadowwh[1];
    } 
    else if(console::key(console::K_DOWN)) //소프트드롭
    {
        if(Game::check(now, wh[0], wh[1]+1)) {
            wh[1]++; //내려오는 속도 빨라짐
        }
    } 
    else if(console::key(console::K_ESC)) //게임종료
    {
        finish = true;
    } 
    else if(console::key(console::K_X)) //시계방향
    {
        Tetromino tmp = now.rotatedCW();
        if(Game::check(tmp, wh[0], wh[1])) {
            now = tmp;
        }
    }
    else if(console::key(console::K_Z)) // 반시계방향
    {
        Tetromino tmp = now.rotatedCCW();
        if(Game::check(tmp, wh[0], wh[1])) {
            now = tmp;
        }
    } 
  
}










