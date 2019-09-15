/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include <time.h>
#include <random>
#include <vector>
#include <sstream>
#include <chrono>
#include <sstream>

const char VERSION[12] = "1.0.2.0612a";

using namespace std;
char* njRandomPic();
char* mfRandomPic();
char* pickCard();
char* pickCardEx();
void pmExtractor(const char* oriMsg, vector<string> &backMsg);
char* openDoor2();
char* openDoor3();
char* pickCardPm(string);
char* pfEmulator();
char* wmSelector(int);
char* lfSilentor(int,int,int,int);



int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;


/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	std::vector<string> backMsg;
	backMsg.clear();
	pmExtractor(msg, backMsg);
	if (!backMsg.empty() && (!backMsg.at(0).compare("/芝麻开门") || !backMsg.at(0).compare("/芝麻") || !backMsg.at(0).compare("/km")))
	{

		if (backMsg.size() < 2)
		{
			backMsg.push_back("2"); //Push_back method means add a element to the end of the vector
		}
		char* buildODMsg = (char*)malloc(100);
		if (!backMsg.at(1).compare("1"))
		{
			CQ_sendPrivateMsg(ac, fromQQ, "册那，一扇门你还纠结个锤子？撞开啊！");
		}
		else if (!backMsg.at(1).compare("2"))
		{
			buildODMsg = openDoor2();
			CQ_sendPrivateMsg(ac, fromQQ, buildODMsg);
		}
		else if (!backMsg.at(1).compare("3"))
		{
			buildODMsg = openDoor3();
			CQ_sendPrivateMsg(ac, fromQQ, buildODMsg);
		}
		else
		{
			CQ_sendPrivateMsg(ac, fromQQ, "芝麻开门-参数错误");
		}
		return EVENT_BLOCK;
	}
if (!backMsg.empty() && !backMsg.at(0).compare("/colahelp"))
{
	CQ_sendPrivateMsg(ac, fromQQ, "[CQ:image,file=colahelp.jpg]");
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/colainfo"))
{
	char* buildInfoMsg = (char*)malloc(100);
	sprintf(buildInfoMsg, "自动可乐机 by Melonsis \n固件版本%s", VERSION);
	CQ_sendPrivateMsg(ac, fromQQ, buildInfoMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/剩余时间"))
{
	char* buildGameTimeMsg = (char*)malloc(300);
	sprintf(buildGameTimeMsg, "%s", "[CQ:share,content=账户余额查询,image=https://pay.sdo.com/img/logo_bottom.png,title=盛大游戏充值中心,url=https://pay.sdo.com/user]");
	CQ_sendPrivateMsg(ac, fromQQ, buildGameTimeMsg);
}
if (!strcmp(msg, "塔塔露，快给我找个男精看看！") || !strcmp(msg, "/男精"))
{
	char* buildNjMsg = (char*)malloc(100);
	buildNjMsg = njRandomPic();
	CQ_sendPrivateMsg(ac, fromQQ, buildNjMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/猫娘"))
{
	char* buildMfMsg = (char*)malloc(100);
	buildMfMsg = mfRandomPic();
	CQ_sendPrivateMsg(ac, fromQQ, buildMfMsg);
	return EVENT_BLOCK;
}
/* if (!strcmp(msg, "/抽卡"))    //1.0.1.1014废除此方法
{
	char* buildPcMsg = (char*)malloc(100);
	buildPcMsg = pickCard();
	CQ_sendPrivateMsg(ac, fromQQ, buildPcMsg);
	return EVENT_BLOCK;
} */
if (!backMsg.empty() && !backMsg.at(0).compare("/抽卡"))
{
	if (backMsg.size() < 2)
	{
		char* buildPcMsg = (char*)malloc(100);
		buildPcMsg = pickCard();
		CQ_sendPrivateMsg(ac, fromQQ, buildPcMsg);
		return EVENT_BLOCK;
	}
	else
	{
		char* buildPcMsg = (char*)malloc(100);
		buildPcMsg = pickCardPm(backMsg.at(1));
		CQ_sendPrivateMsg(ac, fromQQ, buildPcMsg);
		return EVENT_BLOCK;
	}
}
if (!backMsg.empty() && (!backMsg.at(0).compare("/金碟") || !backMsg.at(0).compare("/jd")))
{
	char* buildJdMsg = (char*)malloc(100);
	char* buildOriMsg = (char*)malloc(300);
	sprintf(buildOriMsg, "%s", "[CQ:share,content=呜呜栗子,image=https://tvax1.sinaimg.cn/crop.0.52.546.546.180/685b1e8fly8fx310hcqmnj20f70kbacc.jpg,url=https://weibo.com/wuwulizi]");
	sprintf(buildJdMsg, "%s", "本周金碟信息来自weibo@呜呜栗子，感谢她的辛勤付出和授权！\n[CQ:image,file=jdThisWeek.jpg]");
	CQ_sendPrivateMsg(ac, fromQQ, buildJdMsg);
	CQ_sendPrivateMsg(ac, fromQQ, buildOriMsg);
}
if (!backMsg.empty() && !backMsg.at(0).compare("/袖内抽卡"))
{
	char* buildPcMsg = (char*)malloc(300);
	buildPcMsg = pickCardEx();
	CQ_sendPrivateMsg(ac, fromQQ, buildPcMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/喷风") || !backMsg.at(0).compare("/pf"))
{
	char* buildPfMsg = (char*)malloc(300);
	buildPfMsg = pfEmulator();
	CQ_sendPrivateMsg(ac, fromQQ, buildPfMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/外卖") || !backMsg.at(0).compare("/wm"))
{
	if (backMsg.size() < 2)
	{
		backMsg.push_back("10");
	}
	char* buildWmMsg = (char*)malloc(300);
	int wmParameter = 0;
	stringstream wmSs;
	wmSs << backMsg.at(1);
	wmSs >> wmParameter; //Using string stream to covert string to int
	buildWmMsg = wmSelector(wmParameter);
	CQ_sendPrivateMsg(ac, fromQQ, buildWmMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/cnlogs"))
{
	char* buildCaiMsg = (char*)malloc(300);
	sprintf(buildCaiMsg, "%s", "主人，您这个数据，我用个委婉的说法来说就是，“菜”。");
	CQ_sendPrivateMsg(ac, fromQQ, buildCaiMsg);
}

	return EVENT_IGNORE;
}

/*
* Type=2 群消息
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	std::vector<string> backMsg;
	backMsg.clear();
	pmExtractor(msg, backMsg);
	if (!backMsg.empty() && (!backMsg.at(0).compare("/芝麻开门") || !backMsg.at(0).compare("/芝麻") || !backMsg.at(0).compare("/km")))
	{

		if (backMsg.size() < 2)
		{
			backMsg.push_back("2");
		}
		char* buildODMsg = (char*)malloc(100);
		if (!backMsg.at(1).compare("1"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "册那，一扇门你还纠结个锤子？撞开啊！");
		}
		else if (!backMsg.at(1).compare("2"))
		{
			buildODMsg = openDoor2();
			CQ_sendGroupMsg(ac, fromGroup, buildODMsg);
		}
		else if (!backMsg.at(1).compare("3"))
		{
			buildODMsg = openDoor3();
			CQ_sendGroupMsg(ac, fromGroup, buildODMsg);
		}
		else
		{
			CQ_sendGroupMsg(ac, fromGroup, "芝麻开门-参数错误");
		}
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/colahelp"))
	{
		CQ_sendGroupMsg(ac, fromGroup, "[CQ:image,file=colahelp.jpg]");
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/colainfo"))
	{
		char* buildInfoMsg = (char*)malloc(100);
		sprintf(buildInfoMsg, "自动可乐机 by Melonsis \n固件版本%s", VERSION);
		CQ_sendGroupMsg(ac, fromGroup, buildInfoMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/剩余时间"))
	{
		char* buildGameTimeMsg = (char*)malloc(300);
		sprintf(buildGameTimeMsg, "%s", "[CQ:share,content=账户余额查询,image=https://pay.sdo.com/img/logo_bottom.png,title=盛大游戏充值中心,url=https://pay.sdo.com/user]");
		CQ_sendGroupMsg(ac, fromGroup, buildGameTimeMsg);
	}
	if (!strcmp(msg, "塔塔露，快给我找个男精看看！" )||!strcmp(msg,"/男精"))
	{
		char* buildNjMsg = (char*)malloc(100);
		buildNjMsg = njRandomPic();
		CQ_sendGroupMsg(ac, fromGroup, buildNjMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/猫娘"))
	{
		char* buildMfMsg = (char*)malloc(100);
		buildMfMsg = mfRandomPic();
		CQ_sendGroupMsg(ac, fromGroup, buildMfMsg);
		return EVENT_BLOCK;
	}
	/*if (!strcmp(msg, "/抽卡"))  //1.0.1.1014废除此方法
	{
		char* buildPcMsg = (char*)malloc(100);
		buildPcMsg = pickCard();
		CQ_sendGroupMsg(ac, fromGroup, buildPcMsg);
		return EVENT_BLOCK;
	}*/
	if (!backMsg.empty() && !backMsg.at(0).compare("/抽卡"))
	{
		if (backMsg.size() < 2)
		{
			char* buildPcMsg = (char*)malloc(100);
			buildPcMsg = pickCard();
			CQ_sendGroupMsg(ac, fromGroup, buildPcMsg);
			return EVENT_BLOCK;
		}
		else
		{
			char* buildPcMsg = (char*)malloc(100);
			buildPcMsg = pickCardPm(backMsg.at(1));
			CQ_sendGroupMsg(ac, fromGroup, buildPcMsg);
			return EVENT_BLOCK;
		}
	}
	if (!backMsg.empty() && (!backMsg.at(0).compare("/金碟") || !backMsg.at(0).compare("/jd")))
	{
		char* buildJdMsg = (char*)malloc(100);
		char* buildOriMsg = (char*)malloc(300);
		sprintf(buildOriMsg, "%s", "[CQ:share,content=呜呜栗子,image=https://tvax1.sinaimg.cn/crop.0.52.546.546.180/685b1e8fly8fx310hcqmnj20f70kbacc.jpg,url=https://weibo.com/wuwulizi]");
		sprintf(buildJdMsg, "%s", "本周金碟信息来自weibo@呜呜栗子，感谢她的辛勤付出和授权！\n[CQ:image,file=jdThisWeek.jpg]");
		CQ_sendGroupMsg(ac, fromGroup, buildJdMsg);
		CQ_sendGroupMsg(ac, fromGroup, buildOriMsg);
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/袖内抽卡"))
	{
		char* buildPcMsg = (char*)malloc(300);
		buildPcMsg = pickCardEx();
		CQ_sendGroupMsg(ac, fromGroup, buildPcMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/喷风") || !backMsg.at(0).compare("/pf"))
	{
		char* buildPfMsg = (char*)malloc(300);
		buildPfMsg = pfEmulator();
		CQ_sendGroupMsg(ac, fromGroup, buildPfMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/外卖") || !backMsg.at(0).compare("/wm"))
	{
		if (backMsg.size() < 2)
		{
			backMsg.push_back("10");
		}
		char* buildWmMsg = (char*)malloc(300);
		int wmParameter = 0;
		stringstream wmSs;
		wmSs << backMsg.at(1);
		wmSs >> wmParameter; //Using string stream to covert string to int
		buildWmMsg = wmSelector(wmParameter);
		CQ_sendGroupMsg(ac, fromGroup, buildWmMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/cnlogs"))
	{
		char* buildCaiMsg = (char*)malloc(300);
		sprintf(buildCaiMsg, "%s", "主人，您这个数据，我用个委婉的说法来说就是，“菜”。");
		CQ_sendGroupMsg(ac, fromGroup, buildCaiMsg);
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/0f")) {
		char* build0fMsg = (char*)malloc(100);
		if (backMsg.size() < 2)
		{
			backMsg.push_back("10");
		}
		int lf = 0;
		stringstream lfSs;
		lfSs << backMsg.at(1);
		lfSs >> lf; //Using string stream to covert string to int
		build0fMsg = lfSilentor(lf, fromGroup, fromQQ,ac);
		CQ_sendGroupMsg(ac, fromGroup, build0fMsg);

	}
	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "这是menuB，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}
char* njRandomPic()
{
	srand((int)time(0));
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(1, 8);
	int picNum = dis1(random);
	char* msgPicNum = (char*)malloc(5);
	char* msgFront = "真拿你没办法，好吧！[CQ:image,file=nj";
	char* msgEnd = ".jpg]";
	itoa(picNum, msgPicNum, 10);
	char* njMsg = (char*)malloc(100);
	sprintf(njMsg, "%s%s%s", msgFront, msgPicNum, msgEnd);

	return njMsg;
}
char* mfRandomPic()
{
	srand((int)time(0));
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(1, 8);
	int picNum = dis1(random);
	char* msgPicNum = (char*)malloc(5);
	char* msgFront = "真拿你没办法，好吧！[CQ:image,file=MF";
	char* msgEnd = ".jpg]";
	itoa(picNum, msgPicNum, 10);
	char* mfMsg = (char*)malloc(100);
	sprintf(mfMsg, "%s%s%s", msgFront, msgPicNum, msgEnd);

	return mfMsg;
}
char* pickCard()
{
	int i = 0;
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(0, 5);
	char* pickFront = "随着神光一闪，你抽出了";
	char* cardName[6] = { "【太阳神之衡】！","【河流神之瓶】！","【建筑神之塔】！","【世界树之干】！","【战争神之枪】！","【放浪神之箭】！" };
	char* pcMsg = (char*)malloc(100);
	sprintf(pcMsg, "%s%s", pickFront, cardName[dis1(random)]);
	return pcMsg;
}
char* pickCardEx()
{
	int i = 0;
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(0, 5);
	std::uniform_int_distribution<int> dis2(0, 1);
	std::uniform_int_distribution<int> dis3(0, 2);
	char* pickFront = "将身且坐中军帐，山人八卦袖内藏！你自信地从袖内抽出了";
	char* cardName[6] = { "【太阳神之衡】","【河流神之瓶】","【建筑神之塔】","【世界树之干】","【战争神之枪】","【放浪神之箭】" };
	char* exCardname[2] = { "，附加一张【贵妇】","，附加一张【领主】" };
	char* exBuff[3] = { "，而且【效果变为1.5倍】了！","，而且【持续时间变为2倍】了！","，而且【范围化加效果减半】了！" };
	char* pcMsg = (char*)malloc(300);
	sprintf(pcMsg, "%s%s%s%s%s", pickFront, cardName[dis1(random)], cardName[dis1(random)],exCardname[dis2(random)],exBuff[dis3(random)]);
	return pcMsg;
}
void pmExtractor(const char* oriMsg, vector<string> &backMsg)
{
	string str(oriMsg);
	istringstream strcin(str);
	string s;
	while (strcin >> s) backMsg.push_back(s);
}
char* openDoor2()
{
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(1, 300);
	int num = dis1(random) + dis1(random) * dis1(random);
	if (num % 2 == 0)
	{
		return "嗯嗯，塔塔露觉得应该打开【左侧】的门！";
	}
	if (num % 2 == 1)
	{
		return "嗯嗯，塔塔露觉得应该打开【右侧】的门！";
	}

}
char* openDoor3()
{
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(1, 300);
	int num = dis1(random) + dis1(random) * dis1(random);
	if (num % 3 == 0)
	{
		return "嗯嗯，塔塔露觉得应该打开【左侧】的门！";
	}
	if (num % 3 == 1)
	{
		return "嗯嗯，塔塔露觉得应该打开【中间】的门！";
	}
	if (num % 3 == 2)
	{
		return "嗯嗯，塔塔露觉得应该打开【右侧】的门！";
	}

}
char* pickCardPm(string pMsg)
{
	int i = 0,snum=100;
	if(pMsg.length()!=0)
		snum=pMsg.length();
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(100,500);
	std::uniform_int_distribution<int> dis2(0, snum);
	char* pickFront = "随着神光一闪，你抽出了";
	char* cardName[6] = { "【太阳神之衡】！","【河流神之瓶】！","【建筑神之塔】！","【世界树之干】！","【战争神之枪】！","【放浪神之箭】！" };
	char* pcMsg = (char*)malloc(100);
	sprintf(pcMsg, "%s%s", pickFront, cardName[(dis1(random)+dis2(random))%6]);
	return pcMsg;
}
/*char* goldenPlate_ThisWeek() 开发中功能
{

}*/
char* pfEmulator()
{
	int i = 0;
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(0, 5);
	std::uniform_int_distribution<int> dis2(0, 2);
	char* pickFront = "经过对老师的长期研究，我认为这次应该站在";
	char* directionName[6] = { "【十二点钟方向】","【两点钟方向】","【四点钟方向】","【六点钟方向】","【八点钟方向】","【十点钟方向】" };
	char* positionName[3] = { "，【最外侧】的位置！","，【中段】的位置！","，【靠近中心点】的位置！" };
	char* pfMsg = (char*)malloc(300);
	sprintf(pfMsg, "%s%s%s", pickFront, directionName[dis1(random)],positionName[dis2(random)]);
	return pfMsg;
}
char* wmSelector(int wmParameter)
{
	int i = 0;
	char* wmMsg = (char*)malloc(300);
	if (wmParameter > 4 && wmParameter < 101)
	{
		std::default_random_engine random(time(NULL));
		std::uniform_int_distribution<int> dis1(1, 300);
		std::uniform_int_distribution<int> dis2(1, 300);
		char* pickFront = "又不知道点什么啦？让我来帮你吧！为什么不试试";
		char* brandName[3] = { "【美团】的第【","【饿了么】的第【" };
		char* desicionName = (char*)malloc(10);
		itoa((dis2(random) % wmParameter)+1, desicionName, 10);
		sprintf(wmMsg, "%s%s%s%s", pickFront, brandName[dis2(random) % 2], desicionName, "】家外卖呢？");
	}
	else
	{
		sprintf(wmMsg, "%s", "外卖选择器-请输入5-100内的纯数字参数。");
	}
	return wmMsg;
}
char* lfSilentor(int day, int groupNum,int QQNum,int ac) {
	if (day >= 0 && day < 30)
	{
		if (groupNum == 324720453)
		{
			if (QQNum != 2273374730)
			{
				if (day == 0)
				{
					CQ_setGroupBan(ac, 324720453, 2273374730, 0);
					char* lfMsg = "权限验证通过！你请求的解封操作已成功进行。";
					return lfMsg;
				}
				else
				{
					CQ_setGroupBan(ac, 324720453, 2273374730, day * 3600 * 24);
					char* lfMsg = "乖，滚去睡吧。不睡的话，那就打晕带走。";
					return lfMsg;
				}
			}
			if (QQNum == 2273374730)
			{
			
				CQ_setGroupBan(ac, 324720453, 2273374730, 29 * 3600 * 24);
				char* lfMsg = "权限验证失败，施以顶格禁言处罚。";
				return lfMsg;
			}
		}
		else
		{
			char* lfMsg = "那个男人不在本群。";
			return lfMsg;
		}
	}
	else
	{
		char* lfMsg = "0F沉默器-参数错误。";
		return lfMsg;
	}
	

}

