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
	char* name;             //��������
	char* speciese;
	int skill_1;                 //����ܵĻ����˺�//
	int skill_2;
	int skill_3;
	int skill_4;
	char* skill1_name;           //��������
	char* skill2_name;
	void(*skill1)();               //���ܺ���
	void(*skill2)();
	char* skill3_name;           //��������
	char* skill4_name;
	void(*skill3)();               //���ܺ���
	void(*skill4)();
	int judge;
	int time;                             //��һ��ʱ���Ժ󽫰�ԭλ�õľ�������ˢ�³���
}PK[pokemon_number];
struct ThunderBolt
{
	char* name = "ʮ�����";
	char* speciese = "special";
	int damage = 90;
}ThunderBolt;
struct Growl
{
	char* name = "����";
	char* speciese = "vary";
	int damage = 0;
}Growl;
struct ThunderWave
{
	char* name = "��Ų�";
	char* speciese = "vary";
	int damage = 0;
}ThunderWave;
struct Thunder
{
	char* name = "����";
	char* speciese = "special";
	int damage = 110;
}Thunder;

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
void startup_pokemon_judge();//����ʱ���ʼ��
void startup_pokemon();//����������Գ�ʼ��
void startup_pokemon_bleed();//����Ѫ����ʼ��
void startup_pokemon_desination();//����λ�ó�ʼ��
void startup_map_show();//��ͼ��ʼ��
void startup_music();//���ֲ�����ر�
void closedown_music();
void pokemon_refresh();//����ˢ��
void map_show();//��ͼ��ʾ����
void operate();//������������
void load_PK_picture(pokemon* PK);//����з�����ͼƬ
void load_PK_skill(pokemon* PK, int PK_bleed, int full_bleed);//�з�����
void interface_change_animation();//�����л�����
void enemy_fight_show(pokemon* PK_enemy);//�з�����ͼƬ�����Լ�������ʾ
void fight_show(pokemon* PK_enemy, pokemon* PK_own);//ս����ʾ����
void fight();//ս������
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
void startup_pokemon_judge()
{
	int i;
	for (i = 0; i <= pokemon_number; i++)
	{
		PK[i].time = 0;
	}
}
//����������Գ�ʼ��
void startup_pokemon()
{
	//Ƥ����//
	PK[0].level = 5;
	PK[0].x = 0;
	PK[0].y = 0;
	PK[0].number = 0;
	PK[0].attack = 8 + (PK[0].level - 5) * 2;
	PK[0].bleed = 32 + (PK[0].level - 5) * 3;
	PK[0].defence = 4 + (PK[0].level - 5);
	PK[0].special_attack = 9 + (PK[0].level - 5) * 2;
	PK[0].special_defence = 7 + (PK[0].level - 5);
	PK[0].name = "Ƥ����";
	PK[0].speciese = "��";
	PK[0].skill1_name = ThunderBolt.name;
	PK[0].skill2_name = Growl.name;
	PK[0].skill3_name = ThunderWave.name;
	PK[0].skill4_name = Thunder.name;
	PK[0].skill_1 = ThunderBolt.damage;
	PK[0].skill_2 = Growl.damage;
	PK[0].skill_3 = ThunderWave.damage;
	PK[0].skill_4 = Thunder.damage;
	PK[0].judge = 1;

	//С����//
	PK[1].x = 0;                       //�����ڵ�ͼ�ϵ�����//
	PK[1].y = 0;
	PK[1].number = 1;
	PK[1].attack = 8;                    //�չ�
	PK[1].bleed = 30;                    //����ֵ
	PK[1].defence = 4;                   //�շ�
	PK[1].special_attack = 9;            //�ع�
	PK[1].special_defence = 7;           //�ط�
	PK[1].level = 5;                   //�ȼ�
	PK[1].name = "С����";             //����
	PK[1].speciese = "��";             //Ԫ������
	PK[1].skill1 = &firehigh;
	PK[1].skill2 = &boom;
	PK[1].skill1_name = "����";
	PK[1].skill2_name = "��ը";
	PK[1].skill_1 = 50;                  //����1�˺�
	PK[1].skill_2 = 80;                  //����2�˺�
	PK[1].skill_3 = 0;
	PK[1].skill_4 = 0;
	PK[1].judge = 0;

	//��������//
	PK[2].x = 0;
	PK[2].y = 0;
	PK[2].number = 2;
	PK[2].attack = 6;
	PK[2].bleed = 34;
	PK[2].defence = 3;
	PK[2].special_attack = 8;
	PK[2].special_defence = 6;
	PK[2].level = 5;
	PK[2].name = "��������";
	PK[2].speciese = "��";
	PK[2].skill1 = &rope;
	PK[2].skill2 = &fallingStar;
	PK[2].skill1_name = "��Ҷ�쵶";
	PK[2].skill2_name = "��������";
	PK[2].skill_1 = 30;
	PK[2].skill_2 = 40;
	PK[2].skill_3 = 0;
	PK[2].skill_4 = 0;

	//������//
	PK[3].x = 0;
	PK[3].y = 0;
	PK[3].number = 3;
	PK[3].attack = 7;
	PK[3].bleed = 38;
	PK[3].defence = 5;
	PK[3].special_attack = 10;
	PK[3].special_defence = 6;
	PK[3].level = 5;
	PK[3].name = "������";
	PK[3].speciese = "ˮ";
	PK[3].skill1 = &star;
	PK[3].skill2 = &waterattack;
	PK[3].skill1_name = "��������";
	PK[3].skill2_name = "ˮ��";
	PK[3].skill_1 = 70;
	PK[3].skill_2 = 80;
	PK[3].skill_3 = 0;
	PK[3].skill_4 = 0;

	//������//
	PK[4].x = 0;
	PK[4].y = 0;
	PK[4].number = 4;
	PK[4].attack = 8;
	PK[4].bleed = 42;
	PK[4].defence = 4;
	PK[4].special_attack = 9;
	PK[4].special_defence = 7;
	PK[4].level = 5;
	PK[4].name = "������";
	PK[4].speciese = "��";
	PK[4].skill1 = &stoneattack;
	PK[4].skill2 = &musicattack;
	PK[4].skill1_name = "��ʯͷ";
	PK[4].skill2_name = "����";
	PK[4].skill_1 = 35;
	PK[4].skill_2 = 60;
	PK[4].skill_3 = 0;
	PK[4].skill_4 = 0;

	//������//
	PK[5].x = 0;
	PK[5].y = 0;
	PK[5].number = 5;
	PK[5].attack = 8;
	PK[5].bleed = 46;
	PK[5].defence = 4;
	PK[5].special_attack = 9;
	PK[5].special_defence = 7;
	PK[5].level = 5;
	PK[5].name = "������";
	PK[5].speciese = "";
	PK[5].skill1 = &star;
	PK[5].skill2 = &catchit;
	PK[5].skill1_name = "��������";
	PK[5].skill2_name = "��������";
	PK[5].skill_1 = 35;
	PK[5].skill_2 = 40;
	PK[5].skill_3 = 0;
	PK[5].skill_4 = 0;

	//���з//
	PK[6].x = 0;
	PK[6].y = 0;
	PK[6].number = 6;
	PK[6].attack = 8;
	PK[6].bleed = 50;
	PK[6].defence = 4;
	PK[6].special_attack = 9;
	PK[6].special_defence = 7;
	PK[6].level = 5;
	PK[6].name = "���з";
	PK[6].speciese = "ˮ";
	PK[6].skill1 = &musicattack;
	PK[6].skill2 = &boom;
	PK[6].skill1_name = "��������";
	PK[6].skill2_name = "��ը";
	PK[6].skill_1 = 45;
	PK[6].skill_2 = 83;
	PK[6].skill_3 = 0;
	PK[6].skill_4 = 0;

	//·����ŷ//
	PK[7].x = 0;
	PK[7].y = 0;
	PK[7].number = 7;
	PK[7].attack = 8;
	PK[7].bleed = 100;
	PK[7].defence = 4;
	PK[7].special_attack = 9;
	PK[7].special_defence = 7;
	PK[7].level = 5;
	PK[7].name = "·����ŷ";
	PK[7].speciese = "��";
	PK[7].skill1 = &fallingStar;
	PK[7].skill2 = &eat;
	PK[7].skill1_name = "��������";
	PK[7].skill2_name = "˺ҧ";
	PK[7].skill_1 = 70;
	PK[7].skill_2 = 100;
	PK[7].skill_3 = 0;
	PK[7].skill_4 = 0;

	//����//
	PK[8].x = 0;
	PK[8].y = 0;
	PK[8].number = 8;
	PK[8].attack = 8;
	PK[8].bleed = 54;
	PK[8].defence = 4;
	PK[8].special_attack = 9;
	PK[8].special_defence = 7;
	PK[8].level = 5;
	PK[8].name = "����";
	PK[8].speciese = "����";
	PK[8].skill1 = &musicattack;
	PK[8].skill2 = &lighton;
	PK[8].skill1_name = "����";
	PK[8].skill2_name = "����";
	PK[8].skill_1 = 50;
	PK[8].skill_2 = 80;
	PK[8].skill_3 = 0;
	PK[8].skill_4 = 0;

	//Ƥ��//
	PK[9].x = 0;
	PK[9].y = 0;
	PK[9].number = 9;
	PK[9].attack = 8;
	PK[9].bleed = 58;
	PK[9].defence = 4;
	PK[9].special_attack = 9;
	PK[9].special_defence = 7;
	PK[9].level = 5;
	PK[9].name = "Ƥ��";
	PK[9].speciese = "��";
	PK[9].skill1 = &star;
	PK[9].skill2 = &enemyscream;
	PK[9].skill1_name = "��������";
	PK[9].skill2_name = "���";
	PK[9].skill_1 = 50;
	PK[9].skill_2 = 80;
	PK[9].skill_3 = 0;
	PK[9].skill_4 = 0;

	//С���//
	PK[10].x = 0;
	PK[10].y = 0;
	PK[10].number = 10;
	PK[10].attack = 8;
	PK[10].bleed = 62;
	PK[10].defence = 4;
	PK[10].special_attack = 9;
	PK[10].special_defence = 7;
	PK[10].level = 5;
	PK[10].name = "С���";
	PK[10].speciese = "��";
	PK[10].skill1 = &lighton;
	PK[10].skill2 = &firehigh;
	PK[10].skill1_name = "����";
	PK[10].skill2_name = "����";
	PK[10].skill_1 = 50;
	PK[10].skill_2 = 80;
	PK[10].skill_3 = 0;
	PK[10].skill_4 = 0;

	//����è//
	PK[11].x = 0;
	PK[11].y = 0;
	PK[11].number = 11;
	PK[11].attack = 8;
	PK[11].bleed = 66;
	PK[11].defence = 4;
	PK[11].special_attack = 9;
	PK[11].special_defence = 7;
	PK[11].level = 5;
	PK[11].name = "����è";
	PK[11].speciese = "����";
	PK[11].skill1 = &wind;
	PK[11].skill2 = &sword;
	PK[11].skill1_name = "����";
	PK[11].skill2_name = "����";
	PK[11].skill_1 = 50;
	PK[11].skill_2 = 80;
	PK[11].skill_3 = 0;
	PK[11].skill_4 = 0;

	//�׹�
	PK[12].x = 0;
	PK[12].y = 0;
	PK[12].number = 12;
	PK[12].attack = 8;
	PK[12].bleed = 80;
	PK[12].defence = 4;
	PK[12].special_attack = 9;
	PK[12].special_defence = 7;
	PK[12].level = 5;
	PK[12].name = "�׹�";
	PK[12].speciese = "��";
	PK[12].skill1 = &fallingStar;
	PK[12].skill2 = &catchit;
	PK[12].skill1_name = "��������";
	PK[12].skill1_name = "��������";
	PK[12].skill_1 = 50;
	PK[12].skill_2 = 80;
	PK[12].skill_3 = 0;
	PK[12].skill_4 = 0;

	//�׵�
	PK[13].x = 0;
	PK[13].y = 0;
	PK[13].number = 13;
	PK[13].attack = 8;
	PK[13].bleed = 90;
	PK[13].defence = 4;
	PK[13].special_attack = 9;
	PK[13].special_defence = 7;
	PK[13].level = 5;
	PK[13].name = "�׵�";
	PK[13].speciese = "��";
	PK[13].skill1 = &catchit;
	PK[13].skill2 = &fallingStar;
	PK[13].skill1_name = "��������";
	PK[13].skill2_name = "��������";
	PK[13].skill_1 = 50;
	PK[13].skill_2 = 80;
	PK[13].skill_3 = 0;
	PK[13].skill_4 = 0;

}
//����Ѫ����ʼ��
void startup_pokemon_bleed()
{
	PK[0].bleed = 32 + (PK[0].level - 5) * 3;
	PK[1].bleed = 30;
	PK[2].bleed = 34;
	PK[3].bleed = 38;
	PK[4].bleed = 42;
	PK[5].bleed = 46;
	PK[6].bleed = 50;
	PK[7].bleed = 100;
	PK[8].bleed = 54;
	PK[9].bleed = 58;
	PK[10].bleed = 62;
	PK[11].bleed = 66;
	PK[12].bleed = 80;
	PK[13].bleed = 90;
}
//����λ�ó�ʼ��
void startup_pokemon_destination()
{
	int i;
	srand(time(NULL));
	for (i = 1; i < 4; i++)
	{
		PK[i].x = 556 + rand() % 60;
		PK[i].y = 350 + rand() % 220;
	}

	PK[4].x = 683 + rand() % 50;
	PK[4].y = 340 + rand() % 120;

	for (i = 5; i < 7; i++)
	{
		PK[i].x = 870 + rand() % 200;
		PK[i].y = 600 + rand() % 420;
	}

	for (i = 8; i < 9; i++)
	{
		PK[i].x = 870 + rand() % 200;
		PK[i].y = 600 + rand() % 420;
	}


	for (i = 9; i < 11; i++)
	{
		PK[i].x = 600 + rand() % 200;
		PK[i].y = 835 + rand() % 200;
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
void pokemon_refresh()
{
	int i;
	for (i = 0; i < pokemon_number; i++)
	{
		if (PK[i].judge == 1 && PK[i].time < 10000)       //��һ�����鱻���������judge�ͻ��Ϊ1��Ϊ1�����ʱ�������޷��������ģ�����10000��ѭ��������������ˢ�³���//
		{
			PK[i].time++;
		}
		else if (PK[i].judge == 1 && PK[i].time == 10000)
		{
			PK[i].judge = 0;
			PK[i].time = 0;
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


