class Scene
{
    public:
    Scene(Gfx* ingfx, Bobo* inbobo, Xml* inxml, Game* ingame, Map* inmap);
    ~Scene();
    void animate(int distance, sf::View* view);
    void draw(sf::RenderWindow* app, sf::View* view);
    void keypress(sf::Key::Code inkey);
    void keyrelease(sf::Key::Code inkey);
    void keyleft();
    void keyleftup();
    void keyright();
    void keyrightup();
    void keyup();
    void keyaction();
    void keyactionup();
    void keypetcontrol();
    void generateloot(int inlevel, int inx, int iny);
    Hero* hero;
    Pet* pet;
    vector<Boss*> boss;
    vector<Mob*> mobs;
    vector<Loot*> loots;

    #define MAXMOBS 7

    private:
    Gfx* gfx;
    Bobo* bobo;
    Xml* xml;
    Game* game;
    Map* map;
    World* world;
    vector<Score*> scores;
    vector<Emote*> emotes;
    vector<Magic*> magics;
    vector<Particle*> particles;
    int bossrepop;
    int mobrepop;
    int currenthat;
    int currentdress;
    int currentwand;
};

Scene::Scene(Gfx* ingfx, Bobo* inbobo, Xml* inxml, Game* ingame, Map* inmap)
{
    gfx = ingfx;
    bobo = inbobo;
    xml = inxml;
    game = ingame;
    map = inmap;

    world = new World(gfx, xml);

    hero = new Hero(gfx, xml);
    map->addhero(hero);
    currenthat = xml->infohatindex;
    currentdress = xml->infodressindex;
    currentwand = xml->infowandindex;

    pet = new Pet(gfx, xml);
    map->addpet(pet);

    // mobs

    for (int i = 0; i < MAXMOBS; i++)
    {
        mobs.push_back(new Mob(gfx, xml, hero));
        map->addmob(mobs[i]);
    }
    mobrepop = 0;

    // boss

    boss.push_back(new Boss(gfx, xml, hero));
    map->addboss(boss[0]);
    bossrepop = 0;
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
}

void Scene::animate(int distance, sf::View* view)
{
    // stuff handling

    if (currenthat != xml->infohatindex || currentdress != xml->infodressindex || currentwand != xml->infowandindex)
    {
        delete hero;
        hero = new Hero(gfx, xml);
        currenthat = xml->infohatindex;
        currentdress = xml->infodressindex;
        currentwand = xml->infowandindex;
    }

    // animate

    hero->animate();
    map->updatehero();

    if (xml->infopetx + 64 < view->GetRect().Left || xml->infopetx > view->GetRect().Right)
    {
        pet->visible = false;
        if (xml->infopetx + 64 < view->GetRect().Left)
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

    for (int i = 0; i < (int)xml->worldteleports.size(); i++)
        xml->worldteleports[i].animate(distance);

    // ground (gravity)

    if (hero->y < world->ground[xml->infoherox] - 128 + 8)
        hero->y += distance;
    if (hero->y > world->ground[xml->infoherox] - 128 + 8)
        hero->y -= distance;

    if (pet->y < world->ground[xml->infopetx] - 64 + 8)
        pet->y += distance;
    if (pet->y > world->ground[xml->infopetx] - 64 + 8)
        pet->y -= distance;

    if (boss.size())
    {
        if (boss[0]->y < world->ground[boss[0]->x] - 128 + 8)
            boss[0]->y += distance;
        if (boss[0]->y > world->ground[boss[0]->x] - 128 + 8)
            boss[0]->y -= distance;
    }

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->y < world->ground[mobs[i]->x] - 64 + 8)
            mobs[i]->y += distance;
        if (mobs[i]->y > world->ground[mobs[i]->x] - 64 + 8)
            mobs[i]->y -= distance;
    }

    for (int i = 0; i < (int)xml->worldteleports.size(); i++)
        xml->worldteleports[i].y = world->ground[xml->worldteleports[i].x];

    // hero left

    if (hero->left)
    {
        xml->infoherox -= distance;
        if (xml->infoherox < 0)
            xml->infoherox = 0;
    }

    // hero right

    if (hero->right)
    {
        xml->infoherox += distance;
        if (xml->infoherox > 2400 - 64)
            xml->infoherox = 2400 - 64;
    }

    // hero jump

    if (hero->jump)
    {
        hero->jumpangle += 0.1 * distance;
        hero->y -= 7 * distance * sin(hero->jumpangle);

        if (hero->y >= world->ground[xml->infoherox] - 128 + 8)
        {
            hero->y = world->ground[xml->infoherox] - 128 + 8;
            hero->jumpangle = 0;
            hero->jump = false;
        }
    }

    // hero regen

    if (!hero->action && !hero->hit)
    {
        if (game->life < game->maxlife)
            game->setlife(game->maxlife / 7777);
        if (game->mana < game->maxmana)
            game->setmana(game->maxmana / 777);
    }

    // pet follow

    if (!pet->attack)
    {
        if (xml->infopetx > xml->infoherox + 64)
        {
            pet->left = true;
            xml->infopetx -= distance;
        }
        else
            pet->left = false;

        if (xml->infopetx < xml->infoherox - 64)
        {
            pet->right = true;
            xml->infopetx += distance;
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
                if (magics[j]->type == gfx->magic_spell0)
                {
                    if (mobs[i]->collide(magics[j]))
                    {
                        int dmg = game->sapik(); // damage
                        mobs[i]->hp -= dmg;

                        magics.erase(magics.begin() + j);

                        mobs[i]->hit = true;

                        scores.push_back(new Score(gfx, "damage", dmg, mobs[i]->x, mobs[i]->y));

                        if (mobs[i]->hp <= 0) // mob dead
                        {
                            game->setexp(mobs[i]->level, pet);

                            for (int k = 0; k < 14; k++)
                                particles.push_back(new Particle(gfx, gfx->fx_0, mobs[i]->x, mobs[i]->y));

                            generateloot(mobs[i]->level, mobs[i]->x, mobs[i]->y);

                            emotes.push_back(new Emote(gfx, gfx->emote_dead, mobs[i]->x, mobs[i]->y));

                            delete mobs[i];
                            mobs.erase(mobs.begin() + i);
                            map->deletemob(i);
                        }
                        else // mob emote
                        {
                            if (mobs[i]->idle)
                            {
                                int emote = sf::Randomizer::Random(gfx->emote_aggro, gfx->emote_fear);
                                if (emote == gfx->emote_aggro)
                                {
                                    emotes.push_back(new Emote(gfx, gfx->emote_aggro, mobs[i]->x, mobs[i]->y));

                                    mobs[i]->idle = false;
                                    mobs[i]->aggro = true;
                                }
                                else if (emote == gfx->emote_fear)
                                {
                                    emotes.push_back(new Emote(gfx, gfx->emote_fear, mobs[i]->x, mobs[i]->y));

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
                int dmg = game->sapik(); // damage out
                mobs[i]->hp -= dmg / 2;

                mobs[i]->hit = true;

                if (pet->visible)
                    scores.push_back(new Score(gfx, "damage", dmg, mobs[i]->x, mobs[i]->y));

                if (mobs[i]->aggro)
                {
                    pet->badhit = true; // damage in
                    int life = -game->getdamage(mobs[i]);
                    pet->hp += life;

                    if (pet->hp <= 0)
                        pet->hp = 0;

                    if (pet->visible)
                    {
                        scores.push_back(new Score(gfx, "life", life, xml->infopetx, pet->y));
                        for (int j = 0; j < 7; j++)
                            particles.push_back(new Particle(gfx, gfx->fx_0, xml->infopetx, pet->y));
                    }
                }

                if (mobs[i]->hp <= 0) // mob dead
                {
                    game->setexp(mobs[i]->level, pet);

                    if (pet->visible)
                        for (int k = 0; k < 14; k++)
                            particles.push_back(new Particle(gfx, gfx->fx_0, mobs[i]->x, mobs[i]->y));

                    generateloot(mobs[i]->level, mobs[i]->x, mobs[i]->y);

                    if (pet->visible)
                        emotes.push_back(new Emote(gfx, gfx->emote_dead, mobs[i]->x, mobs[i]->y));

                    delete mobs[i];
                    mobs.erase(mobs.begin() + i);
                    map->deletemob(i);
                }
                else // mob emote
                {
                    if (mobs[i]->idle)
                    {
                        int emote = sf::Randomizer::Random(gfx->emote_aggro, gfx->emote_fear);
                        if (emote == gfx->emote_aggro)
                        {
                            if (pet->visible)
                                emotes.push_back(new Emote(gfx, gfx->emote_aggro, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->aggro = true;
                        }
                        else if (emote == gfx->emote_fear)
                        {
                            if (pet->visible)
                                emotes.push_back(new Emote(gfx, gfx->emote_fear, mobs[i]->x, mobs[i]->y));

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
                if (magics[i]->type == gfx->magic_spell0)
                {
                    if (boss[0]->collide(magics[i]))
                    {
                        int dmg = game->sapik(); // damage
                        boss[0]->hp -= dmg;

                        magics.erase(magics.begin() + i);

                        boss[0]->hit = true;

                        scores.push_back(new Score(gfx, "damage", dmg, boss[0]->x, boss[0]->y));

                        if (boss[0]->hp <= 0) // boss dead
                        {
                            game->setexp(boss[0]->level, pet);

                            for (int j = 0; j < 14; j++)
                                particles.push_back(new Particle(gfx, gfx->fx_0, boss[0]->x, boss[0]->y));

                            generateloot(boss[0]->level, boss[0]->x, boss[0]->y + 64);

                            emotes.push_back(new Emote(gfx, gfx->emote_dead, boss[0]->x, boss[0]->y));

                            boss.clear();
                            map->deleteboss();
                        }
                        else // boss emote
                        {
                            if (boss[0]->idle)
                            {
                                emotes.push_back(new Emote(gfx, gfx->emote_aggro, boss[0]->x, boss[0]->y));

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
            int dmg = game->sapik(); // damage out
            boss[0]->hp -= dmg / 2;

            boss[0]->hit = true;

            if (pet->visible)
                scores.push_back(new Score(gfx, "damage", dmg, boss[0]->x, boss[0]->y));

            pet->badhit = true; // damage in
            int life = -game->getdamage(boss[0]);
            pet->hp += life;

            if (pet->hp <= 0)
                pet->hp = 0;

            if (pet->visible)
            {
                scores.push_back(new Score(gfx, "life", life, xml->infopetx, pet->y));
                for (int j = 0; j < 7; j++)
                    particles.push_back(new Particle(gfx, gfx->fx_0, xml->infopetx, pet->y));
            }

            if (boss[0]->hp <= 0) // mob dead
            {
                game->setexp(boss[0]->level, pet);

                if (pet->visible)
                    for (int i = 0; i < 14; i++)
                        particles.push_back(new Particle(gfx, gfx->fx_0, boss[0]->x, boss[0]->y));

                generateloot(boss[0]->level, boss[0]->x, boss[0]->y + 64);

                if (pet->visible)
                    emotes.push_back(new Emote(gfx, gfx->emote_dead, boss[0]->x, boss[0]->y));

                boss.clear();
                map->deleteboss();
            }
            else // mob emote
            {
                if (boss[0]->idle)
                {
                    if (pet->visible)
                        emotes.push_back(new Emote(gfx, gfx->emote_aggro, boss[0]->x, boss[0]->y));

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
            if (magics[i]->type == gfx->magic_spell1)
            {
                if (pet->collide(magics[i]))
                {
                    magics.erase(magics.begin() + i);
                    if (!pet->goodhit)
                    {
                        pet->goodhit = true;

                        int life = game->sahil();
                        scores.push_back(new Score(gfx, "life", life, xml->infopetx, pet->y));
                        pet->hp += life;
                        if (pet->hp > pet->maxhp)
                            pet->hp = pet->maxhp;
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
                    int life = -game->getdamage(mobs[i]);
                    scores.push_back(new Score(gfx, "life", life, xml->infoherox, hero->y));
                    game->setlife(life);

                    for (int j = 0; j < 7; j++)
                        particles.push_back(new Particle(gfx, gfx->fx_0, xml->infoherox, hero->y));
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
            int life = -game->getdamage(boss[0]);
            scores.push_back(new Score(gfx, "life", life, xml->infoherox, hero->y));
            game->setlife(life);

            for (int i = 0; i < 7; i++)
                particles.push_back(new Particle(gfx, gfx->fx_0, xml->infoherox, hero->y));
        }
    }

    // hero collide loot

    for (int i = 0; i < (int)loots.size(); i++)
        if (loots[i]->visible)
        {
            if (loots[i]->collide(hero))
            {
                if (loots[i]->type == "coin")
                {
                    int money = game->getmoney(loots[i]);
                    scores.push_back(new Score(gfx, "money", money, loots[i]->x, loots[i]->y));
                    game->setmoney(money);
                }
                else if (loots[i]->type == "potionm")
                {
                    int mana = game->getpotionm(loots[i]);
                    scores.push_back(new Score(gfx, "mana", mana, loots[i]->x, loots[i]->y));
                    game->setmana(mana);
                }
                else if (loots[i]->type == "potionl")
                {
                    int life = game->getpotionl(loots[i]);
                    scores.push_back(new Score(gfx, "life", life, loots[i]->x, loots[i]->y));
                    game->setlife(life);
                }
                else if (loots[i]->type == "gift")
                    game->addgift(loots[i]);

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
            Mob* mob = new Mob(gfx, xml, hero);
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
            boss.push_back(new Boss(gfx, xml, hero));
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
        if (emotes[i]->time >= 16)
        {
            delete emotes[i];
            emotes.erase(emotes.begin() + i);
        }

    for (int i = 0; i < (int)particles.size(); i++)
        if (particles[i]->time >= 16)
        {
            delete particles[i];
            particles.erase(particles.begin() + i);
        }

    // bobo teleport

    for (int i = 0; i < (int)xml->worldteleports.size(); i++)
    {
        if (xml->infoherox >= xml->worldteleports[i].x && xml->infoherox <= xml->worldteleports[i].x + 64)
        {
            if (!bobo->firsttimeteleport)
            {
                bobo->firsttimeteleport = true;
                bobo->say("Press 'Jump' to teleport.");
            }
        }
    }
}

void Scene::draw(sf::RenderWindow* app, sf::View* view)
{
    if (xml->infoherox <= 400)
        view->SetFromRect(sf::FloatRect(0, 0, 800, 600));
    else if (xml->infoherox > 400 && xml->infoherox <= 2400 - 400)
        view->SetFromRect(sf::FloatRect(xml->infoherox - 400, 0, xml->infoherox + 400, 600));
    else if (xml->infoherox > 2400 - 400)
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

    for (int i = 0; i < (int)world->occludesprites.size(); i++)
        app->Draw(world->occludesprites[i]);

    for (int i = 0; i < (int)emotes.size(); i++)
        emotes[i]->draw(app);

    for (int i = 0; i < (int)xml->worldteleports.size(); i++)
        xml->worldteleports[i].draw(app);
}

void Scene::keypress(sf::Key::Code inkey)
{
    if (inkey == xml->keyleft)
        keyleft();
    if (inkey == xml->keyright)
        keyright();
    if (inkey == xml->keyjump)
        keyup();
    if (inkey == xml->keyaction)
        keyaction();
    if (inkey == xml->keypet)
        keypetcontrol();
}

void Scene::keyrelease(sf::Key::Code inkey)
{
    if (inkey == xml->keyleft)
        keyleftup();
    if (inkey == xml->keyright)
        keyrightup();
    if (inkey == xml->keyaction)
        keyactionup();
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
    for (int i = 0; i < (int)xml->worldteleports.size(); i++)
    {
        if (xml->infoherox >= xml->worldteleports[i].x && xml->infoherox <= xml->worldteleports[i].x + 64)
        {
            delete world;
            boss.clear();
            mobs.clear();
            scores.clear();
            loots.clear();
            emotes.clear();
            magics.clear();
            particles.clear();

            map->clear();

            xml->infoworldindex = xml->worldteleports[i].worlddest;
            xml->infoherox = xml->worldteleports[i].worldxdest;
            xml->readworlds(xml->infoworldindex);
            world = new World(gfx, xml);
            xml->infopetx = xml->infoherox;

            for (int i = 0; i < MAXMOBS; i++)
            {
                mobs.push_back(new Mob(gfx, xml, hero));
                map->addmob(mobs[i]);
            }
            mobrepop = 0;

            boss.push_back(new Boss(gfx, xml, hero));
            map->addboss(boss[0]);
            bossrepop = 0;
        }
    }

    if (!hero->action)
        hero->jump = true;
}

void Scene::keyaction()
{
    if (!hero->action)
    {
        int manacost = 0;
        if (xml->infomagicindex == gfx->magic_spell0)
            manacost = 5;
        else if (xml->infomagicindex == gfx->magic_spell1)
            manacost = 3;

        if (game->mana >= xml->infolevel * manacost)
        {
            game->setmana(xml->infolevel * -manacost);
            if (game->mana > 0)
            {
                magics.push_back(new Magic(gfx, hero->direction, xml->infoherox, hero->y + 128, xml->infomagicindex));

                hero->action = true;
            }
        }
    }
}

void Scene::keyactionup()
{
    hero->action = false;
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

void Scene::generateloot(int inlevel, int inx, int iny)
{
    int luck = sf::Randomizer::Random(0, 25);
    if (luck >= 0 && luck <= 10) // coin generated
    {
        Loot* loot = new Loot(gfx, xml, "coin", inlevel, inx, iny);
        loots.push_back(loot);
        map->addloot(loot);
    }
    else if (luck >= 11 && luck <= 14) // potionl generated
    {
        Loot* loot = new Loot(gfx, xml, "potionl", inlevel, inx, iny);
        loots.push_back(loot);
        map->addloot(loot);
    }
    else if (luck >= 15 && luck <= 18) // potionm generated
    {
        Loot* loot = new Loot(gfx, xml, "potionm", inlevel, inx, iny);
        loots.push_back(loot);
        map->addloot(loot);
    }
    else if (luck >= 19 && luck <= 20) // gift generated
    {
        Loot* loot = new Loot(gfx, xml, "gift", inlevel, inx, iny);
        loots.push_back(loot);
        map->addloot(loot);
    }
}
