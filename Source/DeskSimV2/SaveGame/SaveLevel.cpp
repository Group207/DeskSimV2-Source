/*****************************************************************/
/**
 * @file   SaveLevel.cpp
 * @brief  Handles what Data that is stored in the save files as well as its default values
 * 
 * @author JohnOle
 * @date   March 2022
 *********************************************************************/
// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "SaveLevel.h"

/**
 * @brief Constructor, Sets Default values.
 * 
 */
USaveLevel::USaveLevel()
{
	UserIndex = 0;
	SaveSlotName = TEXT("Slot1");
}

