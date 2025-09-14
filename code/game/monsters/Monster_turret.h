
#pragma once

class iceMonster_Turret : public idAI
{
	CLASS_PROTOTYPE( iceMonster_Turret );
public:

	virtual void				Init( ) override;
	virtual void				AI_Begin( ) override;

protected:
	//
	// actions
	//


	void						destory( ) { };

	bool						canHit( );
	void						spawn_light( ) { };
	void						light_off( ) { };
	void						light_on( ) { };

	// anim states
	stateResult_t				Torso_Death( stateParms_t* parms );
	stateResult_t				Torso_Idle( stateParms_t* parms ) ;
	stateResult_t				Torso_Attack( stateParms_t* parms );
	stateResult_t				Torso_CustomCycle( stateParms_t* parms );
	stateResult_t				state_Killed( stateParms_t* parms );

	virtual bool				checkForEnemy( float use_fov ) override;
private:
	//
	// States
	//
	stateResult_t				state_WakeUp( stateParms_t* parms );
	stateResult_t				state_Begin( stateParms_t* parms );
	stateResult_t				state_Idle( stateParms_t* parms );
	stateResult_t				state_Combat( stateParms_t* parms );
	stateResult_t				combat_attack( stateParms_t* parms );
	stateResult_t				state_Disabled( stateParms_t* parms );

	bool		fire;
	bool		attack_monsters;
	idEntity	light;
	bool		light_is_on;
	float		attackTime;

	int			barrelCount;
	int			currentBarrel;
	idStr		currentBarrelStr;

	idList< jointHandle_t> flashJointWorldHandles;
};
