class Particle
{
    public:
    Particle(float inx, float iny);
    void animate();
    void draw(sf::RenderWindow* app);
    bool direction;
    float x;
    float y;
    float angle;
    int time;

    private:
    int magic;
    sf::Image particleimage;
    sf::Sprite particlesprite;
    int currentframe;
    int maxframe;
};

Particle::Particle(float inx, float iny)
{
    x = inx + sf::Randomizer::Random(0, 64);
    y = iny + sf::Randomizer::Random(0, 32) - 128;

    direction = sf::Randomizer::Random(DIRECTION_LEFT, DIRECTION_RIGHT);

    magic = sf::Randomizer::Random(8, 16);

    time = 0;

    particleimage.LoadFromFile("gfx/fx0.png");
    particleimage.SetSmooth(false);

    particlesprite.SetImage(particleimage);
    particlesprite.SetX(x);
    particlesprite.SetY(y);

    currentframe = 0;
    maxframe = 7;
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

void Particle::draw(sf::RenderWindow* app)
{
    particlesprite.SetX(x);
    particlesprite.SetY(y);
    app->Draw(particlesprite);
}
