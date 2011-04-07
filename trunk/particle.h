class Particle
{
    public:
    Particle(double inx, double iny);
    ~Particle();
    void animate();
    void draw();
    double x;
    double y;
    bool direction;
    double angle;
    int time;

    private:
    GLuint texture;
    int magic;
    int currentframe;
    int maxframe;
};

Particle::Particle(double inx, double iny)
{
    SDL_Surface* surface = IMG_Load("gfx/fx0.png");
    texture = glloadtexture(surface);
    SDL_FreeSurface(surface);

    x = inx + rand() % 64;
    y = iny + rand() % 32;

    direction = rand() % 2;

    magic = 8 + rand() % 8;

    time = 0;

    currentframe = 0;
    maxframe = 7;
}

Particle::~Particle()
{
    glDeleteTextures(1, &texture);
}

void Particle::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        angle++;

        if (angle < 6)
        {
            if (direction == DIRECTION_RIGHT)
                x += magic;
            else
                x -= magic;

            y += (8 * magic) * sin(-angle);
        }
        else
            y += 4 * magic;

        time++;
    }
}

void Particle::draw()
{
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(x, y, 0);
    glRotated(magic * angle, 0, 0, 1);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -magic / 2);
        glTexCoord2d(0, 0); glVertex2i(0, magic / 2);
        glTexCoord2d(1, 0); glVertex2i(magic, magic / 2);
        glTexCoord2d(1, 1); glVertex2i(magic, -magic / 2);
    glEnd();
}
