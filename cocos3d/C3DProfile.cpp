#include "Base.h"
#include "cocos3d.h"
#include "cocos2d.h"
#include "C3DProfile.h"
#include "CCStdC.h"

using namespace cocos2d;

namespace cocos3d
{
    C3DProfile::C3DProfile()
    {
    }

    C3DProfile::~C3DProfile()
    {
        removeAllProfile();
    }

    C3DProfile* C3DProfile::getInstance()
    {
        static C3DProfile instance;

        return &instance;
    }

    void C3DProfile::beginProfile(const std::string& itemname)
    {
#ifdef ENABLE_C3D_PROFILE
        ProfileItem*& item = _profileItems[itemname];
        if (item == NULL)
        {
            item = new ProfileItem;
            item->_name = itemname;
        }

        struct timeval now;
        gettimeofday( &now, NULL);
        item->_startTime = 1000000 * (now.tv_sec) + (now.tv_usec);
#endif
    }

    void C3DProfile::endProfile(const std::string& itemname)
    {
#ifdef ENABLE_C3D_PROFILE
        struct timeval now;
        gettimeofday( &now, NULL);
        ProfileItem*& item = _profileItems[itemname];
        //CCAssert(item, "profile item null");
        item->_accTime += 1000000 * (now.tv_sec) + (now.tv_usec) - item->_startTime;
#endif
    }

    void C3DProfile::update()
    {
#ifdef ENABLE_C3D_PROFILE
        for (std::map<std::string, ProfileItem*>::iterator it = _profileItems.begin(); it != _profileItems.end(); it++)
        {
            ProfileItem*& item = it->second;

            if (item->numberOfCalls != 0)
            {
                item->_durTime = item->_accTime;
                item->totalTime += item->_durTime;
                item->_avgTime = item->totalTime / item->numberOfCalls;
                item->_startTime = 0;
                if (item->_durTime > item->maxTime)
                    item->maxTime = item->_durTime;
                if (item->_durTime < item->minTime)
                    item->minTime = item->_durTime;
            }
            else
                item->reset();

            item->_accTime = 0;
            item->numberOfCalls++;
        }
#endif
    }

    void C3DProfile::removeAllProfile()
    {
        for (std::map<std::string, ProfileItem*>::iterator it = _profileItems.begin(); it != _profileItems.end(); it++)
        {
            delete it->second;
        }
        _profileItems.clear();
    }

////////////////////////////////Implement of C3DProfileDisplay//////////////////////////////////////////
C3DProfileDisplay::C3DProfileDisplay():_label(NULL)
{
}
C3DProfileDisplay::~C3DProfileDisplay()
{
	CC_SAFE_RELEASE(_label);
}

C3DProfileDisplay* C3DProfileDisplay::getInstance()
{
    static C3DProfileDisplay instance;
    return &instance;
}

void C3DProfileDisplay::setLabelBMPFont(cocos2d::CCLabelBMFont* label)
{
    CC_SAFE_RELEASE(_label);
    _label = label;
    label->retain();
}

void C3DProfileDisplay::update()
{
#ifdef ENABLE_C3D_PROFILE
    if (_label == NULL)
        return;

    std::map<std::string, C3DProfile::ProfileItem*>& items = C3DProfile::getInstance()->_profileItems;
    std::map<std::string, C3DProfile::ProfileItem*>::iterator it = items.begin();
    char str[128];
    std::string strlabel;
    for (; it != items.end(); it++)
    {
        C3DProfile::ProfileItem*& item = it->second;
        sprintf(str, "%10s, frame time: %5d, average frame time: %5d, min: %5d, max: %5d\n", item->_name.c_str(), item->_durTime, item->_avgTime, item->minTime, item->maxTime);
        strlabel += str;
    }
    _label->setString(strlabel.c_str());
#endif
}
}