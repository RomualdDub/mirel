class Particle
{
    public:
    Particle(float inx, float iny);
    void animate(int distance);
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
};

Particle::Particle(float inx, float iny)
{
    x = inx + sf::Randomizer::Random(0, 64);
    y = iny + sf::Randomizer::Random(0, 32) - 128;

    direction = sf::Randomizer::Random(DIRECTION_LEFT, DIRECTION_RIGHT);

    magic = sf::Randomizer::Random(8, 16);

    time = 0;

    particleimage = getimage(GFX_FX0);
    particleimage.SetSmooth(false);

    particlesprite.SetImage(particleimage);
    particlesprite.SetX(x);
    particlesprite.SetY(y);

    currentframe = 0;
}

void Particle::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        angle++;

        if (angle < 6)
        {
            if (direction == DIRECTION_RIGHT)
                x += magic * distance;
            else
                x -= magic * distance;

            y += (8 * magic) * distance * sin(-angle);
        }
        else
            y += 4 * distance * magic;

        time++;
    }
}

void Particle::draw(sf::RenderWindow* app)
{
    particlesprite.SetX(x);
    particlesprite.SetY(y);
    app->Draw(particlesprite);
}
