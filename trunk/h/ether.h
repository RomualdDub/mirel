class Ether
{
    public:
    Ether(Gfx* ingfx, Xml* inxml, Game* ingame, string intype);
    ~Ether();
    void animate(int distance);
    void draw(sf::RenderWindow* app);
    void keypress(sf::Key::Code inkey);
    void keyrelease(sf::Key::Code inkey);
    string type;
    int time;

    private:
    Gfx* gfx;
    Xml* xml;
    Game* game;
    Hero* hero;
    int etherx;
    int ethery;
    vector<sf::Sprite> ethersprites;
    vector<Score*> scores;
    int currentframe;
};

Ether::Ether(Gfx* ingfx, Xml* inxml, Game* ingame, string intype)
{
    gfx = ingfx;
    xml = inxml;
    game = ingame;
    type = intype;

    time = 0;

    if (type == "hl")
    {
        ethersprites.push_back(sf::Sprite(gfx->etherimages[gfx->ether_h]));
        ethersprites.push_back(sf::Sprite(gfx->etherimages[gfx->ether_h]));
        ethersprites[1].SetX(800);
        etherx = 0;
        hero = new Hero(gfx, xml, "ether_hl");
    }
    else if (type == "hr")
    {
        ethersprites.push_back(sf::Sprite(gfx->etherimages[gfx->ether_h]));
        ethersprites.push_back(sf::Sprite(gfx->etherimages[gfx->ether_h]));
        ethersprites[1].SetX(-800);
        etherx = 0;
        hero = new Hero(gfx, xml, "ether_hr");
    }
    else if (type == "v")
    {
        ethersprites.push_back(sf::Sprite(gfx->etherimages[gfx->ether_v]));
        ethersprites.push_back(sf::Sprite(gfx->etherimages[gfx->ether_v]));
        ethersprites[1].SetY(472);
        ethery = 0;
        hero = new Hero(gfx, xml, "ether_v");

        for (int i = 0; i < 77; i++)
        {
            scores.push_back(new Score(gfx, xml, "money", sf::Randomizer::Random(xml->infolevel * -4, xml->infolevel * -8), sf::Randomizer::Random(0, 800), sf::Randomizer::Random(0, 472)));
        }
    }

    currentframe = 0;
}

Ether::~Ether()
{
    delete hero;
    ethersprites.clear();
    scores.clear();
}

void Ether::animate(int distance)
{
    if (type == "v")
    {
        if (hero->eleft)
        {
            hero->xe -= distance;
            if (hero->xe < 0)
                hero->xe = 0;
        }
        if (hero->eright)
        {
            hero->xe += distance;
            if (hero->xe > 800 - 64)
                hero->xe = 800 - 64;
        }

        // score creation deletion

        for (int i = 0; i < (int)scores.size(); i++)
        {
            if (scores[i]->y <= -64)
            {
                delete scores[i];
                scores.erase(scores.begin() + i);
                scores.push_back(new Score(gfx, xml, "money", sf::Randomizer::Random(xml->infolevel * -4, xml->infolevel * -8), sf::Randomizer::Random(0, 800), 472 + 128));
            }
        }

        // hero collide scores

        for (int i = 0; i < (int)scores.size(); i++)
        {
            if (scores[i]->collide(hero))
            {
                int money = scores[i]->val;
                game->setmoney(money);
                delete scores[i];
                scores.erase(scores.begin() + i);
            }
        }
    }

    if (currentframe < MAX_FRAME)
        currentframe++;
    else
    {
        currentframe = 0;
        if (type == "v")
        {
            if (ethery >= 472)
                ethery = 0;
            else
                ethery += distance * 4;

            for (int i = 0; i < (int)scores.size(); i++)
                scores[i]->y -= distance * 4;
        }
        else
        {
            if (etherx >= 800)
                etherx = 0;
            else
                etherx += distance * 8;
        }

        time++;

        if (type == "v")
            hero->ye += 5 * sin(time);
        else
            hero->xe += 5 * sin(time);
    }
}

void Ether::draw(sf::RenderWindow* app)
{
    for (int i = 0; i < (int)ethersprites.size(); i++)
    {
        if (type == "hl")
            ethersprites[i].SetX(i * 800 - etherx);
        else if (type == "hr")
            ethersprites[i].SetX(i * -800 + etherx);
        else if (type == "v")
            ethersprites[i].SetY(i * 472 - ethery);
        app->Draw(ethersprites[i]);
    }

    if (type == "v")
    {
        for (int i = 0; i < (int)scores.size(); i++)
            scores[i]->draw(app);
    }

    hero->draw(app);
}

void Ether::keypress(sf::Key::Code inkey)
{
    if (type == "v")
    {
        if (inkey == xml->keyleft)
        {
            hero->eleft = true;
            hero->eright = false;
        }
        if (inkey == xml->keyright)
        {
            hero->eright = true;
            hero->eleft = false;
        }
    }
}

void Ether::keyrelease(sf::Key::Code inkey)
{
    if (type == "v")
    {
        if (inkey == xml->keyleft)
            hero->eleft = false;
        if (inkey == xml->keyright)
            hero->eright = false;
    }
}
