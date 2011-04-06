class Pet
{
    public:
    Pet();
    ~Pet();
    void animate();
    void draw();
    bool collide(Mob* mob);
    bool left;
    bool right;
    bool attack;
    double x;
    double y;

    #define PETWALK0 0
    #define PETWALK1 1
    #define PETATK 2
    #define PETIDLE 3

    private:
    int index;
    int direction;
    GLuint texture[4];
    int currentframe;
    int maxframe;
};

Pet::Pet()
{
    left = false;
    right = false;
    attack = false;

    direction = DIRECTION_RIGHT;

    index = PETWALK0;

    x = 200 - 128;
    y = 472 / 2;

    SDL_Surface* surface[4];

    surface[PETWALK0] = IMG_Load("gfx/pet0.png");
    surface[PETWALK1] = IMG_Load("gfx/pet1.png");
    surface[PETATK] = IMG_Load("gfx/pet2.png");
    surface[PETIDLE] = IMG_Load("gfx/pet3.png");

    texture[PETWALK0] = glloadtexture(surface[PETWALK0]);
    texture[PETWALK1] = glloadtexture(surface[PETWALK1]);
    texture[PETATK] = glloadtexture(surface[PETATK]);
    texture[PETIDLE] = glloadtexture(surface[PETIDLE]);

    SDL_FreeSurface(surface[PETWALK0]);
    SDL_FreeSurface(surface[PETWALK1]);
    SDL_FreeSurface(surface[PETATK]);
    SDL_FreeSurface(surface[PETIDLE]);

    currentframe = 0;
    maxframe = 7;
}

Pet::~Pet()
{
    glDeleteTextures(4, &texture[PETWALK0]);
}

void Pet::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (left)
            direction = DIRECTION_LEFT;

        if (right)
            direction = DIRECTION_RIGHT;

        if (left || right)
        {
            if (index == PETWALK0)
                index = PETWALK1;
            else if (index == PETWALK1)
                index = PETWALK0;
            else
                index = PETWALK0;
        }

        if (attack)
            index = PETATK;

        if (!left && !right && !attack) // idle
        {
            if (rand() % 10 < 1)
                index = PETIDLE;
            else
                index = PETWALK0;
        }
    }
}

void Pet::draw()
{
    if (direction == DIRECTION_LEFT)
    {
        glMatrixMode(GL_TEXTURE);

        glLoadIdentity();
        glPushMatrix();
        glScaled(-1, 1, 0);
    }

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(x, y, 0);

    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -32);
        glTexCoord2d(0, 0); glVertex2d(0, 32);
        glTexCoord2d(1, 0); glVertex2d(64, 32);
        glTexCoord2d(1, 1); glVertex2d(64, -32);
    glEnd();

    if (direction == DIRECTION_LEFT)
    {
        glMatrixMode(GL_TEXTURE);

        glPopMatrix();
    }
}

bool Pet::collide(Mob* mob)
{
    if (mob->x >= x + 32 || mob->x + 32 <= x || mob->y >= y + 64 || mob->y + 64 <= y)
        return false;
    else
        return true;
}
