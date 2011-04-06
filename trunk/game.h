class Game
{
    public:
    Game();
    ~Game();
    void animate();
    void draw();
    void levelup();
    void setlife(double val);
    void setmana(double val);
    void setexp(int val);
    void setmoney(int val);
    int getindamage(Mob* mob);
    int getoutdamage();
    int getmoney(Loot* loot);
    double getpotionl(Loot* loot);
    double getpotionm(Loot* loot);
    int level;
    double life;
    double maxlife;
    double mana;
    double maxmana;
    int exp;
    int maxexp;
    int money;

    private:
    GLuint uitexture;
    GLuint leveltexture;
    GLuint exptexture;
    GLuint cointexture[4];
    GLuint statetexture;
    GLuint lifetexture;
    GLuint manatexture;
    GLuint moneytexture;
    GLuint lifebartexture;
    GLuint manabartexture;
    GLuint expbartexture;
    TTF_Font* police;
    SDL_Color whitecolor;
    char txt[100];
    int coinindex;
    int currentframe;
    int maxframe;
};

Game::Game()
{
    SDL_Surface* statesurface = IMG_Load("gfx/state.png");
    statetexture = glloadtexture(statesurface);
    SDL_FreeSurface(statesurface);

    SDL_Surface* lifebarsurface = IMG_Load("gfx/barlife.png");
    lifebartexture = glloadtexture(lifebarsurface);
    SDL_FreeSurface(lifebarsurface);

    SDL_Surface* manabarsurface = IMG_Load("gfx/barmana.png");
    manabartexture = glloadtexture(manabarsurface);
    SDL_FreeSurface(manabarsurface);

    SDL_Surface* expbarsurface = IMG_Load("gfx/barexp.png");
    expbartexture = glloadtexture(expbarsurface);
    SDL_FreeSurface(expbarsurface);

    SDL_Surface* uisurface = IMG_Load("gfx/interface.png");
    uitexture = glloadtexture(uisurface);
    SDL_FreeSurface(uisurface);

    SDL_Surface* coinsurface[4];

    coinsurface[0] = IMG_Load("gfx/coin0.png");
    coinsurface[1] = IMG_Load("gfx/coin1.png");
    coinsurface[2] = IMG_Load("gfx/coin2.png");
    coinsurface[3] = IMG_Load("gfx/coin3.png");

    cointexture[0] = glloadtexture(coinsurface[0]);
    cointexture[1] = glloadtexture(coinsurface[1]);
    cointexture[2] = glloadtexture(coinsurface[2]);
    cointexture[3] = glloadtexture(coinsurface[3]);

    SDL_FreeSurface(coinsurface[0]);
    SDL_FreeSurface(coinsurface[1]);
    SDL_FreeSurface(coinsurface[2]);
    SDL_FreeSurface(coinsurface[3]);

    level = 1;
    maxlife = 1000 * level;
    life = maxlife;
    maxmana = 100 * level;
    mana = maxmana;
    exp = 0;
    maxexp = 1000 * level;
    money = 0;
    coinindex = 0;

    whitecolor = {255, 255, 255};

    police = TTF_OpenFont("font/Plakkaat.ttf", 18);

    // dummy textures

    snprintf(txt, 99, "0");
    SDL_Surface* surface = TTF_RenderText_Blended(police, txt, whitecolor);

    leveltexture = glloadtexture(surface);
    lifetexture = glloadtexture(surface);
    manatexture = glloadtexture(surface);
    exptexture = glloadtexture(surface);
    moneytexture = glloadtexture(surface);

    SDL_FreeSurface(surface);

    currentframe = 0;
    maxframe = 7;
}

Game::~Game()
{
    glDeleteTextures(1, &statetexture);
    glDeleteTextures(1, &lifebartexture);
    glDeleteTextures(1, &manabartexture);
    glDeleteTextures(1, &expbartexture);
    glDeleteTextures(1, &uitexture);
    glDeleteTextures(4, &cointexture[0]);
    glDeleteTextures(1, &leveltexture);
    glDeleteTextures(1, &lifetexture);
    glDeleteTextures(1, &manatexture);
    glDeleteTextures(1, &exptexture);
    glDeleteTextures(1, &moneytexture);

    TTF_CloseFont(police);
}

void Game::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;
        coinindex++;
        if (coinindex > 3)
            coinindex = 0;
    }
}

void Game::draw()
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();   // life bar
    glTranslated(life * 256 / maxlife - 256 + 55, 20, 0);

    glBindTexture(GL_TEXTURE_2D, lifebartexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -21);
        glTexCoord2d(0, 0); glVertex2d(0, 21);
        glTexCoord2d(1, 0); glVertex2d(256, 21);
        glTexCoord2d(1, 1); glVertex2d(256, -21);
    glEnd();

    glLoadIdentity();   // mana bar
    glTranslated(mana * 256 / maxmana - 256 + 55, 60, 0);

    glBindTexture(GL_TEXTURE_2D, manabartexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -21);
        glTexCoord2d(0, 0); glVertex2d(0, 21);
        glTexCoord2d(1, 0); glVertex2d(256, 21);
        glTexCoord2d(1, 1); glVertex2d(256, -21);
    glEnd();

    glLoadIdentity();   // exp bar
    glTranslated(exp * 256 / maxexp - 256 + 55, 100, 0);

    glBindTexture(GL_TEXTURE_2D, expbartexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -21);
        glTexCoord2d(0, 0); glVertex2d(0, 21);
        glTexCoord2d(1, 0); glVertex2d(256, 21);
        glTexCoord2d(1, 1); glVertex2d(256, -21);
    glEnd();

    glLoadIdentity();   // state
    glTranslated(0, 64, 0);

    glBindTexture(GL_TEXTURE_2D, statetexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -64);
        glTexCoord2d(0, 0); glVertex2d(0, 64);
        glTexCoord2d(1, 0); glVertex2d(113, 64);
        glTexCoord2d(1, 1); glVertex2d(113, -64);
    glEnd();

    glLoadIdentity();   // level
    glTranslated(42, 64, 0);

    snprintf(txt, 99, "%d", (int)level);
    int len = strlen(txt);
    glDeleteTextures(1, &leveltexture);
    SDL_Surface* surface = TTF_RenderText_Blended(police, txt, whitecolor);
    leveltexture = glloadtexture(surface);
    SDL_FreeSurface(surface);

    glBindTexture(GL_TEXTURE_2D, leveltexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2d(0, 16);
        glTexCoord2d(1, 0); glVertex2d(len * 16, 16);
        glTexCoord2d(1, 1); glVertex2d(len * 16, -16);
    glEnd();

    glLoadIdentity();   // life
    glTranslated(124, 24, 0);

    snprintf(txt, 99, "%d / %d", (int)life, (int)maxlife);
    len = strlen(txt);
    glDeleteTextures(1, &lifetexture);
    surface = TTF_RenderText_Blended(police, txt, whitecolor);
    lifetexture = glloadtexture(surface);
    SDL_FreeSurface(surface);

    glBindTexture(GL_TEXTURE_2D, lifetexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2d(0, 16);
        glTexCoord2d(1, 0); glVertex2d(len * 8, 16);
        glTexCoord2d(1, 1); glVertex2d(len * 8, -16);
    glEnd();

    glTranslated(0, 38, 0); // mana

    snprintf(txt, 99, "%d / %d", (int)mana, (int)maxmana);
    len = strlen(txt);
    glDeleteTextures(1, &manatexture);
    surface = TTF_RenderText_Blended(police, txt, whitecolor);
    manatexture = glloadtexture(surface);
    SDL_FreeSurface(surface);

    glBindTexture(GL_TEXTURE_2D, manatexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2d(0, 16);
        glTexCoord2d(1, 0); glVertex2d(len * 8, 16);
        glTexCoord2d(1, 1); glVertex2d(len * 8, -16);
    glEnd();

    glTranslated(0, 40, 0); // exp

    snprintf(txt, 99, "%d / %d", (int)exp, (int)maxexp);
    len = strlen(txt);
    glDeleteTextures(1, &exptexture);
    surface = TTF_RenderText_Blended(police, txt, whitecolor);
    exptexture = glloadtexture(surface);
    SDL_FreeSurface(surface);

    glBindTexture(GL_TEXTURE_2D, exptexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2d(0, 16);
        glTexCoord2d(1, 0); glVertex2d(len * 8, 16);
        glTexCoord2d(1, 1); glVertex2d(len * 8, -16);
    glEnd();

    glLoadIdentity();   // coin icon
    glTranslated(0, 150, 0);

    glBindTexture(GL_TEXTURE_2D, cointexture[coinindex]);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2d(0, 16);
        glTexCoord2d(1, 0); glVertex2d(32, 16);
        glTexCoord2d(1, 1); glVertex2d(32, -16);
    glEnd();

    glTranslated(40, 0, 0); // money

    snprintf(txt, 99, "%d", money);
    len = strlen(txt);
    surface = TTF_RenderText_Blended(police, txt, whitecolor);
    glDeleteTextures(1, &moneytexture);
    moneytexture = glloadtexture(surface);
    SDL_FreeSurface(surface);

    glBindTexture(GL_TEXTURE_2D, moneytexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2d(0, 16);
        glTexCoord2d(1, 0); glVertex2d(len * 8, 16);
        glTexCoord2d(1, 1); glVertex2d(len * 8, -16);
    glEnd();

    glLoadIdentity();   // interface
    glTranslated(0, 536, 0);

    glBindTexture(GL_TEXTURE_2D, uitexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -64);
        glTexCoord2d(0, 0); glVertex2d(0, 64);
        glTexCoord2d(1, 0); glVertex2d(800, 64);
        glTexCoord2d(1, 1); glVertex2d(800, -64);
    glEnd();
}

void Game::levelup()
{
    level++;
    exp -= maxexp;
    maxexp = 1000 * level;
    maxlife = 1000 * level;
    life = maxlife;
    maxmana = 100 * level;
    mana = maxmana;
}

void Game::setexp(int val)
{
    exp += val * 50 / level; // val = mob level
    if (exp >= maxexp)
        levelup();
}

void Game::setmoney(int val)
{
    money += val; // val = money
}

void Game::setlife(double val)
{
    life += val; // val = life

    if (life > maxlife)
        life = maxlife;
    if (life < 0)
        life = 0;
}

void Game::setmana(double val)
{
    mana += val; // val = mana

    if (mana > maxmana)
        mana = maxmana;
    if (mana < 0)
        mana = 0;
}

int Game::getindamage(Mob* mob)
{
    if (rand() % 16 == 0)
        return 0;
    else
        return mob->level * (25 + rand() % 25);
}

int Game::getoutdamage()
{
    if (rand() % 16 == 0)
        return 0;
    else
        return level * (32 + rand() % 32);
}

int Game::getmoney(Loot* loot)
{
    return loot->level *= 4 + rand() % 4;
}

double Game::getpotionl(Loot* loot)
{
    return loot->level *= 250 + rand() % 250;
}

double Game::getpotionm(Loot* loot)
{
    return loot->level *= 25 + rand() % 25;
}
