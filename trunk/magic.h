class Magic
{
    public:
    Magic(bool indirection, float inx, float iny, int intype);
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    int type;
    float x;
    float y;
    bool direction;
    int time;

    private:
    sf::Image magicimage;
    sf::Sprite magicsprite;
    float angle;
    int currentframe;
};

Magic::Magic(bool indirection, float inx, float iny, int intype)
{
    direction = indirection;
    if (direction == DIRECTION_RIGHT)
        x = inx + 64;
    else
        x = inx - 64;

    y = iny - 64;

    angle = 0;
    time = 0;

    type = intype;

    if (type == MAGIC_SPELL0)
        magicimage = getimage(GFX_SPELL0);
    else if (type == MAGIC_SPELL1)
        magicimage = getimage(GFX_SPELL1);

    magicsprite.SetImage(magicimage);

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
