#include "LAppInterface.hpp"
#include "LAppModel.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppDefine.hpp"

using namespace Csm;
using namespace LAppDefine;
namespace
{
	LAppInterface* s_Instance =	NULL;
}

LAppInterface::LAppInterface()
{
}

LAppInterface::~LAppInterface()
{
}

LAppInterface* LAppInterface::GetInstance()
{
	if (s_Instance == NULL)
	{
		s_Instance = new LAppInterface();
	}
	return s_Instance;
}

void LAppInterface::ReleaseInstance()
{
	if (s_Instance != NULL)
	{
		delete s_Instance;
	}
	s_Instance = NULL;
}

void LAppInterface::SetMotion(MotionType motiontype)
{
	current_motiontype = motiontype;
	LAppLive2DManager::GetInstance()->GetModel(1)->StartMotion(LAppDefine::MotionGroupIdle,1,LAppDefine::PriorityIdle);
}

void LAppInterface::InitializeDelegate()
{
	if (!LAppDelegate::GetInstance()->Initialize())
	{
		LAppDelegate::GetInstance()->Release();
		LAppDelegate::ReleaseInstance();
		return;
	}
	LAppDelegate::GetInstance()->Run();
}
