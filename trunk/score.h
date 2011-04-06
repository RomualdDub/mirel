class Score
{
    public:
    Score(int type, double val, double inx, double iny);
    ~Score();
    void animate();
    void draw();
    double x;
    double y;
    double basey;
    double angle;

    private:
    GLuint texture;
    SDL_Color color0;
    SDL_Color color1;
    TTF_Font* police18;
    TTF_Font* police20;
    bool zoom;
    GLint size;
    GLint maxsize;
    char txt[100];
    int currentframe;
    int maxframe;
};

Score::Score(int type, double val, double inx, double iny)
{
    x = inx;
    y = iny - 32 - rand() % 128;
    basey = y;

    angle = 1 + rand() % 4;

    zoom = true;
    size = 0;
    maxsize = rand() % 14;

    police18 = TTF_OpenFont("font/Plakkaat.ttf", 18);
    police20 = TTF_OpenFont("font/Plakkaat.ttf", 20);

    if (type == SCORE_MONEY)
    {
        color0 = {0, 97, 109};
        color1 = {(Uint8)(rand() % 128), 255, 255};
    }
    else if (type == SCORE_DAMAGE)
    {
        color0 = {109, 0, 77};
        color1 = {(Uint8)(128 + rand() % 128), (Uint8)(128 + rand() % 128), 255};
    }
    else if (type == SCORE_LIFE)
    {
        color0 = {0, 0, 165};
        color1 = {(Uint8)(rand() % 128), 0, 255};
    }
    else if (type == SCORE_MANA)
    {
        color0 = {128, 0, 0};
        color1 = {255, (Uint8)(rand() % 128), 0};
    }

    if (val)
        snprintf(txt, 99, "%d", (int)val);
    else
        snprintf(txt, 99, "Miss");

    SDL_Surface* surface0 = TTF_RenderText_Blended(police20, txt, color0);
    SDL_Surface* surface1 = TTF_RenderText_Blended(police18, txt, color1);
    SDL_BlitSurface(surface1, NULL, surface0, NULL);
    texture = glloadtexture(surface0);
    SDL_FreeSurface(surface0);
    SDL_FreeSurface(surface1);

    currentframe = 0;
    maxframe = 7;
}

Score::~Score()
{
    glDeleteTextures(1, &texture);

    TTF_CloseFont(police18);
    TTF_CloseFont(police20);
}

void Score::animate()
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

        y -= angle;
    }
}

void Score::draw()
{
    int len = strlen(txt);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(x, y, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16 - size);
        glTexCoord2d(0, 0); glVertex2d(0, 16 + size);
        glTexCoord2d(1, 0); glVertex2d(len * 16 + size, 16 + size);
        glTexCoord2d(1, 1); glVertex2d(len * 16 + size, -16 - size);
    glEnd();
}
