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
//ȫ�ֱ���
#define window_height 480
#define window_width  640                  //���ڴ�С

#define pokemon_number 30              //��������

int game_status;
int map_init_x, map_init_y;
int character_x, character_y;
int character_picture_i, character_picture_j;
int pokemon_experiece = 0;                //�����ҷ����龭��
int exercise_Width = 0;                         //�ҷ�����۳���
int canvas[1652][1416] = { 0 };               //���廭������ ʹÿ�����ص�����Ϊ0

							  //NPC��λ������
int doctor_tong_x, doctor_tong_y;
int NPC_1_x, NPC_1_y;
int NPC_2_x, NPC_2_y;
int NPC_3_x, NPC_3_y;
int plot_judge;                       //�ж��ڵ�ͼˢ�µ�ʱ���Ƿ��������

char input;                                         //�����жϱ���

IMAGE main_map;                              //��ͼͼ�����
IMAGE character_vice;
IMAGE character;


//���徫�����ݵĽṹ��
struct pokemon
{
	int x;
	int y;
	int attack;					 //ս������
	int bleed;
	int defence;
	int special_attack;
	int special_defence;
	int level;
	int number;				// ������
	char *name;             //��������
	char *speciese;
	int skill_1;                 //����ܵĻ����˺�//
	int skill_2;
	int skill_3;
	int skill_4;
	char *skill1_name;           //��������
	char *skill2_name;
	void(*skill1)();               //���ܺ���
	void(*skill2)();
	int judge;
	int time;                             //��һ��ʱ���Ժ󽫰�ԭλ�õľ�������ˢ�³���
}PK[pokemon_number];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���к�������
//һ�����麯��
void plot_1();
//�������溯�����ĵ������ȡ����
void start_menu();
void pause_menu();
void readfile();
void writefile();
void aid_menu();
//�������ݸ��º�����ս����������������
void startup_map_show();//��ͼ��ʼ��
void startup_music();//���ֲ�����ر�
void closedown_music();
void map_show();//��ͼ��ʾ����
void operate();//������������
			 ///////////////////////////////////////////////////////////////////////////////////////////////
			 //�����ļ��Ĵ�
void startup_music()
{
	mciSendString(_T("open ��Դ�ļ���\\����\\��������.mp3 alias startmusic"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\ս������.mp3 alias fightmusic"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\��������.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\ʮ�����.mp3 alias sk1"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\���.mp3 alias sk2"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\��Ų�.mp3 alias sk3"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\����.mp3 alias sk4"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\����.mp3 alias ps"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\�����ཻ.mp3 alias enemy_1"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\������ը.mp3 alias enemy_2"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\1.mp3 alias enemy_3"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\2.mp3 alias enemy_4"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\3.mp3 alias enemy_5"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\4.mp3 alias enemy_6"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\5.mp3 alias enemy_7"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\6.mp3 alias enemy_8"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\7.mp3 alias enemy_9"), NULL, 0, NULL);
	mciSendString(_T("open ��Դ�ļ���\\����\\8.mp3 alias enemy_10"), NULL, 0, NULL);
}
//�����ʹ浵�ĺ���
void readfile()
{
	FILE *fp;
	fp = fopen("��Դ�ļ���\\��ʷ\\��Ϸ����.txt", "r");
	fscanf_s(fp, "%d %d %d %d %d %d %d %d", &map_init_x, &map_init_y, &character_picture_i, &character_picture_j, &pokemon_experiece, &PK[0].level, &exercise_Width, &plot_judge);
	fclose(fp);
}

void writefile()
{
	FILE *fp;
	fp = fopen( "��Դ�ļ���\\��ʷ\\��Ϸ����.txt", "w");
	fprintf(fp, "%d %d %d %d %d %d %d %d", map_init_x, map_init_y, character_picture_i, character_picture_j, pokemon_experiece, PK[0].level, exercise_Width, plot_judge);
	fclose(fp);
}

//��Ϸ�����������ͣ����
void start_menu()
{
	IMAGE starup_window;

	loadimage(&starup_window, _T("��Դ�ļ���\\����\\��½����.jpg"), 640, 480);
	putimage(0, 0, 640, 480, &starup_window, 0, 0);

	//���õ�¼��������
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 30;                      // ��������߶�
	wcscpy_s(f.lfFaceName, _T("����"));    // ��������
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(&f);                     // ����������ʽ
	outtextxy(300, 160, _T("1 ����Ϸ"));
	outtextxy(280, 240, _T("2 ��ȡ�浵"));
	outtextxy(310, 320, _T("3 �˳�"));
	outtextxy(310, 400, _T("4 ����"));

	LOGFONT t;
	gettextstyle(&t);                     // ��ȡ��ǰ��������
	t.lfHeight = 68;                      // ��������߶�
	wcscpy_s(t.lfFaceName, _T("����"));
	settextcolor(BLUE);
	t.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&t);                     // ����������ʽ
	outtextxy(200, 40, _T("�ڴ�����"));

	LOGFONT m;
	gettextstyle(&m);                     // ��ȡ��ǰ��������
	m.lfHeight = 30;                      // ��������߶�
	wcscpy_s(m.lfFaceName, _T("����"));
	settextcolor(BLUE);
	m.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&m);                     // ����������ʽ
	outtextxy(460, 70, _T("(���ư�)"));
	FlushBatchDraw();

	//��������      
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

	loadimage(&starup_window, _T("��Դ�ļ���\\����\\��½����.jpg"), 640, 480);
	putimage(0, 0, 640, 480, &starup_window, 0, 0);

	mciSendString(_T("stop bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play startmusic"), NULL, 0, NULL);

	//������ͣ��������
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 30;                      // ��������߶�
	wcscpy_s(f.lfFaceName, _T("����"));    // ��������
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(&f);                     // ����������ʽ
	outtextxy(300, 140, _T("1 ������Ϸ"));
	outtextxy(310, 240, _T("2 �浵"));
	outtextxy(310, 340, _T("3 �˳�"));
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

	loadimage(&aid_window, _T("��Դ�ļ���\\����\\��������.jpg"), 640, 480);
	putimage(0, 0, 640, 480, &aid_window, 0, 0);

	//���õ�¼��������
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 30;                      // ��������߶�
	wcscpy_s(f.lfFaceName, _T("����"));    // ��������
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(&f);                     // ����������ʽ
	outtextxy(50, 50, _T("��ӭ�������1037ɭ�֣��������㽫��"));
	outtextxy(0, 100, _T("��������̽�����顣"));

	settextcolor(RED);
	outtextxy(150, 150, _T("W,S,A,DΪ���������ƶ�"));
	outtextxy(150, 250, _T("ZΪȷ������QΪ�˳�/���˼�"));

	LOGFONT t;
	gettextstyle(&t);                     // ��ȡ��ǰ��������
	t.lfHeight = 25;                      // ��������߶�
	wcscpy_s(t.lfFaceName, _T("����"));    // ��������
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(&t);                     // ����������ʽ
	outtextxy(120, 350, _T("��л������ʦ�������ǵ������֧��~"));
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




//��ͼ��ʼ��
void startup_map_show()
{
	initgraph(window_width, window_height);             //��ʼ����ͼ����

	map_init_x = 298;
	map_init_y = 162;                               //��ͼ��ʼʱʱ��ȡ�ӣ�298,162����ʼ�Ĳ���//

	character_x = 170;                                               //����ĳ�ʼλ��
	character_y = 221;

	doctor_tong_x = 444;                            //NPCλ��
	doctor_tong_y = 400;
	NPC_1_x = 914;
	NPC_1_y = 413;
	NPC_2_x = 1097;
	NPC_2_y = 1048;
	NPC_3_x = 632;
	NPC_3_y = 780;

	character_picture_i = 0;
	character_picture_j = 0;
	char *s = "��Դ�ļ���\\����\\�ռ���ͼ.jpg";
	TCHAR p[40];
	CharToTchar(s, p);
	loadimage(&main_map, p, 1653, 1417);
	loadimage(&character_vice, _T("��Դ�ļ���\\����\\��������ͼ����ͼ.jpg", 136, 200));
	loadimage(&character, _T("��Դ�ļ���\\����\\��������ͼ.jpg", 136, 200));
	BeginBatchDraw();

	//��ͼ���ϰ����������ʽ���0��1��0Ϊ�ϰ������Թ���1Ϊ��ͨ��//
	for (int i = 0; i<1652; i++)
	{
		for (int j = 0; j<1416; j++)
		{
			if (i <= 425 || i >= 1223 - 45 || j <= 210 || j >= 1161 - 50)         //��ͼ�����ܵ���ľ//
			{
				canvas[i][j] = 0;
			}
			else if ((i>430 && i <= 552) && (j>210 && j <= 320))          //���Ϸ��ķ���
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 540 - 45 && i <= 552) && (j>320 && j <= 562))      //���������һ����ľ
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 659 - 45 && i <= 690) && (j >= 300 && j <= 542))     //�ڶ�����ľ
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 799 - 45 && i <= 821) && (j >= 211 && j <= 489))     //��������ľ
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 923 - 45 && i <= 1141) && (j >= 235 && j <= 357))    //���Ϸ��ĳ�������ľ    
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 1121 - 45 && i<1223) && (j >= 404 && j <= 521))
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 699 - 45 && i <= 721) && (j >= 513 && j <= 630))    //�����￪ʼ���м��ɭ�ֿ鶨�����//
			{
				canvas[i][j] = 0;
			}
			else if ((i>721 && i <= 770) && (j >= 543 && j <= 630))       //��������
			{
				canvas[i][j] = 0;
			}
			else if ((i>750 && i <= 917) && (j >= 543 && j <= 732))       //�м�
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 699 - 45 && i <= 917) && (j>702 && j <= 800))    //����
			{
				canvas[i][j] = 0;
			}
			else if ((i>897 && i <= 1007) && (j >= 435 && j <= 997))       //�ұ��������
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 402 && i<664) && (j >= 659 && j <= 725))         //�м䲿��ȫ��������ϣ���������ɢ����ľ�������
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 402 && i <= 587) && (j >= 817 && j <= 1000))       //����
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 646 && i <= 779) && (j >= 862 && j <= 1000))       //������
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 1092 && i<1223 - 45) && (j >= 862 && j <= 1000))   //����
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 828 && i <= 873) && (j >= 992 && j <= 1050))        //������С��
			{
				canvas[i][j] = 0;
			}
			else if ((i >= 1077 && i <= 1125) && (j >= 602 && j <= 808))     //����һ��
			{
				canvas[i][j] = 0;
			}
			else if ((i>426 && i<456) && (j>362 && j<392))       //NPCͯ��ʿ
			{
				canvas[i][j] = 0;
			}
			else if ((i>899 && i<935) && (j>378 && j<423))       //boss�׹�
			{
				canvas[i][j] = 0;
			}
			else if ((i>1077 && i<1125) && (j>1021 && j<1062))     //boss�׵�
			{
				canvas[i][j] = 0;
			}
			else if ((i>618 && i<656) && (j>758 && j<800))        //boss·����ŷ
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
//���ھ��飬�ǵ�ʯ��ʿ�����judge��Ϊ10�����ܵ㿪����һ���׹��ĶԻ���judge��Ϊ11����Ӯ��Ϊ5��������Judge��Ϊ11���������׵۶Ի�����Ӯ��Ϊ13����������BOSS��
//judge�ı仯Ϊ��0-1-10����
//��ʾ����
void map_show()
{
	putimage(0, 0, window_width, window_height, &main_map, map_init_x, map_init_y);
	putimage(character_x, character_y, 45, 50, &character_vice, character_picture_i * 45, character_picture_j * 50, NOTSRCERASE); //ÿ��С�˵������ǣ�45,50��
	putimage(character_x, character_y, 45, 50, &character, character_picture_i * 45, character_picture_j * 50, SRCINVERT);
	if (plot_judge == 1)                                                                           //�����ж�
	{
		plot_1();
		plot_judge = 10;                //���������ǰ�ƽ����������ӣ�ֻ�е���ָ�����ֲ������¼���
	}
	
	else
	{
		FlushBatchDraw();
	}
}

//������������
void operate()
{
	if (_kbhit())
	{                                                     //ÿ���ƶ�20�����ص�λ 
		input = _getch();
		if (character_picture_i == 2)               //ʵ���˽���ȡ����ͼ�Ĳ�ͬ����
		{
			character_picture_i = -1;
		}
		if ((input == 'A' || input == 'a') && canvas[map_init_x + character_x - 20][map_init_y + character_y] != 0)             //���ݷ�������������ݵĸ���
		{
			map_init_x -= 10;                       //10Ϊ�˵Ĳ���//
			character_picture_j = 2;
		}
		else if ((input == 'A' || input == 'a') && canvas[map_init_x + character_x - 20][map_init_y + character_y] == 0)
		{
			character_picture_j = 2;
		}
		if ((input == 'D' || input == 'd') && canvas[map_init_x + character_x + 20][map_init_y + character_y] != 0)           //�����ж����ж��н������Ƿ�Ϊ�ϰ���//
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

//�����л�����
void interface_change_animation()
{
	mciSendString(_T("play ps"), NULL, 0, NULL);
	int i, j;
	i = 320, j = 0;                                            //�����Ϸ���ʼ��
	setlinecolor(BLACK);                              //������ɫ����Ϊ��ɫ
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
	//�����ﶯ������
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���鲿��
void plot_1()
{
	setfillcolor(LIGHTGRAY);                   //�Ի���
	solidroundrect(5, 340, 630, 475, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(15, 350, 620, 465, 10, 10);

	//���½�չ�ֶԻ�����
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 25;                      // ��������߶�
	wcscpy_s(f.lfFaceName, _T("����"));    // ��������
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(&f);                     // ����������ʽ
	outtextxy(16, 350, _T("ʯ��ʿ����ӭ����1037ɭ�֣�����������ѧУ��"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 375, _T("�ص�С���飬����һֱ��������������һƬɭ���У�"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 400, _T("�������ɭ���������һֻ���ޣ����ǵ�С�����"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 425, _T("��ʧ�ٻ������ˣ����������ټ�ȫУ����ʿ��������"));
	FlushBatchDraw();
	Sleep(1000);
	outtextxy(16, 450, _T("������ֻ���ޣ�������ҵر�ѡ���ˡ�"));
	FlushBatchDraw();
	Sleep(1000);
}
//������
void main()
{
	startup_music();
	startup_map_show();                             //�Ե�ͼ���г�ʼ��
	start_menu();                                  //��½����
	while (1)
	{
		map_show();										//��ͼ��ʾ����
		operate();								// �ͻ���������
	}
}


