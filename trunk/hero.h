class Hero
{
    public:
    Hero(int inx);
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
    sf::Sprite herosprite;

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
    sf::Image wandimage0, wandimage1, wandimage2;
    sf::Image feetimage0, feetimage1, feetimage2;
    sf::Image dressimage0, dressimage1, dressimage2;
    sf::Image headimage0, headimage1, headimage2;
    sf::Image hatimage;
    sf::Image heroimage;
    sf::Rect<int> herorect;
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

    wandimage0 = getimage(GFX_WAND0);
    wandimage1 = getimage(GFX_WAND1);
    wandimage2 = getimage(GFX_WAND2);
    feetimage0 = getimage(GFX_FEET0);
    feetimage1 = getimage(GFX_FEET1);
    feetimage2 = getimage(GFX_FEET2);
    dressimage0 = getimage(GFX_DRESS0);
    dressimage1 = getimage(GFX_DRESS1);
    dressimage2 = getimage(GFX_DRESS2);
    headimage0 = getimage(GFX_HEAD0);
    headimage1 = getimage(GFX_HEAD1);
    headimage2 = getimage(GFX_HEAD2);
    hatimage = getimage(GFX_HAT);

    sf::Color color(0, 0, 0, 0);
    heroimage.Create(64 * 9, 128, color);
    heroimage.SetSmooth(false);

    blit(&hatimage, &heroimage, 64, 128, HEROWALK0);
    blit(&headimage0, &heroimage, 64, 128, HEROWALK0);
    blit(&dressimage0, &heroimage, 64, 128, HEROWALK0);
    blit(&wandimage0, &heroimage, 64, 128, HEROWALK0);
    blit(&feetimage0, &heroimage, 64, 128, HEROWALK0);

    blit(&hatimage, &heroimage, 64, 128, HEROWALK1);
    blit(&headimage0, &heroimage, 64, 128, HEROWALK1);
    blit(&dressimage1, &heroimage, 64, 128, HEROWALK1);
    blit(&wandimage0, &heroimage, 64, 128, HEROWALK1);
    blit(&feetimage1, &heroimage, 64, 128, HEROWALK1);

    blit(&hatimage, &heroimage, 64, 128, HEROWALK2);
    blit(&headimage0, &heroimage, 64, 128, HEROWALK2);
    blit(&dressimage2, &heroimage, 64, 128, HEROWALK2);
    blit(&wandimage0, &heroimage, 64, 128, HEROWALK2);
    blit(&feetimage2, &heroimage, 64, 128, HEROWALK2);

    blit(&hatimage, &heroimage, 64, 128, HEROATK0);
    blit(&headimage1, &heroimage, 64, 128, HEROATK0);
    blit(&dressimage1, &heroimage, 64, 128, HEROATK0);
    blit(&wandimage0, &heroimage, 64, 128, HEROATK0);
    blit(&feetimage1, &heroimage, 64, 128, HEROATK0);

    blit(&hatimage, &heroimage, 64, 128, HEROATK1);
    blit(&headimage1, &heroimage, 64, 128, HEROATK1);
    blit(&dressimage1, &heroimage, 64, 128, HEROATK1);
    blit(&wandimage1, &heroimage, 64, 128, HEROATK1);
    blit(&feetimage1, &heroimage, 64, 128, HEROATK1);

    blit(&hatimage, &heroimage, 64, 128, HEROATK2);
    blit(&headimage1, &heroimage, 64, 128, HEROATK2);
    blit(&dressimage2, &heroimage, 64, 128, HEROATK2);
    blit(&wandimage2, &heroimage, 64, 128, HEROATK2);
    blit(&feetimage1, &heroimage, 64, 128, HEROATK2);

    blit(&hatimage, &heroimage, 64, 128, HEROIDLE);
    blit(&headimage2, &heroimage, 64, 128, HEROIDLE);
    blit(&dressimage1, &heroimage, 64, 128, HEROIDLE);
    blit(&wandimage0, &heroimage, 64, 128, HEROIDLE);
    blit(&feetimage1, &heroimage, 64, 128, HEROIDLE);

    blit(&hatimage, &heroimage, 64, 128, HEROJUMP);
    blit(&headimage1, &heroimage, 64, 128, HEROJUMP);
    blit(&dressimage0, &heroimage, 64, 128, HEROJUMP);
    blit(&wandimage0, &heroimage, 64, 128, HEROJUMP);
    blit(&feetimage1, &heroimage, 64, 128, HEROJUMP);

    white(&heroimage, 64, 128, HEROWALK1, HEROHIT);

    herorect.Left = index * 64;
    herorect.Right = herorect.Left + 64;
    herorect.Bottom = 0;
    herorect.Top = 128;
    herosprite.SetImage(heroimage);
    herosprite.SetSubRect(herorect);

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
        herosprite.FlipX(true);
    else
        herosprite.FlipX(false);

    if (index == HEROWALK0)
        herosprite.SetRotation(-2);
    else if (index == HEROWALK1)
        herosprite.SetRotation(0);
    else if (index == HEROWALK2)
        herosprite.SetRotation(2);

    herorect.Left = index * 64;
    herorect.Right = herorect.Left + 64;
    herosprite.SetSubRect(herorect);
    herosprite.SetX(x);
    herosprite.SetY(y);
    app->Draw(herosprite);
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
