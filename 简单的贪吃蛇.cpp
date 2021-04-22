//Use    StartGame_tcs()   to start game

#include <iostream>
#include <windows.h>
#include <conio.h> //控制台输入输出头文件
#define MAXWIDTH 68
#define MAXHEIGH 22
using namespace std;

typedef struct position
{
    int x,y;
    struct position *last,*next;
}pos,*posp;

pos tailx={MAXWIDTH/2-2,MAXHEIGH/2},headx{MAXWIDTH/2,MAXHEIGH/2,NULL,&tailx},*tail=&tailx,*head=&headx;
int direction = 77;   // right

// "□■◇"  横2,纵1

void gotoxy(int x,int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}


class Snake;
class Food
{
public:
    int x,y,score;
    Food()
    {
        x = rand()%((MAXWIDTH-4)/2 +1) * 2+ 2; //2~64,偶数
        y = rand()%(MAXHEIGH-1)+ 1; //1~21
        score = rand()%250;
    }
    void SetFood()
    {
        while(1)
        {
            x = rand()%((MAXWIDTH-4)/2 +1) * 2+ 2; //2~64,偶数
            y = rand()%(MAXHEIGH-1)+ 1; //1~21
            score = rand()%250;

            posp p = new pos;
            p = head;
            while(p)
            {
                if( x==p->x && y==p->y )
                break;

                p = p->next;
            }
            if(!p)  break;
        }
        gotoxy(x,y);
        cout<<"◇";
    }

};

class Snake
{
private:
    int length,score;
    double speed;
public:
    Snake(int lengthx=2,double speedx=1,int scorex=0)
    {
        length = lengthx;
        speed = speedx;
        score = scorex;
    }
    int GetSpeed(){return speed;}
    int GetLength(){return length;}
    int GetScore(){return score;}

    void UpLevel(Food &food)
    {
        length++;
        if(speed<9) speed+=0.5;
        score += food.score;
        gotoxy(MAXWIDTH+4,MAXHEIGH/2);
        cout<<"score: "<<score;

        food.SetFood();
    }
};

void move(Snake &snake,Food &food)
{

    posp p = new pos, temp;
    p->x = head->x;
    p->y = head->y;
    p->last = NULL;
    p->next = head;

//  *“↑”：72
//	*“↓”：80
//	*“→”：77
//	*“←”：75

    if(direction==72||direction=='w')   //up
        p->y = head->y - 1 ;
    else if(direction==80||direction=='s')   //down
        p->y = head->y + 1;
    else if(direction==77||direction=='d')   //right
        p->x = head->x + 2;
    else if(direction==75||direction=='a')   //left
        p->x = head->x - 2;

    head->last = p;
    head = p;
    gotoxy(p->x,p->y);
    cout<<"□";

    if( head->x==food.x && head->y==food.y )    //get food
        snake.UpLevel(food);
    else    //吃到东西不去掉尾部就可以了啊
    {

        gotoxy(tail->x,tail->y);
        cout<<"  "; //两个空格
        temp=tail;
        tail = tail->last;
        tail->next = NULL;
        delete temp;
        gotoxy(0,0);
    }




    if(head->x >= MAXWIDTH-2 || head->x <=0)  exit(snake.GetScore()) ;
    if(head->y >= MAXHEIGH || head->y <=0)  exit(snake.GetScore());
    p = head->next;
    while(p)
    {
        if(p->x==head->x&&p->y==head->y)
            exit(snake.GetScore());

        p=p->next;
    }



}

void control(Snake &snake,Food &food)
{
	int pre_dir = direction;//记录前一个按键的方向
	if (_kbhit())//如果用户按下了键盘中的某个键
	{
		//fflush(stdin);//清空缓冲区的字符
        cin.sync();
		//getch()读取方向键的时候，会返回两次，第一次调用返回0或者224，第二次调用返回的才是实际值
		direction = _getch();//第一次调用返回的不是实际值
		direction = _getch();//第二次调用返回实际值
	}

	if (pre_dir == 72 && direction == 80)      //up->down == up
		direction = 72;
	else if (pre_dir == 80 && direction == 72)      //down->up == down
		direction = 80;
	else if (pre_dir == 75 && direction == 77)      //left->right == left
		direction = 75;
	else if (pre_dir == 77 && direction == 75)      //right->left == right
		direction = 77;

	/**
	*控制台按键所代表的数字
	*“↑”：72
	*“↓”：80
	*“←”：75
	*“→”：77
	*/
    move(snake,food);

}

void StartGame_tcs()
{
    Food food;
    food.SetFood();
    tail->last = head;  //这个是完成初始化
    Snake snake;

    gotoxy(MAXWIDTH+4,MAXHEIGH/2);
    cout<<"score: "<<snake.GetScore();


    posp p;
    p = head;
    while(p)
    {
        gotoxy(p->x,p->y);
        cout<<"□";
        p=p->next;
    }

    for(int i=0;i<MAXWIDTH;i+=2)
    {
        gotoxy(i,0);
        cout<<"■";
        gotoxy(i,MAXHEIGH);
        cout<<"■";
    }
    for(int i=0;i<=MAXHEIGH;i++)
    {
        gotoxy(0,i);
        cout<<"■";
        gotoxy(MAXWIDTH-2,i);
        cout<<"■";
    }

    while(1)
    {
        control(snake,food);
        Sleep( (10-snake.GetSpeed())*50 );
    }
}


