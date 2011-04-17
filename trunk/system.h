class System
{
    public:
    System();
    bool configurekeys;
    bool resetkeys;
    bool sayajoke;
    bool closedialog;
    int keyleft;
    int keyright;
    int keyjump;
    int keyaction;
    int keypet;

    private:
};

System::System()
{
    configurekeys = false;
    resetkeys = false;
    sayajoke = false;
    closedialog = false;

    keyleft = 0;
    keyright = 0;
    keyjump = 0;
    keyaction = 0;
    keypet = 0;
}
