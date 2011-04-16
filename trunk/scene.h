class Scene
{
    public:
    Scene(int inherox, int inpetx, int inlevel, Game* ingame, Map* inmap);
    ~Scene();
    void animate(int distance, sf::View* view);
    void draw(sf::RenderWindow* app, sf::View* view);
    void keyleft();
    void keyleftup();
    void keyright();
    void keyrightup();
    void keyup();
    void keyaction();
    void keyactionup();
    void keypetcontrol();
    Hero* hero;
    Pet* pet;
    vector<Boss*> boss;
    vector<Mob*> mobs;
    vector<Loot*> loots;

    #define MAXMOBS 7

    private:
    Game* game;
    Map* map;
    World* world;
    vector<Score*> scores;
    vector<Emote*> emotes;
    vector<Magic*> magics;
    vector<Particle*> particles;
    vector<sf::Image> groundimages;
    int ground[2400];
    vector <sf::Image> occludemaskimages;
    vector <sf::Image> occludesrcimages;
    vector <sf::Image> occludeimages;
    vector <sf::Sprite> occludesprites;
    int bossrepop;
    int mobrepop;
};

Scene::Scene(int inherox, int inpetx, int inlevel, Game* ingame, Map* inmap)
{
    game = ingame;
    map = inmap;

    world = new World;

    hero = new Hero(inherox);
    map->addhero(hero);

    pet = new Pet(inpetx, inlevel);
    map->addpet(pet);

    // mobs

    for (int i = 0; i < MAXMOBS; i++)
    {
        mobs.push_back(new Mob(1, 100, 1000, hero));
        map->addmob(mobs[i]);
    }
    mobrepop = 0;

    // boss

    boss.push_back(new Boss(5, 800, 1000, hero));
    map->addboss(boss[0]);
    bossrepop = 0;

    // collide

    groundimages.push_back(sf::Image(getimage(GFX_WORLDCOLLIDE01)));
    groundimages.push_back(sf::Image(getimage(GFX_WORLDCOLLIDE02)));
    groundimages.push_back(sf::Image(getimage(GFX_WORLDCOLLIDE03)));

    int index;
    int x2;
    sf::Color pixel;
    for (int x = 0; x < 800 * 3; x++)
    {
        index = x / 800;
        x2 = x - (800 * index);
        for (int y = 0; y < 472; y++)
        {
            pixel = groundimages[index].GetPixel(x2, y);
            if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255)
            {
                ground[x] = y;
                break;
            }
        }
    }

    // occlude

    occludemaskimages.push_back(sf::Image(getimage(GFX_WORLDOCCLUDE01)));
    occludemaskimages.push_back(sf::Image(getimage(GFX_WORLDOCCLUDE02)));
    occludemaskimages.push_back(sf::Image(getimage(GFX_WORLDOCCLUDE03)));
    occludesrcimages.push_back(sf::Image(getimage(GFX_WORLD01)));
    occludesrcimages.push_back(sf::Image(getimage(GFX_WORLD02)));
    occludesrcimages.push_back(sf::Image(getimage(GFX_WORLD03)));
    occludeimages.push_back(sf::Image());
    occludeimages.push_back(sf::Image());
    occludeimages.push_back(sf::Image());

    sf::Color color(0, 0, 0, 0);
    occludeimages[0].Create(800, 472, color);
    occludeimages[0].SetSmooth(false);
    occludeimages[1].Create(800, 472, color);
    occludeimages[1].SetSmooth(false);
    occludeimages[2].Create(800, 472, color);
    occludeimages[2].SetSmooth(false);

    for (int x = 0; x < 800 * 3; x++)
    {
        index = x / 800;
        x2 = x - (800 * index);

        for (int y = 0; y < 472; y++)
        {
            pixel = occludemaskimages[index].GetPixel(x2, y);
            if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255 && pixel.a)
                occludeimages[index].SetPixel(x2, y, occludesrcimages[index].GetPixel(x2, y));
        }
    }

    occludesprites.push_back(sf::Sprite(occludeimages[0]));
    occludesprites.push_back(sf::Sprite(occludeimages[1]));
    occludesprites.push_back(sf::Sprite(occludeimages[2]));

    occludesprites[1].SetX(800);
    occludesprites[2].SetX(800 * 2);
}

Scene::~Scene()
{
    delete world;
    delete hero;
    delete pet;
    boss.clear();
    mobs.clear();
    scores.clear();
    loots.clear();
    emotes.clear();
    magics.clear();
    particles.clear();
    groundimages.clear();
    occludemaskimages.clear();
    occludesrcimages.clear();
    occludeimages.clear();
    occludesprites.clear();
}

void Scene::animate(int distance, sf::View* view)
{
    // animate

    hero->animate();
    map->updatehero(hero);

    if (pet->x + 64 < view->GetRect().Left || pet->x > view->GetRect().Right)
    {
        pet->visible = false;
        if (pet->x + 64 < view->GetRect().Left)
        {
            map->petleft = true;
            map->petright = false;
        }
        else
        {
            map->petleft = false;
            map->petright = true;
        }
    }
    else
    {
        pet->visible = true;
        pet->animate();

        map->petleft = false;
        map->petright = false;
        map->updatepet(pet);
    }

    world->animate(distance);

    if (boss.size())
    {
        if (boss[0]->x + 128 < view->GetRect().Left || boss[0]->x > view->GetRect().Right)
            boss[0]->visible = false;
        else
            boss[0]->visible = true;
        boss[0]->animate(distance);
        map->updateboss(boss[0]);
    }

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->x + (64 * mobs[i]->scalefactor) < view->GetRect().Left || mobs[i]->x > view->GetRect().Right)
            mobs[i]->visible = false;
        else
            mobs[i]->visible = true;
        mobs[i]->animate(distance);
        map->updatemob(i, mobs[i]);
    }

    for (int i = 0; i < (int)loots.size(); i++)
    {
        if (loots[i]->x + 32 < view->GetRect().Left || loots[i]->x > view->GetRect().Right)
            loots[i]->visible = false;
        else
        {
            loots[i]->visible = true;
            loots[i]->animate(distance);
            map->updateloot(i, loots[i]);
        }
    }

    for (int i = 0; i < (int)scores.size(); i++)
        scores[i]->animate(distance);

    for (int i = 0; i < (int)emotes.size(); i++)
        emotes[i]->animate(distance);

    for (int i = 0; i < (int)magics.size(); i++)
        magics[i]->animate(distance);

    for (int i = 0; i < (int)particles.size(); i++)
        particles[i]->animate(distance);

    // ground (gravity)

    if (hero->y < ground[hero->x] - 128 + 8)
        hero->y += distance;
    if (hero->y > ground[hero->x] - 128 + 8)
        hero->y -= distance;

    if (pet->y < ground[pet->x] - 64 + 8)
        pet->y += distance;
    if (pet->y > ground[pet->x] - 64 + 8)
        pet->y -= distance;

    if (boss.size())
    {
        if (boss[0]->y < ground[boss[0]->x] - 128 + 8)
            boss[0]->y += distance;
        if (boss[0]->y > ground[boss[0]->x] - 128 + 8)
            boss[0]->y -= distance;
    }

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->y < ground[mobs[i]->x] - 64 + 8)
            mobs[i]->y += distance;
        if (mobs[i]->y > ground[mobs[i]->x] - 64 + 8)
            mobs[i]->y -= distance;
    }

    // hero left

    if (hero->left)
    {
        hero->x -= distance;
        if (hero->x < 0)
            hero->x = 0;
    }

    // hero right

    if (hero->right)
    {
        hero->x += distance;
        if (hero->x > 2400 - 64)
            hero->x = 2400 - 64;
    }

    // hero jump

    if (hero->jump)
    {
        hero->jumpangle += 0.1 * distance;
        hero->y -= 7 * distance * sin(hero->jumpangle);

        if (hero->y >= ground[(int)hero->x] - 128 + 8)
        {
            hero->y = ground[(int)hero->x] - 128 + 8;
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
            pet->x -= distance;
        }
        else
            pet->left = false;

        if (pet->x < hero->x - 64)
        {
            pet->right = true;
            pet->x += distance;
        }
        else
            pet->right = false;
    }

    // magic collide mob

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->visible)
        {
            for (int j = 0; j < (int)magics.size(); j++)
            {
                if (magics[j]->type == MAGIC_SPELL0)
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
                            game->setexp(mobs[i]->level, pet);

                            for (int k = 0; k < 14; k++)
                                particles.push_back(new Particle(mobs[i]->x, mobs[i]->y));

                            Loot* loot = new Loot(mobs[i]->level, mobs[i]->x, mobs[i]->y);
                            loots.push_back(loot);
                            map->addloot(loot);

                            emotes.push_back(new Emote(EMOTE_DEAD, mobs[i]->x, mobs[i]->y));

                            delete mobs[i];
                            mobs.erase(mobs.begin() + i);
                            map->deletemob(i);
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
        }
    }

    // pet collide mob

    if (pet->attack)
    {
        for (int i = 0; i < (int)mobs.size(); i++)
        {
            if (pet->collide(mobs[i]) && !mobs[i]->hit && pet->hp)
            {
                int dmg = game->getoutdamage(); // damage out
                mobs[i]->hp -= dmg / 2;

                mobs[i]->hit = true;

                if (pet->visible)
                    scores.push_back(new Score(SCORE_DAMAGE, dmg, mobs[i]->x, mobs[i]->y));

                if (mobs[i]->aggro)
                {
                    pet->badhit = true; // damage in
                    int life = -game->getindamage(mobs[i]);
                    pet->hp += life;

                    if (pet->hp <= 0)
                        pet->hp = 0;

                    if (pet->visible)
                    {
                        scores.push_back(new Score(SCORE_LIFE, life, pet->x, pet->y));
                        for (int j = 0; j < 7; j++)
                            particles.push_back(new Particle(pet->x, pet->y));
                    }
                }

                if (mobs[i]->hp <= 0) // mob dead
                {
                    game->setexp(mobs[i]->level, pet);

                    if (pet->visible)
                        for (int k = 0; k < 14; k++)
                            particles.push_back(new Particle(mobs[i]->x, mobs[i]->y));

                    Loot* loot = new Loot(mobs[i]->level, mobs[i]->x, mobs[i]->y);
                    loots.push_back(loot);
                    map->addloot(loot);

                    if (pet->visible)
                        emotes.push_back(new Emote(EMOTE_DEAD, mobs[i]->x, mobs[i]->y));

                    delete mobs[i];
                    mobs.erase(mobs.begin() + i);
                    map->deletemob(i);
                }
                else // mob emote
                {
                    if (mobs[i]->idle)
                    {
                        int emote = rand() % 2;
                        if (emote == EMOTE_AGGRO)
                        {
                            if (pet->visible)
                                emotes.push_back(new Emote(EMOTE_AGGRO, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->aggro = true;
                        }
                        else if (emote == EMOTE_FEAR)
                        {
                            if (pet->visible)
                                emotes.push_back(new Emote(EMOTE_FEAR, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->fear = true;
                        }
                    }
                }
            }
        }
    }

    // magic collide boss

    if (boss.size())
    {
        if (boss[0]->visible)
        {
            for (int i = 0; i < (int)magics.size(); i++)
            {
                if (magics[i]->type == MAGIC_SPELL0)
                {
                    if (boss[0]->collide(magics[i]))
                    {
                        int dmg = game->getoutdamage(); // damage
                        boss[0]->hp -= dmg;

                        magics.erase(magics.begin() + i);

                        boss[0]->hit = true;

                        scores.push_back(new Score(SCORE_DAMAGE, dmg, boss[0]->x, boss[0]->y));

                        if (boss[0]->hp <= 0) // boss dead
                        {
                            game->setexp(boss[0]->level, pet);

                            for (int j = 0; j < 14; j++)
                                particles.push_back(new Particle(boss[0]->x, boss[0]->y));

                            Loot* loot = new Loot(boss[0]->level, boss[0]->x, boss[0]->y + 64);
                            loots.push_back(loot);
                            map->addloot(loot);

                            emotes.push_back(new Emote(EMOTE_DEAD, boss[0]->x, boss[0]->y));

                            boss.clear();
                            map->deleteboss();
                        }
                        else // boss emote
                        {
                            if (boss[0]->idle)
                            {
                                emotes.push_back(new Emote(EMOTE_AGGRO, boss[0]->x, boss[0]->y));

                                boss[0]->idle = false;
                                boss[0]->aggro = true;
                            }
                        }
                    }
                }
            }
        }
    }

    // pet collide boss

    if (pet->attack && boss.size())
    {
        if (pet->collide(boss[0]) && !boss[0]->hit && pet->hp)
        {
            int dmg = game->getoutdamage(); // damage out
            boss[0]->hp -= dmg / 2;

            boss[0]->hit = true;

            if (pet->visible)
                scores.push_back(new Score(SCORE_DAMAGE, dmg, boss[0]->x, boss[0]->y));

            pet->badhit = true; // damage in
            int life = -game->getindamage(boss[0]);
            pet->hp += life;

            if (pet->hp <= 0)
                pet->hp = 0;

            if (pet->visible)
            {
                scores.push_back(new Score(SCORE_LIFE, life, pet->x, pet->y));
                for (int j = 0; j < 7; j++)
                    particles.push_back(new Particle(pet->x, pet->y));
            }

            if (boss[0]->hp <= 0) // mob dead
            {
                game->setexp(boss[0]->level, pet);

                if (pet->visible)
                    for (int i = 0; i < 14; i++)
                        particles.push_back(new Particle(boss[0]->x, boss[0]->y));

                Loot* loot = new Loot(boss[0]->level, boss[0]->x, boss[0]->y + 64);
                loots.push_back(loot);
                map->addloot(loot);

                if (pet->visible)
                    emotes.push_back(new Emote(EMOTE_DEAD, boss[0]->x, boss[0]->y));

                boss.clear();
                map->deleteboss();
            }
            else // mob emote
            {
                if (boss[0]->idle)
                {
                    if (pet->visible)
                        emotes.push_back(new Emote(EMOTE_AGGRO, boss[0]->x, boss[0]->y));

                    boss[0]->idle = false;
                    boss[0]->aggro = true;
                }
            }
        }
    }

    // magic collide pet

    if (pet->visible)
    {
        for (int i = 0; i < (int)magics.size(); i++)
        {
            if (magics[i]->type == MAGIC_SPELL1)
            {
                if (pet->collide(magics[i]))
                {
                    magics.erase(magics.begin() + i);
                    if (!pet->goodhit)
                    {
                        pet->goodhit = true;

                        int life = game->level * sf::Randomizer::Random(250, 500);
                        scores.push_back(new Score(SCORE_LIFE, life, pet->x, pet->y));
                        pet->hp += life;
                        if (pet->hp > game->level * 2000)
                            pet->hp = game->level * 2000;
                    }
                }
            }
        }
    }

    // hero collide mob

    if (!hero->hit)
    {
        for (int i = 0; i < (int)mobs.size(); i++)
        {
            if (mobs[i]->visible)
            {
                if (mobs[i]->collide() && mobs[i]->aggro)
                {
                    hero->hit = true;
                    int life = -game->getindamage(mobs[i]);
                    scores.push_back(new Score(SCORE_LIFE, life, hero->x, hero->y));
                    game->setlife(life);

                    for (int j = 0; j < 7; j++)
                        particles.push_back(new Particle(hero->x, hero->y));
                }
            }
        }
    }

    // hero collide boss

    if (!hero->hit && boss.size())
    {
        if (boss[0]->collide() && boss[0]->aggro)
        {
            hero->hit = true;
            int life = -game->getindamage(boss[0]);
            scores.push_back(new Score(SCORE_LIFE, life, hero->x, hero->y));
            game->setlife(life);

            for (int i = 0; i < 7; i++)
                particles.push_back(new Particle(hero->x, hero->y));
        }
    }

    // hero collide loot

    for (int i = 0; i < (int)loots.size(); i++)
        if (loots[i]->visible)
        {
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
                map->deleteloot(i);
            }
        }

    // hero hit end

    if (hero->time >= 16)
    {
        hero->hit = false;
        hero->time = 0;
    }

    // pet hit end

    if (pet->badtime >= 8)
    {
        pet->badhit = false;
        pet->badtime = 0;
    }
    if (pet->goodtime >= 8)
    {
        pet->goodhit = false;
        pet->goodtime = 0;
    }

    // mobs hit end

    for (int i = 0; i < (int)mobs.size(); i++)
        if (mobs[i]->time >= 8)
        {
            mobs[i]->hit = false;
            mobs[i]->time = 0;
        }

    // boss hit end

    if (boss.size())
    {
        if (boss[0]->time >= 8)
        {
            boss[0]->hit = false;
            boss[0]->time = 0;
        }
    }

    // mobs repop

    if ((int)mobs.size() != MAXMOBS)
    {
        if (mobrepop < 77)
            mobrepop++;
        else
        {
            mobrepop = 0;
            Mob* mob = new Mob(1, 100, 1000, hero);
            mobs.push_back(mob);
            map->addmob(mob);
        }
    }

    // boss repop

    if (!boss.size())
    {
        if (bossrepop < 7777)
            bossrepop++;
        else
        {
            bossrepop = 0;
            boss.push_back(new Boss(5, 800, 1000, hero));
            map->addboss(boss[0]);
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

    if (boss.size())
        if (boss[0]->visible)
            boss[0]->draw(app);

    for (int i = 0; i < (int)mobs.size(); i++)
        if (mobs[i]->visible)
            mobs[i]->draw(app);

    for (int i = 0; i < (int)scores.size(); i++)
        scores[i]->draw(app);

    for (int i = 0; i < (int)loots.size(); i++)
        if (loots[i]->visible)
            loots[i]->draw(app);

    hero->draw(app);

    if (pet->visible)
        pet->draw(app);

    for (int i = 0; i < (int)magics.size(); i++)
        magics[i]->draw(app);

    for (int i = 0; i < (int)occludesprites.size(); i++)
        app->Draw(occludesprites[i]);

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

void Scene::keyaction()
{
    if (!hero->attack)
    {
        int manacost;
        if (game->spellindex == MAGIC_SPELL0)
            manacost = 5;
        else if (game->spellindex == MAGIC_SPELL1)
            manacost = 3;

        if (game->mana >= game->level * manacost)
        {
            game->setmana(game->level * -manacost);
            if (game->mana > 0)
            {
                magics.push_back(new Magic(hero->direction, hero->x, hero->y + 128, game->spellindex));

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
