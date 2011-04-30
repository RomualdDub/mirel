class Mob
{
    public:
    Mob(Gfx* ingfx, Snd* insnd, Xml* inxml, Hero* inhero);
    ~Mob();
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    bool collide(Magic* magic);
    bool collide();
    int getmaxhp();
    bool visible;
    bool idle;
    bool aggro;
    bool fear;
    bool hit;
    string name;
    int direction;
    int x;
    int y;
    int hp;
    int level;
    float scalefactor;
    int time;
    vector<sf::Sprite> mobsprites;
    sf::Sound voicesound;
    sf::Sound hitsound;
    sf::Sound repopsound;

    private:
    Gfx* gfx;
    Snd* snd;
    Xml* xml;
    Hero* hero;
    float size;
    float maxsize;
    bool zoom;
    int index;
    int currentframe;
};

Mob::Mob(Gfx* ingfx, Snd* insnd, Xml* inxml, Hero* inhero)
{
    gfx = ingfx;
    snd = insnd;
    xml = inxml;
    hero = inhero;

    name = xml->mobname;

    visible = false;

    idle = true;
    aggro = false;
    fear = false;
    hit = false;

    index = gfx->mob_normal;

    scalefactor = sf::Randomizer::Random(0.7f, 1.0f);
    x = sf::Randomizer::Random(xml->worldmobposx, xml->worldmobposx + 1000);
    y = 236 + 64;

    direction = sf::Randomizer::Random(DIRECTION_LEFT, DIRECTION_RIGHT);

    level = xml->moblevel;
    hp = getmaxhp();

    zoom = true;
    size = 0.0f;
    maxsize = sf::Randomizer::Random(0.05f, 0.1f);

    time = 0;

    // gfx

    for (int i = 0; i < (int)gfx->mobimages.size(); i++)
    {
        mobsprites.push_back(sf::Sprite(gfx->mobimages[i]));
        mobsprites[i].SetScale(scalefactor, scalefactor);
    }

    // snd

    voicesound.SetBuffer(snd->buffermobvoices.front());
    voicesound.SetPitch(2.f - scalefactor);
	voicesound.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
	voicesound.SetAttenuation(17.5f);

    hitsound.SetBuffer(snd->buffermobhit);
    hitsound.SetPitch(2.f);
	hitsound.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
	hitsound.SetAttenuation(17.5f);

    repopsound.SetBuffer(snd->bufferrepop);
    repopsound.SetPitch(2.f);
	repopsound.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
	repopsound.SetAttenuation(17.5f);

    repopsound.SetPosition(x, y, 0.f);
    repopsound.Play();

    currentframe = 0;
}

Mob::~Mob()
{
    mobsprites.clear();
}

void Mob::animate(int distance)
{
    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;

        if (zoom)
        {
            size += 0.01f * distance;
            if (aggro || fear)
            {
                if (size >= maxsize / 4.0f)
                    zoom = false;
            }
            else
            {
                if (size >= maxsize)
                    zoom = false;
            }
        }
        else
        {
            size -= 0.01f * distance;
            if (aggro || fear)
            {
                if (size <= -maxsize / 4.0f)
                    zoom = true;
            }
            else
            {
                if (size <= -maxsize)
                    zoom = true;
            }
        }

        if (idle)
        {
            if (!sf::Randomizer::Random(0, 20))
            {
                if (direction == DIRECTION_RIGHT)
                    direction = DIRECTION_LEFT;
                else
                    direction = DIRECTION_RIGHT;
                if (visible)
                {
                    voicesound.SetPosition(x, y, 0.f);
                    voicesound.Play();
                }
            }
        }
        else if (aggro)
        {
            if (x <= xml->infoherox)
                direction = DIRECTION_RIGHT;
            else
                direction = DIRECTION_LEFT;
        }
        else if (fear)
        {
            if (x <= xml->infoherox)
                direction = DIRECTION_LEFT;
            else
                direction = DIRECTION_RIGHT;
        }

        if (hit)
            index = gfx->mob_hit;
        else
            index = gfx->mob_normal;

        if (direction == DIRECTION_RIGHT)
            x += distance;
        else
            x -= distance;

        if (hit)
        {
            if (!time)
            {
                hitsound.SetPosition(x, y, 0.f);
                hitsound.Play();
            }

            if (x <= xml->infoherox && x > 0)
            {
                if (aggro)
                    x -= distance;
                else if (fear)
                    x -= 2 * distance;
            }
            else if (x > xml->infoherox && x < 2400 - (64 * scalefactor))
            {
                if (aggro)
                    x += distance;
                else if (fear)
                    x += 2 * distance;
            }

            time++;
        }

        if (x >= 2400 - (64 * scalefactor))
            x = 2400 - (64 * scalefactor);
        else if (x <= 0)
            x = 0;
    }
}

void Mob::draw(sf::RenderWindow* app)
{
    if (direction == DIRECTION_LEFT)
        mobsprites[index].FlipX(true);
    else
        mobsprites[index].FlipX(false);

    if (aggro)
        mobsprites[index].SetColor(sf::Color::Red);
    else if (fear)
        mobsprites[index].SetColor(sf::Color::Cyan);
    if (hit)
        mobsprites[index].SetColor(sf::Color::White);

    mobsprites[index].SetCenter(0, 64 * scalefactor - 64);
    mobsprites[index].SetX(x);
    mobsprites[index].SetY(y);
    mobsprites[index].SetScale(scalefactor + size, scalefactor + size);
    app->Draw(mobsprites[index]);
}

bool Mob::collide(Magic* magic)
{
    if (magic->x + 64 >= x && magic->x <= x + (64 * scalefactor) && magic->y + 32 >= y && magic->y <= y + (64 * scalefactor))
        return true;
    else
        return false;
}

bool Mob::collide()
{
    if (xml->infoherox + 64 >= x && xml->infoherox <= x + (64 * scalefactor) && hero->y + 128 >= y && hero->y <= y + (64 * scalefactor))
        return true;
    else
        return false;
}

int Mob::getmaxhp()
{
    return level * 250;
}
