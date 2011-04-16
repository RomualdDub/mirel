class Hero
{
    public:
    Hero(int inx);
    ~Hero();
    void animate();
    void draw(sf::RenderWindow* app);
    void dowalk();
    void doattack();
    void doidle();
    int direction;
    bool left;
    bool right;
    bool jump;
    bool attack;
    bool idle;
    bool hit;
    int x;
    int y;
    double jumpangle;
    int time;
    vector<sf::Sprite> herosprites;

    #define HEROWALK0 0
    #define HEROWALK1 1
    #define HEROWALK2 2
    #define HEROATK0 3
    #define HEROATK1 4
    #define HEROATK2 5
    #define HEROIDLE 6
    #define HEROJUMP 7
    #define HEROHIT 8

    private:
    vector<sf::Image> wandimages;
    vector<sf::Image> feetimages;
    vector<sf::Image> dressimages;
    vector<sf::Image> headimages;
    sf::Image hatimage;
    vector<sf::Image> heroimages;
    int previndex;
    int index;
    int currentframe;
};

Hero::Hero(int inx)
{
    left = false;
    right = false;
    jump = false;
    attack = false;
    idle = false;
    hit = false;

    time = 0;

    previndex = HEROWALK1;
    index = HEROWALK1;

    x = inx;
    y = 472 / 2 + 64;

    direction = DIRECTION_RIGHT;

    jumpangle = 0;

    // let's create sprite states

    wandimages.push_back(sf::Image(getimage(GFX_WAND0)));
    wandimages.push_back(sf::Image(getimage(GFX_WAND1)));
    wandimages.push_back(sf::Image(getimage(GFX_WAND2)));
    feetimages.push_back(sf::Image(getimage(GFX_FEET0)));
    feetimages.push_back(sf::Image(getimage(GFX_FEET1)));
    feetimages.push_back(sf::Image(getimage(GFX_FEET2)));
    dressimages.push_back(sf::Image(getimage(GFX_DRESS0)));
    dressimages.push_back(sf::Image(getimage(GFX_DRESS1)));
    dressimages.push_back(sf::Image(getimage(GFX_DRESS2)));
    headimages.push_back(sf::Image(getimage(GFX_HEAD0)));
    headimages.push_back(sf::Image(getimage(GFX_HEAD1)));
    headimages.push_back(sf::Image(getimage(GFX_HEAD2)));
    hatimage = getimage(GFX_HAT);

    sf::Color color(0, 0, 0, 0);
    for (int i = 0; i < HEROHIT + 1; i++)
    {
        heroimages.push_back(sf::Image());
        heroimages[i].Create(64, 128, color);
        heroimages[i].SetSmooth(false);
    }

    blit(&hatimage, &heroimages[HEROWALK0]);
    blit(&headimages[0], &heroimages[HEROWALK0]);
    blit(&dressimages[0], &heroimages[HEROWALK0]);
    blit(&wandimages[0], &heroimages[HEROWALK0]);
    blit(&feetimages[0], &heroimages[HEROWALK0]);

    blit(&hatimage, &heroimages[HEROWALK1]);
    blit(&headimages[0], &heroimages[HEROWALK1]);
    blit(&dressimages[1], &heroimages[HEROWALK1]);
    blit(&wandimages[0], &heroimages[HEROWALK1]);
    blit(&feetimages[1], &heroimages[HEROWALK1]);

    blit(&hatimage, &heroimages[HEROWALK2]);
    blit(&headimages[0], &heroimages[HEROWALK2]);
    blit(&dressimages[2], &heroimages[HEROWALK2]);
    blit(&wandimages[0], &heroimages[HEROWALK2]);
    blit(&feetimages[2], &heroimages[HEROWALK2]);

    blit(&hatimage, &heroimages[HEROATK0]);
    blit(&headimages[1], &heroimages[HEROATK0]);
    blit(&dressimages[1], &heroimages[HEROATK0]);
    blit(&wandimages[0], &heroimages[HEROATK0]);
    blit(&feetimages[1], &heroimages[HEROATK0]);

    blit(&hatimage, &heroimages[HEROATK1]);
    blit(&headimages[1], &heroimages[HEROATK1]);
    blit(&dressimages[1], &heroimages[HEROATK1]);
    blit(&wandimages[1], &heroimages[HEROATK1]);
    blit(&feetimages[1], &heroimages[HEROATK1]);

    blit(&hatimage, &heroimages[HEROATK2]);
    blit(&headimages[1], &heroimages[HEROATK2]);
    blit(&dressimages[2], &heroimages[HEROATK2]);
    blit(&wandimages[2], &heroimages[HEROATK2]);
    blit(&feetimages[1], &heroimages[HEROATK2]);

    blit(&hatimage, &heroimages[HEROIDLE]);
    blit(&headimages[2], &heroimages[HEROIDLE]);
    blit(&dressimages[1], &heroimages[HEROIDLE]);
    blit(&wandimages[0], &heroimages[HEROIDLE]);
    blit(&feetimages[1], &heroimages[HEROIDLE]);

    blit(&hatimage, &heroimages[HEROJUMP]);
    blit(&headimages[1], &heroimages[HEROJUMP]);
    blit(&dressimages[0], &heroimages[HEROJUMP]);
    blit(&wandimages[0], &heroimages[HEROJUMP]);
    blit(&feetimages[1], &heroimages[HEROJUMP]);

    white(&heroimages[HEROWALK1], &heroimages[HEROHIT]);

    for (int i = 0; i < (int)heroimages.size(); i++)
        herosprites.push_back(sf::Sprite(heroimages[i]));

    herosprites[HEROWALK0].SetRotation(-2);
    herosprites[HEROWALK1].SetRotation(0);
    herosprites[HEROWALK2].SetRotation(2);

    currentframe = 0;
}

Hero::~Hero()
{
    herosprites.clear();
    wandimages.clear();
    feetimages.clear();
    dressimages.clear();
    headimages.clear();
    heroimages.clear();
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
            index = HEROHIT;
            time++;
        }
        else
        {
            if (jump)
                index = HEROJUMP;
            else
            {
                if (left || right)
                {
                    if (index > HEROWALK2)
                        index = HEROWALK0;
                }
            }

            if (attack)
            {
                if (index < HEROATK0 || index > HEROATK2)
                    index = HEROATK0;
            }
        }

        if (!left && !right && !jump && !attack && !hit)
            idle = true;
        else
            idle = false;

        if (!hit)
        {
            if (left || right)
                dowalk();

            if (attack)
                doattack();

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
    switch (index)
    {
        case HEROWALK0:
            previndex = HEROWALK0;
            index++;
            break;
        case HEROWALK1:
            if (previndex == HEROWALK0)
                index++;
            else
                index--;
            break;
        case HEROWALK2:
            previndex = HEROWALK2;
            index--;
            break;
    }
}

void Hero::doattack()
{
    if (index >= HEROATK0 && index < HEROATK2)
        index++;
    else
    {
        index = HEROWALK1;
        attack = false;
    }
}

void Hero::doidle()
{
    if (!sf::Randomizer::Random(0, 10))
        index = HEROIDLE;
    else
        index = HEROWALK1;
}
