class Mob
{
    public:
    Mob(int inlevel, int inspawn, int inradius);
    void animate(int distance, Hero* hero);
    void draw(sf::RenderWindow* app);
    bool collide(Magic* magic);
    bool collide(Hero* hero);
    bool visible;
    bool idle;
    bool aggro;
    bool fear;
    bool hit;
    char name[100];
    int x;
    int y;
    int hp;
    int level;
    float scalefactor;
    int time;
    sf::Sprite mobsprite;

    #define MOBNORMAL 0
    #define MOBHIT 1

    private:
    float size;
    float maxsize;
    int direction;
    bool zoom;
    sf::Image mobtempimage;
    sf::Image mobimage;
    sf::Rect<int> mobrect;
    int index;
    int currentframe;
};

Mob::Mob(int inlevel, int inspawn, int inradius)
{
    snprintf(name, 99, "%s", "Champipi");

    visible = false;

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

    mobtempimage = getimage(GFX_MOB0);

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
}

void Mob::animate(int distance, Hero* hero)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 0.01f * distance;
            if (size >= maxsize)
                zoom = false;
        }
        else
        {
            size -= 0.01f * distance;
            if (size <= -maxsize)
                zoom = true;
        }

        if (idle)
        {
            if (!sf::Randomizer::Random(0, 10))
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
            x += distance;
        else
            x -= distance;

        if (hit)
        {
            if (x <= hero->x && x > 0)
            {
                if (aggro)
                    x -= distance;
                else if (fear)
                    x -= 2 * distance;
            }
            else if (x > hero->x && x < 2400 - (64 * scalefactor))
            {
                if (aggro)
                    x += distance;
                else if (fear)
                    x += 2 * distance;
            }

            time++;
        }

        if (x >= 2400 - (64 * scalefactor))
            x = 2400 - (64 * scalefactor);
        else if (x <= 0)
            x = 0;
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
    if (magic->x >= x - (64 * scalefactor) && magic->x - 64 <= x && magic->y >= y - (64 * scalefactor) && magic->y - 32 <= y)
        return true;
    else
        return false;
}

bool Mob::collide(Hero* hero)
{
    if (hero->x >= x - (64 * scalefactor) && hero->x - 64 <= x && hero->y >= y - (64 * scalefactor) && hero->y - 128 <= y)
        return true;
    else
        return false;
}
