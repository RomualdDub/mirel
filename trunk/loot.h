class Loot
{
    public:
    Loot(int inlevel, double inx, double iny);
    ~Loot();
    void animate();
    void draw();
    bool collide(Hero* hero);
    double x;
    double y;
    int level;
    int type;
    GLuint cointexture[4];
    GLuint potionmtexture;
    GLuint potionltexture;

    private:
    double angle;
    int index;
    int currentframe;
    int maxframe;
};

Loot::Loot(int inlevel, double inx, double iny)
{
    index = 0;

    level = inlevel;

    x = inx;
    y = iny - 32 - rand() % 64;

    angle = 0;

    type = rand() % 3;

    if (type == LOOT_MONEY)
    {
        SDL_Surface* surface[4];

        surface[0] = IMG_Load("gfx/coin0.png");
        surface[1] = IMG_Load("gfx/coin1.png");
        surface[2] = IMG_Load("gfx/coin2.png");
        surface[3] = IMG_Load("gfx/coin3.png");

        cointexture[0] = glloadtexture(surface[0]);
        cointexture[1] = glloadtexture(surface[1]);
        cointexture[2] = glloadtexture(surface[2]);
        cointexture[3] = glloadtexture(surface[3]);

        SDL_FreeSurface(surface[0]);
        SDL_FreeSurface(surface[1]);
        SDL_FreeSurface(surface[2]);
        SDL_FreeSurface(surface[3]);
    }
    else if (type == LOOT_POTIONM)
    {
        SDL_Surface* surface = IMG_Load("gfx/potionm.png");
        potionmtexture = glloadtexture(surface);
        SDL_FreeSurface(surface);
    }
    else if (type == LOOT_POTIONL)
    {
        SDL_Surface* surface = IMG_Load("gfx/potionl.png");
        potionltexture = glloadtexture(surface);
        SDL_FreeSurface(surface);
    }

    currentframe = 0;
    maxframe = 7;
}

Loot::~Loot()
{
    if (type == LOOT_MONEY)
    {
        glDeleteTextures(4, &cointexture[0]);
    }
    else if (type == LOOT_POTIONM)
    {
        glDeleteTextures(1, &potionmtexture);
    }
    else if (type == LOOT_POTIONL)
    {
        glDeleteTextures(1, &potionltexture);
    }
}

void Loot::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (type == LOOT_MONEY)
        {
            index++;
            if (index > 3)
                index = 0;
        }

        angle++;
        y += 5 * sin(angle);
    }
}

void Loot::draw()
{
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(x, y, 0);

    if (type == LOOT_MONEY)
        glBindTexture(GL_TEXTURE_2D, cointexture[index]);
    else if (type == LOOT_POTIONM)
        glBindTexture(GL_TEXTURE_2D, potionmtexture);
    else if (type == LOOT_POTIONL)
        glBindTexture(GL_TEXTURE_2D, potionltexture);

    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2i(0, 16);
        glTexCoord2d(1, 0); glVertex2i(32, 16);
        glTexCoord2d(1, 1); glVertex2i(32, -16);
    glEnd();
}

bool Loot::collide(Hero* hero)
{
    if (hero->x >= x + 24 || hero->x + 64 <= x + 8 || hero->y >= y + 24 || hero->y + 128 <= y - 8)
        return false;
    else
        return true;
}
