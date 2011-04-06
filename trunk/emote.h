class Emote
{
    public:
    Emote(int inemote, double inx, double iny);
    ~Emote();
    void animate();
    void draw();
    double x;
    double y;
    double angle;
    int time;

    private:
    GLuint texture;
    bool zoom;
    GLint size;
    GLint maxsize;
    int currentframe;
    int maxframe;
};

Emote::Emote(int inemote, double inx, double iny)
{
    x = inx;
    y = iny - 32;

    zoom = true;
    size = 0;
    maxsize = rand() % 4;

    if (inemote == EMOTE_AGGRO)
    {
        SDL_Surface* surface = IMG_Load("gfx/emote_aggro.png");
        texture = glloadtexture(surface);

        SDL_FreeSurface(surface);
    }
    else if (inemote == EMOTE_FEAR)
    {
        SDL_Surface* surface = IMG_Load("gfx/emote_fear.png");
        texture = glloadtexture(surface);

        SDL_FreeSurface(surface);
    }
    else if (inemote == EMOTE_DEAD)
    {
        SDL_Surface* surface = IMG_Load("gfx/emote_dead.png");
        texture = glloadtexture(surface);

        SDL_FreeSurface(surface);
    }

    time = 0;

    currentframe = 0;
    maxframe = 7;
}

Emote::~Emote()
{
    glDeleteTextures(1, &texture);
}

void Emote::animate()
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

        time++;
    }
}

void Emote::draw()
{
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(x, y, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16 - size);
        glTexCoord2d(0, 0); glVertex2d(0, 16 + size);
        glTexCoord2d(1, 0); glVertex2d(size + 32, 16 + size);
        glTexCoord2d(1, 1); glVertex2d(size + 32, -16 - size);
    glEnd();
}
