class Mob
{
    public:
    Mob(int inlevel, double inspawn, int inradius);
    ~Mob();
    void animate(Hero* hero);
    void draw();
    bool collide(Magic* magic);
    bool collide(Hero* hero);
    bool idle;
    bool aggro;
    bool fear;
    double x;
    double y;
    int hp;
    int level;
    bool hit;
    double basesize;
    int time;

    #define MOBNORMAL 0
    #define MOBHIT 1

    private:
    GLint size;
    GLint maxsize;
    int direction;
    bool zoom;
    GLuint texture[2];
    int currentframe;
    int maxframe;
};

Mob::Mob(int inlevel, double inspawn, int inradius)
{
    idle = true;
    aggro = false;
    fear = false;
    hit = false;

    basesize = 64 - rand() % 32;
    x = inspawn + rand() % inradius;
    y = 236 + (64 - basesize);

    direction = rand() % 2;

    level = inlevel;
    hp = level * 250;

    zoom = true;
    size = 0;
    maxsize = rand() % 7;

    time = 0;

    SDL_Surface* surface[2];

    surface[MOBNORMAL] = IMG_Load("gfx/mob0.png");
    texture[MOBNORMAL] = glloadtexture(surface[MOBNORMAL]);

    surface[MOBHIT] = SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 64, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    white(surface[MOBHIT], surface[MOBNORMAL]);
    texture[MOBHIT] = glloadtexture(surface[MOBHIT]);

    SDL_FreeSurface(surface[MOBNORMAL]);
    SDL_FreeSurface(surface[MOBHIT]);

    currentframe = 0;
    maxframe = 7;
}

Mob::~Mob()
{
    glDeleteTextures(2, &texture[MOBNORMAL]);
}

void Mob::animate(Hero* hero)
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 2;
            if (size >= maxsize)
                zoom = false;
        }
        else
        {
            size -= 2;
            if (size <= -maxsize)
                zoom = true;
        }

        if (idle)
        {
            if (rand() % 10 < 1)
            {
                if (direction == DIRECTION_RIGHT)
                    direction = DIRECTION_LEFT;
                else
                    direction = DIRECTION_RIGHT;
            }
        }
        else if (aggro)
        {
            maxsize /= 2;
            if (x <= hero->x)
                direction = DIRECTION_RIGHT;
            else
                direction = DIRECTION_LEFT;
        }
        else if (fear)
        {
            maxsize /= 2;
            if (x <= hero->x)
                direction = DIRECTION_LEFT;
            else
                direction = DIRECTION_RIGHT;
        }

        if (direction == DIRECTION_RIGHT)
        {
            x += 2;
            if (x >= 2400 - basesize)
            {
                direction = DIRECTION_LEFT;
                x -= 2;
            }
        }
        else
        {
            x -= 2;
            if (x <= 0)
            {
                direction = DIRECTION_RIGHT;
                x += 2;
            }
        }

        if (hit)
        {
            if (x <= hero->x && x > 0 + 4)
            {
                if (aggro)
                    x -= 2;
                else if (fear)
                    x -= 4;
            }
            else if (x > hero->x && x < 2400 - basesize - 4)
            {
                if (aggro)
                    x += 2;
                else if (fear)
                    x += 4;
            }

            time++;
        }
    }
}

void Mob::draw()
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

    if (hit)
        glBindTexture(GL_TEXTURE_2D, texture[MOBHIT]);
    else
        glBindTexture(GL_TEXTURE_2D, texture[MOBNORMAL]);

    glBegin(GL_QUADS);
        if (aggro && !hit)
            glColor3d(1, 0, 0);
        else if (fear && !hit)
            glColor3d(0, 0.5, 1);
        glTexCoord2d(0, 1); glVertex2i(0, size - basesize / 2);
            glColor3d(1, 1, 1);
        glTexCoord2d(0, 0); glVertex2d(0, size / 2 + basesize / 2);
        glTexCoord2d(1, 0); glVertex2d(size + basesize, size / 2 + basesize / 2);
        if (aggro && !hit)
            glColor3d(1, 0, 0);
        else if (fear && !hit)
            glColor3d(0, 0.5, 1);
        glTexCoord2d(1, 1); glVertex2d(size + basesize, size - basesize / 2);
            glColor3d(1, 1, 1);
    glEnd();

    if (direction == DIRECTION_LEFT)
    {
        glMatrixMode(GL_TEXTURE);

        glPopMatrix();
    }
}

bool Mob::collide(Magic* magic)
{
    if (magic->x >= x + basesize || magic->x + 64 <= x || magic->y >= y + basesize || magic->y + 32 <= y)
        return false;
    else
        return true;
}

bool Mob::collide(Hero* hero)
{
    if (hero->x >= x + basesize || hero->x + 64 <= x || hero->y >= y + basesize || hero->y + 128 <= y)
        return false;
    else
        return true;
}
