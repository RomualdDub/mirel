class Mob
{
    public:
    Mob(int inlevel, int inspawn, int inradius);
    void animate(Hero* hero);
    void draw(sf::RenderWindow* app);
    bool collide(Magic* magic);
    bool collide(Hero* hero);
    bool idle;
    bool aggro;
    bool fear;
    char name[100];
    int x;
    int y;
    int hp;
    int level;
    bool hit;
    int time;
    sf::Sprite mobsprite;

    #define MOBNORMAL 0
    #define MOBHIT 1

    private:
    float scalefactor;
    float size;
    float maxsize;
    int direction;
    bool zoom;
    sf::Image mobtempimage;
    sf::Image mobimage;
    sf::Rect<int> mobrect;
    int index;
    int currentframe;
    int maxframe;
};

Mob::Mob(int inlevel, int inspawn, int inradius)
{
    snprintf(name, 99, "%s", "Champipi");

    idle = true;
    aggro = false;
    fear = false;
    hit = false;

    index = MOBNORMAL;

    scalefactor = sf::Randomizer::Random(0.7f, 1.0f);
    x = sf::Randomizer::Random(inspawn, inradius);
    y = 236 + 64;

    direction = sf::Randomizer::Random(DIRECTION_LEFT, DIRECTION_RIGHT);

    level = inlevel;
    hp = level * 250;

    zoom = true;
    size = 0.0f;
    maxsize = sf::Randomizer::Random(0.05f, 0.1f);

    time = 0;

    mobtempimage.LoadFromFile("gfx/mob0.png");

    sf::Color color(0, 0, 0, 0);
    mobimage.Create(64 * 2, 64, color);
    mobimage.SetSmooth(false);

    blit(&mobtempimage, &mobimage, 64, 64, MOBNORMAL);
    white(&mobimage, 64, 64, MOBNORMAL, MOBHIT);

    mobrect.Left = index * 64;
    mobrect.Right = mobrect.Left + 64;
    mobrect.Bottom = 0;
    mobrect.Top = 64;
    mobsprite.SetImage(mobimage);
    mobsprite.SetSubRect(mobrect);
    mobsprite.SetX(x);
    mobsprite.SetY(y);
    mobsprite.SetScale(scalefactor, scalefactor);

    currentframe = 0;
    maxframe = 7;
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
            size += 0.01f;
            if (size >= maxsize)
                zoom = false;
        }
        else
        {
            size -= 0.01f;
            if (size <= -maxsize)
                zoom = true;
        }

        if (idle)
        {
            if (sf::Randomizer::Random(0, 10) < 1)
            {
                if (direction == DIRECTION_RIGHT)
                    direction = DIRECTION_LEFT;
                else
                    direction = DIRECTION_RIGHT;
            }
        }
        else if (aggro)
        {
            maxsize /= 2.0f;
            if (x <= hero->x)
                direction = DIRECTION_RIGHT;
            else
                direction = DIRECTION_LEFT;
        }
        else if (fear)
        {
            maxsize /= 2.0f;
            if (x <= hero->x)
                direction = DIRECTION_LEFT;
            else
                direction = DIRECTION_RIGHT;
        }

        if (hit)
            index = MOBHIT;
        else
            index = MOBNORMAL;

        if (direction == DIRECTION_RIGHT)
        {
            x += 2;
            if (x >= 2400 - (64 * scalefactor))
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
            else if (x > hero->x && x < 2400 - (64 * scalefactor) - 4)
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

void Mob::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        mobsprite.FlipX(true);
    else
        mobsprite.FlipX(false);

    if (aggro)
    {
        sf::Color color(255, 0, 0);
        mobsprite.SetColor(color);
    }
    else if (fear)
    {
        sf::Color color(0, 128, 255);
        mobsprite.SetColor(color);
    }
    if (hit)
    {
        sf::Color color(255, 255, 255);
        mobsprite.SetColor(color);
    }

    mobrect.Left = index * 64;
    mobrect.Right = mobrect.Left + 64;
    mobsprite.SetSubRect(mobrect);
    mobsprite.SetX(x);
    mobsprite.SetY(y);
    mobsprite.SetScale(scalefactor + size, scalefactor + size);
    app->Draw(mobsprite);
}

bool Mob::collide(Magic* magic)
{
    if (magic->x >= x - 64 && magic->x - 64 <= x && magic->y >= y - 64 && magic->y - 32 <= y)
        return true;
    else
        return false;
}

bool Mob::collide(Hero* hero)
{
    if (hero->x >= x - 64 && hero->x - 64 <= x && hero->y >= y - 64 && hero->y - 128 <= y)
        return true;
    else
        return false;
}
