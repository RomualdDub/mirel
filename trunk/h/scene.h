class Scene
{
    public:
    Scene(Gfx* ingfx, Snd* insnd, Bobo* inbobo, Xml* inxml, Game* ingame, Map* inmap, System* insys);
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
    vector<Pet*> pets;
    vector<Boss*> boss;
    vector<Mob*> mobs;
    vector<Loot*> loots;

    #define MAXMOBS 7

    private:
    Gfx* gfx;
    Snd* snd;
    Bobo* bobo;
    Xml* xml;
    Game* game;
    Map* map;
    System* sys;
    World* world;
    vector<Score*> scores;
    vector<Emote*> emotes;
    vector<Magic*> magics;
    vector<Particle*> particles;
    vector<Particle*> repopfx;
    int bossrepop;
    int mobrepop;
    int currenthat;
    int currentdress;
    int currentwand;
};

Scene::Scene(Gfx* ingfx, Snd* insnd, Bobo* inbobo, Xml* inxml, Game* ingame, Map* inmap, System* insys)
{
    gfx = ingfx;
    snd = insnd;
    bobo = inbobo;
    xml = inxml;
    game = ingame;
    map = inmap;
    sys = insys;

    world = new World(gfx, xml);

    hero = new Hero(gfx, snd, xml, "general");
    map->addhero(hero);
    snd->herorepop.Play();
    repopfx.push_back(new Particle(gfx, gfx->fx_2_1, xml->infoherox, hero->y));
    currenthat = xml->infohatindex;
    currentdress = xml->infodressindex;
    currentwand = xml->infowandindex;

    pets.push_back(new Pet(gfx, xml, "hedgehog"));
    pets.push_back(new Pet(gfx, xml, "crow"));
    map->addpet(pets.front());

    // mobs

    for (int i = 0; i < MAXMOBS; i++)
    {
        mobs.push_back(new Mob(gfx, xml, hero));
        map->addmob(mobs[i]);
        snd->mobrepop.Play();
        repopfx.push_back(new Particle(gfx, gfx->fx_2_1, mobs[i]->x, mobs[i]->y));
    }
    mobrepop = 0;

    // boss

    boss.push_back(new Boss(gfx, xml, hero));
    map->addboss(boss[0]);
    snd->bossrepop.Play();
    repopfx.push_back(new Particle(gfx, gfx->fx_2_1, boss[0]->x, boss[0]->y));
    bossrepop = 0;
}

Scene::~Scene()
{
    delete world;
    delete hero;
    pets.clear();
    boss.clear();
    mobs.clear();
    loots.clear();
    scores.clear();
    emotes.clear();
    magics.clear();
    particles.clear();
    repopfx.clear();
}

void Scene::animate(int distance, sf::View* view)
{
    // stuff handling

    if (currenthat != xml->infohatindex || currentdress != xml->infodressindex || currentwand != xml->infowandindex)
    {
        delete hero;
        hero = new Hero(gfx, snd, xml, "general");
        currenthat = xml->infohatindex;
        currentdress = xml->infodressindex;
        currentwand = xml->infowandindex;
    }

    // animate

    hero->animate();
    map->updatehero();

    if (xml->infopettankx + 64 < view->GetRect().Left || xml->infopettankx > view->GetRect().Right)
    {
        pets.front()->visible = false;
        if (xml->infopettankx + 64 < view->GetRect().Left)
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
        pets.front()->visible = true;
        pets.front()->animate();

        map->petleft = false;
        map->petright = false;
        map->updatepet(pets.front());
    }

    pets.back()->animate();

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

    for (int i = 0; i < (int)repopfx.size(); i++)
        repopfx[i]->animate(distance);

    for (int i = 0; i < (int)xml->worldteleports.size(); i++)
        xml->worldteleports[i].animate(distance);

    // ground (gravity)

    if (hero->y < world->ground[xml->infoherox] - 128 + 8)
        hero->y += distance;
    if (hero->y > world->ground[xml->infoherox] - 128 + 8)
        hero->y -= distance;

    if (pets.front()->y < world->ground[xml->infopettankx] - 64 + 8)
        pets.front()->y += distance;
    if (pets.front()->y > world->ground[xml->infopettankx] - 64 + 8)
        pets.front()->y -= distance;

    if (pets.back()->y < world->ground[xml->infopetlootx] - 64 - 160)
        pets.back()->y += distance;
    if (pets.back()->y > world->ground[xml->infopetlootx] - 64 - 160)
        pets.back()->y -= distance;

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
        xml->infoherox -= distance;

    // hero right

    if (hero->right)
        xml->infoherox += distance;

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

    // pets follow

    if (!pets.front()->attack)
    {
        if (xml->infopettankx > xml->infoherox + 64)
        {
            pets.front()->left = true;
            xml->infopettankx -= distance;
        }
        else
            pets.front()->left = false;

        if (xml->infopettankx < xml->infoherox - 64)
        {
            pets.front()->right = true;
            xml->infopettankx += distance;
        }
        else
            pets.front()->right = false;
    }

    if (xml->infopetlootx > xml->infoherox + 128)
    {
        pets.back()->left = true;
        xml->infopetlootx -= distance;
    }
    else
        pets.back()->left = false;

    if (xml->infopetlootx < xml->infoherox - 128)
    {
        pets.back()->right = true;
        xml->infopetlootx += distance;
    }
    else
        pets.back()->right = false;

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
                        snd->mobhit.Play();
                        int dmg = game->sapik(); // damage
                        mobs[i]->hp -= dmg;

                        magics.erase(magics.begin() + j);

                        mobs[i]->hit = true;

                        scores.push_back(new Score(gfx, xml, "damage", dmg, mobs[i]->x, mobs[i]->y));

                        if (mobs[i]->hp <= 0) // mob dead
                        {
                            snd->explode.Play();

                            game->setexp(mobs[i]->level, pets.front());

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

    if (pets.front()->attack)
    {
        for (int i = 0; i < (int)mobs.size(); i++)
        {
            if (pets.front()->collide(mobs[i]) && !mobs[i]->hit && pets.front()->hp)
            {
                snd->mobhit.Play();
                int dmg = game->sapik(); // damage out
                mobs[i]->hp -= dmg / 2;

                mobs[i]->hit = true;

                if (pets.front()->visible)
                    scores.push_back(new Score(gfx, xml, "damage", dmg, mobs[i]->x, mobs[i]->y));

                if (mobs[i]->aggro)
                {
                    snd->petbadhit.Play();
                    pets.front()->badhit = true; // damage in
                    int life = -game->getdamage(mobs[i]);
                    pets.front()->hp += life;

                    if (pets.front()->hp <= 0)
                        pets.front()->hp = 0;

                    if (pets.front()->visible)
                    {
                        scores.push_back(new Score(gfx, xml, "life", life, xml->infopettankx, pets.front()->y));
                        for (int j = 0; j < 7; j++)
                            particles.push_back(new Particle(gfx, gfx->fx_0, xml->infopettankx, pets.front()->y));
                    }
                }

                if (mobs[i]->hp <= 0) // mob dead
                {
                    snd->explode.Play();

                    game->setexp(mobs[i]->level, pets.front());

                    if (pets.front()->visible)
                        for (int k = 0; k < 14; k++)
                            particles.push_back(new Particle(gfx, gfx->fx_0, mobs[i]->x, mobs[i]->y));

                    generateloot(mobs[i]->level, mobs[i]->x, mobs[i]->y);

                    if (pets.front()->visible)
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
                            if (pets.front()->visible)
                                emotes.push_back(new Emote(gfx, gfx->emote_aggro, mobs[i]->x, mobs[i]->y));

                            mobs[i]->idle = false;
                            mobs[i]->aggro = true;
                        }
                        else if (emote == gfx->emote_fear)
                        {
                            if (pets.front()->visible)
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
                        snd->bosshit.Play();
                        int dmg = game->sapik(); // damage out
                        boss[0]->hp -= dmg;

                        magics.erase(magics.begin() + i);

                        boss[0]->hit = true;

                        scores.push_back(new Score(gfx, xml, "damage", dmg, boss[0]->x, boss[0]->y));

                        if (boss[0]->hp <= 0) // boss dead
                        {
                            snd->explode.Play();

                            game->setexp(boss[0]->level, pets.front());

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

    if (pets.front()->attack && boss.size())
    {
        if (pets.front()->collide(boss[0]) && !boss[0]->hit && pets.front()->hp)
        {
            snd->bosshit.Play();
            int dmg = game->sapik(); // damage out
            boss[0]->hp -= dmg / 2;

            boss[0]->hit = true;

            if (pets.front()->visible)
                scores.push_back(new Score(gfx, xml, "damage", dmg, boss[0]->x, boss[0]->y));

            snd->petbadhit.Play();
            pets.front()->badhit = true; // damage in
            int life = -game->getdamage(boss[0]);
            pets.front()->hp += life;

            if (pets.front()->hp <= 0)
                pets.front()->hp = 0;

            if (pets.front()->visible)
            {
                scores.push_back(new Score(gfx, xml, "life", life, xml->infopettankx, pets.front()->y));
                for (int j = 0; j < 7; j++)
                    particles.push_back(new Particle(gfx, gfx->fx_0, xml->infopettankx, pets.front()->y));
            }

            if (boss[0]->hp <= 0) // boss dead
            {
                snd->explode.Play();

                game->setexp(boss[0]->level, pets.front());

                if (pets.front()->visible)
                    for (int i = 0; i < 14; i++)
                        particles.push_back(new Particle(gfx, gfx->fx_0, boss[0]->x, boss[0]->y));

                generateloot(boss[0]->level, boss[0]->x, boss[0]->y + 64);

                if (pets.front()->visible)
                    emotes.push_back(new Emote(gfx, gfx->emote_dead, boss[0]->x, boss[0]->y));

                boss.clear();
                map->deleteboss();
            }
            else // mob emote
            {
                if (boss[0]->idle)
                {
                    if (pets.front()->visible)
                        emotes.push_back(new Emote(gfx, gfx->emote_aggro, boss[0]->x, boss[0]->y));

                    boss[0]->idle = false;
                    boss[0]->aggro = true;
                }
            }
        }
    }

    // magic collide pet

    if (pets.front()->visible)
    {
        for (int i = 0; i < (int)magics.size(); i++)
        {
            if (magics[i]->type == gfx->magic_spell1)
            {
                if (pets.front()->collide(magics[i]))
                {
                    magics.erase(magics.begin() + i);
                    if (!pets.front()->goodhit)
                    {
                        pets.front()->goodhit = true;

                        int life = game->sahil();
                        scores.push_back(new Score(gfx, xml, "life", life, xml->infopettankx, pets.front()->y));
                        pets.front()->hp += life;
                        if (pets.front()->hp > pets.front()->maxhp)
                            pets.front()->hp = pets.front()->maxhp;
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
                    snd->herohit.Play();
                    hero->hit = true;
                    int life = -game->getdamage(mobs[i]);
                    scores.push_back(new Score(gfx, xml, "life", life, xml->infoherox, hero->y));
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
            snd->herohit.Play();
            hero->hit = true;
            int life = -game->getdamage(boss[0]);
            scores.push_back(new Score(gfx, xml, "life", life, xml->infoherox, hero->y));
            game->setlife(life);

            for (int i = 0; i < 7; i++)
                particles.push_back(new Particle(gfx, gfx->fx_0, xml->infoherox, hero->y));
        }
    }

    // hero & pet collide loot

    for (int i = 0; i < (int)loots.size(); i++)
        if (loots[i]->visible)
        {
            if (loots[i]->collide(hero) || pets.back()->collide(loots[i]))
            {
                if (loots[i]->type == "coin")
                {
                    snd->coin.Play();
                    int money = game->getmoney(loots[i]);
                    scores.push_back(new Score(gfx, xml, "money", money, loots[i]->x, loots[i]->y));
                    game->setmoney(money);
                }
                else if (loots[i]->type == "potionm")
                {
                    snd->potionm.Play();
                    int mana = game->getpotionm(loots[i]);
                    scores.push_back(new Score(gfx, xml, "mana", mana, loots[i]->x, loots[i]->y));
                    game->setmana(mana);
                }
                else if (loots[i]->type == "potionl")
                {
                    snd->potionl.Play();
                    int life = game->getpotionl(loots[i]);
                    scores.push_back(new Score(gfx, xml, "life", life, loots[i]->x, loots[i]->y));
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

    if (pets.front()->badtime >= 8)
    {
        pets.front()->badhit = false;
        pets.front()->badtime = 0;
    }
    if (pets.front()->goodtime >= 8)
    {
        pets.front()->goodhit = false;
        pets.front()->goodtime = 0;
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
            mobs.push_back(new Mob(gfx, xml, hero));
            map->addmob(mobs.back());
            snd->mobrepop.Play();
            repopfx.push_back(new Particle(gfx, gfx->fx_2_1, mobs.back()->x, mobs.back()->y));
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
            snd->bossrepop.Play();
            repopfx.push_back(new Particle(gfx, gfx->fx_2_1, boss[0]->x, boss[0]->y));
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

    for (int i = 0; i < (int)repopfx.size(); i++)
        if (repopfx[i]->time >= 12)
        {
            delete repopfx[i];
            repopfx.erase(repopfx.begin() + i);
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

    for (int i = 0; i < (int)repopfx.size(); i++)
        repopfx[i]->draw(app);

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

    if (pets.front()->visible)
        pets.front()->draw(app);
    pets.back()->draw(app);

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
            if (!xml->worldteleports[i].direction)
            {
                sys->ether_hl = true;
                xml->infoherodirection = DIRECTION_RIGHT;
            }
            else
            {
                sys->ether_hr = true;
                xml->infoherodirection = DIRECTION_LEFT;
            }
            xml->infoworldindex = xml->worldteleports[i].worlddest;
            xml->infoherox = xml->worldteleports[i].worldxdest;
            xml->readworlds(xml->infoworldindex);
            xml->infopettankx = xml->infoherox;
            xml->infopetlootx = xml->infoherox;
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
    if (pets.front()->attack)
        pets.front()->attack = false;
    else
    {
        pets.front()->attack = true;
        pets.front()->left = false;
        pets.front()->right = false;
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
