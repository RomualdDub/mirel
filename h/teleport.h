class Teleport
{
    public:
    Teleport(Gfx* ingfx);
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    int x;
    int y;
    int worlddest;
    int worldxdest;
    int direction;

    #define MAXPARTICLE 77

    private:
    Gfx* gfx;
    vector<Particle*> particles;
    int currentframe;
};

Teleport::Teleport(Gfx* ingfx)
{
    gfx = ingfx;

    for (int i = 0; i < MAXPARTICLE; i++)
        particles.push_back(new Particle(gfx, gfx->fx_1, x, y));

    currentframe = 0;
}

void Teleport::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        for (int i = 0; i < (int)particles.size(); i++)
        {
            particles[i]->animate(distance);
            if (particles[i]->time >= 8)
            {
                delete particles[i];
                particles.erase(particles.begin() + i);
                particles.push_back(new Particle(gfx, gfx->fx_1, x, y));
            }
        }
    }
}

void Teleport::draw(sf::RenderWindow* app)
{
    for (int i = 0; i < (int)particles.size(); i++)
        particles[i]->draw(app);
}
