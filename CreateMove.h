#pragma once
#include "HookIncludes.h"
#include "Misc.h"
#include "MovementRecorder.h"
#include "RageBot.h"
#include "LegitBot.h"
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "ESP.h"

using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);
QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);
void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
	static auto ofunc = hooks::client.get_original<create_move_t>(22);
	ofunc(g_CHLClient, sequence_number, input_sample_frametime, active);
	C_BaseEntity *local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CInput::CUserCmd *cmd = g_Input->GetUserCmd(0, sequence_number);
	if (!cmd)
		return;

	static float next_time = 0;
	if (!Globals::ySwitch && local->IsAlive() && local->GetVelocity().Length2D() == 0)
	{

		float TickStuff = TICKS_TO_TIME(local->GetTickBase());

		Globals::shouldflip = false;

		bool moveFlip = false;

		Globals::NextTime = next_time;

		if (next_time - TICKS_TO_TIME(local->GetTickBase()) > 1.1)
		{
			next_time = 0;
			// Junk
			bool TAtBDZQsOEmuvnPPp;
			char cbAzuxMheCtHlEGrP = 83769;
			int BxpHooHpKwqyUhRQzHnbnfUvqn = 4587469;
			float VLieXdZuZOsRkRusVoJBrzmWzpoWkjIlEF;
			int mdIgPNPUUhGxpqN = 142493;
			int sNHhLQQRZU = 264324613286844;
			char MzcnO;
			int fAjFrmXOT = sNHhLQQRZU + 76167594;
			int jfrimPMYHchzRIWKBarxBgFcBce = 81129551446996342;
			char fBxvF = 5363;
			char kCOtzGkUbMjrmBSZtssHfROEfWSjRXJh;
			int YuPZWwEIquRp = 1858768498142658617;
			float jhaimuCZFskUidubu;
			const char* wtB;
			const char* UyKabFcvkDZeZtoidukuHjZefczzzBF = "QdpqZiKF";
			int cdHtHxdJG = YuPZWwEIquRp + 43774;
			int AMCeR = 5728588626762;
			float JYtiHrcQaYbNefxnxvgZEdyyhNGYcdr;
			int phmEcKOTRw = AMCeR + 8;
			int ghfOZQVlqfXWwKmfX = 55948846992223;
			int jGHCWMbMxrmHuyfy = 22398466212614373;
			int MyUbzazcpKhkTkydZiVc = jGHCWMbMxrmHuyfy + 5189;
			char iSGeKYRGzvXdHPoamVAFyRAR;
			float KhtqdIheRsigbhzJZUJXMmYspNARAewKEg;
			// JUnk
		}

		if (local->GetVelocity().Length2D() > 1)
		{
			moveFlip = true;
			// Junk
			const char* WqrWwWPzsqMeToGEpSPxNVjiuPalYY = "MkmnUvkL";
			int DkjMjqWIQPuoqhImVFvuZwYvIjgf = 569236254812224168;
			bool JFJn = true;
			float sQCtydteaMdCjBPrHtneG;
			bool XGcjWlgrSm = true;
			int aRWTvJSJB = 45179692553991;
			int wkIFYGTyDhACwYhs = 3129597662241;
			int HnihrEqGJfwKjsMTPJHaEFQzAaUldRch = wkIFYGTyDhACwYhs + 18567382;
			char uYeeGfiVDRcYgUrQdreDAKu;
			int guProQYlOBCwQXkzMAusjgMhCmhGdnm = 35781983;
			const char* RFfaclvgRZkkLdac;
			int eojOMONx = 176882426;
			const char* AcXXaeDrWUdxZRFuSewjTBO = "VyNFhKSTc";
			int umNDDxfdmvzcaUTdCCKXAaYdZsXtTO = 53598233;
			int joGLmKxGbfcpYPcKqChB = umNDDxfdmvzcaUTdCCKXAaYdZsXtTO + 3328;
			bool mqRlXvObt = false;
			int gINfGfRcclMZ = 288222154;
			float hUBsJOlLmsQSghmCPheLsOTFLB;
			int nPcu = 584875955;
			int pqUGRIWNiQXdfJpwPxFB = nPcu + 596;
			int LFJMtkuQPZoMwnlMb = nPcu + 25451292;
			const char* dSYDs = "vFPE";
			int wBoriFSU = 79;
			int MKhsSmVN = 831663;
			// JUnk
		}

		if (local->GetVelocity().Length2D() == 0 && !moveFlip)
		{
			if (TickStuff > next_time + 1.1f)
			{
				next_time = TickStuff + TICKS_TO_TIME(1);
				Globals::shouldflip = true;
				// Junk
				char bABVQoPqfOViUsSQFqW;
				int xDzUjTlntxMRJxX = 15924;
				int IolydniLqA = xDzUjTlntxMRJxX + 736964;
				float HhjmbOCLjARInTdWCldVDXROPByJZW;
				int jUotHWMlvR = 86789271812271;
				char CrSwSa;
				bool oijO = false;
				const char* GoBqpnCKWvNhkeBmTDhVygkJ;
				char xHatMuUDqbhMknHfXAxqxkjlZzQTs = 1449848;
				bool olXJOqkDVfePVlHVHRwTfDUKXffJp;
				int YxLijjHevRyXOZbafpFMoWGrGO = 781369534894;
				int djhmLTedzLOD = YxLijjHevRyXOZbafpFMoWGrGO + 156936132;
				const char* rjkPXInOvgzDt;
				int wOOxVJJmPSVKnHKWJOPMmLebKS = YxLijjHevRyXOZbafpFMoWGrGO + 43795;
				float wCmBVbhRMloWpJBwyvWtGtoqNm;
				int apzARKnnXtuufAr = 36375545679675691;
				int rTJXyaaYKqRxycRxUWjxNpQjNbKcP = apzARKnnXtuufAr + 51329;
				int emVbQLzsVxZKutaPpWGzpDPyZkzCvBn = apzARKnnXtuufAr + 296922;
				// JUnk
			}
		}
		else if (moveFlip && local->GetVelocity().Length2D() > .1)
		{
			if (TickStuff > next_time + 0.22f)
			{
				next_time = TickStuff + TICKS_TO_TIME(1);
				Globals::shouldflip = true;
				moveFlip = false;
			}
		}
	}

	if (!cmd->command_number)
		return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)((uint32_t)g_Input + 0xF8);
	CInput::CVerifiedUserCmd *verified = &(*(CInput::CVerifiedUserCmd**)((DWORD)g_Input + 0xF8))[sequence_number % 150];
	if (!verified)
		return;

	if (!local)
		return;
	QAngle oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;

	backtracking->legitBackTrack(cmd, local);
	backtracking->RageBackTrack(cmd, local);

	RankReveal(cmd);
	misc::FakeWalk(cmd, bSendPacket, local);

	misc::instance().OnCreateMove(cmd, local);

	if (g_Options.Misc.AutoStrafe)
		misc::AutoStrafe(cmd, local);

	legitbot::instance().OnCreateMove(cmd, local);
	ragebot::instance().OnCreateMove(cmd, bSendPacket);



	grenade_prediction::instance().Tick(cmd->buttons);

	if (!bSendPacket && g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;
	else if (bSendPacket && !g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;

	if (g_Options.Misc.antiuntrusted)
		if (!sanitize_angles(cmd->viewangles))
			return;

	movementfix(oldAngle, cmd);

	if (g_Options.Misc.antiuntrusted)
		clampMovement(cmd);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();


	if (Globals::ySwitch) {
		Globals::FakeAngle = cmd->viewangles.y;
	}
	else {
		Globals::RealAngle = cmd->viewangles.y;
	}
}
#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	input_sample_frametime
		PUSH	sequence_number
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}

