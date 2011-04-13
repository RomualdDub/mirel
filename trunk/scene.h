class Scene
{
    public:
    Scene(int inherox, Map* map);
    ~Scene();
    void animate(int distance, sf::View* view, Game* game, Map* map);
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
    sf::Image groundtempimage0, groundtempimage1, groundtempimage2;
    sf::Image groundimage;
    int ground[2400];
    sf::Image occludemasktempimage0, occludemasktempimage1, occludemasktempimage2;
    sf::Image occludesrctempimage0, occludesrctempimage1, occludesrctempimage2;
    sf::Image occludemaskimage, occludesrcimage;
    sf::Image occludedestimage;
    sf::Sprite occludesprite;
    int mobrepop;
};

Scene::Scene(int inherox, Map* map)
{
    world = new World;

    hero = new Hero(inherox);
    map->addhero(hero);

    pet = new Pet(hero);
    map->addpet(pet);

    // mobs

    for (int i = 0; i < MAXMOBS; i++)
    {
        mobs.push_back(new Mob(1, 100, 1000));
        map->addmob(mobs[i]);
    }
    mobrepop = 0;

    // ground

    groundtempimage0 = getimage(GFX_WORLDCOLLIDE01);
    groundtempimage1 = getimage(GFX_WORLDCOLLIDE02);
    groundtempimage2 = getimage(GFX_WORLDCOLLIDE03);

    sf::Color color(0, 0, 0, 0);
    groundimage.Create(800 * 3, 472, color);

    blit(&groundtempimage0, &groundimage, 800, 472, 0);
    blit(&groundtempimage1, &groundimage, 800, 472, 1);
    blit(&groundtempimage2, &groundimage, 800, 472, 2);

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

    occludemasktempimage0 = getimage(GFX_WORLDOCCLUDE01);
    occludemasktempimage1 = getimage(GFX_WORLDOCCLUDE02);
    occludemasktempimage2 = getimage(GFX_WORLDOCCLUDE03);
    occludesrctempimage0 = getimage(GFX_WORLD01);
    occludesrctempimage1 = getimage(GFX_WORLD02);
    occludesrctempimage2 = getimage(GFX_WORLD03);

    occludemaskimage.Create(800 * 3, 472, color);
    occludesrcimage.Create(800 * 3, 472, color);

    blit(&occludemasktempimage0, &occludemaskimage, 800, 472, 0);
    blit(&occludemasktempimage1, &occludemaskimage, 800, 472, 1);
    blit(&occludemasktempimage2, &occludemaskimage, 800, 472, 2);

    blit(&occludesrctempimage0, &occludesrcimage, 800, 472, 0);
    blit(&occludesrctempimage1, &occludesrcimage, 800, 472, 1);
    blit(&occludesrctempimage2, &occludesrcimage, 800, 472, 2);

    occludedestimage.Create(2400, 472, color);
    occludedestimage.SetSmooth(false);

    for (int x = 0; x < 2400; x++)
    {
        for (int y = 0; y < 472; y++)
        {
            pixel = occludemaskimage.GetPixel(x, y);
            if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255 && pixel.a)
                occludedestimage.SetPixel(x, y, occludesrcimage.GetPixel(x, y));
        }
    }

    occludesprite.SetImage(occludedestimage);
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

void Scene::animate(int distance, sf::View* view, Game* game, Map* map)
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

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->x + (64 * mobs[i]->scalefactor) < view->GetRect().Left || mobs[i]->x > view->GetRect().Right)
            mobs[i]->visible = false;
        else
            mobs[i]->visible = true;
        mobs[i]->animate(distance, hero);
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

    if (hero->y < ground[(int)hero->x] + YGROUND)
        hero->y += distance;
    if (hero->y > ground[(int)hero->x] + YGROUND)
        hero->y -= distance;

    if (pet->y < ground[(int)pet->x] + YGROUND / 2)
        pet->y += distance;
    if (pet->y > ground[(int)pet->x] + YGROUND / 2)
        pet->y -= distance;

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->y < ground[(int)(mobs[i]->x)])
            mobs[i]->y += distance;
        if (mobs[i]->y > ground[(int)(mobs[i]->x)])
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

                if (pet->visible)
                    scores.push_back(new Score(SCORE_DAMAGE, dmg, mobs[i]->x, mobs[i]->y));

                if (mobs[i]->hp <= 0) // mob dead
                {
                    game->setexp(mobs[i]->level);

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

    // hero collide mob

    if (!hero->hit)
    {
        for (int i = 0; i < (int)mobs.size(); i++)
            if (mobs[i]->visible)
            {
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
            map->addmob(mob);
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
        if (game->mana >= game->level * 5)
        {
            game->setmana(game->level * -5);
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
