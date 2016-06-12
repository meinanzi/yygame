#ifndef __Landlord_Game_Table_UI_Callback_H__
#define __Landlord_Game_Table_UI_Callback_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "LandlordMessageHead.h"


namespace landlord
{
	/*
 * game ui callback
 */

class GameTableUICallback : public HN::IHNGameLogicBase
{
public:
	virtual void addUser(BYTE seatNo, bool bMe) = 0;
	virtual void removeUser(BYTE seatNo, bool bMe) = 0;

	virtual void setUserName(BYTE seatNo, const std::string& name) = 0;
	virtual void showUserMoney(BYTE seatNo, bool visible) = 0;

	virtual void setUserMoney(BYTE seatNo, const std::string& money) = 0;						//比赛改
	virtual void contestleaveTableDlg() = 0;                                                    //比赛场离开游戏警告框
	virtual void showGameContestKick() = 0;                                                     //比赛淘汰
	virtual void showGameContestWaitOver() = 0;                                                 //等待比赛结束
	virtual void showGameContestOver(MSG_GR_ContestAward* contestAward) = 0;				    //比赛结束
	virtual void updateMyRankNum(int value) = 0;												//更新比赛排名
	virtual void addContestUI() = 0;															//添加比赛局数、排名UI
	virtual void setPaiMing(const std::string& str) = 0;										//设置排名
	virtual void setJuShu(const std::string& str) = 0;											//设置局数
	virtual void playContestAnimate() = 0;												//播放比赛动画
	virtual void removeContestAnimate() = 0;													//删除比赛动画
	virtual void showLeftUpPNG(GameTaskStruct gameTask) = 0;				//显示左上角

	virtual void setUserAutoHead(BYTE seatNo) = 0;
	virtual void setUserHead(BYTE seatNo, bool isEmpty) = 0;
	virtual void setUserGameHead(BYTE seatNo, bool isLord) = 0;
	virtual void showUserHandCard(BYTE seatNo, const std::vector<BYTE>& values, bool isUp, bool isOnce) = 0;
	virtual void showUserHandCardCount(BYTE seatNo, bool visible) = 0;
	virtual void setUserHandCardCount(BYTE seatNo, INT count) = 0;
	virtual void showUserHandCardOut(BYTE seatNo, const std::vector<BYTE>& cards) = 0;
	virtual void showUserOutCardType(BYTE seatNo, BYTE type) = 0;
	virtual void setUpCardList(BYTE seatNo, const std::vector<BYTE>& upCards) = 0;
	virtual void getUpCardList(BYTE seatNo, std::vector<BYTE>* upCards) = 0;

	virtual void showLandlordCard(bool visible, const std::vector<BYTE>& cards) = 0;

	virtual void showBackCardTypeAndMutiple(BYTE cardType, BYTE cardMutiple) = 0;

	virtual void showLandlord(BYTE seatNo, bool visible) = 0;

	virtual void downUserCards(BYTE seatNo, bool bMe) = 0;

	virtual void showActionButton(BYTE flag) = 0;
	virtual void showWin(BYTE seatNo) = 0;	
	virtual void showLoading(bool bVisible) = 0;
	virtual void showNotice(const std::string &message) = 0;
	virtual void showUserProfit(BYTE seatNo, LLONG money) = 0;
	virtual void setWaitTime(BYTE seatNo, BYTE time, bool visible) = 0;
	virtual void showDashboard(bool visible) = 0;
	virtual void showUserCallScore(BYTE seatNo) = 0;
	virtual void showUserCallScoreResult(BYTE seatNo, int score) = 0;
	virtual void showUserAddDoubleResult(BYTE seatNo, int value) = 0;
	virtual void setBasePoint(int point) = 0;
	virtual void setMultiple(int multiple) = 0;
	virtual void showCancelAutoMask(bool visible) = 0;
	

	/*
	 * my action button
	 * @param: seatNo user's seat No.
	 * @param: visible show or not.
	 */
	virtual void setOutVisible(bool visible, bool firstOut) = 0;

	/*
	 * show user action result
	 * @param: seatNo user's seat No.
	 * @param: visible show or not.
	 */
	virtual void showUserCallNT(BYTE seatNo, bool visible) = 0;
	virtual void showUserNotCallNT(BYTE seatNo, bool visible) = 0;
	virtual void showUserRobNT(BYTE seatNo, bool visible) = 0; 
	virtual void showUserNotRobNT(BYTE seatNo, bool visible) = 0;
	virtual void showUserReady(BYTE seatNo, bool visible) = 0;
	virtual void showUserDouble(BYTE seatNo, bool visible) = 0;
	virtual void showUserNotDouble(BYTE seatNo, bool visible) = 0;
	virtual void showUserPublishCard(BYTE seatNo, bool visible) = 0;
	virtual void showUserNotOut(BYTE seatNo, bool visible) = 0;
	virtual void hideUserStatus(BYTE seatNo) = 0;
	virtual void showUserInvalidOut(bool visible) = 0;
	virtual void showUserCannotOut(bool visible) = 0;
	virtual void hideOutCardTip() = 0;
    
    /*
     * play sound.
     */
    virtual void playSingle(BYTE seatNo, int value) = 0;
    virtual void playDouble(BYTE seatNo, int value) = 0;
    virtual void playThree(BYTE seatNo, int value) = 0;// value = 0, 1, 2
    virtual void playStraight(BYTE seatNo) = 0;
	virtual void playDoubleSequence(BYTE seatNo) = 0;
	virtual void playPlan(BYTE seatNo) = 0;
	virtual void playFourTake(BYTE seatNo, bool isDouble) = 0;
	virtual void playRocket(BYTE seatNo) = 0;
	virtual void playBomb(BYTE seatNo) = 0;
	virtual void playCardLast(BYTE seatNo, int value) = 0;
    virtual void playWin() = 0;
    virtual void playLose() = 0;

	/*
	 * something about desk.
	 */
	virtual void clearDesk(bool isContestEnd) = 0;
	virtual void leaveDesk(bool isForceQuit) = 0;

	virtual void showUserChatMsg(BYTE seatNo, CHAR msg[]) = 0;
};
}


#endif