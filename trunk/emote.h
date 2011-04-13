class Emote
{
    public:
    Emote(int inemote, int inx, int iny);
    void animate(int distance);
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
};

Emote::Emote(int inemote, int inx, int iny)
{
    x = inx;
    y = iny - 64;

    zoom = true;
    size = 1.0f;
    maxsize = sf::Randomizer::Random(1.2f, 1.4f);;

    if (inemote == EMOTE_AGGRO)
        emoteimage = getimage(GFX_EMOTEAGGRO);
    else if (inemote == EMOTE_FEAR)
        emoteimage = getimage(GFX_EMOTEFEAR);
    else if (inemote == EMOTE_DEAD)
        emoteimage = getimage(GFX_EMOTEDEAD);

    emoteimage.SetSmooth(false);

    emotesprite.SetImage(emoteimage);

    time = 0;

    currentframe = 0;
}

void Emote::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 0.05 * distance;
            if (size >= maxsize)
                zoom = false;
        }
        else
        {
            size -= 0.05 * distance;
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
