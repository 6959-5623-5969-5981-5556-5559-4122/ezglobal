#include "CommonIncludes.h"
#include "SDK.h"

#include <vector>

namespace Hacks
{
	void SetupHacks();
	void DrawHacks();
	void MoveHacks(CUserCmd *pCmd, bool &bSendPacket);
}

class CHack
{
public:
	virtual void Draw() = 0;
	virtual void Move(CUserCmd *pCmd, bool &bSendPacket) = 0;
	virtual void Init() = 0;
};

class HackManager
{
public:
	void RegisterHack(CHack* hake);
	void Ready();

	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	C_BaseEntity *pLocal();
private:
	std::vector<CHack*> Hacks;
	C_BaseEntity *pLocalInstance;
	bool IsReady;
};

extern HackManager hackManager;