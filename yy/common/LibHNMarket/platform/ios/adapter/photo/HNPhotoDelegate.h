//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved



#ifndef HNMutlLobby_HNPhotoDelegate_h
#define HNMutlLobby_HNPhotoDelegate_h

#include <string>

class HNPhotoDelegate
{
public:
    virtual void HandleGetPhoto(const std::string& data) = 0;
};

#endif
