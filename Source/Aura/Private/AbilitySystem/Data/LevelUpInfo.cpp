// -sam2099


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const 
{
	// start at index 1 because the first entry will be for level "0" which means nothing here and is just a placeholder spot
	for (int32 i = 1; i < LevelUpInformation.Num(); i++)
	{
		if (XP < LevelUpInformation[i].LevelUpRequirement)
		{
			return i;
		}
	}
	return 1;
}
