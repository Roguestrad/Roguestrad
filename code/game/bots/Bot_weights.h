/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2021 Justin Marshall

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#define MAX_INVENTORYVALUE			999999
#define EVALUATERECURSIVELY

#define MAX_WEIGHT_FILES			128
#define MAX_FUZZY_OPERATORS			8192

class idBotFuzzyWeightManager
{
public:
	// Init the fuzzy weight manager.
	void Init();

	// Parses a weight config file.
	weightconfig_t* ReadWeightConfig( char* filename );

	void BotShutdownWeights();

	// Fuzzy Weight simulation functions.
	int FindFuzzyWeight( weightconfig_t* wc, char* name );
	float FuzzyWeight( int* inventory, weightconfig_t* wc, int weightnum );
	float FuzzyWeightUndecided( int* inventory, weightconfig_t* wc, int weightnum );
	void EvolveFuzzySeperator_r( fuzzyseperator_t* fs );
	void EvolveWeightConfig( weightconfig_t* config );
	void ScaleWeight( weightconfig_t* config, char* name, float scale );
	void ScaleFuzzyBalanceRange( weightconfig_t* config, float scale );
	void InterbreedWeightConfigs( weightconfig_t* config1, weightconfig_t* config2, weightconfig_t* configout );
	void FreeWeightConfig( weightconfig_t* config );
private:
	fuzzyseperator_t* AllocFuzzyWeight();
	bool ReadValue( idParser& source, float* value );
	int ReadFuzzyWeight( idParser& source, fuzzyseperator_t* fs );

	int InterbreedFuzzySeperator_r( fuzzyseperator_t* fs1, fuzzyseperator_t* fs2, fuzzyseperator_t* fsout );

	fuzzyseperator_t* ReadFuzzySeperators_r( idParser& source );

	void FreeWeightConfig2( weightconfig_t* config );
	void FreeFuzzySeperators_r( fuzzyseperator_t* fs );

	void ScaleFuzzySeperatorBalanceRange_r( fuzzyseperator_t* fs, float scale );

	void ScaleFuzzySeperator_r( fuzzyseperator_t* fs, float scale );
	float FuzzyWeight_r( int* inventory, fuzzyseperator_t* fs );
	float FuzzyWeightUndecided_r( int* inventory, fuzzyseperator_t* fs );

	weightconfig_t weightFileList[MAX_WEIGHT_FILES];
	fuzzyseperator_t fuzzyseperators[MAX_FUZZY_OPERATORS];
};

extern idBotFuzzyWeightManager botFuzzyWeightManager;