class World
{
    public:
    World();
    ~World();
    void animate();
    void draw();

    private:
    GLuint skytexture;
    GLuint worldtexture;
    GLuint skyx;
    int currentframe;
    int maxframe;
};

World::World()
{
    skyx = 0;

    SDL_Surface* skysurface = IMG_Load("gfx/sky0.png");
    skytexture = glloadtexture(skysurface);

    SDL_Surface* worldsurface = IMG_Load("gfx/world0.png");
    worldtexture = glloadtexture(worldsurface);

    SDL_FreeSurface(skysurface);
    SDL_FreeSurface(worldsurface);

    currentframe = 0;
    maxframe = 7;
}

World::~World()
{
    glDeleteTextures(1, &skytexture);
    glDeleteTextures(1, &worldtexture);
}

void World::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;
        if (skyx == 800)
            skyx = 0;
        else
            skyx++;
    }
}

void World::draw()
{
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(0, 236, 0);

    glBindTexture(GL_TEXTURE_2D, skytexture);
    glBegin(GL_QUADS);
        glTexCoord2d((double)skyx / 800.0 - 0, 1); glVertex2i(0, -236);
        glTexCoord2d((double)skyx / 800.0 - 0, 0); glVertex2d(0, 236);
        glTexCoord2d((double)skyx / 800.0 - 3, 0); glVertex2d(2400, 236);
        glTexCoord2d((double)skyx / 800.0 - 3, 1); glVertex2d(2400, -236);
    glEnd();

    glLoadIdentity();
    glTranslated(0, 236, 0);

    glBindTexture(GL_TEXTURE_2D, worldtexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -236);
        glTexCoord2d(0, 0); glVertex2d(0, 236);
        glTexCoord2d(1, 0); glVertex2d(2400, 236);
        glTexCoord2d(1, 1); glVertex2d(2400, -236);
    glEnd();
}
