class Pet
{
    public:
    Pet(int inx, int inlevel);
    ~Pet();
    void animate();
    void draw(sf::RenderWindow* app);
    bool collide(Mob* mob);
    bool collide(Boss* boss);
    bool collide(Magic* magic);
    bool visible;
    bool left;
    bool right;
    bool attack;
    bool badhit;
    bool goodhit;
    char name[100];
    int x;
    int y;
    int hp;
    int goodtime;
    int badtime;
    vector<sf::Sprite> petsprites;

    #define PETWALK0 0
    #define PETWALK1 1
    #define PETIDLE 2
    #define PETATK 3
    #define PETHIT 4

    private:
    int index;
    int direction;
    vector<sf::Image> petimages;
    int currentframe;
};

Pet::Pet(int inx, int inlevel)
{
    snprintf(name, 99, "%s", "Zuzu");

    visible = true;
    left = false;
    right = false;
    attack = false;
    goodhit = false;
    badhit = false;

    direction = DIRECTION_RIGHT;

    hp = inlevel * 2000;

    index = PETWALK0;

    x = inx;
    y = 472 / 2 + 64;

    petimages.push_back(sf::Image(getimage(GFX_ZUZU0)));
    petimages.push_back(sf::Image(getimage(GFX_ZUZU1)));
    petimages.push_back(sf::Image(getimage(GFX_ZUZU2)));
    petimages.push_back(sf::Image(getimage(GFX_ZUZU3)));
    petimages.push_back(sf::Image());

    sf::Color color(0, 0, 0, 0);
    petimages[PETHIT].Create(64, 64, color);
    petimages[PETHIT].SetSmooth(false);

    white(&petimages[PETATK], &petimages[PETHIT]);

    for (int i = 0; i < (int)petimages.size(); i++)
        petsprites.push_back(sf::Sprite(petimages[i]));

    goodtime = 0;
    badtime = 0;

    currentframe = 0;
}

Pet::~Pet()
{
    petimages.clear();
    petsprites.clear();
}

void Pet::animate()
{
    if (left)
        direction = DIRECTION_LEFT;

    if (right)
        direction = DIRECTION_RIGHT;

    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (badhit)
        {
            index = PETHIT;
            badtime++;
        }
        else
        {
            if (goodhit)
                goodtime++;

            if (left || right)
            {
                if (index == PETWALK0)
                    index = PETWALK1;
                else if (index == PETWALK1)
                    index = PETWALK0;
                else
                    index = PETWALK0;
            }

            if (attack)
                index = PETATK;

            if (!left && !right && !attack) // idle
            {
                if (!sf::Randomizer::Random(0, 10))
                    index = PETIDLE;
                else
                    index = PETWALK0;
            }
        }
    }
}

void Pet::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        petsprites[index].FlipX(true);
    else
        petsprites[index].FlipX(false);

    petsprites[index].SetX(x);
    petsprites[index].SetY(y);

    app->Draw(petsprites[index]);
}

bool Pet::collide(Mob* mob)
{
    if (mob->x + (64 * mob->scalefactor) >= x && mob->x <= x + 64 && mob->y + (64 * mob->scalefactor) >= y && mob->y <= y + 64)
        return true;
    else
        return false;
}

bool Pet::collide(Boss* boss)
{
    if (boss->x + 128 >= x && boss->x <= x + 64 && boss->y + 128 >= y && boss->y <= y + 64)
        return true;
    else
        return false;
}

bool Pet::collide(Magic* magic)
{
    if (magic->x + 64 >= x && magic->x <= x + 64 && magic->y + 32 >= y && magic->y <= y + 64)
        return true;
    else
        return false;
}
