class Magic
{
    public:
    Magic(bool indirection, int inx, int iny);
    ~Magic();
    void animate();
    void draw();
    double x;
    double y;
    bool direction;
    int time;

    private:
    GLuint texture;
    double angle;
    int currentframe;
    int maxframe;
};

Magic::Magic(bool indirection, int inx, int iny)
{
    SDL_Surface* surface = IMG_Load("gfx/pyro0.png");
    texture = glloadtexture(surface);
    SDL_FreeSurface(surface);

    direction = indirection;
    if (direction == DIRECTION_RIGHT)
        x = inx + 64;
    else
        x = inx - 64;

    y = iny;

    time = 0;

    currentframe = 0;
    maxframe = 7;
}

Magic::~Magic()
{
    glDeleteTextures(1, &texture);
}

void Magic::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (direction == DIRECTION_RIGHT)
            x += 16;
        else
            x -= 16;

        y += 16 * sin(angle);
        angle++;

        time++;
    }
}

void Magic::draw()
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

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -16);
        glTexCoord2d(0, 0); glVertex2i(0, 16);
        glTexCoord2d(1, 0); glVertex2i(64, 16);
        glTexCoord2d(1, 1); glVertex2i(64, -16);
    glEnd();

    if (direction == DIRECTION_LEFT)
    {
        glMatrixMode(GL_TEXTURE);

        glPopMatrix();
    }
}
