class Pet
{
    public:
    Pet(Gfx* ingfx, Xml* inxml);
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
    string name;
    int direction;
    int y;
    int hp;
    int maxhp;
    int goodtime;
    int badtime;
    vector<sf::Sprite> petsprites;

    private:
    Gfx* gfx;
    Xml* xml;
    int index;
    int currentframe;
};

Pet::Pet(Gfx* ingfx, Xml* inxml)
{
    gfx = ingfx;
    xml = inxml;

    name = "Zuzu";

    visible = true;
    left = false;
    right = false;
    attack = false;
    goodhit = false;
    badhit = false;

    direction = DIRECTION_RIGHT;

    hp = xml->infolevel * 2000;
    maxhp = hp;

    y = 472 / 2 + 64;

    gfx->loadpet();

    index = gfx->pet_walk0;

    for (int i = 0; i < (int)gfx->petimages.size(); i++)
        petsprites.push_back(sf::Sprite(gfx->petimages[i]));

    goodtime = 0;
    badtime = 0;

    currentframe = 0;
}

Pet::~Pet()
{
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
            index = gfx->pet_hit;
            badtime++;
        }
        if (goodhit)
            goodtime++;

        if (!badhit)
        {
            if (left || right)
            {
                if (index == gfx->pet_walk0)
                    index = gfx->pet_walk1;
                else if (index == gfx->pet_walk1)
                    index = gfx->pet_walk0;
                else
                    index = gfx->pet_walk0;
            }

            if (attack)
                index = gfx->pet_atk;

            if (!left && !right && !attack) // idle
            {
                if (!sf::Randomizer::Random(0, 10))
                    index = gfx->pet_idle;
                else
                    index = gfx->pet_walk0;
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

    petsprites[index].SetX(xml->infopetx);
    petsprites[index].SetY(y);

    app->Draw(petsprites[index]);
}

bool Pet::collide(Mob* mob)
{
    if (mob->x + (64 * mob->scalefactor) >= xml->infopetx && mob->x <= xml->infopetx + 64 && mob->y + (64 * mob->scalefactor) >= y && mob->y <= y + 64)
        return true;
    else
        return false;
}

bool Pet::collide(Boss* boss)
{
    if (boss->x + 128 >= xml->infopetx && boss->x <= xml->infopetx + 64 && boss->y + 128 >= y && boss->y <= y + 64)
        return true;
    else
        return false;
}

bool Pet::collide(Magic* magic)
{
    if (magic->x + 64 >= xml->infopetx && magic->x <= xml->infopetx + 64 && magic->y + 32 >= y && magic->y <= y + 64)
        return true;
    else
        return false;
}
