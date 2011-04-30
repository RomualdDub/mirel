class Magic
{
    public:
    Magic(Gfx* ingfx, bool indirection, float inx, float iny, int intype);
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    int type;
    float x;
    float y;
    bool direction;
    int time;

    private:
    Gfx* gfx;
    sf::Sprite magicsprite;
    float angle;
    int currentframe;
};

Magic::Magic(Gfx* ingfx, bool indirection, float inx, float iny, int intype)
{
    gfx = ingfx;

    direction = indirection;
    if (direction == DIRECTION_RIGHT)
        x = inx + 64;
    else
        x = inx - 64;

    y = iny - 64;

    angle = 0;
    time = 0;

    type = intype;
    magicsprite.SetImage(gfx->magicimages[type]);

    currentframe = 0;
}

void Magic::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (direction == DIRECTION_RIGHT)
            x += 16 * distance;
        else
            x -= 16 * distance;

        angle++;
        y += 16 * distance * sin(angle);

        time++;
    }
}

void Magic::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        magicsprite.FlipX(true);
    else
        magicsprite.FlipX(false);

    magicsprite.SetX(x);
    magicsprite.SetY(y);
    app->Draw(magicsprite);
}
