class Emote
{
    public:
    Emote(int inemote, int inx, int iny);
    void animate();
    void draw(sf::RenderWindow* app);
    int x;
    int y;
    float angle;
    int time;

    private:
    sf::Image emoteimage;
    sf::Sprite emotesprite;
    bool zoom;
    float size;
    float maxsize;
    int currentframe;
    int maxframe;
};

Emote::Emote(int inemote, int inx, int iny)
{
    x = inx;
    y = iny - 64;

    zoom = true;
    size = 1.0f;
    maxsize = sf::Randomizer::Random(1.2f, 1.4f);;

    if (inemote == EMOTE_AGGRO)
        emoteimage.LoadFromFile("gfx/emote_aggro.png");
    else if (inemote == EMOTE_FEAR)
        emoteimage.LoadFromFile("gfx/emote_fear.png");
    else if (inemote == EMOTE_DEAD)
        emoteimage.LoadFromFile("gfx/emote_dead.png");

    emoteimage.SetSmooth(false);

    emotesprite.SetImage(emoteimage);
    emotesprite.SetX(x);
    emotesprite.SetY(y);

    time = 0;

    currentframe = 0;
    maxframe = 7;
}

void Emote::animate()
{
    if (currentframe < maxframe)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 0.05;
            if (size >= maxsize)
                zoom = false;
        }
        else
        {
            size -= 0.05;
            if (size <= -maxsize)
                zoom = true;
        }

        time++;
    }
}

void Emote::draw(sf::RenderWindow* app)
{
    emotesprite.SetX(x);
    emotesprite.SetY(y);
    emotesprite.SetScale(size, size);
    app->Draw(emotesprite);
}
