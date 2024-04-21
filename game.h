
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include "console/console.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];
  
  int frame = 0;
  int linecount=40;
  bool finish = false;
  Tetromino rTetromino[7] = {Tetromino::I, Tetromino::O, Tetromino::T, Tetromino::S, Tetromino::Z, Tetromino::J, Tetromino::L};
  Tetromino now=Tetromino:: I, next=Tetromino::O , hold;
  int wh[2];
  int shadowwh[2];




  std::string timelog();
  void win();
  void lineCount();
  void printtime();
  void lost();
  Tetromino randomTetromino();
  void nextTetromino();
  void printTetromino();
  void printNext();
  void printHold();
  void Ground();
  bool check(Tetromino now, int x, int y);
  bool isGround();
  void fix();
  void shadow();
  void remove();
  void keyborad();
  

public:
 // 게임의 한 프레임을 처리한다. 프레임 출력을위해 없데이트 할 값을 정하는건가?
  void update()
  {
    lineCount();
    printtime();
    shadow();
    printTetromino();
    printNext();
    Ground();

    keyborad();
    frame++;
       if(frame>= 60) 
       {
        frame = 0;
        if(isGround()) // 닿았는지 확인
        {
            fix();
            remove();
            nextTetromino();

        } else {
            wh[1]++;// 프레임당 한칸
        }
    }
  }


    // 게임 화면을 그린다.
  void draw()
  {
    console::drawBox(0, 0, 11, 21); //메인박스 그리기
    console::drawBox(13, 0, 18, 5); //next박스 그리기
    console::drawBox(20, 0, 25, 5); //hold박스 그리기
    console::draw(15,0,"Next"); // next글자
    console::draw(22,0,"Hold"); // hold글자

  }
 

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit()//esc누르거나 40줄클리어 새로운 테트로미노를 생성 움직이지 못할경우
  {
    return finish;
  }
 
  Game()//초기상태 설정
  {//왜 여기서 텐트로미노의 기본생성자오류가 뜨는지 모르겠습니다
  
    
    for(int i = 0; i < 10; i++) 
    {
        for(int j = 0; j < 20; j++) 
        {
            board_[i][j] = false;
        }
    }

    now = Game::randomTetromino();
    next = Game::randomTetromino();
    
    wh[0] = (5-now.size()/2)+1;
    wh[1] = 1;
    
    
  }
};
#endif