#include "stdafx.h"
#include "GameTest.h"
#include "Serialization\Serialization.h"
#include "Serialization\Decorators.h"
#include "Client.h"
#include "MainWindow.h"
#include "UserInterface\XmlRpc\RpcEnums.h"
#include "UserInterface\XmlRpc\RpcTypes.h"

GameTest::GameTest()
{
	logined_ = false;
	session_.generate();
	memset(callLock_, 0, CALL_LOCK_SIZE);

	score_ = 1;
	scoreType_ = 0;
	startScores_ = 0;
}

void GameTest::serialize(Archive& ar)
{
	ar.serialize(login_, "login", "���");
	ar.serialize(pass_, "pass", "������");

	ButtonDecorator new_user_button("������������������");
	ar.serialize(new_user_button, "new_user", "<");
	if(new_user_button)
		registerUser(login_.c_str(), pass_.c_str());

	ButtonDecorator login_button("�����");
	ar.serialize(login_button, "login", "<");
	if(login_button)
		login(login_.c_str(), pass_.c_str());

	ButtonDecorator logout_button("�����");
	ar.serialize(logout_button, "logout", "<");
	if(logout_button)
		logout(session_);

	ar.serialize(score_, "score", "����");
	ar.serialize(scoreType_, "scoreType", "��� �����");

	ButtonDecorator addScoreBySession_button("�������� ���� � ������");
	ar.serialize(addScoreBySession_button, "addbys", "<");
	if(addScoreBySession_button)
		addScoreBySession(session_, score_, scoreType_);

	ButtonDecorator addScoreByLogin_button("�������� ���� ������������");
	ar.serialize(addScoreByLogin_button, "addbyu", "<");
	if(addScoreByLogin_button)
		addScoreByName(login_.c_str(), score_, scoreType_);

	ar.serialize(startScores_, "startScores", "�������� ������� � �������");
	ButtonDecorator getScores_button("�������� ����");
	ar.serialize(getScores_button, "getscores", "<");
	if(getScores_button)
		getScores(startScores_, 3);
}

void GameTest::log(const char* msg)
{
	client->getMainWindow()->addLogRecord(msg);
}

bool GameTest::setCallLock(CallLock type)
{
	if(callLock_[type])
		return false;
	callLock_[type] = true;
	return true;
}

void GameTest::releaseCallLock(CallLock type)
{
	xassert(callLock_[type]);
	callLock_[type] = false;
}

void GameTest::registerUser(const char* n, const char* p)
{
	if(!setCallLock(LOCK1))
		return;

	log((XBuffer() < "�����������: " < n < ", " < p).c_str());
	
	typedef RpcSimpleMethodAsynchCall<GameTest, RpcType::LoginData> MethodRegister;

	RpcType::LoginData data;
	data.login = n;
	data.pass = p;
	MethodRegister call("Register", this, &GameTest::registerUserHandler, data);

	client->rpcAsynchCall(&call);
}

void GameTest::login(const char* n, const char* p)
{
	if(!setCallLock(LOCK1))
		return;

	log((XBuffer() < "����: " < n < ", " < p).c_str());

	typedef RpcSimpleMethodAsynchCall<GameTest, RpcType::LoginData> MethodLogin;

	RpcType::LoginData data;
	data.login = n;
	data.pass = p;
	data.session = session_;
	MethodLogin call("Login", this, &GameTest::loginHandler, data);

	client->rpcAsynchCall(&call);
}

void GameTest::logout(const XGUID& session)
{
	if(!setCallLock(LOCK1))
		return;

	log("�����");

	typedef RpcSimpleMethodAsynchCall<GameTest, XGUID> MethodLogout;

	MethodLogout call("Logout", this, &GameTest::logoutHandler, session);

	client->rpcAsynchCall(&call);
}

void GameTest::addScoreBySession(const XGUID& session, int score, unsigned type)
{
	if(!setCallLock(LOCK1))
		return;

	log("addScoreBySession");

	typedef RpcSimpleMethodAsynchCall<GameTest, RpcType::ScoreData> MethodAddScore;

	MethodAddScore call("AddScore", this, &GameTest::addScoreHandler,
		RpcType::ScoreData(score, type, "", &session));

	client->rpcAsynchCall(&call);
}

void GameTest::addScoreByName(const char* n, int score, unsigned type)
{
	if(!setCallLock(LOCK1))
		return;

	log("addScoreByName");

	typedef RpcSimpleMethodAsynchCall<GameTest, RpcType::ScoreData> MethodAddScore;

	MethodAddScore call("AddScore", this, &GameTest::addScoreHandler,
		RpcType::ScoreData(score, type, n));

	client->rpcAsynchCall(&call);
}

void GameTest::getScores(unsigned long start, unsigned count)
{
	if(!setCallLock(LOCK2))
		return;

	log((XBuffer() < "getScores(" <= start < ", " <= count < ")").c_str());

	typedef RpcMethodAsynchCall<GameTest, RpcType::GetScore, RpcType::ReturnScore> MethodGetScores;

	MethodGetScores call("GetScores", this, &GameTest::getScoresHandler,
		RpcType::GetScore(start, count));

	client->rpcAsynchCall(&call);
}

void GameTest::registerUserHandler(int status)
{
	switch(status){
	case STATUS_GOOD:
		log("registerUser: ����������� ������ �������");
		break;
	case STATUS_USER_NAME_EXIST:
		log("registerUser: ����� ��� ����");
	    break;
	default:
		log("registerUserHandler: ������");
	    break;
	}

	releaseCallLock(LOCK1);

}

void GameTest::loginHandler(int status)
{
	switch(status){
	case STATUS_GOOD:
		logined_ = true;
		log("login: �������� ����");
		break;
	case STATUS_BAD_USER_OR_PASSWORD:
		log("login: ��� ������ ������������ ��� �������� ������");
		break;
	default:
		log("login: ������");
		break;
	}

	releaseCallLock(LOCK1);

}

void GameTest::logoutHandler(int status)
{
	switch(status){
	case STATUS_GOOD:
		logined_ = false;
		log("logout: �������� �����");
		break;
	case STATUS_DOUBLE_OR_NOT_LOGON:
		log("logout: ���� � ���� ������ �� ����������");
		break;
	default:
		log("logout: ������");
		break;
	}

	releaseCallLock(LOCK1);
}


void GameTest::addScoreHandler(int status)
{
	switch(status){
	case STATUS_GOOD:
		logined_ = false;
		log("addScore: �������� ���������");
		break;
	case STATUS_DOUBLE_OR_NOT_LOGON:
		log("addScore: ���� � ���� ������ �� ����������");
		break;
	case STATUS_BAD_USER_OR_PASSWORD:
		log("addScore: ������ ���");
		break;
	default:
		log("logout: ������");
		break;
	}

	releaseCallLock(LOCK1);
}

void GameTest::getScoresHandler(int status, const RpcType::ReturnScore* scores)
{
	if(status == STATUS_GOOD && scores){
		log("getScoresHandler: �������� ����� �� ��������� �����");
		unsigned rank = scores->startRank;
		RpcType::Scores::const_iterator it = scores->scores.begin();
		for(;it != scores->scores.end(); ++it){
			int allscore = 0;
			RpcType::ScoreNode::UserScores::const_iterator it2 = it->scores.begin();
			for(;it2 != it->scores.end(); ++it2){
				allscore += it2->score;
				log((XBuffer() < "Type " <= it2->type < " Score " <= it2->score).c_str());
			}
			++rank;
			log((XBuffer() < "#" <= rank < "> " < it->login.c_str() < " --> " <= allscore).c_str());
		}
	}
	else
		log("getScoresHandler: ������ ���������");

	releaseCallLock(LOCK2);
}