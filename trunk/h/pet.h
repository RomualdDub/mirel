class Pet
{
    public:
    Pet(Gfx* ingfx, Snd* insnd, Xml* inxml, string intype);
    ~Pet();
    void animate();
    void draw(sf::RenderWindow* app);
    bool collide(Mob* mob);
    bool collide(Boss* boss);
    bool collide(Magic* magic);
    bool collide(Loot* loot);
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
    string type;
    vector<sf::Sprite> petsprites;
    sf::Sound badhitsound;

    private:
    Gfx* gfx;
    Snd* snd;
    Xml* xml;
    int index;
    int currentframe;
};

Pet::Pet(Gfx* ingfx, Snd* insnd, Xml* inxml, string intype)
{
    gfx = ingfx;
    snd = insnd;
    xml = inxml;
    type = intype;

    direction = DIRECTION_RIGHT;

    y = 472 / 2 + 64;

    if (type == "hedgehog")
    {
        name = "Zuzu";

        visible = true;
        left = false;
        right = false;
        attack = false;
        goodhit = false;
        badhit = false;

        goodtime = 0;
        badtime = 0;

        hp = xml->infolevel * 2000;
        maxhp = hp;

        gfx->loadzuzu();

        index = gfx->pet_walk0;

        // gfx

        for (int i = gfx->pet_walk0; i < gfx->pet_hit + 1; i++)
            petsprites.push_back(sf::Sprite(gfx->zuzuimages[i]));

        // snd

        badhitsound.SetBuffer(snd->bufferherohit);
        badhitsound.SetPitch(4.f);
        badhitsound.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
        badhitsound.SetAttenuation(17.5f);
    }
    if (type == "crow")
    {
        name = "Crocro";

        left = false;
        right = false;

        gfx->loadcrocro();

        index = gfx->pet_fly0;

        // gfx

        for (int i = gfx->pet_fly0; i < gfx->pet_fly1 + 1; i++)
            petsprites.push_back(sf::Sprite(gfx->crocroimages[i]));
    }

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

        if (type == "crow")
        {
            if (left || right)
            {
                    if (!sf::Randomizer::Random(0, 5))
                        index = gfx->pet_fly0;
                    else
                        index = gfx->pet_fly1;
            }
            else
            {
                if (index == gfx->pet_fly1)
                    index = gfx->pet_fly0;
                else if (index == gfx->pet_fly0)
                    index = gfx->pet_fly1;
            }
        }
        else if (type == "hedgehog")
        {
            if (badhit)
            {
                if (!badtime)
                {
                    badhitsound.SetPosition(xml->infopettankx, y, 0.f);
                    badhitsound.Play();
                }
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
}

void Pet::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        petsprites[index].FlipX(true);
    else
        petsprites[index].FlipX(false);

    if (type == "hedgehog")
        petsprites[index].SetX(xml->infopettankx);
    else
        petsprites[index].SetX(xml->infopetlootx);
    petsprites[index].SetY(y);

    app->Draw(petsprites[index]);
}

bool Pet::collide(Mob* mob)
{
    if (mob->x + (64 * mob->scalefactor) >= xml->infopettankx && mob->x <= xml->infopettankx + 64 && mob->y + (64 * mob->scalefactor) >= y && mob->y <= y + 64)
        return true;
    else
        return false;
}

bool Pet::collide(Boss* boss)
{
    if (boss->x + 128 >= xml->infopettankx && boss->x <= xml->infopettankx + 64 && boss->y + 128 >= y && boss->y <= y + 64)
        return true;
    else
        return false;
}

bool Pet::collide(Magic* magic)
{
    if (magic->x + 64 >= xml->infopettankx && magic->x <= xml->infopettankx + 64 && magic->y + 32 >= y && magic->y <= y + 64)
        return true;
    else
        return false;
}

bool Pet::collide(Loot* loot)
{
    if (loot->x + 32 >= xml->infopetlootx && loot->x <= xml->infopetlootx + 64 && loot->y + 32 >= y && loot->y <= y + 64)
        return true;
    else
        return false;
}
