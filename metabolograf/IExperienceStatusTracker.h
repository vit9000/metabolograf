#pragma once

class IExperienceStatusTracker
{
public:
	virtual bool GetExperienceStatus()=0;
	virtual void SetExperienceMode()=0;
};

