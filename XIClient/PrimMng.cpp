#include "PrimMng.h"
#include "CTkMenuPrimitive.h"
#include "CTkMenuCtrlData.h"
#include "Strings.h"
#include <string.h>

#define addr(x) (CTkMenuPrimitive**)(&x)
using namespace FFXI::CTk;

CTkHelp* PrimMng::g_pTkHelp{ nullptr };
CTkTitle* PrimMng::g_pTkTitle{ nullptr };
CTkEventMsg* PrimMng::g_pTkEventMsg{ nullptr };

CIwPtcBgWin* PrimMng::g_pIwPtcBgWin{ nullptr };
CIwTopMenu* PrimMng::g_pIwTopMenu{ nullptr };
CIwLobbyMenu* PrimMng::g_pIwLobbyMenu{ nullptr };
CIwPatchSub4* PrimMng::g_pIwPatchSub4{ nullptr };
CIwLicenceMenu* PrimMng::g_pIwLicenceMenu{ nullptr };
CIwNetWin* PrimMng::g_pIwNetWin{ nullptr };
CIwNetWinSub1* PrimMng::g_pIwNetWinSub1{ nullptr };
CIwSelectMenu* PrimMng::g_pIwSelectMenu{ nullptr };
CIwChfWin* PrimMng::g_pIwChfWin{ nullptr };
CIwOnePic* PrimMng::g_pIwOnePic{ nullptr };
CIwRaceMenu* PrimMng::g_pIwRaceMenu{ nullptr };
CIwFaceMenu* PrimMng::g_pIwFaceMenu{ nullptr };
CIwHairMenu* PrimMng::g_pIwHairMenu{ nullptr };
CIwSizeMenu* PrimMng::g_pIwSizeMenu{ nullptr };
CIwJobsMenu* PrimMng::g_pIwJobsMenu{ nullptr };
CYkWndPartyList* PrimMng::g_pYkWndPartyList{ nullptr };
CTkDrawMessageWindow* PrimMng::g_pTkDrawMessageWindow{ nullptr };
CTkTarget* PrimMng::g_pTkTarget{ nullptr };
CTkQueryWindow* PrimMng::g_pTkQueryWindow{ nullptr };
CTkDrawMessageWindow2* PrimMng::g_pTkDrawMessageWindow2{ nullptr };
YkWndYesno* PrimMng::g_pYkWndYesno{ nullptr };
CTkGMTellWindow* PrimMng::g_pTkGMTellWindow{ nullptr };
CIwOkMenu* PrimMng::g_pIwOkMenu{ nullptr };
CFsConf6Win* PrimMng::g_pFsConf6Win{ nullptr };
CFsTitleConfigMenu* PrimMng::g_pFsTitleConfigMenu{ nullptr };

PrimTableEntry PrimMng::PrimTable[] = {
	{ "menu    menuwind", nullptr, 2, '\x01', '\x02', 129},
	{ "menu    socialme", nullptr, 2, '\x01', '\x02', 129 },
	{ "menu    mogcont ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    pupequip", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    region  ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    inline  ", nullptr, 10, '5', '\x06', 0 },
	{ "menu    keypad  ", nullptr, 65538, '\xFF', '\a', 0 },
	{ "menu    keypadus", nullptr, 65538, '\xFF', '\a', 0 },
	{ "menu    keypad2 ", nullptr, 65538, '\xFF', '\a', 0 },
	{ "menu    keyselec", nullptr, 65538, '\xFF', '\a', 0 },
	{ "menu    keypad3 ", nullptr, 65538, '\xFF', '\a', 0 },
	{ "menu    fep     ", nullptr, 8454154, '5', '\a', 0 },
	{ "menu    macro   ", nullptr, 131074, '\x01', '\x02', 137 },
	{ "menu    mcr1pall", nullptr, 262154, '\x01', '\x02', 0 },
	{ "menu    mcr2pall", nullptr, 262154, '\x01', '\x02', 0 },
	{ "menu    mcr1edit", nullptr, 131074, '\x01', '\x02', 129 },
	{ "menu    mcr2edit", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    mcr1long", nullptr, 262154, '\x01', '\x02', 0 },
	{ "menu    mcr2long", nullptr, 262154, '\x01', '\x02', 0 },
	{ "menu    mcr1edlo", nullptr, 131074, '\x01', '\x02', 129 },
	{ "menu    mcr2edlo", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    mcrbedit", nullptr, 65538, '\x01', '\x02', 0 },
	{ "menu    mcrledit", nullptr, 65538, '\x01', '\x02', 0 },
	{ "menu    mcrmenu ", nullptr, 2228226, '\x01', '\x03', 129 },
	{ "menu    mcrselec", nullptr, 2228226, '\x01', '\x03', 129 },
	{ "menu    mcrselop", nullptr, 2162688, '\x01', '\x04', 0 },
	{ "menu    mcres20 ", nullptr, 2228226, '\x01', '\x03', 129 },
	{ "menu    mcresmn ", nullptr, 2162688, '\x01', '\x04', 0 },
	{ "menu    feppalle", nullptr, 65579, '5', '\a', 0 },
	{ "menu    helpwind", addr(g_pTkHelp), 32915, '\x05', '\x02', 0 },
	{ "menu    titlewin", addr(g_pTkTitle), 32915, '\x05', '\x02', 32 },
	{ "menu    logwindo", nullptr, 147, '%', '\x01', 0 },
	{ "menu    logwin2 ", nullptr, 147, '%', '\x01', 0 },
	{ "menu    fulllog ", nullptr, 131074, '!', '\x03', 28 },
	{ "menu    charlnk ", nullptr, 589826, '!', '\x03', 1 },
	{ "menu    rem4line", nullptr, 18, '\x05', '\x04', 0 },
	{ "menu    rem4li2 ", nullptr, 18, '\x05', '\x04', 0 },
	{ "menu    spoolmsg", nullptr, 17, '\x05', '\x03', 0 },
	{ "menu    splmsg2 ", nullptr, 17, '\x05', '\x03', 0 },
	{ "menu    query   ", nullptr, 69650, '\x05', '\x02', 12 },
	{ "menu    gmtell  ", nullptr, 69650, '\xFF', '\x04', 0 },
	{ "menu    equip   ", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    inspect ", nullptr, 131074, '\x01', '\x02', 16 },
	{ "menu    trade   ", nullptr, 196610, '\x01', '\x02', 16 },
	{ "menu    money   ", nullptr, 3, '\x01', '\x02', 4 },
	{ "menu    moneyctr", nullptr, 2, '\x01', '\x02', 10 },
	{ "menu    mcresed ", nullptr, 131074, '\x01', '\x02', 145 },
	{ "menu    gaugewin", nullptr, 3075, '\x01', '\x02', 16 },
	{ "menu    casttime", nullptr, 16777219, '\xFF', '\a', 0 },
	{ "menu    eventtim", nullptr, 3, '\x05', '\a', 0 },
	{ "menu    cnttime ", nullptr, 262155, '\x05', '\x02', 0 },
	{ "menu    dead    ", nullptr, 65538, '\x01', '\x02', 0 },
	{ "menu    bank    ", nullptr, 65538, '\x01', '\x02', 16 },
	{ "menu    delivery", nullptr, 65538, '\x01', '\x02', 16 },
	{ "menu    handover", nullptr, 66050, '\x01', '\x02', 16 },
	{ "menu    gift    ", nullptr, 65538, '\x01', '\x02', 0 },
	{ "menu    post1   ", nullptr, 65538, '\x01', '\x02', 16 },
	{ "menu    post2   ", nullptr, 65538, '\x01', '\x02', 0 },
	{ "menu    stringdl", nullptr, 65538, '\x01', '\x02', 0 },
	{ "menu    titlehan", nullptr, 32787, '\x05', '\x03', 32 },
	{ "menu    targetwi", nullptr, 33563649, '%', '\x02', 2 },
	{ "menu    subwindo", nullptr, 16385, '\x01', '\x02', 128 },
	{ "menu    playermo", nullptr, 134290434, '!', '\x02', 4 },
	{ "menu    chatctrl", nullptr, 135682, '\x01', '\x02', 8 },
	{ "menu    magselec", nullptr, 536877568, '\x01', '\x02', 8 },
	{ "menu    mgcmenu ", nullptr, 538968064, '\x01', '\x02', 129 },
	{ "menu    abiselec", nullptr, 538972672, '\x01', '\x02', 4 },
	{ "menu    abimenu ", nullptr, 538968064, '\x01', '\x02', 129 },
	{ "menu    loot    ", nullptr, 196610, '\x01', '\x02', 16 },
	{ "menu    lnowin  ", nullptr, 524290, '\x01', '\x02', 0 },
	{ "menu    lootope ", nullptr, 65536, '\x01', '\x02', 0 },
	{ "menu    inventor", nullptr, 196610, '\x01', '\x02', 0 },
	{ "menu    iteminfo", nullptr, 20971523, '\x05', '\x02', 8 },
	{ "menu    itemxinf", nullptr, 29360130, '\x05', '\x06', 0 },
	{ "menu    itemyinf", nullptr, 29360131, '\x05', '\x06', 0 },
	{ "menu    iuse    ", nullptr, 2162690, '\x01', '\x02', 129 },
	{ "menu    itmsortw", nullptr, 524290, '\x01', '\x02', 1 },
	{ "menu    itemctrl", nullptr, 524290, '\x01', '\x02', 4 },
	{ "menu    statcom ", nullptr, 2228226, '\x01', '\x02', 129 },
	{ "menu    persona ", nullptr, 1, '\x01', '\x02', 16 },
	{ "menu    level   ", nullptr, 1, '\x01', '\x02', 4 },
	{ "menu    joblevel", nullptr, 65537, 'A', '\x02', 0 },
	{ "menu    profile ", nullptr, 1, '\x01', '\x02', 0 },
	{ "menu    btlskill", nullptr, 1, '\x01', '\x02', 0 },
	{ "menu    mgcskill", nullptr, 1, '\x01', '\x02', 0 },
	{ "menu    trdskill", nullptr, 1, '\x01', '\x02', 0 },
	{ "menu    partywin", nullptr, 131, '\x05', '\x03', 0 },
	{ "menu    raid1   ", nullptr, 3, '\x05', '\x02', 0 },
	{ "menu    raid2   ", nullptr, 3, '\x05', '\x02', 0 },
	{ "menu    prty1   ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty5   ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty5us ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty5fr ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty5de ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    alarm   ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    levelsyn", nullptr, 196608, '\x01', '\x02', 28 },
	{ "menu    prty2   ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty3   ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty7   ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty4   ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prty8   ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    prtyjoin", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    myroom  ", nullptr, 4098, '\x01', '\x02', 4 },
	{ "menu    rmpost  ", nullptr, 4096, '\x01', '\x02', 4 },
	{ "menu    mogdoor ", nullptr, 200706, '\x01', '\x02', 4 },
	{ "menu    mogext  ", nullptr, 4098, '\x01', '\x02', 4 },
	{ "menu    mogpost ", nullptr, 4098, '\x01', '\x02', 4 },
	{ "menu    roomlist", nullptr, 200706, '\x01', '\x02', 4 },
	{ "menu    bankmenu", nullptr, 131074, '\x01', '\x02', 129 },
	{ "menu    jobchang", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    jobcsel ", nullptr, 135168, '\x01', '\x02', 16 },
	{ "menu    jobcselu", nullptr, 135168, '\x01', '\x02', 16 },
	{ "menu    tskill1 ", nullptr, 196610, '\x01', '\x02', 16 },
	{ "menu    tskill2 ", nullptr, 1, '\x01', '\x02', 0 },
	{ "menu    trddummy", nullptr, 2, '1', '\x02', 0 },
	{ "menu    link5   ", nullptr, 196608, '%', '\x02', 0 },
	{ "menu    lscswin ", nullptr, 524290, '%', '\x02', 1 },
	{ "menu    link12  ", nullptr, 196608, '\x01', '\x02', 12 },
	{ "menu    link11  ", nullptr, 65538, '\x01', '\x02', 16 },
	{ "menu    link6   ", nullptr, 1, '%', '\x02', 0 },
	{ "menu    link1   ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link2   ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link3   ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link4   ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link13  ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link7   ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link8   ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link9   ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    link10  ", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    scresult", nullptr, 196608, '\x01', '\x02', 28 },
	{ "menu    scoption", nullptr, 2162688, '\x01', '\x02', 129 },
	{ "menu    searchma", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    scsibori", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    arealist", nullptr, 200706, '%', '\x03', 0 },
	{ "menu    searchjo", nullptr, 2228224, '\x01', '\x03', 131 },
	{ "menu    scchar  ", nullptr, 131074, '\x01', '\x02', 129 },
	{ "menu    searchle", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    searchna", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    searchra", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    scname  ", nullptr, 131074, '\x01', '\x02', 129 },
	{ "menu    comgenre", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    sccompar", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    sccomite", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    sccomls ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    bluinven", nullptr, 2, '\x05', '\x02', 129 },
	{ "menu    bluehelp", nullptr, 3, '\x05', '\x03', 0 },
	{ "menu    bluequip", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    bluepoin", nullptr, 1, '\x01', '\x02', 12 },
	{ "menu    bluesibo", nullptr, 2097152, '\x01', '\x03', 0 },
	{ "menu    automato", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    statpup ", nullptr, 2097153, '\x01', '\x02', 28 },
	{ "menu    pupsibor", nullptr, 0, '\x01', '\x02', 0 },
	{ "menu    prvdungp", nullptr, 4325378, '\x05', '\x02', 193 },
	{ "menu    prvdunsi", nullptr, 0, '\x01', '\x02', 0 },
	{ "menu    chocobor", nullptr, 196608, '\x05', '\x02', 92 },
	{ "menu    racecard", nullptr, 196608, '\x05', '\x02', 92 },
	{ "menu    shop    ", nullptr, 196610, '\x01', '\x02', 16 },
	{ "menu    shopmain", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    shopbuy ", nullptr, 2, '\x01', '\x02', 4 },
	{ "menu    shopsell", nullptr, 2, '\x01', '\x02', 4 },
	{ "menu    guildsho", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    bazaar  ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    comment ", nullptr, 131074, '\x01', '\x02', 12 },
	{ "menu    friend  ", nullptr, 196608, '\x01', '\x02', 28 },
	{ "menu    flistmai", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    flmes   ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    olstat  ", nullptr, 0, '\x01', '\x02', 16 },
	{ "menu    oplev   ", nullptr, 0, '\x01', '\x02', 0 },
	{ "menu    commenu ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    msgline ", nullptr, 2, '\x01', '\x03', 0 },
	{ "menu    msglist ", nullptr, 196608, '\x01', '\x02', 28 },
	{ "menu    mes1rcv ", nullptr, 2097152, '\x01', '\x02', 129 },
	{ "menu    mes2frnd", nullptr, 2097152, '\x01', '\x02', 129 },
	{ "menu    netstat ", nullptr, 32915, '\x05', '\x02', 0 },
	{ "menu    blklist ", nullptr, 196608, '\x01', '\x02', 28 },
	{ "menu    blkmain ", nullptr, 2097152, '\x01', '\x02', 129 },
	{ "menu    statcom2", nullptr, 2228226, '\x01', '\x02', 129 },
	{ "menu    meritcat", nullptr, 196610, '\x01', '\x02', 28 },
	{ "menu    merit1  ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    merit2  ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    merit3  ", nullptr, 65536, '\x01', '\x02', 129 },
	{ "menu    levmerit", nullptr, 1, '\x01', '\x02', 4 },
	{ "menu    meritinf", nullptr, 1, '\x01', '\x02', 0 },
	{ "menu    merityn ", nullptr, 200706, 'A', '\b', 0 },
	{ "menu    merit2ca", nullptr, 196610, '\x01', '\x02', 28 },
	{ "menu    levmeri2", nullptr, 1, '\x01', '\x02', 0 },
	{ "menu    jbpcat  ", nullptr, 196610, '\x01', '\x02', 28 },
	{ "menu    ut_menu ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    ut_point", nullptr, 8193, '%', '\x02', 2 },
	{ "menu    map0    ", nullptr, 2228226, '\x05', '\x02', 129 },
	{ "menu    mapframe", nullptr, 131138, '\x05', '\0', 94 },
	{ "menu    mapv3   ", nullptr, 2097154, '\x05', '\x02', 129 },
	{ "menu    mapv2   ", nullptr, 2097152, '\x05', '\x02', 0 },
	{ "menu    scanlist", nullptr, 65536, '\x05', '\x02', 0 },
	{ "menu    mapscan ", nullptr, 2097152, '\x05', '\x02', 129 },
	{ "menu    maplist ", nullptr, 200706, '\x05', '\x03', 0 },
	{ "menu    cnqframe", nullptr, 131138, '\x01', '\0', 94 },
	{ "menu    conquer1", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    conq6sta", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    conq3pts", nullptr, 8193, '\x01', '\x02', 0 },
	{ "menu    beseige1", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    beseige2", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    imperial", nullptr, 8193, '\x01', '\x02', 0 },
	{ "menu    campresu", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    campsan ", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    camparea", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    allied  ", nullptr, 8193, '\x01', '\x02', 0 },
	{ "menu    coloresu", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    colorank", nullptr, 1, '\x01', '\x02', 129 },
	{ "menu    colopoin", nullptr, 8193, '\x01', '\x02', 0 },
	{ "menu    auc1    ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    auc2    ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucweapo", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucammo ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucarmor", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucmagic", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucmater", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucfood ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucmeals", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    aucitem ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    auclist ", nullptr, 196608, '\x01', '\x02', 16 },
	{ "menu    auchisto", nullptr, 196608, '\x01', '\x02', 16 },
	{ "menu    auc3    ", nullptr, 2097152, '\x01', '\x02', 129 },
	{ "menu    auc4    ", nullptr, 2097152, '\x01', '\x02', 129 },
	{ "menu    wepsort ", nullptr, 2097152, '\x01', '\x02', 129 },
	{ "menu    armsort ", nullptr, 2097152, '\x01', '\x02', 129 },
	{ "menu    comyn   ", nullptr, 200706, 'A', '\b', 0 },
	{ "menu    resyn   ", nullptr, 69634, '\x01', '\x02', 0 },
	{ "menu    resynus ", nullptr, 69634, '\x01', '\x02', 0 },
	{ "menu    resynde ", nullptr, 69634, '\x01', '\x02', 0 },
	{ "menu    resynfr ", nullptr, 69634, '\x01', '\x02', 0 },
	{ "menu    passinpu", nullptr, 18, '\x05', '\x03', 0 },
	{ "menu    ranking ", nullptr, 16, '\x05', '\x03', 0 },
	{ "menu    faqsub  ", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    faqmain ", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    quest00 ", nullptr, 196610, '\x01', '\x02', 28 },
	{ "menu    quest01 ", nullptr, 65546, '\x01', '\x02', 0 },
	{ "menu    miss00  ", nullptr, 196610, '\x01', '\x02', 28 },
	{ "menu    missionm", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    evitem  ", nullptr, 196610, '\x05', '\x02', 28 },
	{ "menu    evitem01", nullptr, 65546, '\x01', '\x02', 0 },
	{ "menu    mgcsortw", nullptr, 524290, '\x01', '\x02', 1 },
	{ "menu    blusortw", nullptr, 524290, '\x01', '\x02', 1 },
	{ "menu    magic   ", nullptr, 536875010, '\x01', '\x02', 12 },
	{ "menu    sortyn  ", nullptr, 2097154, '\x01', '\x02', 0 },
	{ "menu    abisortw", nullptr, 524290, '\x01', '\x02', 1 },
	{ "menu    ability ", nullptr, 536875010, '\x01', '\x02', 12 },
	{ "menu    emote   ", nullptr, 536875010, '\x01', '\x02', 12 },
	{ "menu    mount   ", nullptr, 536875010, '\x01', '\x02', 12 },
	{ "menu    buff    ", nullptr, 147, '\x15', '\0', 64 },
	{ "menu    guide00 ", nullptr, 196610, '\x01', '\x02', 28 },
	{ "menu    guide01 ", nullptr, 65546, '\x01', '\x02', 0 },
	{ "menu    configwi", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    conf1win", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    conf1win", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    conf2win", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    k1assign", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    k2assign", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    keylayou", nullptr, 3, '\x01', '\x04', 0 },
	{ "menu    cfilter ", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    conftxtc", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    textcol1", nullptr, 196610, '\x01', '\x02', 12 },
	{ "menu    textcol2", nullptr, 0, '\x01', '\x02', 16 },
	{ "menu    textcol3", nullptr, 131074, '\x01', '\x02', 16 },
	{ "menu    confyn  ", nullptr, 65536, '\x01', '\x02', 0 },
	{ "menu    conf11m ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    conf11l ", nullptr, 2293760, '\x01', '\x02', 129 },
	{ "menu    conf11s ", nullptr, 196610, '\x01', '\x02', 12 },
	{ "menu    conf3win", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    conf7   ", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    fxfilter", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    conf4   ", nullptr, 131072, '\x01', '\x02', 28 },
	{ "menu    conf5m  ", nullptr, 2228224, '\x01', '\x02', 129 },
	{ "menu    conf5win", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    conf5w1 ", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    conf5w2 ", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    conf6win", addr(g_pFsConf6Win), 131074, '\x01', '\x02', 28 },
	{ "menu    conf12wi", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    conf13wi", nullptr, 131074, '\x01', '\x02', 28 },
	{ "menu    rmlo1   ", nullptr, 135168, '\x01', '\x02', 4 },
	{ "menu    rmlo2   ", nullptr, 135168, '\x01', '\x02', 4 },
	{ "menu    rmlo4   ", nullptr, 135168, '\x01', '\x02', 4 },
	{ "menu    plnt1   ", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    plnt2   ", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    plnt3   ", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    plnt4   ", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    plnt5   ", nullptr, 135170, '\x01', '\x02', 4 },
	{ "menu    plnt6   ", nullptr, 135170, '\x01', '\x02', 4 },
	{ FFXI::Constants::Strings::MenuCharMakeRace, addr(g_pIwRaceMenu), 65538, '\b', '\x02', 0 },
	{ FFXI::Constants::Strings::MenuCharMakeFace, addr(g_pIwFaceMenu), 65538, '\b', '\x02', 0 },
	{ FFXI::Constants::Strings::MenuCharMakeHair, addr(g_pIwHairMenu), 65538, '\b', '\x02', 0 },
	{ FFXI::Constants::Strings::MenuCharMakeSize, addr(g_pIwSizeMenu), 65538, '\b', '\x02', 0 },
	{ FFXI::Constants::Strings::MenuCharMakeJobs, addr(g_pIwJobsMenu), 65538, '\b', '\x02', 0 },
	{ "menu    chmkname", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    chmkserv", nullptr, 65539, '\b', '\x02', 0 },
	{ "menu    chmktown", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    chmkpass", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    worldsel", nullptr, 65538, '\b', '\x03', 0 },
	{ "menu    race1   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    race2   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    race3   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    race4   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    race5   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    race6   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    race7   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    race8   ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    nation1 ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    nation2 ", nullptr, 35, '\b', '\x02', 0 },
	{ "menu    nation3 ", nullptr, 35, '\b', '\x02', 0 },
	{ Constants::Strings::MenuLoby1Win, addr(g_pIwTopMenu), 15, '\b', '\x02', 0 },
	{ Constants::Strings::MenuLoby2Win, addr(g_pIwLobbyMenu), 65582, '\b', '\x02', 0 },
	{ Constants::Strings::MenuPtcBGWin, addr(g_pIwPtcBgWin), 1114159, '\b', '\x02', 0 },
	{ Constants::Strings::MenuDbNameSE, addr(g_pIwSelectMenu), 65582, '\b', '\x02', 0 },
	{ "menu    hnbackwi", nullptr, 1114159, '\b', '\x02', 0 },
	{ Constants::Strings::MenuChfWin, addr(g_pIwChfWin), 524290, '\b', '\x02', 1 },
	{ "menu    chswin  ", nullptr, 65538, '\b', '\x03', 1 },
	{ "tkdebug dbdelsel", nullptr, 65582, '\b', '\x02', 0 },
	{ Constants::Strings::MenuLobycwin, addr(g_pFsTitleConfigMenu), 65582, '\b', '\x02', 0 },
	{ Constants::Strings::MenuNetWait, addr(g_pIwNetWin), 65542, '\xFF', '\x03', 0 },
	{ Constants::Strings::MenuNetBar, addr(g_pIwNetWinSub1), 65574, '\xFF', '\x03', 0 },
	{ "menu    ptc1prog", nullptr, 65582, '\b', '\x02', 0 },
	{ "menu    ptc10che", nullptr, 65582, '\b', '\x02', 0 },
	{ "menu    ptc3bar1", nullptr, 65574, '\b', '\x03', 0 },
	{ "menu    ptc4bar2", nullptr, 65574, '\b', '\x03', 0 },
	{ Constants::Strings::MenuLobyHelp, addr(g_pIwPatchSub4), 65539, '\b', '\x03', 0 },
	{ "menu    ptc2warn", nullptr, 65543, '\b', '\x03', 0 },
	{ Constants::Strings::MenuOk, addr(g_pIwOkMenu), 196674, '\xFF', '\b', 0 },
	{ "menu    ptc6yesn", nullptr, 65538, '\b', '\x03', 0 },
	{ "menu    ptc9dele", nullptr, 65538, '\b', '\x03', 0 },
	{ Constants::Strings::MenuLobyLice, addr(g_pIwLicenceMenu), 65538, '\b', '\x03', 0 },
	{ "menu    ptc7vup ", nullptr, 65538, '\b', '\x03', 0 },
	{ "menu    hnhead  ", nullptr, 2, '\b', '\x02', 0 },
	{ "menu    hn1blank", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hn1exist", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hn2blank", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hn2exist", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hn3blank", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hn3exist", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hnwarnin", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hncansel", nullptr, 589826, '\b', '\x02', 0 },
	{ "menu    hnyesno ", nullptr, 65538, '\b', '\x02', 0 },
	{ "menu    hnnaming", nullptr, 65538, '\b', '\x02', 0 },
	{ FFXI::Constants::Strings::MenuDummy, addr(g_pIwOnePic), 65895, '\xFF', '\x02', 0 },
	{ "menu    mp_dead ", nullptr, 67174402, '\x01', '\x02', 0 },
	{ "menu    mp_stat ", nullptr, 67108865, '\x01', '\x02', 16 },
	{ "menu    mp_pmode", nullptr, 67181570, '!', '\x02', 4 },
	{ "menu    mp_abisc", nullptr, 2232832, '\x01', '\x02', 4 },
	{ "menu    mp_mnabi", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    mp_mncst", nullptr, 131074, '\x01', '\x02', 129 },
	{ "menu    mp_invnt", nullptr, 131074, '\x01', '\x02', 129 },
	{ "menu    mp_mnnm ", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    mp_cstm ", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    mp_mntp2", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    mp_kncst", nullptr, 131074, '\x01', '\x02', 0 },
	{ "menu    mnstorag", nullptr, 131074, '\x01', '\x03', 0 },
	{ "menu    mcrmogc ", nullptr, 131074, '\x01', '\x03', 0 },
	{ "menu    cmbmenu ", nullptr, 131072, '\x01', '\x02', 129 },
	{ "menu    cmbhlst ", nullptr, 196610, '\x01', '\x02', 0 },
	{ "menu    cmbhwin ", nullptr, 524290, '\x01', '\x02', 0 },
	{"", 0, 0, 1, 2, 0}
};


CTkMenuPrimitive* PrimMng::FindPrimMenu(char* a1, bool* a2)
{
	int v2 = 0;
	PrimTableEntry* v3{ nullptr }, * v4{ nullptr };

	if (PrimMng::PrimTable[0].MenuName[0]) {
		v3 = PrimMng::PrimTable;
		v4 = v3;
		while (strncmp(v3->MenuName, a1, 0x10u)) {
			char v5 = v4[1].MenuName[0];
			++v4;
			++v2;
			v3 = v4;
			if (!v5) {
				if (a2)
					*a2 = false;
				return nullptr;
			}
		}

		if (a2)
			*a2 = true;
		if (!PrimMng::PrimTable[v2].PrimMenuPtr)
			return nullptr;

		return *PrimMng::PrimTable[v2].PrimMenuPtr;
	}

	if (a2)
		*a2 = false;

	return nullptr;
}

void PrimMng::MenuPrimitiveLink(CTkMenuCtrlData* a1, char* a2)
{

	if (PrimMng::PrimTable[0].MenuName[0]) {
		PrimTableEntry* pte = PrimMng::PrimTable;
		while (strcmp(a2, pte->MenuName)) {
			++pte;
			if (!pte->MenuName[0])
				return;
		}
		a1->field_30 = pte->SomeByte1;
		a1->field_34 = pte->SomeInt;
		a1->MaybeDrawLayer = pte->SomeByte2;
		a1->field_38 = pte->SomeWord;
		if (pte->PrimMenuPtr) {
			a1->MenuPrimitive = *pte->PrimMenuPtr;
			a1->MenuPrimitive->MenuCtrlData = a1;
		}
		else {
			a1->MenuPrimitive = nullptr;
		}
		a1->field_99 = (a1->field_34 & 0x100000) != 0;
	}
}