//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved



#ifndef LibHNMarket_HNPayDelegate_h
#define LibHNMarket_HNPayDelegate_h


class HNPayDelegate
{
public:
    virtual void HandlePayResult(bool bSuccess,int platform) = 0;
};

#endif
