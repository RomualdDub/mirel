class Hero
{
    public:
    Hero(Gfx* ingfx, int inx);
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
    int x;
    int y;
    double jumpangle;
    int time;

    private:
    Gfx* gfx;
    vector<sf::Sprite> herosprites;
    int previndex;
    int index;
    int currentframe;
};

Hero::Hero(Gfx* ingfx, int inx)
{
    gfx = ingfx;

    left = false;
    right = false;
    jump = false;
    action = false;
    idle = false;
    hit = false;

    time = 0;

    x = inx;
    y = 472 / 2 + 64;

    direction = DIRECTION_RIGHT;

    jumpangle = 0;

    gfx->loadhero();

    for (int i = 0; i < (int)gfx->heroimages.size(); i++)
        herosprites.push_back(sf::Sprite(gfx->heroimages[i]));

    herosprites[gfx->hero_walk0].SetRotation(-2);
    herosprites[gfx->hero_walk1].SetRotation(0);
    herosprites[gfx->hero_walk2].SetRotation(2);

    previndex = gfx->hero_walk1;
    index = gfx->hero_walk1;

    currentframe = 0;
}

void Hero::animate()
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

    herosprites[index].SetX(x);
    herosprites[index].SetY(y);
    app->Draw(herosprites[index]);
}

void Hero::dowalk()
{
    if (index == gfx->hero_walk0)
    {
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
