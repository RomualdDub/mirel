class Scene
{
    public:
    Scene(Map* map);
    ~Scene();
    void animate(Game* game, Map* map);
    void draw(sf::RenderWindow* app, sf::View* view);
    void keyleft();
    void keyleftup();
    void keyright();
    void keyrightup();
    void keyup();
    void keyaction(Game* game);
    void keyactionup();
    void keypetcontrol();
    Hero* hero;
    Pet* pet;
    vector<Mob*> mobs;
    vector<Loot*> loots;

    #define MAXMOBS 7
    #define YGROUND 8

    private:
    World* world;
    vector<Score*> scores;
    vector<Emote*> emotes;
    vector<Magic*> magics;
    vector<Particle*> particles;
    sf::Image groundimage;
    int ground[2400];
    sf::Image occludemask, occludesrc, occludedest;
    sf::Sprite occludesprite;
    int mobrepop;
};

Scene::Scene(Map* map)
{
    world = new World;

    hero = new Hero;
    map->herolabel = new Label(hero->x, LABEL_HERO, NULL, NULL, NULL, NULL);

    pet = new Pet;
    map->petlabel = new Label(pet->x, LABEL_PET, pet->level, pet->hp, &pet->petsprite, pet->name);

    // mobs

    for (int i = 0; i < MAXMOBS; i++)
    {
        mobs.push_back(new Mob(1, 100, 1000));
        map->moblabels.push_back(new Label(mobs[i]->x, LABEL_MOB, mobs[i]->level, mobs[i]->hp, &mobs[i]->mobsprite, mobs[i]->name));
    }
    mobrepop = 0;

    // ground

    groundimage.LoadFromFile("gfx/world00.png");

    sf::Color pixel;
    for (int x = 0; x < 2400; x++)
    {
        for (int y = 0; y < 472; y++)
        {
            pixel = groundimage.GetPixel(x, y);
            if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255)
            {
                ground[x] = y;
                break;
            }
        }
    }

    // occlude

    occludemask.LoadFromFile("gfx/world000.png");
    occludesrc.LoadFromFile("gfx/world0.png");

    sf::Color color(0, 0, 0, 0);
    occludedest.Create(2400, 472, color);
    occludedest.SetSmooth(false);

    for (int x = 0; x < 2400; x++)
    {
        for (int y = 0; y < 472; y++)
        {
            pixel = occludemask.GetPixel(x, y);
            if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255 && pixel.a)
                occludedest.SetPixel(x, y, occludesrc.GetPixel(x, y));
        }
    }

    occludesprite.SetImage(occludedest);
}

Scene::~Scene()
{
    delete world;
    delete hero;
    delete pet;
    mobs.clear();
    scores.clear();
    loots.clear();
    emotes.clear();
    magics.clear();
    particles.clear();
}

void Scene::animate(Game* game, Map* map)
{
    // animate

    hero->animate();
    map->herolabel->x = hero->x;

    pet->animate();
    map->petlabel->x = pet->x;
    map->petlabel->hp = pet->hp;

    world->animate();

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        mobs[i]->animate(hero);
        map->moblabels[i]->x = mobs[i]->x;
        map->moblabels[i]->hp = mobs[i]->hp;
    }

    for (int i = 0; i < (int)loots.size(); i++)
    {
        loots[i]->animate();
        map->lootlabels[i]->x = loots[i]->x;
    }

    for (int i = 0; i < (int)scores.size(); i++)
        scores[i]->animate();

    for (int i = 0; i < (int)emotes.size(); i++)
        emotes[i]->animate();

    for (int i = 0; i < (int)magics.size(); i++)
        magics[i]->animate();

    for (int i = 0; i < (int)particles.size(); i++)
        particles[i]->animate();

    // ground (gravity)

    if (hero->y < ground[(int)hero->x] + YGROUND)
        hero->y++;
    if (hero->y > ground[(int)hero->x] + YGROUND)
        hero->y--;

    if (pet->y < ground[(int)pet->x] + YGROUND / 2)
        pet->y++;
    if (pet->y > ground[(int)pet->x] + YGROUND / 2)
        pet->y--;

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->y < ground[(int)(mobs[i]->x)])
            mobs[i]->y++;
        if (mobs[i]->y > ground[(int)(mobs[i]->x)])
            mobs[i]->y--;
    }

    // hero left

    if (hero->left)
    {
        hero->x--;
        if (hero->x < 0)
            hero->x = 0;
    }

    // hero right

    if (hero->right)
    {
        hero->x++;
        if (hero->x > 2400 - 64)
            hero->x = 2400 - 64;
    }

    // hero jump

    if (hero->jump)
    {
        hero->jumpangle += 0.1;
        hero->y -= 7 * sin(hero->jumpangle);

        if (hero->y >= ground[(int)hero->x] + YGROUND)
        {
            hero->y = ground[(int)hero->x] + YGROUND;
            hero->jumpangle = 0;
            hero->jump = false;
        }
    }

    // hero regen

    if (!hero->attack && !hero->hit)
    {
        if (game->life < game->maxlife)
            game->setlife(game->maxlife / 7777);
        if (game->mana < game->maxmana)
            game->setmana(game->maxmana / 777);
    }

    // pet follow

    if (!pet->attack)
    {
        if (pet->x > hero->x + 64)
        {
            pet->left = true;
            pet->x--;
        }
        else
            pet->left = false;

        if (pet->x < hero->x - 64)
        {
            pet->right = true;
            pet->x++;
        }
        else
            pet->right = false;
    }

    // magic collide mob

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        for (int j = 0; j < (int)magics.size(); j++)
        {
            if (mobs[i]->collide(magics[j]))
            {
                int dmg = game->getoutdamage(); // damage
                mobs[i]->hp -= dmg;

                magics.erase(magics.begin() + j);

                mobs[i]->hit = true;

                scores.push_back(new Score(SCORE_DAMAGE, dmg, mobs[i]->x, mobs[i]->y));

                if (mobs[i]->hp <= 0) // mob dead
                {
                    game->setexp(mobs[i]->level);

                    for (int k = 0; k < 14; k++)
                        particles.push_back(new Particle(mobs[i]->x, mobs[i]->y));

                    Loot* loot = new Loot(mobs[i]->level, mobs[i]->x, mobs[i]->y);
                    loots.push_back(loot);
                    map->lootlabels.push_back(new Label(loot->x, LABEL_LOOT, NULL, NULL, &loot->lootsprite, NULL));

                    emotes.push_back(new Emote(EMOTE_DEAD, mobs[i]->x, mobs[i]->y));

                    delete mobs[i];
                    mobs.erase(mobs.begin() + i);
                    delete map->moblabels[i];
                    map->moblabels.erase(map->moblabels.begin() + i);
                }
                else // mob emote
                {
                    if (mobs[i]->idle)
                    {
                        int emote = rand() % 2;
                        if (emote == EMOTE_AGGRO)
                        {
                            emotes.push_back(new Emote(EMOTE_AGGRO, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->aggro = true;
                        }
                        else if (emote == EMOTE_FEAR)
                        {
                            emotes.push_back(new Emote(EMOTE_FEAR, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->fear = true;
                        }
                    }
                }
            }
        }
    }

    // pet collide mob

    if (pet->attack)
    {
        for (int i = 0; i < (int)mobs.size(); i++)
        {
            if (pet->collide(mobs[i]) && !mobs[i]->hit)
            {
                int dmg = game->getoutdamage(); // damage
                mobs[i]->hp -= dmg / 2;

                mobs[i]->hit = true;

                scores.push_back(new Score(SCORE_DAMAGE, dmg, mobs[i]->x, mobs[i]->y));

                if (mobs[i]->hp <= 0) // mob dead
                {
                    game->setexp(mobs[i]->level);

                    for (int k = 0; k < 14; k++)
                        particles.push_back(new Particle(mobs[i]->x, mobs[i]->y));

                    Loot* loot = new Loot(mobs[i]->level, mobs[i]->x, mobs[i]->y);
                    loots.push_back(loot);
                    map->lootlabels.push_back(new Label(loot->x, LABEL_LOOT, NULL, NULL, &loot->lootsprite, NULL));

                    emotes.push_back(new Emote(EMOTE_DEAD, mobs[i]->x, mobs[i]->y));

                    delete mobs[i];
                    mobs.erase(mobs.begin() + i);
                    delete map->moblabels[i];
                    map->moblabels.erase(map->moblabels.begin() + i);
                }
                else // mob emote
                {
                    if (mobs[i]->idle)
                    {
                        int emote = rand() % 2;
                        if (emote == EMOTE_AGGRO)
                        {
                            emotes.push_back(new Emote(EMOTE_AGGRO, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->aggro = true;
                        }
                        else if (emote == EMOTE_FEAR)
                        {
                            emotes.push_back(new Emote(EMOTE_FEAR, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->fear = true;
                        }
                    }
                }
            }
        }
    }

    // hero collide mob

    if (!hero->hit)
    {
        for (int i = 0; i < (int)mobs.size(); i++)
            if (mobs[i]->collide(hero) && mobs[i]->aggro)
            {
                hero->hit = true;
                int life = -game->getindamage(mobs[i]);
                scores.push_back(new Score(SCORE_LIFE, life, hero->x, hero->y));
                game->setlife(life);

                for (int j = 0; j < 7; j++)
                    particles.push_back(new Particle(hero->x, hero->y));
            }
    }

    // hero collide loot

    for (int i = 0; i < (int)loots.size(); i++)
        if (loots[i]->collide(hero))
        {
            if (loots[i]->type == LOOT_MONEY)
            {
                int money = game->getmoney(loots[i]);
                scores.push_back(new Score(SCORE_MONEY, money, loots[i]->x, hero->y));
                game->setmoney(money);
            }
            else if (loots[i]->type == LOOT_POTIONM)
            {
                int mana = game->getpotionm(loots[i]);
                scores.push_back(new Score(SCORE_MANA, mana, loots[i]->x, hero->y));
                game->setmana(mana);
            }
            else if (loots[i]->type == LOOT_POTIONL)
            {
                int life = game->getpotionl(loots[i]);
                scores.push_back(new Score(SCORE_LIFE, life, loots[i]->x, hero->y));
                game->setlife(life);
            }

            delete loots[i];
            loots.erase(loots.begin() + i);
            delete map->lootlabels[i];
            map->lootlabels.erase(map->lootlabels.begin() + i);
        }

    // hero hit end

    if (hero->time >= 16)
    {
        hero->hit = false;
        hero->time = 0;
    }

    // mobs hit end

    for (int i = 0; i < (int)mobs.size(); i++)
        if (mobs[i]->time >= 8)
        {
            mobs[i]->hit = false;
            mobs[i]->time = 0;
        }

    // mobs repop

    if ((int)mobs.size() != MAXMOBS)
    {
        if (mobrepop < 7)
            mobrepop++;
        else
        {
            mobrepop = 0;
            Mob* mob = new Mob(1, 100, 1000);
            mobs.push_back(mob);
            map->moblabels.push_back(new Label(mob->x, LABEL_MOB, mob->level, mob->hp, &mob->mobsprite, mob->name));
        }
    }

    // vectors end

    for (int i = 0; i < (int)magics.size(); i++)
        if (magics[i]->time >= 16)
        {
            delete magics[i];
            magics.erase(magics.begin() + i);
        }

    for (int i = 0; i < (int)scores.size(); i++)
        if (scores[i]->y <= scores[i]->basey - 16 * scores[i]->angle)
        {
            delete scores[i];
            scores.erase(scores.begin() + i);
        }

    for (int i = 0; i < (int)emotes.size(); i++)
        if (emotes[i]->time == 16)
        {
            delete emotes[i];
            emotes.erase(emotes.begin() + i);
        }

    for (int i = 0; i < (int)particles.size(); i++)
        if (particles[i]->time == 16)
        {
            delete particles[i];
            particles.erase(particles.begin() + i);
        }
}

void Scene::draw(sf::RenderWindow* app, sf::View* view)
{
    if (hero->x <= 400)
        view->SetFromRect(sf::FloatRect(0, 0, 800, 600));
    else if (hero->x > 400 && hero->x <= 2400 - 400)
        view->SetFromRect(sf::FloatRect(hero->x - 400, 0, hero->x + 400, 600));
    else if (hero->x > 2400 - 400)
        view->SetFromRect(sf::FloatRect(2400 - 800, 0, 2400, 600));

    world->draw(app);

    for (int i = 0; i < (int)particles.size(); i++)
        particles[i]->draw(app);

    for (int i = 0; i < (int)mobs.size(); i++)
        mobs[i]->draw(app);

    for (int i = 0; i < (int)scores.size(); i++)
        scores[i]->draw(app);

    for (int i = 0; i < (int)loots.size(); i++)
        loots[i]->draw(app);

    hero->draw(app);
    pet->draw(app);

    for (int i = 0; i < (int)magics.size(); i++)
        magics[i]->draw(app);

    app->Draw(occludesprite);

    for (int i = 0; i < (int)emotes.size(); i++)
        emotes[i]->draw(app);
}

void Scene::keyleft()
{
    hero->left = true;
    hero->right = false;
}

void Scene::keyleftup()
{
    hero->left = false;
}

void Scene::keyright()
{
    hero->right = true;
    hero->left = false;
}

void Scene::keyrightup()
{
    hero->right = false;
}

void Scene::keyup()
{
    if (!hero->attack)
        hero->jump = true;
}

void Scene::keyaction(Game* game)
{
    if (!hero->attack)
    {
        if (game->mana >= game->level * 10)
        {
            game->setmana(game->level * -10);
            if (game->mana > 0)
            {
                magics.push_back(new Magic(hero->direction, hero->x, hero->y));

                hero->attack = true;
            }
        }
    }
}

void Scene::keyactionup()
{
    hero->attack = false;
}

void Scene::keypetcontrol()
{
    if (pet->attack)
        pet->attack = false;
    else
    {
        pet->attack = true;
        pet->left = false;
        pet->right = false;
    }
}
