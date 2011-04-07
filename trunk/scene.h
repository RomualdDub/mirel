using namespace std;

class Scene
{
    public:
    Scene();
    ~Scene();
    void animate(Game* game);
    void draw();
    void keyleft();
    void keyleftup();
    void keyright();
    void keyrightup();
    void keyup();
    void keyaction(Game* game);
    void keyactionup();
    void keycontrol();
    Hero* hero;
    Pet* pet;
    vector<Mob*> mobs;
    vector<Loot*> loots;

    #define YGROUND 56

    private:
    World* world;
    vector<Score*> scores;
    vector<Emote*> emotes;
    vector<Magic*> magics;
    vector<Particle*> particles;
    int ground[2400];
    GLuint occludetexture;
};

Scene::Scene()
{
    world = new World;
    hero = new Hero;
    pet = new Pet;

    // mobs

    for (int i = 0; i < 7; i++)
        mobs.push_back(new Mob(1, 100, 1000));

    // ground

    SDL_Surface* surface = IMG_Load("gfx/world00.png");

    Uint8 r,g,b,a;
    for(int x = 0; x < 2400; x++)
    {
        for(int y = 0; y < 472; y++)
        {
            Uint32 pixel = getpixel(surface, x, y);
            SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
            if (r == 0xFF && g == 0xFF && b == 0xFF)
            {
                ground[x] = y;
                break;
            }
        }
    }

    SDL_FreeSurface(surface);

    // occlude

    SDL_Surface* surfacemask = IMG_Load("gfx/world000.png");
    SDL_Surface* surfacesrc = IMG_Load("gfx/world0.png");
    SDL_Surface* surfacedest = SDL_CreateRGBSurface(SDL_SWSURFACE, 2400, 472, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    for(int x = 0; x < 2400; x++)
    {
        for(int y = 0; y < 472; y++)
        {
            Uint32 pixel = getpixel(surfacemask, x, y);
            SDL_GetRGBA(pixel, surfacesrc->format, &r, &g, &b, &a);
            if (r == 0xFF && g == 0xFF && b == 0xFF && a > 0x80)
            {
                Uint32 pixel = getpixel(surfacesrc, x, y);
                putpixel(surfacedest, x, y, pixel);
            }
        }
    }

    occludetexture = glloadtexture(surfacedest);

    SDL_FreeSurface(surfacemask);
    SDL_FreeSurface(surfacesrc);
    SDL_FreeSurface(surfacedest);
}

Scene::~Scene()
{
    delete world;
    delete hero;
    delete pet;
    mobs.clear();
    scores.clear();
    loots.clear();
    magics.clear();
    particles.clear();
}

void Scene::animate(Game* game)
{
    // animate

    hero->animate();
    pet->animate();
    world->animate();

    for (int i = 0; i < (int)mobs.size(); i++)
        mobs[i]->animate(hero);

    for (int i = 0; i < (int)loots.size(); i++)
        loots[i]->animate();

    for (int i = 0; i < (int)scores.size(); i++)
        scores[i]->animate();

    for (int i = 0; i < (int)emotes.size(); i++)
        emotes[i]->animate();

    for (int i = 0; i < (int)magics.size(); i++)
        magics[i]->animate();

    for (int i = 0; i < (int)particles.size(); i++)
        particles[i]->animate();

    // ground (gravity)

    if (hero->y < ground[(int)hero->x] - YGROUND)
        hero->y++;
    if (hero->y > ground[(int)hero->x] - YGROUND)
        hero->y--;

    if (pet->y < ground[(int)pet->x] - YGROUND / 2)
        pet->y++;
    if (pet->y > ground[(int)pet->x] - YGROUND / 2)
        pet->y--;

    for (int i = 0; i < (int)mobs.size(); i++)
    {
        if (mobs[i]->y < ground[(int)(mobs[i]->x)] - mobs[i]->basesize / 2)
            mobs[i]->y++;
        if (mobs[i]->y > ground[(int)(mobs[i]->x)] - mobs[i]->basesize / 2)
            mobs[i]->y--;
    }

    for (int i = 0; i < (int)loots.size(); i++)
        loots[i]->y -= ground[(int)loots[i]->x] / 472;

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

        if (hero->y >= ground[(int)hero->x] - YGROUND)
        {
            hero->y = ground[(int)hero->x] - YGROUND;
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

                    loots.push_back(new Loot(mobs[i]->level, mobs[i]->x, mobs[i]->y));

                    emotes.push_back(new Emote(EMOTE_DEAD, mobs[i]->x, mobs[i]->y));

                    delete mobs[i];
                    mobs.erase(mobs.begin() + i);
                    mobs.push_back(new Mob(1, 100, 1000));
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

                    loots.push_back(new Loot(mobs[i]->level, mobs[i]->x, mobs[i]->y));

                    emotes.push_back(new Emote(EMOTE_DEAD, mobs[i]->x, mobs[i]->y));

                    delete mobs[i];
                    mobs.erase(mobs.begin() + i);
                    mobs.push_back(new Mob(1, 100, 1000));
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
        }

    // hero hit end

    if (hero->time >= 32)
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

void Scene::draw()
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if (hero->x <= 400)
        gluOrtho2D(0, 800, 600, 0);
    else if (hero->x > 400 && hero->x <= 2400 - 400)
        gluOrtho2D(hero->x - 400, hero->x + 400, 600, 0);
    else if (hero->x > 2400 - 400)
        gluOrtho2D(2400 - 800, 2400, 600, 0);

    world->draw();

    for (int i = 0; i < (int)particles.size(); i++)
        particles[i]->draw();

    for (int i = 0; i < (int)mobs.size(); i++)
        mobs[i]->draw();

    for (int i = 0; i < (int)scores.size(); i++)
        scores[i]->draw();

    for (int i = 0; i < (int)loots.size(); i++)
        loots[i]->draw();

    hero->draw();

    pet->draw();

    for (int i = 0; i < (int)magics.size(); i++)
        magics[i]->draw();

    // occlude

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslated(0, 472 / 2, 0);

    glBindTexture(GL_TEXTURE_2D, occludetexture);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2i(0, -236);
        glTexCoord2d(0, 0); glVertex2i(0, 236);
        glTexCoord2d(1, 0); glVertex2i(2400, 236);
        glTexCoord2d(1, 1); glVertex2i(2400, -236);
    glEnd();

    for (int i = 0; i < (int)emotes.size(); i++)
        emotes[i]->draw();
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
        game->setmana(game->level * -5);
        if (game->mana > 0)
        {
            magics.push_back(new Magic(hero->direction, hero->x, hero->y));

            hero->attack = true;
        }
    }
}

void Scene::keyactionup()
{
    hero->attack = false;
}

void Scene::keycontrol()
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
