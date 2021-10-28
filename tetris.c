//terminal font 24,  자간 8로 설정
//글자 두배로 키우는 기능  <—  ESC#3, ESC#4

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termio.h>
#include <unistd.h>
#include <wchar.h>

void gotoxy(int x, int y);
int getch();
void print_block(int (*)[4]);
void print_frame();
void f1(int ** shape);


typedef struct {
    int * x_arr;
    int * y_arr;
    int shape[4][4];
    void (*spin)(int **);
} block_struct;

int main()
{
    srand(time(NULL));
    printf("\033#3");

    block_struct block[6] ={
        {(int[16]){0}, (int[16]){0}, 
        {
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0}
        },
        f1},
        {(int[16]){0}, (int[16]){0}, 
        {
         {0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}
        },
        f1},
        {(int[16]){0}, (int[16]){0}, 
        {
         {0, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 1, 0}
        },
        f1},
        {(int[16]){0}, (int[16]){0}, 
        {
         {0, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 1, 0}
        },
        f1},
        {(int[16]){0}, (int[16]){0}, 
        {
         {0, 0, 0, 0},
         {0, 0, 1, 0},
         {0, 1, 1, 1},
         {0, 0, 0, 0}
        },
        f1},
        {(int[16]){0}, (int[16]){0}, 
        {
         {0, 0, 0, 0},
         {0, 0, 1, 0},
         {0, 1, 1, 1},
         {0, 0, 1, 0}
        },
        f1}
    };

    printf("\e[?25l"); //커서 깜빡임 제거

    
    int x = 6;
    int y = 1;
    int key = 0;
    int current = (rand() % 6);
    while(1)
    {
        printf("\033[2J");//tput clear
        gotoxy(1,1);
        print_frame();//게임틀 출력
        gotoxy(x,y);
        print_block(block[current].shape);
        //print_block(block[rand()%6].shape);
        //printf("□");
        //printf("■■■■■■■■■■■\n");
        
        key = getch();
        if(key == 66)
        {
            y++;
            //아래로 떨구기
        }
        if(key == 67)
        {
            x++;
        }
        if(key == 68)
        {
            x--;
        }
    }
    
}

void f1(int ** shape)
{
    int cp_shape[4][4];
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cp_shape[i][j] = shape[i][j];
        }
    }
   for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = cp_shape[j][3-i];
        }
    }
}


void gotoxy(int x, int y) 
{
     printf("\033[%d;%df",y,x);

     fflush(stdout);
}



int getch()
{
    int c;
    struct termios oldattr, newattr;

    tcgetattr(STDIN_FILENO, &oldattr);           // 현재 터미널 설정 읽음
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);         // CANONICAL과 ECHO 끔
    newattr.c_cc[VMIN] = 1;                      // 최소 입력 문자 수를 1로 설정
    newattr.c_cc[VTIME] = 0;                     // 최소 읽기 대기 시간을 0으로 설정
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);  // 터미널에 설정 입력
    c = getchar();                               // 키보드 입력 읽음
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);  // 원래의 설정으로 복구
    return c;
}

void print_block(int (*shape)[4])
{
    for(int i = 0; i < 4; i ++)
    {
        printf("\033[s");//커서 위치 저장
        for(int j = 0; j < 4; j++)
        {
            if(shape[i][j] == 0)
            printf(" ");
            if(shape[i][j] == 1)
            printf("□");
        }
        printf("\033[u");//저장된 커서 위치로 이동
        printf("\033[%dB", 1);
        //printf("\n");
    }
}

void print_frame()
{
    
    int frame[20][12] = 
    {
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1}
    };
    for(int i = 0; i < 20; i++)
    {
        
        for(int j = 0; j < 12; j++)
        {
            if(frame[i][j] == 0)
            {
                printf(" ");
            }
            else if (frame[i][j] == 1)
            {
                printf("▨");
            }
            else if (frame[i][j] == 2)
            {
                printf("■");
            }
        }
        printf("\n");
    }
    
    /*
    for(int i = 0; i < 20; i++)
    {
        printf("%s\n", frame[i]);
    }
    */
    /*
    wchar_t frame[20][10] = 
    {
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨'}
    };
    wprintf(L"%ls\n", frame[1][1]);
    //wchar_t (*p)[10] = frame;
    for(int i = 0; i < 20; i++)
    {
        wprintf(L"%ls\n", frame[i]);
    }
    */
}