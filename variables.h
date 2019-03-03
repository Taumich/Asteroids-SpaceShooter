#define MAX_BULLETS 20
#define BULLET_INTERVAL 4
#define MAX_ASTEROIDS 15
#define SPAWN_INTERVAL 20
#define AST_INACTIVE -7
#define SCOREPANEL 63

uint8_t displaybuffer[512];
static int numbers[] = {
  14,17,17,14, // 0
  0,18,31,16, // 1
  25,21,21,18, // 2
  17,21,21,10, // 3
  7,4,4,31, // 4
  23,21,21,9, // 5
  14,21,21,8, // 6
  1,25,5,3, // 7
  10,21,21,10, // 8
  2,21,21,14, // 9
  0,4,4,4, // -
  24,12,6,3, // /
  0,0,10,0, // :
  1,21,5,2 // ?
};
static int letters[] = {
  0,30,5,5,30, // A
  0,31,21,21,10, // B
  0,14,17,17,17, // C
  0,31,17,17,14, // D
  0,31,21,21,17, // E
  0,31,5,5,1, // F
  0,14,17,21,29, // G
  0,31,4,4,31, // H
  0,17,31,17,0, // I
  0,9,17,17,15, // J
  0,31,4,10,17, // K
  0,31,16,16,16, // L
  31,2,4,2,31, // M
  0,31,2,4,31, // N
  0,14,17,17,14, // O
  0,31,5,5,2, // P
  0,14,17,17,46, // Q
  0,31,5,13,18, // R
  0,18,21,21,9, // S
  0,1,31,1,0, // T
  0,15,16,16,15, // U
  0,15,16,8,7, // V
  15,16,12,16,15, // W
  0,27,4,4,27, // X
  0,7,24,7,0, // Y
  0,25,21,21,19, // Z
  0,8,20,12,24, // a
  0,31,20,20,8, // b
  0,8,20,20,0, // c
  0,8,20,20,31, // d
  0,12,22,22,4, // e
  0,4,30,5,5, // f
  0,16,44,44,24, // g
  0,31,4,12,24, // h
  0,0,29,0,0, // i
  0,16,32,32,29, // j
  0,31,8,20,0, // k
  0,0,31,16,0, // l
  28,4,8,4,24, // m
  0,28,4,4,24, // n
  0,8,20,20,8, // o
  0,56,20,20,8, // p
  0,16,62,22,6, // q
  0,28,8,4,4, // r
  0,16,28,4,0, // s
  0,4,15,20,16, // t
  0,12,16,28,0, // u
  0,12,16,12,0, // v
  12,16,8,16,12, // w
  0,20,8,20,0, // x
  0,44,40,40,60, // y
  0,26,26,22,0, // z
};
static int symbols[] = {
  0,14,17,21,17,14, // Button
  14,17,17,14,4,31, // Stick left
  0,38,41,57,41,38, // Stick up
  31,4,14,17,17,14, // Stick right
  0,25,37,39,37,25, // Stick down
  12,18,45,45,18,12 // Stick press
};
// Define list of sprites
static int ship_v1[] = {73,85,85,42,20,28,8};
static int ship_v2[] = {85,127,107,85,85,28,8};
static int ship_v3[] = {54,73,85,107,99,54,20};
static int active_ship[3] = {ship_v1, ship_v2, ship_v3};

static int asteroid_1[] = {126,255,255,255,255,255,126};
static int asteroid_2[] = {126,255,207,195,207,255,126};
static int asteroid_3[] = {126,207,195,129,195,207,126};
static int asteroid_4[] = {126,129,129,129,129,129,126};
static int asteroid[4] = {asteroid_4, asteroid_3, asteroid_2, asteroid_1};

static int giant[] = {9096,20194,24632,57806,18948,16934,16706,37003,43033,38929,36945,16394,8454,23560,13300,6144};

static int bullet_level1[] = {2,2,0};
static int bullet_level2[] = {2,7,2};
static int bullet_level3[] = {7,5,7};
//bullet data: {appearance, speed, damage}
static int bullet[3] = {bullet_level1, bullet_level2, bullet_level3};
int bullets_level[MAX_BULLETS];

//static int pixel[1] = {1};
int gamemode;

int xpos;
int ypos;
int stickX;
int stickY;
static int stickPull = 0;
int buttonj;
int button4;
int button3;
int button2;
static int rep = 0;
int bulletPositions[MAX_BULLETS*2];
static int bulletCount = 0;
int asteroidPositions[MAX_ASTEROIDS*2];
int asteroidHealth[MAX_ASTEROIDS];
int playerEnergy; //health and energy is the same (shields)
int score;
