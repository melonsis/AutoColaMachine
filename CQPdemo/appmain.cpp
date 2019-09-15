/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
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



int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;


/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	std::vector<string> backMsg;
	backMsg.clear();
	pmExtractor(msg, backMsg);
	if (!backMsg.empty() && (!backMsg.at(0).compare("/֥�鿪��") || !backMsg.at(0).compare("/֥��") || !backMsg.at(0).compare("/km")))
	{

		if (backMsg.size() < 2)
		{
			backMsg.push_back("2"); //Push_back method means add a element to the end of the vector
		}
		char* buildODMsg = (char*)malloc(100);
		if (!backMsg.at(1).compare("1"))
		{
			CQ_sendPrivateMsg(ac, fromQQ, "���ǣ�һ�����㻹��������ӣ�ײ������");
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
			CQ_sendPrivateMsg(ac, fromQQ, "֥�鿪��-��������");
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
	sprintf(buildInfoMsg, "�Զ����ֻ� by Melonsis \n�̼��汾%s", VERSION);
	CQ_sendPrivateMsg(ac, fromQQ, buildInfoMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/ʣ��ʱ��"))
{
	char* buildGameTimeMsg = (char*)malloc(300);
	sprintf(buildGameTimeMsg, "%s", "[CQ:share,content=�˻�����ѯ,image=https://pay.sdo.com/img/logo_bottom.png,title=ʢ����Ϸ��ֵ����,url=https://pay.sdo.com/user]");
	CQ_sendPrivateMsg(ac, fromQQ, buildGameTimeMsg);
}
if (!strcmp(msg, "����¶��������Ҹ��о�������") || !strcmp(msg, "/�о�"))
{
	char* buildNjMsg = (char*)malloc(100);
	buildNjMsg = njRandomPic();
	CQ_sendPrivateMsg(ac, fromQQ, buildNjMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/è��"))
{
	char* buildMfMsg = (char*)malloc(100);
	buildMfMsg = mfRandomPic();
	CQ_sendPrivateMsg(ac, fromQQ, buildMfMsg);
	return EVENT_BLOCK;
}
/* if (!strcmp(msg, "/�鿨"))    //1.0.1.1014�ϳ��˷���
{
	char* buildPcMsg = (char*)malloc(100);
	buildPcMsg = pickCard();
	CQ_sendPrivateMsg(ac, fromQQ, buildPcMsg);
	return EVENT_BLOCK;
} */
if (!backMsg.empty() && !backMsg.at(0).compare("/�鿨"))
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
if (!backMsg.empty() && (!backMsg.at(0).compare("/���") || !backMsg.at(0).compare("/jd")))
{
	char* buildJdMsg = (char*)malloc(100);
	char* buildOriMsg = (char*)malloc(300);
	sprintf(buildOriMsg, "%s", "[CQ:share,content=��������,image=https://tvax1.sinaimg.cn/crop.0.52.546.546.180/685b1e8fly8fx310hcqmnj20f70kbacc.jpg,url=https://weibo.com/wuwulizi]");
	sprintf(buildJdMsg, "%s", "���ܽ����Ϣ����weibo@�������ӣ���л�������ڸ�������Ȩ��\n[CQ:image,file=jdThisWeek.jpg]");
	CQ_sendPrivateMsg(ac, fromQQ, buildJdMsg);
	CQ_sendPrivateMsg(ac, fromQQ, buildOriMsg);
}
if (!backMsg.empty() && !backMsg.at(0).compare("/���ڳ鿨"))
{
	char* buildPcMsg = (char*)malloc(300);
	buildPcMsg = pickCardEx();
	CQ_sendPrivateMsg(ac, fromQQ, buildPcMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/���") || !backMsg.at(0).compare("/pf"))
{
	char* buildPfMsg = (char*)malloc(300);
	buildPfMsg = pfEmulator();
	CQ_sendPrivateMsg(ac, fromQQ, buildPfMsg);
	return EVENT_BLOCK;
}
if (!backMsg.empty() && !backMsg.at(0).compare("/����") || !backMsg.at(0).compare("/wm"))
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
	sprintf(buildCaiMsg, "%s", "���ˣ���������ݣ����ø�ί���˵����˵���ǣ����ˡ���");
	CQ_sendPrivateMsg(ac, fromQQ, buildCaiMsg);
}

	return EVENT_IGNORE;
}

/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	std::vector<string> backMsg;
	backMsg.clear();
	pmExtractor(msg, backMsg);
	if (!backMsg.empty() && (!backMsg.at(0).compare("/֥�鿪��") || !backMsg.at(0).compare("/֥��") || !backMsg.at(0).compare("/km")))
	{

		if (backMsg.size() < 2)
		{
			backMsg.push_back("2");
		}
		char* buildODMsg = (char*)malloc(100);
		if (!backMsg.at(1).compare("1"))
		{
			CQ_sendGroupMsg(ac, fromGroup, "���ǣ�һ�����㻹��������ӣ�ײ������");
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
			CQ_sendGroupMsg(ac, fromGroup, "֥�鿪��-��������");
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
		sprintf(buildInfoMsg, "�Զ����ֻ� by Melonsis \n�̼��汾%s", VERSION);
		CQ_sendGroupMsg(ac, fromGroup, buildInfoMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/ʣ��ʱ��"))
	{
		char* buildGameTimeMsg = (char*)malloc(300);
		sprintf(buildGameTimeMsg, "%s", "[CQ:share,content=�˻�����ѯ,image=https://pay.sdo.com/img/logo_bottom.png,title=ʢ����Ϸ��ֵ����,url=https://pay.sdo.com/user]");
		CQ_sendGroupMsg(ac, fromGroup, buildGameTimeMsg);
	}
	if (!strcmp(msg, "����¶��������Ҹ��о�������" )||!strcmp(msg,"/�о�"))
	{
		char* buildNjMsg = (char*)malloc(100);
		buildNjMsg = njRandomPic();
		CQ_sendGroupMsg(ac, fromGroup, buildNjMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/è��"))
	{
		char* buildMfMsg = (char*)malloc(100);
		buildMfMsg = mfRandomPic();
		CQ_sendGroupMsg(ac, fromGroup, buildMfMsg);
		return EVENT_BLOCK;
	}
	/*if (!strcmp(msg, "/�鿨"))  //1.0.1.1014�ϳ��˷���
	{
		char* buildPcMsg = (char*)malloc(100);
		buildPcMsg = pickCard();
		CQ_sendGroupMsg(ac, fromGroup, buildPcMsg);
		return EVENT_BLOCK;
	}*/
	if (!backMsg.empty() && !backMsg.at(0).compare("/�鿨"))
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
	if (!backMsg.empty() && (!backMsg.at(0).compare("/���") || !backMsg.at(0).compare("/jd")))
	{
		char* buildJdMsg = (char*)malloc(100);
		char* buildOriMsg = (char*)malloc(300);
		sprintf(buildOriMsg, "%s", "[CQ:share,content=��������,image=https://tvax1.sinaimg.cn/crop.0.52.546.546.180/685b1e8fly8fx310hcqmnj20f70kbacc.jpg,url=https://weibo.com/wuwulizi]");
		sprintf(buildJdMsg, "%s", "���ܽ����Ϣ����weibo@�������ӣ���л�������ڸ�������Ȩ��\n[CQ:image,file=jdThisWeek.jpg]");
		CQ_sendGroupMsg(ac, fromGroup, buildJdMsg);
		CQ_sendGroupMsg(ac, fromGroup, buildOriMsg);
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/���ڳ鿨"))
	{
		char* buildPcMsg = (char*)malloc(300);
		buildPcMsg = pickCardEx();
		CQ_sendGroupMsg(ac, fromGroup, buildPcMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/���") || !backMsg.at(0).compare("/pf"))
	{
		char* buildPfMsg = (char*)malloc(300);
		buildPfMsg = pfEmulator();
		CQ_sendGroupMsg(ac, fromGroup, buildPfMsg);
		return EVENT_BLOCK;
	}
	if (!backMsg.empty() && !backMsg.at(0).compare("/����") || !backMsg.at(0).compare("/wm"))
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
		sprintf(buildCaiMsg, "%s", "���ˣ���������ݣ����ø�ί���˵����˵���ǣ����ˡ���");
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
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}
char* njRandomPic()
{
	srand((int)time(0));
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(1, 8);
	int picNum = dis1(random);
	char* msgPicNum = (char*)malloc(5);
	char* msgFront = "������û�취���ðɣ�[CQ:image,file=nj";
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
	char* msgFront = "������û�취���ðɣ�[CQ:image,file=MF";
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
	char* pickFront = "�������һ����������";
	char* cardName[6] = { "��̫����֮�⡿��","��������֮ƿ����","��������֮������","��������֮�ɡ���","��ս����֮ǹ����","��������֮������" };
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
	char* pickFront = "���������о��ʣ�ɽ�˰������ڲأ������ŵش����ڳ����";
	char* cardName[6] = { "��̫����֮�⡿","��������֮ƿ��","��������֮����","��������֮�ɡ�","��ս����֮ǹ��","��������֮����" };
	char* exCardname[2] = { "������һ�š��󸾡�","������һ�š�������" };
	char* exBuff[3] = { "�����ҡ�Ч����Ϊ1.5�����ˣ�","�����ҡ�����ʱ���Ϊ2�����ˣ�","�����ҡ���Χ����Ч�����롿�ˣ�" };
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
		return "���ţ�����¶����Ӧ�ô򿪡���ࡿ���ţ�";
	}
	if (num % 2 == 1)
	{
		return "���ţ�����¶����Ӧ�ô򿪡��Ҳࡿ���ţ�";
	}

}
char* openDoor3()
{
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(1, 300);
	int num = dis1(random) + dis1(random) * dis1(random);
	if (num % 3 == 0)
	{
		return "���ţ�����¶����Ӧ�ô򿪡���ࡿ���ţ�";
	}
	if (num % 3 == 1)
	{
		return "���ţ�����¶����Ӧ�ô򿪡��м䡿���ţ�";
	}
	if (num % 3 == 2)
	{
		return "���ţ�����¶����Ӧ�ô򿪡��Ҳࡿ���ţ�";
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
	char* pickFront = "�������һ����������";
	char* cardName[6] = { "��̫����֮�⡿��","��������֮ƿ����","��������֮������","��������֮�ɡ���","��ս����֮ǹ����","��������֮������" };
	char* pcMsg = (char*)malloc(100);
	sprintf(pcMsg, "%s%s", pickFront, cardName[(dis1(random)+dis2(random))%6]);
	return pcMsg;
}
/*char* goldenPlate_ThisWeek() �����й���
{

}*/
char* pfEmulator()
{
	int i = 0;
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis1(0, 5);
	std::uniform_int_distribution<int> dis2(0, 2);
	char* pickFront = "��������ʦ�ĳ����о�������Ϊ���Ӧ��վ��";
	char* directionName[6] = { "��ʮ�����ӷ���","�������ӷ���","���ĵ��ӷ���","�������ӷ���","���˵��ӷ���","��ʮ���ӷ���" };
	char* positionName[3] = { "��������ࡿ��λ�ã�","�����жΡ���λ�ã�","�����������ĵ㡿��λ�ã�" };
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
		char* pickFront = "�ֲ�֪����ʲô��������������ɣ�Ϊʲô������";
		char* brandName[3] = { "�����š��ĵڡ�","������ô���ĵڡ�" };
		char* desicionName = (char*)malloc(10);
		itoa((dis2(random) % wmParameter)+1, desicionName, 10);
		sprintf(wmMsg, "%s%s%s%s", pickFront, brandName[dis2(random) % 2], desicionName, "���������أ�");
	}
	else
	{
		sprintf(wmMsg, "%s", "����ѡ����-������5-100�ڵĴ����ֲ�����");
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
					char* lfMsg = "Ȩ����֤ͨ����������Ľ������ѳɹ����С�";
					return lfMsg;
				}
				else
				{
					CQ_setGroupBan(ac, 324720453, 2273374730, day * 3600 * 24);
					char* lfMsg = "�ԣ���ȥ˯�ɡ���˯�Ļ����Ǿʹ��δ��ߡ�";
					return lfMsg;
				}
			}
			if (QQNum == 2273374730)
			{
			
				CQ_setGroupBan(ac, 324720453, 2273374730, 29 * 3600 * 24);
				char* lfMsg = "Ȩ����֤ʧ�ܣ�ʩ�Զ�����Դ�����";
				return lfMsg;
			}
		}
		else
		{
			char* lfMsg = "�Ǹ����˲��ڱ�Ⱥ��";
			return lfMsg;
		}
	}
	else
	{
		char* lfMsg = "0F��Ĭ��-��������";
		return lfMsg;
	}
	

}

