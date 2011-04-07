class Label
{
    public:
    Label(int inx, int intype, int inlevel, int inmaxhp, GLuint intexture, char* inname);
    ~Label();
    void draw();
    double x;
    int hp;

    private:
    SDL_Color color;
    TTF_Font* police;
    int type;
    int maxhp;
    double y;
    char name[100];
    GLuint texture;
    GLuint nametexture;
    GLuint hptexture;
};

Label::Label(int inx, int intype, int inlevel, int inmaxhp, GLuint intexture, char* inname)
{
    x = inx;
    y = 488;

    type = intype;

    maxhp = inmaxhp;
    hp = inmaxhp;

    texture = intexture;

    if (type == LABEL_HERO)
    {
        SDL_Surface* surface = IMG_Load("gfx/fx0.png");
        texture = glloadtexture(surface);
        SDL_FreeSurface(surface);
    }
    else if (type == LABEL_PET || type == LABEL_MOB)
    {
        color = {255, 255, 255};
        police = TTF_OpenFont("font/Plakkaat.ttf", 12);

        snprintf(name, 99, "%s (lvl %i)", inname, inlevel);

        SDL_Surface* surface = TTF_RenderText_Blended(police, name, color);
        nametexture = glloadtexture(surface);
        SDL_FreeSurface(surface);

        SDL_Surface* hpsurface = IMG_Load("gfx/minihp.png");
        hptexture = glloadtexture(hpsurface);
        SDL_FreeSurface(hpsurface);
    }
}

Label::~Label()
{
    TTF_CloseFont(police);

    if (type == LABEL_HERO)
        glDeleteTextures(1, &texture);
    else if (type == LABEL_PET || LABEL_MOB)
    {
        glDeleteTextures(1, &nametexture);
        glDeleteTextures(1, &hptexture);
    }
}

void Label::draw()
{
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(x, y, 0);

    if (type == LABEL_HERO)
        glTranslated(0, 16, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -8);
        glTexCoord2d(0, 0); glVertex2i(0, 8);
        glTexCoord2d(1, 0); glVertex2i(16, 8);
        glTexCoord2d(1, 1); glVertex2i(16, -8);
    glEnd();

    if (type == LABEL_PET || type == LABEL_MOB)
    {
        int len = strlen(name);
        glTranslated(16, 0, 0);

        glBindTexture(GL_TEXTURE_2D, nametexture);
        glBegin(GL_QUADS);
            glTexCoord2d(0, 1); glVertex2i(0, -8);
            glTexCoord2d(0, 0); glVertex2i(0, 8);
            glTexCoord2d(1, 0); glVertex2i(len * 8, 8);
            glTexCoord2d(1, 1); glVertex2i(len * 8, -8);
        glEnd();

        glTranslated(-16, 16, 0);

        glBindTexture(GL_TEXTURE_2D, hptexture);
        glBegin(GL_QUADS);
                if ((float)hp / (float)maxhp == 1)
                    glColor3d(0, 1, 0);
                else if ((float)hp / (float)maxhp > 0.375 && (float)hp / (float)maxhp < 1)
                    glColor3d(1, 1, 0);
                else if ((float)hp / (float)maxhp <= 0.375)
                    glColor3d(1, 0, 0);
            glTexCoord2d(0, 1); glVertex2i(0, -8);
            glTexCoord2d(0, 0); glVertex2i(0, 8);
            glTexCoord2d(1, 0); glVertex2i((float)hp / (float)maxhp * 32, 8);
            glTexCoord2d(1, 1); glVertex2i((float)hp / (float)maxhp * 32, -8);
                glColor3d(1, 1, 1);
        glEnd();
    }
}
