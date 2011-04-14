class Pet
{
    public:
    Pet(int inx);
    ~Pet();
    void animate();
    void draw(sf::RenderWindow* app);
    bool collide(Mob* mob);
    bool visible;
    bool left;
    bool right;
    bool attack;
    char name[100];
    int x;
    int y;
    int hp;
    vector<sf::Sprite> petsprites;

    #define PETWALK0 0
    #define PETWALK1 1
    #define PETIDLE 2
    #define PETATK 3

    private:
    int index;
    int direction;
    vector<sf::Image> petimages;
    int currentframe;
};

Pet::Pet(int inx)
{
    snprintf(name, 99, "%s", "Zuzu");

    visible = true;

    left = false;
    right = false;
    attack = false;

    direction = DIRECTION_RIGHT;

    hp = 250;

    index = PETWALK0;

    x = inx;
    y = 472 / 2 + 64;

    petimages.push_back(sf::Image(getimage(GFX_ZUZU0)));
    petimages.push_back(sf::Image(getimage(GFX_ZUZU1)));
    petimages.push_back(sf::Image(getimage(GFX_ZUZU2)));
    petimages.push_back(sf::Image(getimage(GFX_ZUZU3)));

    for (int i = 0; i < (int)petimages.size(); i++)
        petsprites.push_back(sf::Sprite(petimages[i]));

    currentframe = 0;
}

Pet::~Pet()
{
    petimages.clear();
    petsprites.clear();
}

void Pet::animate()
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (left)
            direction = DIRECTION_LEFT;

        if (right)
            direction = DIRECTION_RIGHT;

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
    if (mob->x >= x - 32 && mob->x - 32 <= x && mob->y >= y - 64 && mob->y - 64 <= y)
        return true;
    else
        return false;
}

