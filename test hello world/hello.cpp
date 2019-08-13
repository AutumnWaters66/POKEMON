#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "hanshuku.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//全局变量
#define window_height 480
#define window_width  640                  //窗口大小

#define pokemon_number 30              //精灵数量

int game_status;
int map_init_x, map_init_y;
int character_x, character_y;
int character_picture_i, character_picture_j;
int pokemon_experiece = 0;                //定义我方精灵经验
int exercise_Width = 0;                         //我方经验槽长度
int canvas[1652][1416] = { 0 };               //定义画布坐标 使每个像素点坐标为0

							  //NPC的位置坐标
int doctor_tong_x, doctor_tong_y;
int NPC_1_x, NPC_1_y;
int NPC_2_x, NPC_2_y;
int NPC_3_x, NPC_3_y;
int plot_judge;                       //判断在地图刷新的时候是否切入剧情

char input;                                         //输入判断变量

IMAGE main_map;                              //地图图像对象
IMAGE character_vice;
IMAGE character;


//定义精灵数据的结构体
struct pokemon
{
	int x;
	int y;
	int attack;					 //战斗属性
	int bleed;
	int defence;
	int special_attack;
	int special_defence;
	int level;
	int number;				// 精灵编号
	char *name;             //精灵名称
	char *speciese;
	int skill_1;                 //四项技能的基础伤害//
	int skill_2;
	int skill_3;
	int skill_4;
	char *skill1_name;           //技能名字
	char *skill2_name;
	void(*skill1)();               //技能函数
	void(*skill2)();
	int judge;
	int time;                             //在一定时间以后将把原位置的精灵重新刷新出来
}PK[pokemon_number];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//所有函数声明
//一、剧情函数
void plot_1();
//二、界面函数，文档保存读取函数
void start_menu();
void pause_menu();
void readfile();
void writefile();
void aid_menu();
//三、数据更新函数，战斗函数，动画函数
void startup_map_show();//地图初始化
void startup_music();//音乐播放与关闭
void closedown_music();
void map_show();//地图显示函数
void operate();//交互操作函数
			 ///////////////////////////////////////////////////////////////////////////////////////////////
			 //音乐文件的打开
void startup_music()
{
	mciSendString(_T("open 资源文件夹\\音乐\\开场音乐.mp3 alias startmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\战斗音乐.mp3 alias fightmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\背景音乐.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\十万伏特.mp3 alias sk1"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\尖叫.mp3 alias sk2"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\电磁波.mp3 alias sk3"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\打雷.mp3 alias sk4"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\闪白.mp3 alias ps"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\武器相交.mp3 alias enemy_1"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\多声爆炸.mp3 alias enemy_2"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\1.mp3 alias enemy_3"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\2.mp3 alias enemy_4"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\3.mp3 alias enemy_5"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\4.mp3 alias enemy_6"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\5.mp3 alias enemy_7"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\6.mp3 alias enemy_8"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\7.mp3 alias enemy_9"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件夹\\音乐\\8.mp3 alias enemy_10"), NULL, 0, NULL);
}
//读档和存档的函数
void readfile()
{
	FILE *fp;
	fp = fopen("资源文件夹\\历史\\游戏数据.txt", "r");
	fscanf_s(fp, "%d %d %d %d %d %d %d %d", &map_init_x, &map_init_y, &character_picture_i, &character_picture_j, &pokemon_experiece, &PK[0].level, &exercise_Width, &plot_judge);
	fclose(fp);
}

void writefile()
{
	FILE *fp;
	fp = fopen( "资源文件夹\\历史\\游戏数据.txt", "w");
	fprintf(fp, "%d %d %d %d %d %d %d %d", map_init_x, map_init_y, character_picture_i, character_picture_j, pokemon_experiece, PK[0].level, exercise_Width, plot_judge);
	fclose(fp);
}

//游戏开场界面和暂停界面
void start_menu()
{
	IMAGE starup_window;

	loadimage(&starup_window, _T("资源文件夹\\其它\\登陆界面.jpg"), 640, 480);
	putimage(0, 0, 640, 480, &starup_window, 0, 0);

	//设置登录画面文字
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 30;                      // 设置字体高度
	wcscpy_s(f.lfFaceName, _T("楷体"));    // 设置字体
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(&f);                     // 设置字体样式
	outtextxy(300, 160, _T("1 新游戏"));
	outtextxy(280, 240, _T("2 读取存档"));
	outtextxy(310, 320, _T("3 退出"));
	outtextxy(310, 400, _T("4 帮助"));

	LOGFONT t;
	gettextstyle(&t);                     // 获取当前字体设置
	t.lfHeight = 68;                      // 设置字体高度
	wcscpy_s(t.lfFaceName, _T("楷体"));
	settextcolor(BLUE);
	t.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&t);                     // 设置字体样式
	outtextxy(200, 40, _T("口袋妖怪"));

	LOGFONT m;
	gettextstyle(&m);                     // 获取当前字体设置
	m.lfHeight = 30;                      // 设置字体高度
	wcscpy_s(m.lfFaceName, _T("楷体"));
	settextcolor(BLUE);
	m.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&m);                     // 设置字体样式
	outtextxy(460, 70, _T("(华科版)"));
	FlushBatchDraw();

	//播放音乐      
	mciSendString(_T("play startmusic"), NULL, 0, NULL);

loop:
	input = _getch();
	if (input == '1')
	{
		mciSendString(_T("close startmusic"), NULL, 0, NULL);
		mciSendString(_T("play bkmusic"), NULL, 0, NULL);
		game_status = 1;
	}
	else if (input == '2')
	{
		readfile();
		mciSendString(_T("close startmusic"), NULL, 0, NULL);
		mciSendString(_T("play bkmusic"), NULL, 0, NULL);
		game_status = 1;
	}
	else if (input == '3')
	{
		game_status = 0;
		exit(0);
	}
	else if (input == '4')
	{
		aid_menu();
	}
	else
	{
		goto loop;
	}
}

void pause_menu()
{
	IMAGE starup_window;

	loadimage(&starup_window, _T("资源文件夹\\其它\\登陆界面.jpg"), 640, 480);
	putimage(0, 0, 640, 480, &starup_window, 0, 0);

	mciSendString(_T("stop bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play startmusic"), NULL, 0, NULL);

	//设置暂停画面文字
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 30;                      // 设置字体高度
	wcscpy_s(f.lfFaceName, _T("楷体"));    // 设置字体
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(&f);                     // 设置字体样式
	outtextxy(300, 140, _T("1 继续游戏"));
	outtextxy(310, 240, _T("2 存档"));
	outtextxy(310, 340, _T("3 退出"));
	FlushBatchDraw();

loop:
	input = _getch();
	if (input == '1')
	{
		game_status = 1;
		mciSendString(_T("close startmusic"), NULL, 0, NULL);
		mciSendString(_T("play bkmusic"), NULL, 0, NULL);
	}
	else if (input == '2')
	{
		writefile();
		mciSendString(_T("close startmusic"), NULL, 0, NULL);
		mciSendString(_T("play bkmusic"), NULL, 0, NULL);
		game_status = 1;
	}
	else if (input == '3')
	{
		game_status = 0;
		exit(0);
	}
	else
	{
		goto loop;
	}
}
void aid_menu()
{
	IMAGE aid_window;

	loadimage(&aid_window, _T("资源文件夹\\其它\\帮助界面.jpg"), 640, 480);
	putimage(0, 0, 640, 480, &aid_window, 0, 0);

	//设置登录画面文字
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 30;                      // 设置字体高度
	wcscpy_s(f.lfFaceName, _T("楷体"));    // 设置字体
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(&f);                     // 设置字体样式
	outtextxy(50, 50, _T("欢迎大家来到1037森林，在这里你将经"));
	outtextxy(0, 100, _T("历别样的探险体验。"));

	settextcolor(RED);
	outtextxy(150, 150, _T("W,S,A,D为上下左右移动"));
	outtextxy(150, 250, _T("Z为确定键，Q为退出/后退键"));

	LOGFONT t;
	gettextstyle(&t);                     // 获取当前字体设置
	t.lfHeight = 25;                      // 设置字体高度
	wcscpy_s(t.lfFaceName, _T("楷体"));    // 设置字体
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(&t);                     // 设置字体样式
	outtextxy(120, 350, _T("感谢所有老师和朋友们的陪伴与支持~"));
	FlushBatchDraw();

loop:
	input = _getch();
	if (input == 'q' || input == 'Q')
	{
		start_menu();
	}
	else
	{
		goto loop;
	}
}




//地图初始化
void startup_map_show()
{
	initgraph(window_width, window_height);             //初始化绘图环境

	map_init_x = 298;
	map_init_y = 162;                               //地图初始时时截取从（298,162）开始的部分//

	character_x = 170;                                               //人物的初始位置
	character_y = 221;

	doctor_tong_x = 444;                            //NPC位置
	doctor_tong_y = 400;
	NPC_1_x = 914;
	NPC_1_y = 413;
	NPC_2_x = 1097;
	NPC_2_y = 1048;
	NPC_3_x = 632;
	NPC_3_y = 780;

	character_picture_i = 0;
	character_picture_j = 0;
	char *s = "资源文件夹\\其它\\终极地图.jpg";
	TCHAR p[40];
	CharToTchar(s, p);
	loadimage(&main_map, p, 1653, 1417);
	loadimage(&character_vice, _T("资源文件夹\\其它\\桌面行走图遮罩图.jpg", 136, 200));
	loadimage(&character, _T("资源文件夹\\其它\\桌面行走图.jpg", 136, 200));
	BeginBatchDraw();

	//地图上障碍以数组的形式存放0，1，0为障碍不可以过，1为可通行//
	for (int i = 0; i<1652; i++)
	{
		for (int j = 0; j<1416; j++)
		{
			if (i <= 425 || i >= 1223 - 45 || j <= 210 || j >= 1161 - 50)         //地图最四周的树木//
			{
				canvas[i][j] = 0;
			}
			else if ((i>430 && i <= 552) && (j>210 && j <= 320))          //左上方的房子
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 540 - 45 && i <= 552) && (j>320 && j <= 562))      //房子下面的一排树木
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 659 - 45 && i <= 690) && (j >= 300 && j <= 542))     //第二排树木
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 799 - 45 && i <= 821) && (j >= 211 && j <= 489))     //第三排树木
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 923 - 45 && i <= 1141) && (j >= 235 && j <= 357))    //右上方的长方形树木    
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 1121 - 45 && i<1223) && (j >= 404 && j <= 521))
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 699 - 45 && i <= 721) && (j >= 513 && j <= 630))    //从这里开始将中间的森林块定义完毕//
			{
				canvas[i][j] = 0;
			}
			else if ((i>721 && i <= 770) && (j >= 543 && j <= 630))       //中上两棵
			{
				canvas[i][j] = 0;
			}
			else if ((i>750 && i <= 917) && (j >= 543 && j <= 732))       //中间
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 699 - 45 && i <= 917) && (j>702 && j <= 800))    //左下
			{
				canvas[i][j] = 0;
			}
			else if ((i>897 && i <= 1007) && (j >= 435 && j <= 997))       //右边最长的两排
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 402 && i<664) && (j >= 659 && j <= 725))         //中间部分全部设置完毕，将四周零散的树木设置完毕
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 402 && i <= 587) && (j >= 817 && j <= 1000))       //左下
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 646 && i <= 779) && (j >= 862 && j <= 1000))       //左下中
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 1092 && i<1223 - 45) && (j >= 862 && j <= 1000))   //右下
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 828 && i <= 873) && (j >= 992 && j <= 1050))        //单独的小树
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 1077 && i <= 1125) && (j >= 602 && j <= 808))     //右中一排
			{
				canvas[i][j] = 0;
			}
			else if ((i>426 && i<456) && (j>362 && j<392))       //NPC童博士
			{
				canvas[i][j] = 0;
			}
			else if ((i>899 && i<935) && (j>378 && j<423))       //boss雷公
			{
				canvas[i][j] = 0;
			}
			else if ((i>1077 && i<1125) && (j>1021 && j<1062))     //boss炎帝
			{
				canvas[i][j] = 0;
			}
			else if ((i>618 && i<656) && (j>758 && j<800))        //boss路卡利欧
			{
				canvas[i][j] = 0;
			}
			else
			{
				canvas[i][j] = 1;
			}
		}

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//对于剧情，是到石博士那里后judge变为10，才能点开与下一个雷公的对话，judge变为11，打赢变为5，才能让Judge变为11，才能与炎帝对话，打赢变为13，能与最终BOSS打
//judge的变化为：0-1-10……
//显示函数
void map_show()
{
	putimage(0, 0, window_width, window_height, &main_map, map_init_x, map_init_y);
	putimage(character_x, character_y, 45, 50, &character_vice, character_picture_i * 45, character_picture_j * 50, NOTSRCERASE); //每个小人的像素是（45,50）
	putimage(character_x, character_y, 45, 50, &character, character_picture_i * 45, character_picture_j * 50, SRCINVERT);
	if (plot_judge == 1)                                                                           //剧情判断
	{
		plot_1();
		plot_judge = 10;                //对于情节往前推进的数字链接，只有到达指定数字才能往下继续
	}
	
	else
	{
		FlushBatchDraw();
	}
}

//交互操作函数
void operate()
{
	if (_kbhit())
	{                                                     //每次移动20个像素单位 
		input = _getch();
		if (character_picture_i == 2)               //实现了交替取人物图的不同部分
		{
			character_picture_i = -1;
		}
		if ((input == 'A' || input == 'a') && canvas[map_init_x + character_x - 20][map_init_y + character_y] != 0)             //根据方向操作进行数据的更新
		{
			map_init_x -= 10;                       //10为人的步长//
			character_picture_j = 2;
		}
		else if ((input == 'A' || input == 'a') && canvas[map_init_x + character_x - 20][map_init_y + character_y] == 0)
		{
			character_picture_j = 2;
		}
		if ((input == 'D' || input == 'd') && canvas[map_init_x + character_x + 20][map_init_y + character_y] != 0)           //最后的判断是判断行进方向是否为障碍物//
		{
			map_init_x += 10;
			character_picture_j = 1;
		}
		else if ((input == 'D' || input == 'd') && canvas[map_init_x + character_x + 20][map_init_y + character_y] == 0)
		{
			character_picture_j = 1;
		}
		if ((input == 'S' || input == 's') && canvas[map_init_x + character_x][map_init_y + character_y + 20] != 0)
		{
			map_init_y += 10;
			character_picture_j = 0;
		}
		else if ((input == 'S' || input == 's') && canvas[map_init_x + character_x][map_init_y + character_y + 20] == 0)
		{
			character_picture_j = 0;
		}
		if ((input == 'W' || input == 'w') && canvas[map_init_x + character_x][map_init_y + character_y - 20] != 0)
		{
			map_init_y -= 10;
			character_picture_j = 3;
		}
		else if ((input == 'W' || input == 'w') && canvas[map_init_x + character_x][map_init_y + character_y - 20] == 0)
		{
			character_picture_j = 3;
		}
		if ((input == 'Z' || input == 'z') && abs(map_init_x + character_x - doctor_tong_x) <= 30 && abs(map_init_y + character_y - doctor_tong_y) <= 30 && plot_judge == 0)
		{
			plot_judge = 1;
		}
		else if ((input == 'Z' || input == 'z') && abs(map_init_x + character_x - NPC_1_x) <= 30 && abs(map_init_y + character_y - NPC_1_y) <= 30 && plot_judge == 10)
		{
			plot_judge = 2;
		}
		else if ((input == 'Z' || input == 'z') && abs(map_init_x + character_x - NPC_2_x) <= 30 && abs(map_init_y + character_y - NPC_2_y) <= 30 && plot_judge == 12)
		{
			plot_judge = 3;
		}
		else if ((input == 'Z' || input == 'z') && abs(map_init_x + character_x - NPC_3_x) <= 30 && abs(map_init_y + character_y - NPC_3_y) <= 30 && plot_judge == 13)
		{
			plot_judge = 4;
		}
		if (input == 'q' || input == 'Q')
		{
			pause_menu();
		}
		character_picture_i++;
	}
}

//界面切换动画
void interface_change_animation()
{
	mciSendString(_T("play ps"), NULL, 0, NULL);
	int i, j;
	i = 320, j = 0;                                            //从正上方开始画
	setlinecolor(BLACK);                              //画线颜色设置为黑色
	while (!(i == 319 && j == 0))
	{
		line(320, 240, i, j);
		FlushBatchDraw();
		if (i == 640)
			j++;
		if (i == 0)
			j--;
		if (j == 600)
			i--;
		if (j == 0)
			i++;
	}
	//到这里动画结束
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//剧情部分
void plot_1()
{
	setfillcolor(LIGHTGRAY);                   //对话框
	solidroundrect(5, 340, 630, 475, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(15, 350, 620, 465, 10, 10);

	//往下将展现对话内容
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 25;                      // 设置字体高度
	wcscpy_s(f.lfFaceName, _T("楷体"));    // 设置字体
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(&f);                     // 设置字体样式
	outtextxy(16, 350, _T("石博士：欢迎来到1037森林，这里有我们学校独"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 375, _T("特的小精灵，它们一直和睦地生活在这一片森林中，"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 400, _T("但是最近森林深处出现了一只恶兽，我们的小精灵很"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 425, _T("多失踪或者受伤，现在我们召集全校的勇士帮助我们"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 450, _T("击败这只恶兽，你很荣幸地被选中了。"));
	FlushBatchDraw();
	Sleep(1000);
}
//主函数
void main()
{
	startup_music();
	startup_map_show();                             //对地图进行初始化
	start_menu();                                  //登陆界面
	while (1)
	{
		map_show();										//地图显示函数
		operate();								// 客户交互函数
	}
}


