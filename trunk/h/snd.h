class Snd
{
    public:
    Snd();
    sf::Sound explode;
    sf::Sound coin;
    sf::Sound potionl;
    sf::Sound potionm;
    sf::Sound mobhit;
    sf::Sound bosshit;
    sf::Sound herohit;
    sf::Sound petbadhit;
    sf::Sound mobrepop;
    sf::Sound bossrepop;
    sf::Sound herorepop;
    sf::Sound footstep;

    private:
    sf::SoundBuffer bufferexplode;
    sf::SoundBuffer buffercoin;
    sf::SoundBuffer bufferpotionl;
    sf::SoundBuffer bufferpotionm;
    sf::SoundBuffer buffermobhit;
    sf::SoundBuffer bufferherohit;
    sf::SoundBuffer bufferrepop;
    sf::SoundBuffer bufferfootstep;
};

Snd::Snd()
{
    // load universal resources

    bufferexplode.LoadFromFile("sounds/explode.ogg");
    explode.SetBuffer(bufferexplode);

    buffercoin.LoadFromFile("sounds/coin.ogg");
    coin.SetBuffer(buffercoin);

    bufferpotionl.LoadFromFile("sounds/potionl.ogg");
    potionl.SetBuffer(bufferpotionl);

    bufferpotionm.LoadFromFile("sounds/potionm.ogg");
    potionm.SetBuffer(bufferpotionm);

    buffermobhit.LoadFromFile("sounds/mobhit.ogg");
    bosshit.SetBuffer(buffermobhit);
    mobhit.SetBuffer(buffermobhit);
    mobhit.SetPitch(2);

    bufferherohit.LoadFromFile("sounds/herohit.ogg");
    herohit.SetBuffer(bufferherohit);
    herohit.SetPitch(2);

    petbadhit.SetBuffer(bufferherohit);
    petbadhit.SetPitch(4);

    bufferrepop.LoadFromFile("sounds/repop.ogg");
    bossrepop.SetBuffer(bufferrepop);
    mobrepop.SetBuffer(bufferrepop);
    mobrepop.SetPitch(2);
    herorepop.SetBuffer(bufferrepop);
    herorepop.SetPitch(4);

    bufferfootstep.LoadFromFile("sounds/footstep.ogg");
    footstep.SetBuffer(bufferfootstep);
}
