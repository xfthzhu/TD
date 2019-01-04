
#include <graphics.h>
#include <vector>
#include <ctime>
#include <cmath>
#pragma comment (lib,"MSIMG32.lib")
#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000?1:0)
using namespace std;

// IMAGE
IMAGE buffer(500,400),background,player,missile,planes[3],planebomb;

// ���x���꣬���y���꣬װ��ʣ��ʱ�䣬����ֵ���÷�
int playerx=0,playery=286,firereload=5,strength=50,score=0;

// ��ҷ��յ���
struct PLAYERMISL
{
    int x;
    int y;
};

// �ɻ�����
struct BOMBPLANE
{
    int speed;  // �ɻ��ٶ�
    int width;  // �ɻ����
    int height; // �ɻ��߶�
} bombplane[3]= {{15,85,22},{12,81,22},{8,105,50}};

// ը��
struct BOMB
{
    int x;
    int y;
};

// �ɻ�
struct PLANE
{
    int x;
    int y;
    int type;
    int reload;// ���ж���֡������ը��
};

vector<PLAYERMISL> misl;
vector<PLANE> plane;
vector<BOMB> bomb;


// ��װ�õ�͸����ͼ����
void putpicture(int x,int y,IMAGE img)
{
    HDC dstDC = GetImageHDC(&buffer);
    HDC srcDC = GetImageHDC(&img);
    TransparentBlt(dstDC, x, y, img.getwidth(), img.getheight(), srcDC, 0, 0, img.getwidth(), img.getheight(), RGB(40,112,162));
}


// ��ȷ��ʱ
void sleep(int ms)
{
    static clock_t oldclock = clock();      // ��̬��������¼��һ�� tick
    oldclock += ms * CLOCKS_PER_SEC / 1000; // ���� tick
    if (clock() > oldclock)                 // ����Ѿ���ʱ��������ʱ
    {
        oldclock = clock();
    }
    else
        while(clock() < oldclock)           // ��ʱ
        {
            Sleep(1);    // �ͷ� CPU ����Ȩ������ CPU ռ����
        }
}


// ������ײ��⺯��
bool collision(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2)
{
    if((abs((x1 + w1 / 2) - (x2 + w2/2)) < (w1 + w2) / 2)&&abs((y1 + h1 / 2) - (y2 + h2/2) )< (h1 + h2) / 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}


// ����һ�ŷ��յ���
void createmisl()
{
    PLAYERMISL playermisl;
    playermisl.x=playerx+12;
    playermisl.y=playery-4;
    misl.push_back(playermisl);
}


// ����һ��ը��
void createbomb(PLANE p)
{
    BOMB b;
    b.x=p.x+bombplane[p.type].width/2-4;
    b.y=p.y+bombplane[p.type].height+2;
    bomb.push_back(b);
}


// ����һ�ܷɻ�
void createplane()
{
    PLANE newplane;
    newplane.x=500;
    newplane.y=rand()%181+20;
    newplane.type=rand()%3;
    newplane.reload=10;            // ը��������ʱ��Ϊ10֡
    plane.push_back(newplane);
}


// ����ͼƬ��Դ
void loadres()
{
    loadimage(&background,"IMAGE","BG");
    loadimage(&player,"IMAGE","PLAYER");
    loadimage(&planebomb,"IMAGE","BOMB");
    loadimage(&missile,"IMAGE","MISL");
    char filename[10];
    for(int i=0; i<3; i++)
    {
        sprintf(filename,"PLANE%d",i);
        loadimage(&planes[i],"IMAGE",filename);
    }
}


// ÿ֡��ͼ
void render()
{
    char info[25];
    SetWorkingImage(&buffer);                           // ���ڻ�������ͼ
    putimage(0,0,&background);
    putpicture(playerx,playery,player);                 // ��ʾ���ճ�
    vector<PLAYERMISL>::iterator it1;
    vector<PLANE>::iterator it2;
    vector<BOMB>::iterator it3;
    for(it1=misl.begin(); it1!=misl.end(); it1++)
    {
        putpicture(it1->x,it1->y,missile);    // ��ʾ���յ���
    }
    for(it2=plane.begin(); it2!=plane.end(); it2++)
    {
        putpicture(it2->x,it2->y,planes[it2->type]);    // ��ʾ�ɻ�
    }
    for(it3=bomb.begin(); it3!=bomb.end(); it3++)
    {
        putpicture(it3->x,it3->y,planebomb);    // ��ʾը��
    }
    sprintf(info,"����ֵ��%d �÷֣�%d",strength,score);
    outtextxy(0,0,info);                                // ��ʾ��Ϸ����
    SetWorkingImage();
    putimage(0,0,&buffer);                              // �ѻ�������ͼ����һ���Ի�����ȥ��������������˸
}


int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
    srand(time(NULL));
    loadres();
    initgraph(500,400);
    setbkmode(TRANSPARENT);
    vector<PLAYERMISL>::iterator it1;
    vector<PLANE>::iterator it2;
    vector<BOMB>::iterator it3;

gamestart:
    bool eraseyes=false;
    while(true)
    {
        if(strength<1)
        {
            break;
        }
        if(plane.size()<3)
        {
            createplane();
        }
        for(it1=misl.begin(); it1!=misl.end();)
        {
            it1->y-=10;
            if(it1->y<0)
            {
                it1=misl.erase(it1);
            }
            else
            {
                ++it1;
            }
        }
        for(it2=plane.begin(); it2!=plane.end();)
        {
            it2->x-=bombplane[it2->type].speed;
            if(it2->x+bombplane[it2->type].width<0)
            {
                plane.erase(it2);
            }
            else
            {
                if(it2->reload==0)
                {
                    it2->reload=10;
                    createbomb(*it2);
                }
                else
                {
                    it2->reload-=1;
                }
                ++it2;
            }
        }
        for(it2=plane.begin(); it2!=plane.end();)
        {
            for(it1=misl.begin(); it1!=misl.end();)
            {
                if(collision(it1->x,it1->y,5,7,it2->x,it2->y,bombplane[it2->type].width,bombplane[it2->type].height))
                {
                    misl.erase(it1);
                    plane.erase(it2);
                    eraseyes=true;
                    score+=1;
                }
                else
                {
                    ++it1;
                }
            }
            if(eraseyes==false)
            {
                it2++;
            }
            else
            {
                eraseyes=false;
            }
        }
        for(it3=bomb.begin(); it3!=bomb.end();)
        {
            it3->y+=5;
            if(it3->y>400)
            {
                it3=bomb.erase(it3);
            }
            else
            {
                ++it3;
            }
        }
        for(it3=bomb.begin(); it3!=bomb.end();)
        {
            if(collision(it3->x,it3->y,5,7,playerx,playery,58,49))
            {
                bomb.erase(it3);
                strength-=1;
            }
            else
            {
                ++it3;
            }
        }
        if(KEY_DOWN(VK_LEFT)&&playerx>=10)
        {
            playerx-=10;
        }
        if(KEY_DOWN(VK_RIGHT)&&playerx<=384)
        {
            playerx+=10;
        }
        if(KEY_DOWN(VK_SPACE)&&firereload==0)
        {
            createmisl();
            firereload=5;
        }
        if(firereload>=1)
        {
            firereload-=1;
        }
        render();
        sleep(33);
    }

    char result[50];
    sprintf(result,"���ε÷֣�%d�֣����ٽ��������Ƿ����¿�ʼ��Ϸ��",score);
    if(MessageBox(GetHWnd(),result,"��Ϸ����",MB_YESNO)==IDYES)
    {
        playerx=0;
        playery=286;
        strength=50;
        score=0;
        firereload=5;
        misl.clear();
        plane.clear();
        bomb.clear();
        goto gamestart;
    }

    return 0;
}
