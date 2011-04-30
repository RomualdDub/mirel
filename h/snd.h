class Snd
{
    public:
    Snd();
    ~Snd();
    void loadmob(int mobindex);
    void loadboss(int bossindex);
    sf::Sound explode;
    sf::Sound coin;
    sf::Sound potionl;
    sf::Sound potionm;
    sf::SoundBuffer buffercoin;
    sf::SoundBuffer bufferpotionl;
    sf::SoundBuffer bufferpotionm;
    sf::SoundBuffer bufferrepop;
    sf::SoundBuffer bufferherohit;
    sf::SoundBuffer bufferfootstep;
    sf::SoundBuffer buffermobhit;
    sf::SoundBuffer bufferexplode;
    vector<sf::SoundBuffer> buffermobvoices;
    vector<sf::SoundBuffer> bufferbossvoices;

    private:
    std::ostringstream txt;
};

Snd::Snd()
{
    // load universal resources

    buffercoin.LoadFromFile("sounds/coin.ogg");
    coin.SetBuffer(buffercoin);
    coin.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
    coin.SetAttenuation(17.5f);

    bufferpotionl.LoadFromFile("sounds/potionl.ogg");
    potionl.SetBuffer(bufferpotionl);
    potionl.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
    potionl.SetAttenuation(17.5f);

    bufferpotionm.LoadFromFile("sounds/potionm.ogg");
    potionm.SetBuffer(bufferpotionm);
    potionm.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
    potionm.SetAttenuation(17.5f);

    bufferexplode.LoadFromFile("sounds/explode.ogg");
    explode.SetBuffer(bufferexplode);
    explode.SetMinDistance(sqrt(200.f * 200.f + 300.f * 300.f));
    explode.SetAttenuation(17.5f);

    buffermobhit.LoadFromFile("sounds/mobhit.ogg");
    bufferherohit.LoadFromFile("sounds/herohit.ogg");
    bufferrepop.LoadFromFile("sounds/repop.ogg");
    bufferfootstep.LoadFromFile("sounds/footstep.ogg");
}

Snd::~Snd()
{
    buffermobvoices.clear();
    bufferbossvoices.clear();
}

void Snd::loadmob(int mobindex)
{
    buffermobvoices.clear();
    buffermobvoices.push_back(sf::SoundBuffer());
    txt << "sounds/mob" << setfill('0') << setw(3) << mobindex << ".ogg";
    buffermobvoices.front().LoadFromFile(txt.str());
    txt.str("");
}

void Snd::loadboss(int bossindex)
{
    bufferbossvoices.clear();
    bufferbossvoices.push_back(sf::SoundBuffer());
    txt << "sounds/boss" << setfill('0') << setw(3) << bossindex << ".ogg";
    bufferbossvoices.front().LoadFromFile(txt.str());
    txt.str("");
}
