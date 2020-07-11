#pragma once
#include "gameNode.h"

enum class WEAPON_CODE;

class wormManager;

class uiManager : public gameNode
{
private:
	wormManager* _wormManager;

	bool _timerVisible = false;
	int _timer = 0;

	bool _windVisible = false;
	float _wind = 0;
	int _windOffset = 0;

	bool _teamHpVisible = false;
	vector<int> _teamHps;
	int _maxHp;

	bool _weaponVisible = false;
	vector<pair<RECT, WEAPON_CODE>> _weaponRC;
	map<WEAPON_CODE, int> _weaponCount;
	map<WEAPON_CODE, string> _weaponName;

	void drawTimer();
	void drawWind();
	void drawTeamHp();
	void drawWeapons();

	HBRUSH _blackBrush;

	HPEN _white2Pen;
	HPEN _white1Pen;
	HPEN _black1Pen;

	HFONT _timerFont;
	HFONT _wormFont;
	HFONT _weaponFont;

	vector<COLORREF> _fontColors {RGB(233, 153, 228), RGB(233, 235, 145), RGB(109, 204, 107), RGB(168, 168, 210), RGB(123, 208, 222)};
	vector<HBRUSH> _teamBrush;
	vector<HPEN> _teamPen;

public:
	uiManager() {}
	~uiManager() {}

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setWormManager(wormManager* wormManager) { _wormManager = wormManager; }

	void setTimerVisible(bool visible) { _timerVisible = visible; }
	void setTimer(int timer) { _timer = timer; }

	void setWindVisible(bool visible) { _windVisible = visible; }
	void setWind(float wind) { _wind = wind; }

	void setTeamHpVisible(bool visible) { _teamHpVisible = visible; };
	void setTeamHp(int maxHp, vector<int> hps);

	bool getWeaponVisible() { return _weaponVisible; }
	void setWeaponToggle() { _weaponVisible = !_weaponVisible; }
	void setWeaponVisible(bool visible) { _weaponVisible = visible;}
	void setWeapons(map<WEAPON_CODE, int> weaponCount);

	void drawWormUI(float x, float y, int hp, int playerIndex, string name);
};

