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
    bool useitem;
    bool deleteitem;
    bool closedialogitem;
    int itemindex;
    bool sceneether;
    bool ether_v;
    bool ether_hl;
    bool ether_hr;
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

    useitem = false;
    deleteitem = false;
    closedialogitem = false;
    itemindex = 0;

    sceneether = false;
    ether_v = false;
    ether_hl = false;
    ether_hr = false;
}
