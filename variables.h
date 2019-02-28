#define MAX_BULLETS 20
#define BULLET_INTERVAL 4
#define MAX_ASTEROIDS 15
#define SPAWN_INTERVAL 20
#define AST_INACTIVE -7

uint8_t displaybuffer[512];
// Define list of sprites
static const int ship_v1[] = {73,85,85,42,20,28,8};
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
static int playerEnergy = 8; //health and energy is the same (shields)
static int score = 0;
