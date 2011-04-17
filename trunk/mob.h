class Mob
{
    public:
    Mob(Gfx* ingfx, int inlevel, int inspawn, int inradius, Hero* inhero);
    ~Mob();
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    bool collide(Magic* magic);
    bool collide();
    int getmaxhp();
    bool visible;
    bool idle;
    bool aggro;
    bool fear;
    bool hit;
    char name[100];
    int direction;
    int x;
    int y;
    int hp;
    int level;
    float scalefactor;
    int time;
    vector<sf::Sprite> mobsprites;

    private:
    Gfx* gfx;
    Hero* hero;
    float size;
    float maxsize;
    bool zoom;
    int index;
    int currentframe;
};

Mob::Mob(Gfx* ingfx, int inlevel, int inspawn, int inradius, Hero* inhero)
{
    gfx = ingfx;
    hero = inhero;

    snprintf(name, 99, "%s", "Champipi");

    visible = false;

    idle = true;
    aggro = false;
    fear = false;
    hit = false;

    index = gfx->mob_normal;

    scalefactor = sf::Randomizer::Random(0.7f, 1.0f);
    x = sf::Randomizer::Random(inspawn, inradius);
    y = 236 + 64;

    direction = sf::Randomizer::Random(DIRECTION_LEFT, DIRECTION_RIGHT);

    level = inlevel;
    hp = getmaxhp();

    zoom = true;
    size = 0.0f;
    maxsize = sf::Randomizer::Random(0.05f, 0.1f);

    time = 0;

    for (int i = 0; i < (int)gfx->mobimages.size(); i++)
    {
        mobsprites.push_back(sf::Sprite(gfx->mobimages[i]));
        mobsprites[i].SetScale(scalefactor, scalefactor);
    }

    currentframe = 0;
}

Mob::~Mob()
{
    mobsprites.clear();
}

void Mob::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 0.01f * distance;
            if (aggro || fear)
            {
                if (size >= maxsize / 4.0f)
                    zoom = false;
            }
            else
            {
                if (size >= maxsize)
                    zoom = false;
            }
        }
        else
        {
            size -= 0.01f * distance;
            if (aggro || fear)
            {
                if (size <= -maxsize / 4.0f)
                    zoom = true;
            }
            else
            {
                if (size <= -maxsize)
                    zoom = true;
            }
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
            if (x <= hero->x)
                direction = DIRECTION_RIGHT;
            else
                direction = DIRECTION_LEFT;
        }
        else if (fear)
        {
            if (x <= hero->x)
                direction = DIRECTION_LEFT;
            else
                direction = DIRECTION_RIGHT;
        }

        if (hit)
            index = gfx->mob_hit;
        else
            index = gfx->mob_normal;

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
        mobsprites[index].FlipX(true);
    else
        mobsprites[index].FlipX(false);

    if (aggro)
        mobsprites[index].SetColor(sf::Color::Red);
    else if (fear)
        mobsprites[index].SetColor(sf::Color::Cyan);
    if (hit)
        mobsprites[index].SetColor(sf::Color::White);

    mobsprites[index].SetCenter(0, 64 * scalefactor - 64);
    mobsprites[index].SetX(x);
    mobsprites[index].SetY(y);
    mobsprites[index].SetScale(scalefactor + size, scalefactor + size);
    app->Draw(mobsprites[index]);
}

bool Mob::collide(Magic* magic)
{
    if (magic->x + 64 >= x && magic->x <= x + (64 * scalefactor) && magic->y + 32 >= y && magic->y <= y + (64 * scalefactor))
        return true;
    else
        return false;
}

bool Mob::collide()
{
    if (hero->x + 64 >= x && hero->x <= x + (64 * scalefactor) && hero->y + 128 >= y && hero->y <= y + (64 * scalefactor))
        return true;
    else
        return false;
}

int Mob::getmaxhp()
{
    return level * 250;
}
