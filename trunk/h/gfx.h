class Gfx
{
    public:
    Gfx();
    ~Gfx();
    void loadhero();
    void loadboss(int bossindex);
    void loadmob(int mobindex);
    void loadpet();
    void loadworld(int worldindex, int worldsplit);
    void blit(sf::Image* srcimage, sf::Image* destimage);
    void white(sf::Image* srcimage, sf::Image* destimage);

    sf::Font font;

    vector<sf::Image> boboimages;
    vector<sf::Image> bossimages;
    vector<sf::Image> dialogimages;
    vector<sf::Image> emoteimages;
    vector<sf::Image> fximages;
    vector<sf::Image> interfaceimages;
    vector<sf::Image> lootimages;
    vector<sf::Image> magicimages;
    vector<sf::Image> mapimages;
    vector<sf::Image> mirelimages;
    vector<sf::Image> mobimages;
    vector<sf::Image> petimages;
    vector<sf::Image> heroimages;
    vector<sf::Image> hatimages;
    vector<sf::Image> dressimages;
    vector<sf::Image> wandimages;
    vector<sf::Image> worldskyimages;
    vector<sf::Image> worldbgimages;
    vector<sf::Image> worldcollideimages;
    vector<sf::Image> worldoccludeimages;

    int bobo_normal;
    int bobo_idle;
    int bobo_talk;
    int boss_normal;
    int boss_hit;
    int dialog_corner;
    int dialog_dot;
    int dialog_extension;
    int emote_aggro;
    int emote_dead;
    int emote_fear;
    int fx_0;
    int fx_1;
    int interface_barexp;
    int interface_barlife;
    int interface_barmana;
    int interface_state;
    int interface_ui;
    int interface_spell0;
    int interface_spell1;
    int loot_coin0;
    int loot_coin1;
    int loot_coin2;
    int loot_coin3;
    int loot_potionl;
    int loot_potionm;
    int magic_spell0;
    int magic_spell1;
    int map_arrow;
    int map_minihp;
    int mirel_feet0;
    int mirel_feet1;
    int mirel_feet2;
    int mirel_head0;
    int mirel_head1;
    int mirel_head2;
    int hero_walk0;
    int hero_walk1;
    int hero_walk2;
    int hero_act0;
    int hero_act1;
    int hero_act2;
    int hero_idle;
    int hero_jump;
    int hero_hit;
    int mob_normal;
    int mob_hit;
    int pet_walk0;
    int pet_walk1;
    int pet_idle;
    int pet_atk;
    int pet_hit;

    private:
    std::ostringstream txt;
    vector<sf::Image>worldmaskimages;
};

Gfx::Gfx()
{
    // load universal resources

    font.LoadFromFile("font/Plakkaat.ttf");

    boboimages.push_back(sf::Image());
    boboimages[0].LoadFromFile("gfx/bobo0.png");
    boboimages.push_back(sf::Image());
    boboimages[1].LoadFromFile("gfx/bobo1.png");
    boboimages.push_back(sf::Image());
    boboimages[2].LoadFromFile("gfx/bobo2.png");
    bobo_normal = 0;
    bobo_idle = 1;
    bobo_talk = 2;

    for (int i = 0; i < (int)boboimages.size(); i++)
        boboimages[i].SetSmooth(false);

    dialogimages.push_back(sf::Image());
    dialogimages[0].LoadFromFile("gfx/dialog_corner.png");
    dialogimages.push_back(sf::Image());
    dialogimages[1].LoadFromFile("gfx/dialog_dot.png");
    dialogimages.push_back(sf::Image());
    dialogimages[2].LoadFromFile("gfx/dialog_extension.png");
    dialog_corner = 0;
    dialog_dot = 1;
    dialog_extension = 2;

    for (int i = 0; i < (int)dialogimages.size(); i++)
        dialogimages[i].SetSmooth(false);

    emoteimages.push_back(sf::Image());
    emoteimages[0].LoadFromFile("gfx/emote_aggro.png");
    emoteimages.push_back(sf::Image());
    emoteimages[1].LoadFromFile("gfx/emote_fear.png");
    emoteimages.push_back(sf::Image());
    emoteimages[2].LoadFromFile("gfx/emote_dead.png");
    emote_aggro = 0;
    emote_fear = 1;
    emote_dead = 2;

    for (int i = 0; i < (int)emoteimages.size(); i++)
        emoteimages[i].SetSmooth(false);

    fximages.push_back(sf::Image());
    fximages[0].LoadFromFile("gfx/fx0.png");
    fximages.push_back(sf::Image());
    fximages[1].LoadFromFile("gfx/fx1.png");
    fx_0 = 0;
    fx_1 = 1;

    for (int i = 0; i < (int)fximages.size(); i++)
        fximages[i].SetSmooth(false);

    interfaceimages.push_back(sf::Image());
    interfaceimages[0].LoadFromFile("gfx/interface_barexp.png");
    interfaceimages.push_back(sf::Image());
    interfaceimages[1].LoadFromFile("gfx/interface_barlife.png");
    interfaceimages.push_back(sf::Image());
    interfaceimages[2].LoadFromFile("gfx/interface_barmana.png");
    interfaceimages.push_back(sf::Image());
    interfaceimages[3].LoadFromFile("gfx/interface_state.png");
    interfaceimages.push_back(sf::Image());
    interfaceimages[4].LoadFromFile("gfx/interface_ui.png");
    interfaceimages.push_back(sf::Image());
    interfaceimages[5].LoadFromFile("gfx/interface_spell0.png");
    interfaceimages.push_back(sf::Image());
    interfaceimages[6].LoadFromFile("gfx/interface_spell1.png");
    interface_barexp = 0;
    interface_barlife = 1;
    interface_barmana = 2;
    interface_state = 3;
    interface_ui = 4;
    interface_spell0 = 5;
    interface_spell1 = 6;

    for (int i = 0; i < (int)interfaceimages.size(); i++)
        interfaceimages[i].SetSmooth(false);

    lootimages.push_back(sf::Image());
    lootimages[0].LoadFromFile("gfx/loot_coin0.png");
    lootimages.push_back(sf::Image());
    lootimages[1].LoadFromFile("gfx/loot_coin1.png");
    lootimages.push_back(sf::Image());
    lootimages[2].LoadFromFile("gfx/loot_coin2.png");
    lootimages.push_back(sf::Image());
    lootimages[3].LoadFromFile("gfx/loot_coin3.png");
    lootimages.push_back(sf::Image());
    lootimages[4].LoadFromFile("gfx/loot_potionl.png");
    lootimages.push_back(sf::Image());
    lootimages[5].LoadFromFile("gfx/loot_potionm.png");
    loot_coin0 = 0;
    loot_coin1 = 1;
    loot_coin2 = 2;
    loot_coin3 = 3;
    loot_potionl = 4;
    loot_potionm = 5;

    for (int i = 0; i < (int)lootimages.size(); i++)
        lootimages[i].SetSmooth(false);

    magicimages.push_back(sf::Image());
    magicimages[0].LoadFromFile("gfx/magic0.png");
    magicimages.push_back(sf::Image());
    magicimages[1].LoadFromFile("gfx/magic1.png");
    magic_spell0 = 0;
    magic_spell1 = 1;

    for (int i = 0; i < (int)magicimages.size(); i++)
        magicimages[i].SetSmooth(false);

    mapimages.push_back(sf::Image());
    mapimages[0].LoadFromFile("gfx/map_arrow.png");
    mapimages.push_back(sf::Image());
    mapimages[1].LoadFromFile("gfx/map_minihp.png");
    map_arrow = 0;
    map_minihp = 1;

    for (int i = 0; i < (int)mapimages.size(); i++)
        mapimages[i].SetSmooth(false);

    mirelimages.push_back(sf::Image());
    mirelimages[0].LoadFromFile("gfx/mirel_feet0.png");
    mirelimages.push_back(sf::Image());
    mirelimages[1].LoadFromFile("gfx/mirel_feet1.png");
    mirelimages.push_back(sf::Image());
    mirelimages[2].LoadFromFile("gfx/mirel_feet2.png");
    mirelimages.push_back(sf::Image());
    mirelimages[3].LoadFromFile("gfx/mirel_head0.png");
    mirelimages.push_back(sf::Image());
    mirelimages[4].LoadFromFile("gfx/mirel_head1.png");
    mirelimages.push_back(sf::Image());
    mirelimages[5].LoadFromFile("gfx/mirel_head2.png");
    mirel_feet0 = 0;
    mirel_feet1 = 1;
    mirel_feet2 = 2;
    mirel_head0 = 3;
    mirel_head1 = 4;
    mirel_head2 = 5;
}

Gfx::~Gfx()
{
    boboimages.clear();
    bossimages.clear();
    dialogimages.clear();
    emoteimages.clear();
    fximages.clear();
    interfaceimages.clear();
    lootimages.clear();
    magicimages.clear();
    mapimages.clear();
    mirelimages.clear();
    mobimages.clear();
    petimages.clear();
    heroimages.clear();
    hatimages.clear();
    dressimages.clear();
    wandimages.clear();
    worldskyimages.clear();
    worldbgimages.clear();
    worldcollideimages.clear();
    worldoccludeimages.clear();
}

void Gfx::loadhero()
{
    hatimages.clear();
    dressimages.clear();
    wandimages.clear();
    heroimages.clear();

    hatimages.push_back(sf::Image());
    hatimages[0].LoadFromFile("gfx/hat001.png");
    dressimages.push_back(sf::Image());
    dressimages[0].LoadFromFile("gfx/dress001_0.png");
    dressimages.push_back(sf::Image());
    dressimages[1].LoadFromFile("gfx/dress001_1.png");
    dressimages.push_back(sf::Image());
    dressimages[2].LoadFromFile("gfx/dress001_2.png");
    wandimages.push_back(sf::Image());
    wandimages[0].LoadFromFile("gfx/wand001_0.png");
    wandimages.push_back(sf::Image());
    wandimages[1].LoadFromFile("gfx/wand001_1.png");
    wandimages.push_back(sf::Image());
    wandimages[2].LoadFromFile("gfx/wand001_2.png");

    hero_walk0 = 0;
    hero_walk1 = 1;
    hero_walk2 = 2;
    hero_act0 = 3;
    hero_act1 = 4;
    hero_act2 = 5;
    hero_idle = 6;
    hero_jump = 7;
    hero_hit = 8;

    sf::Color color(0, 0, 0, 0);
    for (int i = 0; i < hero_hit + 1; i++)
    {
        heroimages.push_back(sf::Image());
        heroimages[i].Create(64, 128, color);
        heroimages[i].SetSmooth(false);
    }

    blit(&hatimages[0], &heroimages[hero_walk0]);
    blit(&mirelimages[mirel_head0], &heroimages[hero_walk0]);
    blit(&dressimages[0], &heroimages[hero_walk0]);
    blit(&wandimages[0], &heroimages[hero_walk0]);
    blit(&mirelimages[mirel_feet0], &heroimages[hero_walk0]);

    blit(&hatimages[0], &heroimages[hero_walk1]);
    blit(&mirelimages[mirel_head0], &heroimages[hero_walk1]);
    blit(&dressimages[1], &heroimages[hero_walk1]);
    blit(&wandimages[0], &heroimages[hero_walk1]);
    blit(&mirelimages[mirel_feet1], &heroimages[hero_walk1]);

    blit(&hatimages[0], &heroimages[hero_walk2]);
    blit(&mirelimages[mirel_head0], &heroimages[hero_walk2]);
    blit(&dressimages[2], &heroimages[hero_walk2]);
    blit(&wandimages[0], &heroimages[hero_walk2]);
    blit(&mirelimages[mirel_feet2], &heroimages[hero_walk2]);

    blit(&hatimages[0], &heroimages[hero_act0]);
    blit(&mirelimages[mirel_head1], &heroimages[hero_act0]);
    blit(&dressimages[1], &heroimages[hero_act0]);
    blit(&wandimages[0], &heroimages[hero_act0]);
    blit(&mirelimages[mirel_feet1], &heroimages[hero_act0]);

    blit(&hatimages[0], &heroimages[hero_act1]);
    blit(&mirelimages[mirel_head1], &heroimages[hero_act1]);
    blit(&dressimages[1], &heroimages[hero_act1]);
    blit(&wandimages[1], &heroimages[hero_act1]);
    blit(&mirelimages[mirel_feet1], &heroimages[hero_act1]);

    blit(&hatimages[0], &heroimages[hero_act2]);
    blit(&mirelimages[mirel_head2], &heroimages[hero_act2]);
    blit(&dressimages[2], &heroimages[hero_act2]);
    blit(&wandimages[2], &heroimages[hero_act2]);
    blit(&mirelimages[mirel_feet1], &heroimages[hero_act2]);

    blit(&hatimages[0], &heroimages[hero_idle]);
    blit(&mirelimages[mirel_head2], &heroimages[hero_idle]);
    blit(&dressimages[1], &heroimages[hero_idle]);
    blit(&wandimages[0], &heroimages[hero_idle]);
    blit(&mirelimages[mirel_feet1], &heroimages[hero_idle]);

    blit(&hatimages[0], &heroimages[hero_jump]);
    blit(&mirelimages[mirel_head1], &heroimages[hero_jump]);
    blit(&dressimages[0], &heroimages[hero_jump]);
    blit(&wandimages[0], &heroimages[hero_jump]);
    blit(&mirelimages[mirel_feet1], &heroimages[hero_jump]);

    white(&heroimages[hero_walk1], &heroimages[hero_hit]);
}

void Gfx::loadboss(int bossindex)
{
    bossimages.clear();

    bossimages.push_back(sf::Image());
    txt << "gfx/boss" << setfill('0') << setw(3) << bossindex << ".png";
    bossimages[0].LoadFromFile(txt.str());
    txt.str("");
    bossimages[0].SetSmooth(false);
    bossimages.push_back(sf::Image());

    boss_normal = 0;
    boss_hit = 1;

    sf::Color color(0, 0, 0, 0);
    bossimages[boss_hit].Create(128, 128, color);
    bossimages[boss_hit].SetSmooth(false);

    white(&bossimages[boss_normal], &bossimages[boss_hit]);
}

void Gfx::loadmob(int mobindex)
{
    mobimages.clear();

    mobimages.push_back(sf::Image());
    txt << "gfx/mob" << setfill('0') << setw(3) << mobindex << ".png";
    mobimages[0].LoadFromFile(txt.str());
    txt.str("");
    mobimages[0].SetSmooth(false);
    mobimages.push_back(sf::Image());

    mob_normal = 0;
    mob_hit = 1;

    sf::Color color(0, 0, 0, 0);
    mobimages[mob_hit].Create(64, 64, color);
    mobimages[mob_hit].SetSmooth(false);

    white(&mobimages[mob_normal], &mobimages[mob_hit]);
}

void Gfx::loadpet()
{
    petimages.clear();

    petimages.push_back(sf::Image());
    petimages[0].LoadFromFile("gfx/pet_zuzu0.png");
    petimages.push_back(sf::Image());
    petimages[1].LoadFromFile("gfx/pet_zuzu1.png");
    petimages.push_back(sf::Image());
    petimages[2].LoadFromFile("gfx/pet_zuzu2.png");
    petimages.push_back(sf::Image());
    petimages[3].LoadFromFile("gfx/pet_zuzu3.png");
    petimages.push_back(sf::Image());

    for (int i = 0; i < (int)petimages.size() - 1; i++)
        petimages[i].SetSmooth(false);

    pet_walk0 = 0;
    pet_walk1 = 1;
    pet_idle = 2;
    pet_atk = 3;
    pet_hit = 4;

    sf::Color color(0, 0, 0, 0);
    petimages[pet_hit].Create(64, 64, color);
    petimages[pet_hit].SetSmooth(false);

    white(&petimages[pet_atk], &petimages[pet_hit]);
}

void Gfx::loadworld(int worldindex, int worldsplit)
{
    worldskyimages.clear();
    worldbgimages.clear();
    worldcollideimages.clear();
    worldmaskimages.clear();
    worldoccludeimages.clear();

    worldskyimages.push_back(sf::Image());
    txt << "gfx/world_sky" << setfill('0') << setw(3) << worldindex << ".png";
    worldskyimages[0].LoadFromFile(txt.str());
    txt.str("");
    worldskyimages[0].SetSmooth(false);

    for (int i = 0; i < worldsplit; i++)
    {
        worldbgimages.push_back(sf::Image());
        txt << "gfx/world_bg" << setfill('0') << setw(3) << worldindex << "_" << i << ".png";
        worldbgimages[i].LoadFromFile(txt.str());
        txt.str("");
        worldbgimages[i].SetSmooth(false);
    }

    for (int i = 0; i < worldsplit; i++)
    {
        worldcollideimages.push_back(sf::Image());
        txt << "gfx/world_collide" << setfill('0') << setw(3) << worldindex << "_" << i << ".png";
        worldcollideimages[i].LoadFromFile(txt.str());
        txt.str("");
        worldcollideimages[i].SetSmooth(false);
    }

    sf::Color color(0, 0, 0, 0);
    for (int i = 0; i < worldsplit; i++)
    {
        worldmaskimages.push_back(sf::Image());
        txt << "gfx/world_occlude" << setfill('0') << setw(3) << worldindex << "_" << i << ".png";
        worldmaskimages[i].LoadFromFile(txt.str());
        txt.str("");
        worldmaskimages[i].SetSmooth(false);
        worldoccludeimages.push_back(sf::Image());
        worldoccludeimages[i].Create(800, 472, color);
        worldoccludeimages[i].SetSmooth(false);
    }

    int index;
    int x2;
    sf::Color pixel;
    for (int x = 0; x < 800 * worldsplit; x++)
    {
        index = x / 800;
        x2 = x - (800 * index);

        for (int y = 0; y < 472; y++)
        {
            pixel = worldmaskimages[index].GetPixel(x2, y);
            if (pixel.r == 255 && pixel.g == 255 && pixel.b == 255 && pixel.a)
                worldoccludeimages[index].SetPixel(x2, y, worldbgimages[index].GetPixel(x2, y));
        }
    }
}

void Gfx::blit(sf::Image* srcimage, sf::Image* destimage)
{
    sf::Color pixel0, pixel1, pixel2;
    for(int x = 0; x < (int)srcimage->GetWidth(); x++)
    {
        for(int y = 0; y < (int)srcimage->GetHeight(); y++)
        {
            pixel0 = srcimage->GetPixel(x, y);
            pixel1 = destimage->GetPixel(x, y);
            pixel2.r = pixel1.r;
            pixel2.g = pixel1.g;
            pixel2.b = pixel1.b;
            pixel2.a = 255;

            if (pixel0.a < 255 && pixel1.a == 255)
                destimage->SetPixel(x, y, pixel1);
            else if (!pixel1.a)
                destimage->SetPixel(x, y, pixel0);
            else if (pixel0.a && pixel1.a)
                destimage->SetPixel(x, y, pixel2);
        }
    }
}

void Gfx::white(sf::Image* srcimage, sf::Image* destimage)
{
    sf::Color pixel;
    for(int x = 0; x < (int)srcimage->GetWidth(); x++)
    {
        for(int y = 0; y < (int)srcimage->GetHeight(); y++)
        {
            pixel = srcimage->GetPixel(x, y);
            pixel.r = 255;
            pixel.g = 255;
            pixel.b = 255;

            destimage->SetPixel(x, y, pixel);
        }
    }
}
