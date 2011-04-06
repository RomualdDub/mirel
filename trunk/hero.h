class Hero
{
    public:
    Hero();
    ~Hero();
    void animate();
    void draw();
    void dowalk();
    void doattack();
    void doidle();
    int direction;
    bool left;
    bool right;
    bool jump;
    bool attack;
    bool idle;
    bool hit;
    double x;
    double y;
    double jumpangle;
    int time;

    #define HEROWALK0 0
    #define HEROWALK1 1
    #define HEROWALK2 2
    #define HEROATK0 3
    #define HEROATK1 4
    #define HEROATK2 5
    #define HEROIDLE 6
    #define HEROJUMP 7
    #define HEROHIT 8

    private:
    int previndex;
    int index;
    GLuint texture[9];
    int currentframe;
    int maxframe;
};

Hero::Hero()
{
    left = false;
    right = false;
    jump = false;
    attack = false;
    idle = false;
    hit = false;

    time = 0;

    x = 200;
    y = 472 / 2;
    jumpangle = 0;

    previndex = HEROWALK1;
    index = HEROWALK1;

    direction = DIRECTION_RIGHT;

    // let's create sprite states

    SDL_Surface* surface[9];

    for (int i = 0; i < 10; i++)
        surface[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 128, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    SDL_Surface* hatsurface;
    SDL_Surface* headsurface[3];
    SDL_Surface* dresssurface[3];
    SDL_Surface* feetsurface[3];

    hatsurface = IMG_Load("gfx/hat.png");
    headsurface[0] = IMG_Load("gfx/head0.png");
    headsurface[1] = IMG_Load("gfx/head1.png");
    headsurface[2] = IMG_Load("gfx/head2.png");
    dresssurface[0] = IMG_Load("gfx/dress0.png");
    dresssurface[1] = IMG_Load("gfx/dress1.png");
    dresssurface[2] = IMG_Load("gfx/dress2.png");
    feetsurface[0] = IMG_Load("gfx/feet0.png");
    feetsurface[1] = IMG_Load("gfx/feet1.png");
    feetsurface[2] = IMG_Load("gfx/feet2.png");

    blit(surface[HEROWALK0], feetsurface[0]);
    blit(surface[HEROWALK0], dresssurface[0]);
    blit(surface[HEROWALK0], headsurface[0]);
    blit(surface[HEROWALK0], hatsurface);
    texture[HEROWALK0] = glloadtexture(surface[HEROWALK0]);

    blit(surface[HEROWALK1], feetsurface[1]);
    blit(surface[HEROWALK1], dresssurface[1]);
    blit(surface[HEROWALK1], headsurface[0]);
    blit(surface[HEROWALK1], hatsurface);
    texture[HEROWALK1] = glloadtexture(surface[HEROWALK1]);

    blit(surface[HEROWALK2], feetsurface[2]);
    blit(surface[HEROWALK2], dresssurface[2]);
    blit(surface[HEROWALK2], headsurface[0]);
    blit(surface[HEROWALK2], hatsurface);
    texture[HEROWALK2] = glloadtexture(surface[HEROWALK2]);

    blit(surface[HEROATK0], feetsurface[1]);
    blit(surface[HEROATK0], dresssurface[1]);
    blit(surface[HEROATK0], headsurface[1]);
    blit(surface[HEROATK0], hatsurface);
    texture[HEROATK0] = glloadtexture(surface[HEROATK0]);

    blit(surface[HEROATK1], feetsurface[1]);
    blit(surface[HEROATK1], dresssurface[1]);
    blit(surface[HEROATK1], headsurface[1]);
    blit(surface[HEROATK1], hatsurface);
    texture[HEROATK1] = glloadtexture(surface[HEROATK1]);

    blit(surface[HEROATK2], feetsurface[1]);
    blit(surface[HEROATK2], dresssurface[2]);
    blit(surface[HEROATK2], headsurface[1]);
    blit(surface[HEROATK2], hatsurface);
    texture[HEROATK2] = glloadtexture(surface[HEROATK2]);

    blit(surface[HEROIDLE], feetsurface[1]);
    blit(surface[HEROIDLE], dresssurface[1]);
    blit(surface[HEROIDLE], headsurface[2]);
    blit(surface[HEROIDLE], hatsurface);
    texture[HEROIDLE] = glloadtexture(surface[HEROIDLE]);

    blit(surface[HEROJUMP], feetsurface[1]);
    blit(surface[HEROJUMP], dresssurface[0]);
    blit(surface[HEROJUMP], headsurface[1]);
    blit(surface[HEROJUMP], hatsurface);
    texture[HEROJUMP] = glloadtexture(surface[HEROJUMP]);

    white(surface[HEROHIT], surface[HEROWALK1]);
    texture[HEROHIT] = glloadtexture(surface[HEROHIT]);

    for (int i = 0; i < 10; i++)
        SDL_FreeSurface(surface[i]);

    SDL_FreeSurface(hatsurface);
    SDL_FreeSurface(headsurface[0]);
    SDL_FreeSurface(headsurface[1]);
    SDL_FreeSurface(headsurface[2]);
    SDL_FreeSurface(dresssurface[0]);
    SDL_FreeSurface(dresssurface[1]);
    SDL_FreeSurface(dresssurface[2]);
    SDL_FreeSurface(feetsurface[0]);
    SDL_FreeSurface(feetsurface[1]);
    SDL_FreeSurface(feetsurface[2]);

    currentframe = 0;
    maxframe = 7;
}

Hero::~Hero()
{
    glDeleteTextures(9, &texture[0]);
}

void Hero::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (hit)
        {
            index = HEROHIT;
            time++;
        }
        else
        {
            if (jump)
                index = HEROJUMP;
            else
            {
                if (left || right)
                {
                    if (index > HEROWALK2)
                        index = HEROWALK0;
                }
            }

            if (attack)
            {
                if (index < HEROATK0 || index > HEROATK2)
                    index = HEROATK0;
            }
        }

        if (left)
            direction = DIRECTION_LEFT;

        if (right)
            direction = DIRECTION_RIGHT;

        if (!left && !right && !jump && !attack && !hit)
            idle = true;
        else
            idle = false;

        if (!hit)
        {
            if (left || right)
                dowalk();

            if (attack)
                doattack();

            if (idle)
                doidle();
        }
    }
}

void Hero::draw()
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

    if (index == HEROWALK0)
        glRotated(-4, 0, 0, 1);
    else if (index == HEROWALK2)
        glRotated(4, 0, 0, 1);

    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -64);
        glTexCoord2d(0, 0); glVertex2d(0, 64);
        glTexCoord2d(1, 0); glVertex2d(64, 64);
        glTexCoord2d(1, 1); glVertex2d(64, -64);
    glEnd();

    if (direction == DIRECTION_LEFT)
    {
        glMatrixMode(GL_TEXTURE);

        glPopMatrix();
    }
}

void Hero::dowalk()
{
    switch (index)
    {
        case HEROWALK0:
            previndex = HEROWALK0;
            index++;
            break;
        case HEROWALK1:
            if (previndex == HEROWALK0)
                index++;
            else
                index--;
            break;
        case HEROWALK2:
            previndex = HEROWALK2;
            index--;
            break;
    }
}

void Hero::doattack()
{
    if (index >= HEROATK0 && index < HEROATK2)
        index++;
    else
    {
        index = HEROWALK1;
        attack = false;
    }
}

void Hero::doidle()
{
    if (rand() % 10 < 1)
        index = HEROIDLE;
    else
        index = HEROWALK1;
}
