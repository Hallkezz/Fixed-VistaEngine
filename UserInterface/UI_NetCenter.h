#ifndef __UI_NET_CENTER_H__
#define __UI_NET_CENTER_H__

#include "UI_Enums.h"
#include "Game\GlobalStatistics.h"
#include "Network\ExternalTask.h"
class Archive;
struct sGameHostInfo;
struct ChatChanelInfo;
class MissionDescription;
enum ScoresID;

class WBuffer;

typedef unsigned __int64 ChatChannelID;

struct ShowStatisticType;
typedef vector<ShowStatisticType> ShowStatisticTypes;

typedef std::vector<std::string> ComboStrings;
typedef vector<wstring> ComboWStrings;

class UI_NetCenter
{
	mutable MTSection lock_;
public:
	enum NetType{
		LAN,
		ONLINE,
		DIRECT
	};

	UI_NetCenter();
	~UI_NetCenter();

	/// ������ ������ ������� ����
	bool isNetGame() const;
	/// ��� ��������� ���� (host)
	bool isServer() const;

	/// ���� ��������� ����
	bool gameSelected() const;
	/// ���� ��������
	bool gameCreated() const { return gameCreated_; }

	/// ���������� ������
	void setPassword(const char* pass);
	/// ���������� ��������� ������
	void setPass2(const char* pass);
	/// �������� ������
	void resetPasswords();

	/// ������ ���������� ��������� �������
	//UI_NetStatus status() const { return netStatus_; }
	UI_NetStatus status() const;
	/// �������� ���������� ���������� �������
	//bool acyncEventWaiting() const { return netStatus_ == UI_NET_WAITING; }
	bool acyncEventWaiting() const;
	/// ���������� ������ ������� ����������� ������� (��������� ����������), �������� � ������� ����
	//void commit(UI_NetStatus status);

	/// ������� ������� ����������
	void create(NetType type);
	
	/// ������� �������� ������ ���
	void refreshGameList();
	/// �������������� ��������� �������� ������ � ������� ��������
	bool updateGameList();
	/// ������� ���� �� �������� ������
	void selectGame(int idx);
	/// ������� ������ ����� � ��������� ������� ��������� ���
	int getGameList(const GameListInfoTypes& infoType, ComboWStrings& strings, const Color4c& started);
	/// ���������� ���� ��������� ������� ����
	const MissionDescription& selectedGame() const { return selectedGame_; }
	/// ��� ��������� ����
	const wchar_t* selectedGameName(WBuffer& buf) const;
	/// ����� �������� �������
	const wchar_t* currentServerAddress(WBuffer& buf) const;
	
	void login();
	void logout();
	void quickStart();
	void createAccount();
	void changePassword();
	void deleteAccount();

	bool canCreateGame() const;
	/// ������� ������ ������� ����
	void createGame();
	bool canJoinGame() const;
	/// �������������� � ��������� ������� ����
	void joinGame();
	bool canJoinDirectGame() const;
	/// �������������� � ���������� �������
	void joinDirectGame();
	/// ���������� ������ ����
	void startGame();

	const wchar_t* natType() const;
	
	/// �������� ������� ��������
	void abortCurrentOperation();
	
	/// �������� ������� ����, ������ ������ �����
	void reset(bool flag_internalReset=false);
	/// ��������� ��������� ������ � �����
	void release();

	void teamConnect(int teemIndex);
	void teamDisconnect(int teemIndex, int cooperativeIndex);

	/// ��������� ������ ��� �������
	void setChatString(const wchar_t* stringForSend);
	/// �������� ������
	bool sendChatString(int rawIntData);
	/// ���������� ��������� ��������� ���� �� ����
	void handleChatString(const wchar_t* str);
	/// �������� ������ ��������� ���� � ������� ��������� �������
	void getChatBoard(ComboWStrings &board) const;
	/// �������������������� ���
	void resetChatBoard(bool unsubscribe = true);
	void clearChatBoard();
	
	/// ������� �� ������ �������� ������ ������ ������ ���� � ������� � ����
	void refreshChatChannels();
	/// ���������������� ��� � ������� ��������
	void updateChatChannels();
	/// �������� ������ ������ ����
	int getChatChannels(ComboWStrings& channels) const;
	/// �������� ��� ������� ��������� ������� ����
	void getCurrentChatChannelName(wstring& name) const;
	/// ������� ������� � ����
	void selectChatChannel(int channel);
	/// ����� � ��������� ������� � ����
	void enterChatChannel(ChatChannelID forceID = -1);
	/// ������ �������� ����� ��������� � ���
	bool autoSubscribeMode() const { return autoSubscribeMode_; }

	void chatSubscribeOK();
	void chatSubscribeFailed();

	void updateChatUsers();
	int getChatUsers(ComboWStrings& users) const;

	void queryGlobalStatistic(bool force = false);
	void getGlobalStatisticFromBegin();
	void getAroundMeGlobalStats();
	bool canGetPrevGlobalStats() const;
	void getPrevGlobalStats();
	bool canGetNextGlobalStats() const;
	void getNextGlobalStats();
	int getGlobalStatistic(const ShowStatisticTypes& format, ComboWStrings &board) const;
	void selectGlobalStaticticEntry(int idx);
	int getCurrentGlobalStatisticValue(StatisticType type);
	
	void setPausePlayerList(const ComboWStrings& playerList);
	void getPausePlayerList(ComboWStrings& playerList) const;
	bool isOnPause() const { return isOnPause_; }

	void updateFilter();

	void quant();
private:
	void clear();
	void autoSubscribeChatChannel();

	//UI_NetStatus netStatus_;
	bool flag_lastNetCommandOk;

	enum DelayOperationType{
	//	DELAY_NONE,
	//	CREATE_GAME,
	//	ONLINE_CREATE_LOGIN,
	//	ONLINE_NEW_LOGIN,
	//	ONLINE_DELETE_LOGIN,
	//	ONLINE_CHECK_VERSION,
	//	QUERY_GLOBAL_STATISTIC
	};
	DelayOperationType delayOperation_;

	typedef vector<sGameHostInfo> GameHostInfos;
	GameHostInfos netGames_;
	int selectedGameIndex_;

	sGameHostInfo& selectedGameInfo_;
	MissionDescription& selectedGame_;

	bool gameCreated_;
	bool onlineLogined_;

	string password_;
	string pass2_;

	typedef vector<ChatChanelInfo> ChatChanelInfos;
	ChatChanelInfos chatChanelInfos_;
	ChatChannelID subscribedChannel_;
	ChatChannelID subscribeWaitingChannel_;
	ChatChannelID selectedChannel_;
	bool autoSubscribeMode_;
	bool autoSubscribeUnsusseful_;
	bool lastChannelSubscribeAttemptMode_;
	ChatChannelID lastSubscribeAttempt_;
	wstring currentChatChannelName_;

	wstring currentChatString_;
	
	ComboWStrings chatUsers_;
	ComboWStrings chatBoard_;

	ComboWStrings pausePlayerList_;
	bool isOnPause_;

	//void setStatus(UI_NetStatus status);

	int currentStatisticFilterRace_;
	int currentStatisticFilterGamePopulation_;
	int firstScorePosition_;
	//GlobalStatistics gsForRead_;
	GlobalStatistics globalStatistics_;
	int selectedGlobalStatisticIndex_;
	void applyNewGlobalStatistic(const GlobalStatistics& stats);
	XBuffer version_;
	void queryGameVersion();
	bool setGameVersion();

	ExternalNetTask_Init extNetTask_Init;
	ENTCreateAccount extNetTask_CreateAccount;
	ENTDeleteAccount extNetTask_DeleteAccount;
	ENTChangePassword extNetTask_ChangePassword;
	ENTLogin extNetTask_Login;
	ENTDownloadInfoFile extNetTask_DownloadInfoFile;
	ENTReadGlobalStats extNetTask_ReadGlobalStats;
	ENTSubscribe2ChatChannel extNetTask_Subscribe2ChatChannel;
	ENTGame extNetTask_Game;
};

#endif // __UI_NET_CENTER_H__