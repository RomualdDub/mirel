class Hero
{
    public:
    Hero(Gfx* ingfx, Snd* insnd, Xml* inxml, string intype);
    ~Hero();
    void animate();
    void draw(sf::RenderWindow* app);
    void dowalk();
    void doaction();
    void doidle();
    int direction;
    bool left;
    bool right;
    bool jump;
    bool action;
    bool idle;
    bool hit;
    bool eleft;
    bool eright;
    int y;
    int xe;
    int ye;
    double jumpangle;
    int time;

    private:
    Gfx* gfx;
    Snd* snd;
    Xml* xml;
    string type;
    vector<sf::Sprite> herosprites;
    int previndex;
    int index;
    int currentframe;
};

Hero::Hero(Gfx* ingfx, Snd* insnd, Xml* inxml, string intype)
{
    gfx = ingfx;
    snd = insnd;
    xml = inxml;
    type = intype;

    left = false;
    right = false;
    jump = false;
    action = false;
    idle = false;
    hit = false;
    eleft = false;
    eright = false;

    time = 0;

    y = 472 / 2 + 64;

    direction = xml->infoherodirection;

    jumpangle = 0;

    gfx->loadhero(xml->infohatindex, xml->infodressindex, xml->infowandindex);

    for (int i = 0; i < (int)gfx->heroimages.size(); i++)
        herosprites.push_back(sf::Sprite(gfx->heroimages[i]));

    if (type == "general")
    {
        herosprites[gfx->hero_walk0].SetRotation(-2);
        herosprites[gfx->hero_walk1].SetRotation(0);
        herosprites[gfx->hero_walk2].SetRotation(2);
        previndex = gfx->hero_walk1;
        index = gfx->hero_walk1;
    }
    else if (type == "ether_hl")
    {
        herosprites[gfx->hero_jump].SetRotation(-90);
        index = gfx->hero_jump;
        jump = true;
        xe = 128 + 64;
        ye = 472 / 2 - 32;
    }
    else if (type == "ether_hr")
    {
        herosprites[gfx->hero_jump].SetRotation(90);
        index = gfx->hero_jump;
        jump = true;
        xe = 800 - 128 - 64;
        ye = 472 / 2 + 32;
    }
    else if (type == "ether_v")
    {
        herosprites[gfx->hero_jump].FlipY(true);
        index = gfx->hero_jump;
        jump = true;
        xe = 800 / 2;
        ye = 472 / 2 - 32;
    }

    currentframe = 0;
}

Hero::~Hero()
{
    herosprites.clear();
}

void Hero::animate()
{
    if (left)
        direction = DIRECTION_LEFT;

    if (right)
        direction = DIRECTION_RIGHT;

    if (left && xml->infoherox <= 0)
    {
        xml->infoherox = 0;
        left = false;
    }

    if (right && xml->infoherox >= 2400 - 64)
    {
        xml->infoherox = 2400 - 64;
        right = false;
    }

    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (hit)
        {
            index = gfx->hero_hit;
            time++;
        }
        else
        {
            if (jump)
                index = gfx->hero_jump;
            else
            {
                if (left || right)
                {
                    if (index > gfx->hero_walk2)
                        index = gfx->hero_walk0;
                }
            }

            if (action)
            {
                if (index < gfx->hero_act0 || index > gfx->hero_act2)
                    index = gfx->hero_act0;
            }
        }

        if (!left && !right && !jump && !action && !hit)
            idle = true;
        else
            idle = false;

        if (!hit)
        {
            if (left || right)
                dowalk();

            if (action)
                doaction();

            if (idle)
                doidle();
        }
    }
}

void Hero::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        herosprites[index].FlipX(true);
    else
        herosprites[index].FlipX(false);

    if (type == "general")
    {
        herosprites[index].SetX(xml->infoherox);
        herosprites[index].SetY(y);
    }
    else
    {
        herosprites[index].SetX(xe);
        herosprites[index].SetY(ye);
    }
    app->Draw(herosprites[index]);
}

void Hero::dowalk()
{
    if (index == gfx->hero_walk0)
    {
        snd->footstep.Play();
        previndex = gfx->hero_walk0;
        index++;
    }
    else if (index == gfx->hero_walk1)
        if (previndex == gfx->hero_walk0)
            index++;
        else
            index--;
    else if (index == gfx->hero_walk2)
    {
        previndex = gfx->hero_walk2;
        index--;
    }
}

void Hero::doaction()
{
    if (index >= gfx->hero_act0 && index < gfx->hero_act2)
        index++;
    else
    {
        index = gfx->hero_walk1;
        action = false;
    }
}

void Hero::doidle()
{
    if (!sf::Randomizer::Random(0, 10))
        index = gfx->hero_idle;
    else
        index = gfx->hero_walk1;
}
