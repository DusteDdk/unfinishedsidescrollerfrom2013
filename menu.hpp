#ifndef menuClassHeader
#define menuClassHeader

#include <vector>
#include "button.hpp"
#include "checkbox.hpp"
#include "control.hpp"
#include "label.hpp"

#include "buttonset.hpp"
#include "background.hpp"
#include "settings.hpp"
#include "sprite.hpp"

#include "dirs.hpp"
#include "playerprofile.hpp"
#include "itemInfo.hpp"

#include "editor.hpp"

#include "game.hpp"

#include "missioninfo.hpp"

#include "ents/playership.hpp"

#define NUMITEMSLOTS 9

class menuClass {
  private:
    inputClass* inp;
    spriteManagerClass* sprMan;

    spriteClass* curSprite;

    glTextClass* glText;
    bool showCursor;
    void drawCursor();
    uint menuState;
    void updatePlayerInfoLabels();
    enum { menuMain, menuSettings, menuCreateProfile, menuLoadProfile, menuBriefing, menuShop, menuEditor, newGame, runGame,cleanUp, menuQuit };

    void infoBox(string msg);
    void runInfoBox();
    string infoBoxMsg;
    bool showInfoBox;
    int infoBoxClicksLeft;
    spriteClass* infoBoxSprite;
    spriteClass* loadingSprite;

    //Fade effect
    int fadeTo, fadeLeft, fadeTime;
    bool fadeDir;
    GLfloat fadeAlpha;
    //
    playerInfo* playerInfos[8];
    void refreshProfiles();
    spriteClass* pics[4];

    //Pointer to the actual game :s
    gameClass* game;
    gameInfo_t gameInfo; //Passed to the gameClass ctor

    //Mission info
    missionInfoClass mission;

    //MainMenu
    labelClass* profIdInfoLbl[5];
    buttonBase* greyPlayBtn;
    spriteClass* cardSprite[2]; //The "Void" and "loaded" id cards that display various stats
    spriteClass* planet; //The coool planet
    static void quitCall(void* c);
    static void settingsCall(void* c);
    static void createProfileCall(void* c);
    static void loadProfileCall(void* c);
    static void mainMenuPlayCall(void* c);

    //Callbacks (shared)
    static void mainMenuCall(void* c);
    static void mainFadeToMenuCall(void* c);

    //menuBriefing
    static void briefingPlayCall(void* c);
    static void briefingShopCall(void* c);

    //menuShop
    int shopSelItem; //Item selected in shop
    vector<itemInfo> shopItems[5];
    int shopSelHull; //Hull selected in shop
    vector<itemInfo> shopHulls;
    spriteClass* ship; //Pointer to currently selected hull sprite
    int shopNumItems[5];
    int shopItemSlot; //The slot the player selected
    checkBoxClass* itemSlotsChk[NUMITEMSLOTS]; //Slot 1-5 + eng, top, bottom, front
    bool loadedShop; //if 0, load data files for the shop/briefing screens
    void loadShop();
    static void itemChkChangedCall(void* c); //This is called whenever a checkbox is checked/unchecked
    static void shopBackCall(void* c);

    static void shopPrevHullCall(void* c);
    static void shopNextHullCall(void* c);
    static void shopBuyHullCall(void* c);
    static void shopSellHullCall(void* c);

    static void shopPrevItemCall(void* c);
    static void shopNextItemCall(void* c);
    static void shopBuyItemCall(void* c);
    static void shopSellItemCall(void* c);

    //menuCreateProfile
    labelClass *nameLbl, *unameLbl, *saveLbl, *picNextLbl, *picPrevLbl;
    int selPic;
    static void saveProfileCall(void* c);
    static void newUnameCall(void* c);
    static void picNextCall(void* c);
    static void picPrevCall(void* c);

    //Load Career
    int selectedProfile;
    labelClass* profileLbl[8];
    labelClass *loadLbl, *delLbl;
    static void selectProfileCall(void* c);
    static void useProfileCall(void* c);
    static void delProfileCall(void* c);

    //Buttons
    buttonSet* btnSet[10];

    //Settings-Main
    checkBoxClass* chkFullScreen;
    labelClass *applyLbl, *backLbl;
    labelClass *resNextLbl, *resPrevLbl,*chosenResLbl;
    int selMode, numModes;
    SDL_Rect **modes;
    char buf[256];
    static void settingsNextResCall(void* c);
    static void settingsPrevResCall(void* c);
    static void settingsApplyCall(void* c);

    //The editor
    editorClass* editor;

    //Backgrounds
    backgroundClass* bg[10];

  public:

    void setup(inputClass* i, spriteManagerClass* r, glTextClass* t);
    menuClass();
    bool run();
};

#endif
