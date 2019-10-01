#include "missioninfo.hpp"

missionInfoClass::missionInfoClass()
{
  //Load info from missions.txt
  string fileName = dir.data+"missions/missionlist.txt";
  ifstream file;
  string temp;

  file.open( fileName.data() );
  if(file.is_open())
  {
    while(!file.eof())
    {
      getline( file, temp );
      if(temp.size() > 0)
        fileNames.push_back( temp );
    }
  } else {
    printc("Error: Couldn't open '%s' for reading.",fileName.data());
  }

  printc("Read '%s'",fileName.data());
  printc("Found %i missions.",fileNames.size());
  for(int i=0; i < fileNames.size(); i++)
  {
    printc("Mission: %s",fileNames.at(i).data());
  }

  setNotFound();
}

void missionInfoClass::setNotFound()
{
  info.num=-1;
  info.missionBounty=0;
  info.missionFile="nofile";
  info.missionBriefing="For some reason, the mission was not found.\nTry to delete your profile and create a new.\nSorry.";
  info.missionName="No Mission Found.";
}

bool missionInfoClass::setMission(int misNum)
{
  bool readbrief=0;
  if(fileNames.size() > misNum)
  {
    string fileName = dir.data+"missions/"+fileNames.at(misNum);
    ifstream file;
    string temp;
    string val,set;

    file.open(fileName.data());
    if(file.is_open())
    {
      while(!file.eof())
      {
        getline(file, temp);
        if(temp.compare("--brief--")==0)
        {
          readbrief=1;
          info.missionBriefing.clear();
        } else if(readbrief)
        {
          info.missionBriefing += temp;
          info.missionBriefing += "\n";
        }
        if(temp.find('=') != string::npos)
        {
          set=temp.substr(0,temp.find('='));
          val=temp.substr(temp.find('=')+1);
          if(set=="name")
          {
            info.missionName=val;
          } else if(set=="bounty")
          {
            info.missionBounty=atoi(val.data());
          } else if(set=="file")
          {
            info.missionFile=dir.data+"missions/"+val;
            info.num=misNum;
            //cout << "File is:" << info.missionFile << " for mission " << misNum <<endl;
          }
        }
      }
    } else {
      printc("Error: Couldn't open '%s' for reading.",fileName.data());
      setNotFound();
      return(0);
    }

    return(1);
  } else {
    printc("Error: mission %i out of bounds.",misNum);
    setNotFound();
    return(0);
  }
}
