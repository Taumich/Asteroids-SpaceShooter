#define MAX_BULLETS 20
#define BULLET_INTERVAL 4
#define MAX_ASTEROIDS 15
#define SPAWN_INTERVAL 20
#define AST_INACTIVE -7
#define SCOREPANEL 63

uint8_t displaybuffer[512];
static int numbers[] = {
  14,17,17,14,
  0,18,31,16,
  25,21,21,18,
  17,21,21,10,
  7,4,4,31,
  23,21,21,9,
  14,21,21,8,
  1,25,5,3,
  10,21,21,10,
  2,21,21,14
};
static int letters[] = {
  0,30,5,5,30,
  0,31,21,21,10,
  0,14,17,17,17,
  0,31,17,17,14,
  0,31,21,21,17,
  0,31,5,5,1,
  0,14,17,21,29,
  0,31,4,4,31,
  0,17,31,17,0,
  0,9,17,17,15,
  0,31,4,10,17,
  0,31,16,16,16,
  31,2,4,2,31,
  0,31,2,4,31,
  0,14,17,17,14,
  0,31,5,5,2,
  0,14,17,17,46,
  0,31,5,13,18,
  0,18,21,21,9,
  0,1,31,1,0


}
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
