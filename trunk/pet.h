class Pet
{
    public:
    Pet();
    void animate();
    void draw(sf::RenderWindow* app);
    bool collide(Mob* mob);
    bool left;
    bool right;
    bool attack;
    char name[100];
    int x;
    int y;
    int hp;
    int level;
    sf::Sprite petsprite;

    #define PETWALK0 0
    #define PETWALK1 1
    #define PETIDLE 2
    #define PETATK 3

    private:
    int index;
    int direction;
    sf::Image petimage;
    sf::Rect<int> petrect;
    int currentframe;
    int maxframe;
};

Pet::Pet()
{
    snprintf(name, 99, "%s", "Zuzu");

    left = false;
    right = false;
    attack = false;

    direction = DIRECTION_RIGHT;

    level = 1;
    hp = level * 250;

    index = PETWALK0;

    x = 200 - 128;
    y = 472 / 2 + 64;

    petimage.LoadFromFile("gfx/pet.png");
    petimage.SetSmooth(false);

    petrect.Left = index * 64;
    petrect.Right = petrect.Left + 64;
    petrect.Bottom = 0;
    petrect.Top = 64;
    petsprite.SetImage(petimage);
    petsprite.SetSubRect(petrect);
    petsprite.SetX(x);
    petsprite.SetY(y);

    currentframe = 0;
    maxframe = 7;
}

void Pet::animate()
{
    if (currentframe < maxframe)
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
            if (sf::Randomizer::Random(0, 10) < 1)
                index = PETIDLE;
            else
                index = PETWALK0;
        }
    }
}

void Pet::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        petsprite.FlipX(true);
    else
        petsprite.FlipX(false);

    petrect.Left = index * 64;
    petrect.Right = petrect.Left + 64;
    petsprite.SetSubRect(petrect);
    petsprite.SetX(x);
    petsprite.SetY(y);

    app->Draw(petsprite);
}

bool Pet::collide(Mob* mob)
{
    if (mob->x >= x - 32 && mob->x - 32 <= x && mob->y >= y - 64 && mob->y - 64 <= y)
        return true;
    else
        return false;
}

