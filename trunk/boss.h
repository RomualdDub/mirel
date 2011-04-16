class Boss
{
    public:
    Boss(int inlevel, int inspawn, int inradius, Hero* inhero);
    ~Boss();
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    bool collide(Magic* magic);
    bool collide();
    bool visible;
    bool idle;
    bool aggro;
    bool hit;
    char name[100];
    int x;
    int y;
    int hp;
    int level;
    int time;
    vector<sf::Sprite> bosssprites;

    #define BOSSNORMAL 0
    #define BOSSHIT 1

    private:
    Hero* hero;
    float size;
    float maxsize;
    int direction;
    bool zoom;
    vector<sf::Image> bossimages;
    int index;
    int currentframe;
};

Boss::Boss(int inlevel, int inspawn, int inradius, Hero* inhero)
{
    hero = inhero;

    snprintf(name, 99, "%s", "Mega Champipi Of The Dead");

    visible = false;

    idle = true;
    aggro = false;
    hit = false;

    index = BOSSNORMAL;

    x = sf::Randomizer::Random(inspawn, inradius);
    y = 236 + 128;

    direction = sf::Randomizer::Random(DIRECTION_LEFT, DIRECTION_RIGHT);

    level = inlevel;
    hp = level * 750;

    zoom = true;
    size = 0.0f;
    maxsize = sf::Randomizer::Random(0.05f, 0.1f);

    time = 0;

    bossimages.push_back(sf::Image(getimage(GFX_BOSS0)));
    bossimages.push_back(sf::Image());

    sf::Color color(0, 0, 0, 0);
    bossimages[1].Create(128, 128, color);
    bossimages[1].SetSmooth(false);

    white(&bossimages[0], &bossimages[1]);

    for (int i = 0; i < (int)bossimages.size(); i++)
        bosssprites.push_back(sf::Sprite(bossimages[i]));

    currentframe = 0;
}

Boss::~Boss()
{
    bossimages.clear();
    bosssprites.clear();
}

void Boss::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 0.01f * distance;
            if (aggro)
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
            if (aggro)
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

        if (hit)
            index = BOSSHIT;
        else
            index = BOSSNORMAL;

        if (direction == DIRECTION_RIGHT)
            x += distance;
        else
            x -= distance;

        if (hit)
        {
            if (x <= hero->x && x > 0)
            {
                if (aggro)
                    x -= distance / 2;
            }
            else if (x > hero->x && x < 2400 - 128)
            {
                if (aggro)
                    x += distance / 2;
            }

            time++;
        }

        if (x >= 2400 - 128)
            x = 2400 - 128;
        else if (x <= 0)
            x = 0;
    }
}

void Boss::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        bosssprites[index].FlipX(true);
    else
        bosssprites[index].FlipX(false);

    if (aggro)
        bosssprites[index].SetColor(sf::Color::Red);
    if (hit)
        bosssprites[index].SetColor(sf::Color::White);

    bosssprites[index].SetX(x);
    bosssprites[index].SetY(y);
    bosssprites[index].SetScale(1.0f + size, 1.0f + size);
    app->Draw(bosssprites[index]);
}

bool Boss::collide(Magic* magic)
{
    if (magic->x + 64 >= x && magic->x <= x + 128 && magic->y + 32 >= y && magic->y <= y + 128)
        return true;
    else
        return false;
}

bool Boss::collide()
{
    if (hero->x + 64 >= x && hero->x <= x + 128 && hero->y + 128 >= y && hero->y <= y + 128)
        return true;
    else
        return false;
}
