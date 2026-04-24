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

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of
the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#define MAX_INVENTORYVALUE 999999
#define EVALUATERECURSIVELY

#define MAX_WEIGHT_FILES	128
#define MAX_FUZZY_OPERATORS 8192

/*!
	\class idBotFuzzyWeightManager
	\brief Manages fuzzy weight configurations and calculations for bot behavior.

	The idBotFuzzyWeightManager class provides functionality for initializing, configuring, and computing fuzzy weights used in bot decision-making. It handles parsing weight configuration files,
   managing fuzzy separator structures, and performing operations like weight scaling, evolution, and interbreeding. The class supports both basic fuzzy weight calculations and specialized
   computations for undecided states. It also includes memory management functions for allocating and freeing fuzzy separator structures and weight configurations. The manager is designed to work with
   inventory-based inputs to determine appropriate fuzzy weights for various bot behaviors.

*/
class idBotFuzzyWeightManager
{
public:
	//! Initializes the fuzzy weight manager by clearing the fuzzy separators array.
	void			Init();

	//! Parses a weight configuration file and returns a pointer to the parsed configuration
	weightconfig_t* ReadWeightConfig( char* filename );

	//! Initializes the weight file list for the bot fuzzy weight manager.
	void			BotShutdownWeights();

	//! Finds the index of a fuzzy weight by its name in the weight configuration
	int				FindFuzzyWeight( weightconfig_t* wc, char* name );

	//! Computes a fuzzy weight value based on inventory and weight configuration.
	float			FuzzyWeight( int* inventory, weightconfig_t* wc, int weightnum );

	//! Calculates a fuzzy weight value for an undecided state based on inventory and weight configuration.
	float			FuzzyWeightUndecided( int* inventory, weightconfig_t* wc, int weightnum );

	//! Recursively evolves a fuzzy separator by applying random mutations to its weight within defined bounds.
	void			EvolveFuzzySeperator_r( fuzzyseperator_t* fs );

	//! Evolve the fuzzy separator configuration for each weight in the provided configuration.
	void			EvolveWeightConfig( weightconfig_t* config );

	//! Scales the fuzzy weights for a specified configuration by a given factor
	void			ScaleWeight( weightconfig_t* config, char* name, float scale );

	//! Scales the fuzzy balance range for a given weight configuration by a specified factor.
	void			ScaleFuzzyBalanceRange( weightconfig_t* config, float scale );

	//! Interbreeds two fuzzy weight configurations into a third configuration.
	void			InterbreedWeightConfigs( weightconfig_t* config1, weightconfig_t* config2, weightconfig_t* configout );

	//! Frees the memory allocated for a weight configuration object.
	void			FreeWeightConfig( weightconfig_t* config );

private:
	//! Allocates and returns a new fuzzy weight structure from the manager's pool.
	fuzzyseperator_t* AllocFuzzyWeight();

	//! Reads a floating-point value from the parser, handling negative values by setting them to zero.
	bool			  ReadValue( idParser& source, float* value );

	//! Reads a fuzzy weight configuration from a parser source and stores it in the provided fuzzy separator structure.
	int				  ReadFuzzyWeight( idParser& source, fuzzyseperator_t* fs );

	//! Combines two fuzzy separator configurations into a third configuration through recursive interbreeding.
	int				  InterbreedFuzzySeperator_r( fuzzyseperator_t* fs1, fuzzyseperator_t* fs2, fuzzyseperator_t* fsout );

	//! Parses fuzzy separator definitions from a parser source and returns a linked list of fuzzy separator structures.
	fuzzyseperator_t* ReadFuzzySeperators_r( idParser& source );

	//! Frees the memory associated with a weight configuration's separators
	void			  FreeWeightConfig2( weightconfig_t* config );

	//! Frees all fuzzy separators in the given fuzzy separator tree.
	void			  FreeFuzzySeperators_r( fuzzyseperator_t* fs );

	//! Scales the weight range of balance fuzzy separators recursively
	void			  ScaleFuzzySeperatorBalanceRange_r( fuzzyseperator_t* fs, float scale );

	//! Recursively scales fuzzy separator weights by a given factor while maintaining bounds.
	void			  ScaleFuzzySeperator_r( fuzzyseperator_t* fs, float scale );

	//! Recursively computes a fuzzy weight based on inventory values and fuzzy logic separators.
	float			  FuzzyWeight_r( int* inventory, fuzzyseperator_t* fs );

	//! Computes a fuzzy weight for an undecided state based on inventory and fuzzy separator values.
	float			  FuzzyWeightUndecided_r( int* inventory, fuzzyseperator_t* fs );

	weightconfig_t	  weightFileList[MAX_WEIGHT_FILES];
	fuzzyseperator_t  fuzzyseperators[MAX_FUZZY_OPERATORS];
};

extern idBotFuzzyWeightManager botFuzzyWeightManager;
