#include "menu.hpp"

extern bool initGL(int x, int y, bool fullScreen);
extern settingsClass set;
extern struct sceneInfo_t sceneInfo;
extern playerClass player;
extern dirsClass dir;
extern int ticks;

menuClass::menuClass() {

    /* Get available fullscreen/hardware modes */
  modes=SDL_ListModes(NULL, SDL_FULLSCREEN);

  /* Check is there are any modes available */
  if(modes == (SDL_Rect **)0){
    cout << "SDL Error: No video modes found!" << endl;
  }

  selMode=0;
  for(int i=0; modes[i]; ++i)
  {
    if(set.gameX == modes[i]->w && set.gameY == modes[i]->h)
    {
      selMode=i;
    }
    numModes = i;
  }

  sprintf(buf,"%i x %i",modes[selMode]->w, modes[selMode]->h);

  for(int i=0; i < 8; i++)
  {
    playerInfos[i]=NULL;
  }

}

void menuClass::refreshProfiles()
{
  //Clear any existing
  for(int i=0; i < 8; i++)
  {
    if(playerInfos[i])
    {
      delete playerInfos[i];
    }
  }

  player.loadProfiles(playerInfos);
  for(int i=0; i < 8; i++)
  {
    if(playerInfos[i])
    {
       profileLbl[i]->disable=0;
       profileLbl[i]->selected=0;
       profileLbl[i]->text = playerInfos[i]->name;
    } else {
      profileLbl[i]->text = "Empty Slot";
       profileLbl[i]->disable=1;
       profileLbl[i]->selected=0;
    }
  }
}

void menuClass::updatePlayerInfoLabels()
{
      sprintf(buf, "Pilot:%s", playerInfos[selectedProfile]->name);
      profIdInfoLbl[0]->text = buf;
      sprintf(buf, "Ship:%s", playerInfos[selectedProfile]->shipName);
      profIdInfoLbl[1]->text = buf;
      sprintf(buf, "Mission:%i", playerInfos[selectedProfile]->mission);
      profIdInfoLbl[2]->text = buf;
      sprintf(buf, "Kills:%i", playerInfos[selectedProfile]->enemiesKilled);
      profIdInfoLbl[3]->text = buf;
      sprintf(buf, "Credits:%i", playerInfos[selectedProfile]->credits);
      profIdInfoLbl[4]->text =buf;
}

void menuClass::infoBox(string msg)
{
  infoBoxMsg=msg;
  showInfoBox=1;
  infoBoxClicksLeft=2;
  SDL_WarpMouse(400,300);
}

void menuClass::runInfoBox()
{
  //Check if we should stop showing it
  for(int i=8; i < 128; i++)
  {
    if(inp->keyStates[i])
    {
      inp->keyStates[i]=0;
      infoBoxClicksLeft--;
    }
  }

  if(inp->mouseclick)
  {
    inp->mouseclick=0;
    infoBoxClicksLeft--;
  }

  if(infoBoxClicksLeft<1)
  {
    showInfoBox=0;
    return;
  }

  glDisable( GL_TEXTURE_2D );
  glColor4f( 0,0,0, 0.7 );

  glBegin( GL_QUADS );
  glVertex2f( 0,0 );
  glVertex2f( sceneInfo.X, 0 );
  glVertex2f( sceneInfo.X, sceneInfo.Y );
  glVertex2f( 0, sceneInfo.Y );
  glEnd();
  glEnable( GL_TEXTURE_2D );

  glColor4f( 1,1,1,1 );

  infoBoxSprite->draw();
  glColor4f( 0,1,0,0.5 );
  glText->write( infoBoxMsg, FONT_SMALL, infoBoxSprite->pos.x+88,infoBoxSprite->pos.y+53);

}

void menuClass::loadShop()
{
  bg[menuShop] = new backgroundClass();
  bg[menuShop]->load("/gfx/menu/shopmenu.jpg");


  btnSet[menuShop] = new buttonSet;
  btnSet[menuShop]->init( sprMan );

  //hullPrevtBtn
  btnSet[menuShop]->texture( "largetrans.tex", "hull-prev-hover.tex", "hull-prev-click.tex" );
  btnSet[menuShop]->add(shopPrevHullCall, this, 156, 106 );

  //hullNextBtn
  btnSet[menuShop]->texture( "largetrans.tex", "hull-next-hover.tex", "hull-next-click.tex" );
  btnSet[menuShop]->add(shopNextHullCall, this, 377, 106 );

  //hullBuyBtn
  btnSet[menuShop]->texture( "smalltrans.tex", "hull-buy-hover.tex", "hull-buy-click.tex" );
  btnSet[menuShop]->add(shopBuyHullCall, this, 466, 230 );

  //hullSellBtn
  btnSet[menuShop]->texture( "smalltrans.tex", "hull-sell-hover.tex", "hull-sell-click.tex" );
  btnSet[menuShop]->add(shopSellHullCall, this, 515, 230 );

  //itemPrevBtn
  btnSet[menuShop]->texture( "largetrans.tex", "item-prev-hover.tex", "item-prev-click.tex" );
  btnSet[menuShop]->add(shopPrevItemCall, this, 358, 349 );

  //itemNextBtn
  btnSet[menuShop]->texture( "largetrans.tex", "item-next-hover.tex", "item-next-click.tex" );
  btnSet[menuShop]->add(shopNextItemCall, this, 580, 349 );

  //itemBuyBtn
  btnSet[menuShop]->texture( "smalltrans.tex", "item-buy-hover.tex", "item-buy-click.tex" );
  btnSet[menuShop]->add(shopBuyItemCall, this, 203, 474 );

  //itemSellBtn
  btnSet[menuShop]->texture( "smalltrans.tex", "item-sell-hover.tex", "item-sell-click.tex" );
  btnSet[menuShop]->add(shopSellItemCall, this, 252, 474 );


  //BackBtn
  btnSet[menuShop]->texture( "shop-back-trans.tex", "shop-back-hover.tex", "shop-back-click.tex" );
  btnSet[menuShop]->geometry( 12,2,4,18 );
  btnSet[menuShop]->add(shopBackCall, this, 36, 490 );

  itemInfo tempItem;
  //Init the hulls
  //This is the empty hull, that means that the player has no hull (hull nr 0)
  tempItem.infoPic = sprMan->spawnByFile( "/gfx/menu/buttons/briefbacktrans.tex" );
  tempItem.infoPic->position( 220,162 );
  tempItem.infoTxt = "You have no hull, you\n"
                     "need to buy a new one before\n"
                     "going on a mission.";
  tempItem.price = 0;
  shopHulls.push_back( tempItem );

  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/testhull.tex" ) );
  tempItem.infoPic->position( 220,162 );
  tempItem.infoTxt = "Standard issue\n"
                     "farming class hull.";
  tempItem.price = 150000;
  shopHulls.push_back( tempItem );

  ship = shopHulls.at(0).infoPic;

  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/testhull2.png" ) );
  tempItem.infoPic->position( 220,162 );
  tempItem.infoTxt = "Retired 'n' Refurbished\nlightweight military class\nfigther hull.";
  tempItem.price = 200000;
  shopHulls.push_back( tempItem );


  //Add empty in each
  tempItem.price=0;
  tempItem.infoTxt="This slot is empty.";
  shopItems[0].push_back( tempItem );
  shopItems[1].push_back( tempItem );
  shopItems[2].push_back( tempItem );
  shopItems[3].push_back( tempItem );
  shopItems[4].push_back( tempItem );

  //Items in cargo slots
  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/nuke.tex" ) );
  tempItem.infoPic->position( 419,362 );
  tempItem.infoTxt = "Boomba A-Series\n"
                      "This refurbished nuclear bomb\n"
                      "was.. salvaged from a russian\n"
                      "military storage in the start\n"
                      "of the 21st century. Since\n"
                      "it's universially banned\n"
                      "you didn't buy this from us!\n";
  tempItem.price = 50000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/menu/shop/nukeicon.tex" ) );
  //We don't position cargo slot sprites, they are positioned to their according slot-checkbox
  shopItems[0].push_back( tempItem );
  //cargo box
  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/cargobox.tex" ) );
  tempItem.infoPic->position( 419,362 );
  tempItem.infoTxt = "Cargobox";
  tempItem.price = 15000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/menu/shop/cargoicon.png" ) );
  //We don't position cargo slot sprites, they are positioned to their according slot-checkbox
  shopItems[0].push_back( tempItem );

  //Items in engine slot
  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/gatling2.tex" ) );
  tempItem.infoPic->position( 419,362 );
  tempItem.infoTxt = "Engine";
  tempItem.price = 5000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/ship/engine1.tex" ) );
  tempItem.sprite->position( ship->pos.x+2-30, ship->pos.y+14 );
  shopItems[1].push_back( tempItem );

  //Items in top slot
  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/gatling2.tex" ) );
  tempItem.infoPic->position( 419,362 );
  tempItem.infoTxt = "Top Mount\nGatling type\n3000";
  tempItem.price = 3000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/weapons/topgatling.tex" ) );
  tempItem.sprite->position( ship->pos.x+17, ship->pos.y+2-19 );
  shopItems[2].push_back( tempItem );

  //Items in front slot
  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/gatling2.tex" ) );
  tempItem.infoPic->position( 419,362 );
  tempItem.infoTxt = "Front Mount\nGatling type\n3000";
  tempItem.price = 3000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/weapons/frontgatling.tex" ) );
  tempItem.sprite->position( ship->pos.x+111, ship->pos.y+18 );
  shopItems[3].push_back( tempItem );


  //Items for sale for the bottom slot
  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/gatling2.tex" ) );
  tempItem.infoPic->position( 419,362 );
  tempItem.infoTxt = "4 Barrel Gatling\n"
                     "Standard aviation unit\n"
                     "with slight modifications to\n"
                     "accommodate operation in\n"
                     "deep-space enviroments.\n"
                     "Delivers considerable damage\n"
                     "against small targets.";
  tempItem.price = 3000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/weapons/gatling2.tex" ) );
  tempItem.sprite->position( ship->pos.x+19, ship->pos.y+45 );
  shopItems[4].push_back( tempItem );

  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/gatling1.tex" ) );
  tempItem.infoPic->position( 419,362 );
  tempItem.infoTxt = "8 Barrel Gatling\nDesigned for space combat\nthis is the absolute\npinnacle of human projectile\ntechnology, uses same ammo\nas it's little sister, but at\nquadruple the rate.";
  tempItem.price = 6000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/weapons/gatling1.tex" ) );
  tempItem.sprite->position( ship->pos.x+19, ship->pos.y+45 );
  shopItems[4].push_back( tempItem );


  tempItem.infoPic = sprMan->spawn( sprMan->load( "/gfx/menu/shop/missilebox1.tex" ) );
  tempItem.infoPic->position( 419,400 );
  tempItem.infoTxt = "Box o' missiles (TM)\nThis bottom mounted box\nwill launch a large volume\nof heavy-duty self-propulsed\nmissiles at various angles\nin order to impact a large\narea of heavier targets.";
  tempItem.price = 12000;
  tempItem.sprite = sprMan->spawn( sprMan->load( "/gfx/weapons/missilebox1.tex" ) );
  tempItem.sprite->position( ship->pos.x+16, ship->pos.y+45 );
  shopItems[4].push_back( tempItem );


  shopNumItems[0] = shopItems[0].size();
  shopNumItems[1] = shopItems[1].size();
  shopNumItems[2] = shopItems[2].size();
  shopNumItems[3] = shopItems[3].size();
  shopNumItems[4] = shopItems[4].size();

  shopSelItem=1; //Since 0 = nonexisting

  int itemChkSpriteId[2];
  itemChkSpriteId[0] = sprMan->load( "/gfx/menu/shop/itemslotunsel.tex" );
  itemChkSpriteId[1] = sprMan->load( "/gfx/menu/shop/itemslotsel.tex" );

  //Items 1-5 (0-4)
  itemSlotsChk[0] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[0]->setPos( 49,69 );
  itemSlotsChk[0]->setCallBack( itemChkChangedCall, this );

  itemSlotsChk[1] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[1]->setPos( 49,113 );
  itemSlotsChk[1]->setCallBack( itemChkChangedCall, this );

  itemSlotsChk[2] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[2]->setPos( 49,158 );
  itemSlotsChk[2]->setCallBack( itemChkChangedCall, this );

  itemSlotsChk[3] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[3]->setPos( 49,202 );
  itemSlotsChk[3]->setCallBack( itemChkChangedCall, this );

  itemSlotsChk[4] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[4]->setPos( 49,247 );
  itemSlotsChk[4]->setCallBack( itemChkChangedCall, this );

  //Engine, Top, Front, Bottom
  itemSlotsChk[5] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[5]->setPos( ship->pos.x+3-40, ship->pos.y+14 );
  itemSlotsChk[5]->setCallBack( itemChkChangedCall, this );

  itemSlotsChk[6] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[6]->setPos( ship->pos.x+19, ship->pos.y+3-40 );
  itemSlotsChk[6]->setCallBack( itemChkChangedCall, this );

  itemSlotsChk[7] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[7]->setPos( ship->pos.x+111, ship->pos.y+18 );
  itemSlotsChk[7]->setCallBack( itemChkChangedCall, this );

  itemSlotsChk[8] = new checkBoxClass( itemChkSpriteId, inp, 0, sprMan );
  itemSlotsChk[8]->setPos( ship->pos.x+19, ship->pos.y+46 );

  itemSlotsChk[8]->setCallBack( itemChkChangedCall, this );
//Make this slot chosen
  shopItemSlot=8;
  itemSlotsChk[8]->checked=1;
  itemChkChangedCall(this);


  //chkFullScreen = new checkBoxClass( chkBoxSpriteID, inp, set.fullScreen, sprMan );
  //chkFullScreen->setPos( glText->getLength("Fullscreen ", FONT_MEDIUM)+288,210 );


  loadedShop=1;
}

void menuClass::setup(inputClass* i, spriteManagerClass* r, glTextClass* t)
{
  editor=NULL;
  inp=i;
  glText=t;
  sprMan = r;
  game=NULL;

  menuState=menuMain;
  fadeTo=menuMain;
  fadeDir=0;
  fadeTime=250;
  fadeLeft=0;
  fadeAlpha=1;

  //The "loading" sprite.
  loadingSprite=sprMan->spawnByFile( "/gfx/menu/loading.png" );
  loadingSprite->position( sceneInfo.X/2.0-loadingSprite->halfSize.x, sceneInfo.Y/2.0-loadingSprite->halfSize.y );

  //Info box
  showInfoBox=0;
  infoBoxSprite = sprMan->spawnByFile( "/gfx/menu/infobox.png" );
  infoBoxSprite->position( 400-infoBoxSprite->halfSize.x,300-infoBoxSprite->halfSize.y );

  //bg
  bg[menuMain] = new backgroundClass();
  bg[menuMain]->load("/gfx/menu/mainmenu.jpg"); //The main bg

  bg[menuSettings] = new backgroundClass();
  bg[menuSettings]->load("/gfx/menu/settingsmenu.jpg");

  bg[menuBriefing] = new backgroundClass();
  bg[menuBriefing]->load("/gfx/menu/briefingmenu.jpg");

  //Ny cursor:
  int curSpriteId = sprMan->load("/gfx/menu/cursor.tex");
  curSprite = sprMan->spawn(curSpriteId);
  curSprite->play(1,0,0);
  showCursor=1;


  //Greyed out play btn
  greyPlayBtn = new buttonBase(mainMenuPlayCall, this, 139, 159);
  greyPlayBtn->texture( "maintrans-idle.tex", "play-grey.tex", "play-grey.tex", sprMan );
  greyPlayBtn->setGeometry( 4, 12, 1, -8 );

  //MainScreen buttonset
  btnSet[menuMain] = new buttonSet;
  btnSet[menuMain]->init( sprMan );

  btnSet[menuMain]->geometry( 4, 12, 1, -8);//
  btnSet[menuMain]->texture( "maintrans-idle.tex", "play-hover.tex", "play-click.tex" );
  btnSet[menuMain]->add(mainMenuPlayCall, this, 139, 159 );

  btnSet[menuMain]->geometry( 4, 12, 1, -8);//
  btnSet[menuMain]->texture( "maintrans-idle.tex", "career-hover.tex", "career-click.tex" );
  btnSet[menuMain]->add(createProfileCall, this, 139, 216 );

  btnSet[menuMain]->geometry( 4, 12, 1, -7);//
  btnSet[menuMain]->texture( "maintrans-idle.tex", "loadcareer-hover.tex", "loadcareer-click.tex" );
  btnSet[menuMain]->add(loadProfileCall, this, 139, 273 );

  btnSet[menuMain]->geometry( 4, 12, 1, -6);//
  btnSet[menuMain]->texture( "maintrans-idle.tex", "settings-hover.tex", "settings-click.tex" );
  btnSet[menuMain]->add(settingsCall, this, 139, 329 );

  btnSet[menuMain]->geometry( 4, 11, 1, -6);//
  btnSet[menuMain]->texture( "maintrans-idle.tex", "quit-hover.tex", "quit-click.tex" );
  btnSet[menuMain]->add(quitCall, this, 139, 386 );

  cardSprite[0] = sprMan->spawn(sprMan->load("/gfx/menu/idcard-void.tex"));
  cardSprite[1] = sprMan->spawn(sprMan->load("/gfx/menu/idcard.tex"));

  profIdInfoLbl[0] = new labelClass("Name", glText, FONT_SMALL);
  profIdInfoLbl[0]->setTxtColor(1,0,0,1);
  profIdInfoLbl[1] = new labelClass("Ship", glText, FONT_SMALL);
  profIdInfoLbl[1]->setTxtColor(1,0,0,1);
  profIdInfoLbl[2] = new labelClass("Mission", glText, FONT_TINY);
  profIdInfoLbl[2]->setTxtColor(0,0,0,1);
  profIdInfoLbl[3] = new labelClass("Kills", glText, FONT_TINY);
  profIdInfoLbl[3]->setTxtColor(0,0,0,1);
  profIdInfoLbl[4] = new labelClass("Credits", glText, FONT_TINY);
  profIdInfoLbl[4]->setTxtColor(0,0,0,1);


  int chkBoxSpriteID[2];
  chkBoxSpriteID[0] = sprMan->load("/gfx/menu/checkboxoff.tex");
  chkBoxSpriteID[1] = sprMan->load("/gfx/menu/checkboxon.tex");

  //Settings-Main

  resPrevLbl = new labelClass(" <", glText, FONT_MEDIUM);
  resPrevLbl->setPos( 288+glText->getLength("Res:", FONT_MEDIUM), 177 );
  resPrevLbl->setType(LABEL_CLICKABLE, settingsNextResCall, this, inp);
  resPrevLbl->setHoverBgColor( 0, 0.5, 0, 0.5 );
  resPrevLbl->setTxtColor( 0, 1, 0,  0.5 );

  sprintf(buf, "%i x %i", set.gameX, set.gameY);
  chosenResLbl = new labelClass(buf, glText, FONT_MEDIUM);
  chosenResLbl->setPos( 288+glText->getLength("Res: < > ", FONT_MEDIUM), 177);
  chosenResLbl->setTxtColor( 0, 1, 0,  0.5 );

  resNextLbl = new labelClass(  "> ", glText, FONT_MEDIUM );
  resNextLbl->setPos(288+glText->getLength("Res: < ", FONT_MEDIUM), 177 );
  resNextLbl->setType(LABEL_CLICKABLE, settingsPrevResCall, this, inp);
  resNextLbl->setHoverBgColor( 0, 0.5, 0, 0.5 );
  resNextLbl->setTxtColor( 0, 1, 0,  0.5 );

  backLbl = new labelClass(  "< Back", glText, FONT_MEDIUM );
  backLbl->setPos(288, 385 );
  backLbl->setType(LABEL_CLICKABLE, mainMenuCall, this, inp);
  backLbl->setHoverBgColor( 0, 0.5, 0, 0.5 );
  backLbl->setTxtColor( 0, 1, 0,  0.5 );

  applyLbl= new labelClass(  "Apply", glText, FONT_MEDIUM );
  applyLbl->setPos(570, 385 );
  applyLbl->setType(LABEL_CLICKABLE, settingsApplyCall, this, inp);
  applyLbl->setHoverBgColor( 0, 0.5, 0, 0.5 );
  applyLbl->setTxtColor( 0, 1, 0,  0.5 );


  chkFullScreen = new checkBoxClass( chkBoxSpriteID, inp, set.fullScreen, sprMan );
  chkFullScreen->setPos( glText->getLength("Fullscreen ", FONT_MEDIUM)+288,210 );

  // --- Create Profile/Career
  nameLbl = new labelClass("Name:", glText, FONT_MEDIUM);
  nameLbl->setPos( 288, 305 );
  nameLbl->setTxtColor( 0, 1, 0, 0.5 );
  unameLbl = new labelClass("", glText, FONT_MEDIUM);
  unameLbl->setPos( 288+glText->getLength("Name:", FONT_MEDIUM), 305 );
  unameLbl->setTxtColor( 0, 1, 0, 0.5);
  unameLbl->setType( LABEL_EDITABLE, newUnameCall, this, inp );
  unameLbl->setHoverBgColor(0,0.5,0,0.5);
  unameLbl->maxLen=17;
  saveLbl = new labelClass("Save", glText, FONT_MEDIUM);
  saveLbl->setPos(570,385);
  saveLbl->setTxtColor(0, 1, 0, 0.5);
  saveLbl->setType( LABEL_CLICKABLE, saveProfileCall, this, inp );
  saveLbl->setHoverBgColor(0,0.5,0,0.5);
  picNextLbl = new labelClass(" <", glText, FONT_DEFAULT);
  picNextLbl->setPos(362, 233);
  picNextLbl->setHoverBgColor(0,0.5,0,0.5);
  picNextLbl->setTxtColor(0, 1, 0, 0.5);
  picNextLbl->setType(LABEL_CLICKABLE, picNextCall, this, inp );
  picPrevLbl = new labelClass("> ", glText, FONT_DEFAULT);
  picPrevLbl->setPos(531, 233);
  picPrevLbl->setHoverBgColor(0,0.5,0,0.5);
  picPrevLbl->setType(LABEL_CLICKABLE, picPrevCall, this, inp );
  picPrevLbl->setTxtColor(0, 1, 0, 0.5);


  pics[0] = sprMan->spawn( sprMan->load("/gfx/menu/faces/0.tex") );
  pics[1] = sprMan->spawn( sprMan->load("/gfx/menu/faces/1.tex") );
  pics[2] = sprMan->spawn( sprMan->load("/gfx/menu/faces/2.tex") );
  pics[3] = sprMan->spawn( sprMan->load("/gfx/menu/faces/3.tex") );

  //Load profile
  for(int i=0; i < 8; i++)
  {
    profileLbl[i]= new labelClass("Empty Slot", glText, FONT_MEDIUMSMALL);
    profileLbl[i]->setPos( 288, 150+i*glText->getHeight(FONT_MEDIUMSMALL) );
    profileLbl[i]->setTxtColor(0,1,0,0.5);
    profileLbl[i]->setHoverBgColor(0,0.5,0,0.5);
    profileLbl[i]->setSelectedBgColor(0,0.5,0,0.5);
    profileLbl[i]->setType(LABEL_CLICKABLE, selectProfileCall, this, inp);
    profileLbl[i]->bgLen= 362;
  }

  loadLbl= new labelClass(  "Load", glText, FONT_MEDIUM );
  loadLbl->setPos(570, 385 );
  loadLbl->setType(LABEL_CLICKABLE, useProfileCall, this, inp);
  loadLbl->setHoverBgColor( 0, 0.5, 0, 0.5 );
  loadLbl->setTxtColor( 0, 1, 0,  0.5 );

  delLbl = new labelClass( "Delete", glText, FONT_MEDIUM );
  delLbl->setPos( 422, 385 );
  delLbl->setType(LABEL_CLICKABLE, delProfileCall, this, inp);
  delLbl->setHoverBgColor( 0, 0.5, 0, 0.5 );
  delLbl->setTxtColor( 0, 1, 0,  0.5 );

  //Load profile
  refreshProfiles();
  selectedProfile=set.lastCareer;
  if(selectedProfile != -1)
    updatePlayerInfoLabels();

  //briefing
  btnSet[menuBriefing] = new buttonSet;
  btnSet[menuBriefing]->init( sprMan );
  //Back button
  btnSet[menuBriefing]->geometry( 8, 4, 5, 13);//
  btnSet[menuBriefing]->texture( "briefbacktrans.tex", "briefbackhover.tex", "briefbackclick.tex" );
  btnSet[menuBriefing]->add(mainFadeToMenuCall, this, 200, 466 );
  //Shop Button
  btnSet[menuBriefing]->geometry( 5, 5, 9, 13);//
  btnSet[menuBriefing]->texture( "briefshoptrans.tex", "briefshophover.tex", "briefshopclick.tex" );
  btnSet[menuBriefing]->add(briefingShopCall, this, 264, 464 );
  //play
  btnSet[menuBriefing]->geometry( 2, 2, 32, 33);//
  btnSet[menuBriefing]->texture( "briefplaytrans.tex", "briefplayhover.tex", "briefplayclick.tex" );
  btnSet[menuBriefing]->add(briefingPlayCall, this, 459, 444 );

  //Shop Menu
  loadedShop=0;

  planet = sprMan->spawn( sprMan->load("gfx/menu/planet.tex") );
  planet->setZoom(2.0);
  planet->position( sceneInfo.X-(planet->size.x*2),sceneInfo.Y-(planet->size.y*2) );

}

bool menuClass::run()
{
  switch(menuState)
  {
    case menuMain:
      if(inp->keyPress==SDLK_e)
      {
        inp->keyPress=0;
        fadeTo=menuEditor;
      }
      bg[menuMain]->draw();

      glText->write("Unnamed Sidescrolling Space-Shoot'em up", FONT_DEFAULT, 30,30);
      planet->draw();

      btnSet[menuMain]->run(inp,1);

      GLfloat ofx,ofy;
      ofx = sceneInfo.X - cardSprite[0]->size.x - 64;
      ofy = sceneInfo.Y - cardSprite[0]->size.y - 64;
      if(selectedProfile != -1)
      {
        pics[playerInfos[selectedProfile]->pic]->position(171+ofx, 50+ofy);
        pics[playerInfos[selectedProfile]->pic]->setZoom( 0.578125 );
        pics[playerInfos[selectedProfile]->pic]->draw();
        cardSprite[1]->position(ofx, ofy);
        cardSprite[1]->draw();
        for(int i=0; i < 5; i++)
        {
          profIdInfoLbl[i]->setPos(ofx+4, 32+ofy+i*glText->getHeight(FONT_SMALL));
          profIdInfoLbl[i]->draw();
        }

      } else {
        cardSprite[0]->position(ofx, ofy);
        cardSprite[0]->draw();
        greyPlayBtn->check(inp);
        greyPlayBtn->draw();
      }

    break;
    case menuSettings:
      bg[menuSettings]->draw();
      planet->draw();
      glColor4f( 0,1,0, 0.5);
      glText->write("Settings", FONT_DEFAULT, 385, 119);
      glText->write("Res:", FONT_MEDIUM, 288, 177);
      resPrevLbl->draw();
      resNextLbl->draw();
      chosenResLbl->draw();
      backLbl->draw();
      applyLbl->draw();
      glColor4f( 0,1,0, 0.5);
      glText->write("Fullscreen", FONT_MEDIUM, 288, 210);
      chkFullScreen->draw();
      glColor4f( 1,1,1,1 );
    break;
    case menuCreateProfile:
      bg[menuSettings]->draw();
      planet->draw();
      glColor4f( 0,1,0, 0.5);
      glText->write("New Career", FONT_DEFAULT, 365, 119);
      picPrevLbl->draw();
      pics[selPic]->position(402, 164);
      pics[selPic]->setZoom(1.0);
      pics[selPic]->draw();
      picNextLbl->draw();
      nameLbl->draw();
      unameLbl->draw();
      saveLbl->draw();
      backLbl->draw();
    break;
    case menuLoadProfile:
      bg[menuSettings]->draw();
      planet->draw();
      glColor4f( 0,1,0, 0.5);
      glText->write("Load Career", FONT_DEFAULT, 355, 119);
      for(int i=0; i < 8; i++)
      {
        profileLbl[i]->draw();
      }
      loadLbl->draw();
      delLbl->draw();
      backLbl->draw();
    break;
    case menuBriefing:
      showCursor=1; //Because game returns hereto
      bg[menuBriefing]->draw();
      btnSet[menuBriefing]->run(inp, 1);
      //Mission Name
      glColor4f( 0,1,0, 0.5);
      glText->write( mission.info.missionName, FONT_MEDIUM, 150, 54 );
      //Mission Num //Bounty
      sprintf(buf, "Mission %i Bounty $%i", mission.info.num, mission.info.missionBounty );
      glText->write(buf, FONT_MEDIUMSMALL, 150,77);
      //Draw beiefing text
      glText->write( mission.info.missionBriefing, FONT_TINY, 150, 100);
      //Draw horizontal line
      glText->write( "-----------------------------------------+",FONT_TINY, 150,290);
      //Draw vert line
      glText->write( "|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n \n|\n|\n|\n|",FONT_TINY, 480,54);
      //Draw info about the player
      sprintf(buf, "\nCapt. %s flying %s\n"
                   "\%i Credits and %i points.\n"
                   "%i Kills.(%i shots fired.)\n"
                   "Played for %s",
                    playerInfos[selectedProfile]->name,playerInfos[selectedProfile]->shipName,
                    playerInfos[selectedProfile]->credits,playerInfos[selectedProfile]->score,
                    playerInfos[selectedProfile]->enemiesKilled,playerInfos[selectedProfile]->shotsFired,
                    secToStr(playerInfos[selectedProfile]->timePlayed) );
      glText->write( buf,FONT_TINY, 150,290);
      glColor4f(1,1,1,1);
    break;
    case menuShop:
      if( loadedShop )
      {
        if(inp->keyPress == SDLK_DOLLAR )
        {
          playerInfos[selectedProfile]->credits += 10000;
          inp->keyPress=0;
        }
        bg[menuShop]->draw();
        //Draw credits
        sprintf( buf, "$%i", playerInfos[selectedProfile]->credits );
        glColor4f( 0, 1, 0, 0.5 );
        glText->write( buf, FONT_SMALL, 310,289 );
        //The price
        if( playerInfos[selectedProfile]->shipSlot[shopItemSlot]==shopSelItem )
          sprintf( buf, "Sell for $%i", (int)((float)shopItems[(shopItemSlot < 5)?0:shopItemSlot-4].at(shopSelItem).price/100*80)  );
        else
          sprintf( buf, "Buy for $%i", shopItems[(shopItemSlot < 5)?0:shopItemSlot-4].at(shopSelItem).price );
        if( shopItems[(shopItemSlot < 5)?0:shopItemSlot-4].at(shopSelItem).price >  playerInfos[selectedProfile]->credits && playerInfos[selectedProfile]->shipSlot[shopItemSlot]!=shopSelItem )
          glColor4f( 1,0,0, 0.5 );
        glText->write( buf, FONT_SMALL, 393,355 );
        glText->writeoutlined( shopItems[(shopItemSlot < 5)?0:shopItemSlot-4].at(shopSelItem).infoTxt, FONT_TINY, 145,338 );
        glColor4f( 1, 1, 1, 1 );
        if(shopSelItem > 0)
          shopItems[(shopItemSlot < 5)?0:shopItemSlot-4].at(shopSelItem).infoPic->draw();

        btnSet[menuShop]->run(inp,1);
        shopHulls.at(shopSelHull).infoPic->draw();
        glColor4f( 0, 1, 0, 0.5 );

        if( shopSelHull == playerInfos[selectedProfile]->shipHull )
        {
          int sellPrice = (int)((float)shopHulls.at(shopSelHull).price/100.0*80.0);
         // if(sellPrice+playerInfos[selectedProfile]->credits < shopHulls.at(shopSelHull).price)
            sprintf( buf, "Sell for $%i", (int)((float)shopHulls.at(shopSelHull).price/100.0*80.0));

        } else {
        //  if((int)playerInfos[selectedProfile]->shipHull!=0)
            sprintf( buf, "Buy for $%i", shopHulls.at(shopSelHull).price);

          if(shopHulls.at(shopSelHull).price > playerInfos[selectedProfile]->credits)
          {
            glColor4f( 1,0,0, 0.5 );
          }
        }
        glText->write(buf, FONT_SMALL, 192,112);
        glColor4f( 1, 1, 1, 1 );
        glText->writeoutlined( shopHulls.at(shopSelHull).infoTxt, FONT_TINY, 414,88 );

        //draw currently mounted stuff
        for(int i=0; i < NUMITEMSLOTS; i++)
        {
          itemSlotsChk[i]->draw();

          if( playerInfos[selectedProfile]->shipSlot[i] != 0)
          {
            if( i < slotEngine )
            {
              shopItems[(i < 5)?0:i-4].at( playerInfos[selectedProfile]->shipSlot[i] ).sprite->position( itemSlotsChk[i]->px, itemSlotsChk[i]->py );
            }
            shopItems[(i < 5)?0:i-4].at( playerInfos[selectedProfile]->shipSlot[i] ).sprite->draw();
          }
        }
      } else {
        loadShop();
      }
    break;
    case menuEditor:
      if(editor==NULL)
        editor = new editorClass( inp, glText );
      else
        menuState = editor->run();
    break;
    case newGame: //Init game class
      showCursor=0;
      glColor4f(1,1,1,1);
      loadingSprite->draw();
      if( fadeTo == -1 )
      {
        gameInfo.mission=&mission.info;
        gameInfo.player=playerInfos[selectedProfile];
        game = new gameClass(sprMan, glText, inp, &gameInfo);
        fadeTo=runGame;
      }
    break;
    case runGame: //Run game
      if(!game->run())
        fadeTo=cleanUp;
        //menuState=cleanUp;
    break;
    case cleanUp: //Remove game class
      glColor4f(1,1,1,1);
      loadingSprite->draw();
      if(game)
      {
        delete game;
        game=NULL;
        //Save profile
        player.saveProfile( playerInfos[selectedProfile] );
      }
      fadeTo=menuBriefing;
    break;
    case menuQuit:
      return(1);
    default:
    //nop :P
    break;
  }

  drawCursor();

  if(showInfoBox)
  {
    runInfoBox();
  }
  ///TODO: replace this with glFx->queue(fadetoblack, time);
  if(fadeTo != -1)
  {
    //printc("Alpha: %.2f Ticks: %i Menu: %i", fadeAlpha, ticks, menuState);
    if(ticks > 34) ticks=16;
    if(!fadeDir)
    {
      fadeLeft -=ticks;
      fadeAlpha += 1.0/(float)fadeTime*(float)ticks;
      if( fadeLeft < 1 )
      {
          menuState=fadeTo;
          fadeDir=1;
      }
    } else {
      fadeLeft +=ticks;
      fadeAlpha -= 1.0/(float)fadeTime*(float)ticks;
      if(fadeLeft > fadeTime-1)
      {
          fadeLeft=fadeTime;
          fadeDir=0;
          fadeTo=-1;
          fadeAlpha=0;
      }
    }

    glDisable( GL_TEXTURE_2D );
    glColor4f( 0,0,0, fadeAlpha );
    glBegin( GL_QUADS );
      glVertex2f( 0, 0 ),
      glVertex2f( sceneInfo.X, 0 );
      glVertex2f( sceneInfo.X,  sceneInfo.Y);
      glVertex2f( 0, sceneInfo.Y );
    glEnd( );
    glEnable( GL_TEXTURE_2D );
  }

  return(0);

}

void menuClass::drawCursor()
{
  if(showCursor)
  {
    curSprite->position(inp->mousex, inp->mousey);
    curSprite->draw();
  }
}

void menuClass::quitCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->fadeTo=menuQuit;
}

void menuClass::settingsCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->menuState=menuSettings;
}

///
void menuClass::mainMenuCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->menuState=menuMain;
  that->backLbl->selected=0;

  //Clean up if was called by "Load Profile"
  that->refreshProfiles();
}

void menuClass::mainFadeToMenuCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  if(that->selectedProfile != -1)
  {
    that->fadeTo=menuMain;
  }
  //Refresh info labels on the front screen
  that->updatePlayerInfoLabels();
}

void menuClass::settingsNextResCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->selMode++;
  if(that->selMode > that->numModes)
    that->selMode = 0;
  sprintf(that->buf, "%i x %i", that->modes[that->selMode]->w,that->modes[that->selMode]->h);
  that->chosenResLbl->text = that->buf;
  that->resPrevLbl->selected=0;
}

void menuClass::settingsPrevResCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->selMode--;
  if(that->selMode < 0)
    that->selMode = that->numModes;
  sprintf(that->buf, "%i x %i", that->modes[that->selMode]->w,that->modes[that->selMode]->h);
  that->chosenResLbl->text = that->buf;
  that->resNextLbl->selected=0;
}

void menuClass::settingsApplyCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  //Free GL stuff before loosing it
  that->sprMan->freeGL();
  set.gameX = that->modes[that->selMode]->w;
  set.gameY = that->modes[that->selMode]->h;
  set.fullScreen = that->chkFullScreen->checked;
  if(initGL(set.gameX,set.gameY,set.fullScreen))
  {
    //Save settings
    set.save();
  }
  that->sprMan->refresh();
  that->applyLbl->selected=0;
  that->planet->position( sceneInfo.X-(that->planet->size.x*2),sceneInfo.Y-(that->planet->size.y*2) );

}

//from mainmenu
void menuClass::createProfileCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->menuState=menuCreateProfile;
  that->unameLbl->text = "Name";
  that->selPic=0;

  int free=8;
  for(int i=0; i < 8; i++)
  {
    if(that->playerInfos[i])
    {
      free--;
    }
  }
  if(free==0)
    that->menuState=menuLoadProfile;

}

void menuClass::loadProfileCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->menuState=menuLoadProfile;
}

void menuClass::newUnameCall(void* c)
{

}

void menuClass::saveProfileCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->saveLbl->selected=0;

  if(that->unameLbl->editing)
  {
    //cut last char from
    that->unameLbl->text = that->unameLbl->text.substr(0,that->unameLbl->text.length()-1);
    that->unameLbl->editing=0;
    that->unameLbl->selected=0;
  }

  playerInfo tp;
  tp.file[0] = 0;
  sprintf(tp.name, "%s", that->unameLbl->text.data());
  tp.pic = that->selPic;
  tp.credits=10000;
  sprintf(tp.shipName, "Yggdrasil");
  tp.enemiesKilled = 0;
  tp.mission=(int8_t)0;
  tp.shotsFired=0;
  tp.score=0;
  tp.timePlayed=0;
  for(int i=0; i < 9; i++)
  {
    tp.shipSlot[i]=0;
  }
  tp.shipSlot[slotEngine]=1;
  tp.shipSlot[slotBottom]=1;
  tp.shipHull=1;

  player.saveProfile(&tp);

  that->refreshProfiles();
  that->menuState=menuLoadProfile;
}

void menuClass::picNextCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->selPic++;
  if(that->selPic ==4)
    that->selPic=0;
  that->picNextLbl->selected=0;
}

void menuClass::picPrevCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->selPic--;
  if(that->selPic == -1)
    that->selPic=3;
  that->picPrevLbl->selected=0;
}

void menuClass::selectProfileCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  static bool prevSelected[8]; //Static initialized to 0

  for(int i=0; i < 8; i++)
  {
    if(prevSelected[i])
    {
      that->profileLbl[i]->selected=0;
      prevSelected[i]=0;
    }
    if(that->profileLbl[i]->selected && !prevSelected[i])
    {
      prevSelected[i]=1;
    }
  }
}

void menuClass::useProfileCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);

  bool sel=0;
  for(int i=0; i < 8; i++)
  {
    if(that->profileLbl[i]->selected)
    {
      sel=1;
      that->selectedProfile=i;
      that->updatePlayerInfoLabels();
      set.lastCareer=i;
      set.save();
    }
  }

  that->loadLbl->selected=0;

  if(sel)
  {
    that->menuState=menuMain;
  }
}

void menuClass::delProfileCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);

  bool sel=0;
  int i;
  for(i=0; i < 8; i++)
  {
    if(that->profileLbl[i]->selected)
    {
      sel=1;
      break;
    }
  }

  that->delLbl->selected=0;

  if(sel)
  {
    player.deleteProfile(i);
    that->refreshProfiles();

    if(i == that->selectedProfile)
    {
      that->selectedProfile = -1;
      set.lastCareer=-1;
      set.save();
    }
  }
}

void menuClass::mainMenuPlayCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  if(that->selectedProfile != -1)
  {
    that->fadeTo=menuBriefing;
    that->mission.setMission(that->playerInfos[that->selectedProfile]->mission);
  }
}

void menuClass::briefingPlayCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  //Check that player has a ship with a hull and an engine
  if((int)that->playerInfos[that->selectedProfile]->shipHull==0)
  {
    that->infoBox("Going on mission\nwithout a ship?");
    return;
  }
  if((int)that->playerInfos[that->selectedProfile]->shipSlot[slotEngine]==0)
  {
    that->infoBox("Flying without an engine?");
    return;
  }

  if(that->mission.info.num!=-1)
  {
    that->fadeTo=newGame;
  }
}

void menuClass::briefingShopCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->fadeTo=menuShop;
  //Make the hull that the player owns the default
  that->shopSelHull = that->playerInfos[that->selectedProfile]->shipHull;
}

void menuClass::shopBackCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->fadeTo = menuBriefing;
  player.saveProfile( that->playerInfos[that->selectedProfile] );
}

void menuClass::shopPrevHullCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  if(that->shopSelHull > 1) that->shopSelHull--;
}

void menuClass::shopNextHullCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  if((int)that->shopSelHull+1 < that->shopHulls.size() ) that->shopSelHull++;
}

void menuClass::shopBuyHullCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  //Check that there is enough currency.
  if(that->playerInfos[that->selectedProfile]->credits < that->shopHulls.at(that->shopSelHull).price) return;

  //If there is an existing hull, don't buy.
  if((int)that->playerInfos[that->selectedProfile]->shipHull != 0)
  {
    that->infoBox("Sorry, you need to\nsell your old hull\nbefore buying a new one.");
    return;
  }

  //Draw money
  that->playerInfos[that->selectedProfile]->credits -= that->shopHulls.at(that->shopSelHull).price;

  //Give item.
  that->playerInfos[that->selectedProfile]->shipHull = that->shopSelHull;
}

void menuClass::shopSellHullCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);

  //Do he own the one he want's to sell?
  if(that->shopSelHull != (int)that->playerInfos[that->selectedProfile]->shipHull) return;
  //Don't sell if the money won't cover buying a new hull.
  int sellPrice = (int)((float)that->shopHulls.at(that->shopSelHull).price/100.0*80.0);
  if(that->playerInfos[that->selectedProfile]->credits + sellPrice < that->shopHulls.at(1).price)
  {
    that->infoBox("You don't have enough\nmoney to buy a new hull.");
    return;
  }

  //Give him the money
  that->playerInfos[that->selectedProfile]->credits += sellPrice;

  //Take his hull.
  that->playerInfos[that->selectedProfile]->shipHull=0;

}

void menuClass::shopPrevItemCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->shopSelItem--;
  if(that->shopSelItem < 1)
  {
    that->shopSelItem=that->shopNumItems[ (that->shopItemSlot < 5)?0:that->shopItemSlot-4 ]-1;
  }
}

void menuClass::shopNextItemCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  that->shopSelItem++;
  if(that->shopSelItem > that->shopNumItems[ (that->shopItemSlot < 5)?0:that->shopItemSlot-4  ]-1)
    that->shopSelItem=1;
}

//Toggles between the checkboxes and sets which is currently selected in shopItemSlot
void menuClass::itemChkChangedCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  static bool shopPrevSelectedItemSlot[NUMITEMSLOTS]; //Static initializes to 0
  for(int i=0; i < NUMITEMSLOTS; i++)
  {
    if(!that->itemSlotsChk[i]->checked && shopPrevSelectedItemSlot[i])
    {
      that->itemSlotsChk[i]->checked=1;
    } else if(that->itemSlotsChk[i]->checked && shopPrevSelectedItemSlot[i])
    {
      that->itemSlotsChk[i]->checked=0;
      shopPrevSelectedItemSlot[i]=0;
    } else if( that->itemSlotsChk[i]->checked )
    {
      shopPrevSelectedItemSlot[i]=1;
      that->shopItemSlot=i;
      //Display what is already in the slot, or if nothing, show first thing
      int typeInSlot = (int)that->playerInfos[ that->selectedProfile ]->shipSlot[ that->shopItemSlot ];
      if( typeInSlot > 0 )
      {
        that->shopSelItem=typeInSlot; //This should be overwritten with whatever is in selected slot if anything
      } else {
        that->shopSelItem=1; //This should be overwritten with whatever is in selected slot if anything
      }
    }
  }
}

void menuClass::shopSellItemCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);
  int type = (int)that->playerInfos[ that->selectedProfile ]->shipSlot[ that->shopItemSlot ];
  int slot = that->shopItemSlot;
  int price = that->shopItems[ (slot<5)?0:slot-4 ].at(type).price;
  //Only if theres something in the slot, and the item is currently selected in the shop
  if( type > 0 && that->shopSelItem == type )
  {
    //Set item in that slot to 0
    that->playerInfos[ that->selectedProfile ]->shipSlot[ that->shopItemSlot ] = 0;
    //Give player 80% of the price back
    that->playerInfos[ that->selectedProfile ]->credits += (price/100*80);
  }
}

void menuClass::shopBuyItemCall(void* c)
{
  menuClass* that = static_cast<menuClass*>(c);

  //Don't buy if player does not have a hull
  //(this avoids a situation where the player have sold his hull,
  //and then spends enough money on gear that he won't be able to buy a new hull)
  if((int)that->playerInfos[that->selectedProfile]->shipHull == 0) return;

  int type = that->shopSelItem;
  int slot = that->shopItemSlot;
  int price = that->shopItems[ (slot<5)?0:slot-4 ].at(type).price;
  //only if the selected slot is empty
  if( (int)that->playerInfos[ that->selectedProfile ]->shipSlot[ that->shopItemSlot ]==0 )
  {
    //And only if he have the credits for it
    if( (int)that->playerInfos[ that->selectedProfile ]->credits >= price )
    {
      //Set item in that slot to that type
      that->playerInfos[ that->selectedProfile ]->shipSlot[ that->shopItemSlot ] = type;
      //Subtract money
      that->playerInfos[ that->selectedProfile ]->credits -= price;
    } else {
        //TODO: Insert BEEEP sound?
    }
  }

}
