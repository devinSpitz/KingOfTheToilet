//------------------------------------------------------------------------------------------------
class RLC_KOTTAddScoreActionContext : ScriptedUserAction
{
	
	[Attribute("", UIWidgets.ResourceNamePicker, desc: "Sound project (acp)")]
	private ResourceName m_SoundProject;
	
	protected AudioHandle m_AudioHandle;
	private SCR_ScoringSystemComponent m_scoreManager;
	float lastActiveTimer = 0;
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{				
		if(!(lastActiveTimer<=GetGame().GetWorld().GetWorldTime()))
		{
			return;
		}
		GetScoreManager();
		SCR_ChimeraCharacter cc = SCR_ChimeraCharacter.Cast(pUserEntity);
		m_scoreManager.AddFactionObjective(cc.GetFaction(),1);
		lastActiveTimer = GetGame().GetWorld().GetWorldTime()+10000;
		PlaySound(pOwnerEntity);
		//m_scoreManager
	}
	
	void PlaySound(IEntity pOwnerEntity)
	{
	// Get global signals manager
		GameSignalsManager globalSignalsManager = GetGame().GetSignalsManager();
		
		// SetSignals
		ref array<string> signalName = new array<string>;
		ref array<float> signalValue = new array<float>;
		
		signalName.Insert("GInterior");
		signalName.Insert("GIsThirdPersonCam");
		signalName.Insert("GCurrVehicleCoverage");
		
		foreach(string signal : signalName)
		{
			signalValue.Insert(globalSignalsManager.GetSignalValue(globalSignalsManager.AddOrFindSignal(signal)));
		}
			
		// Set sound position
		vector mat[4];		
		mat[3] = pOwnerEntity.GetOrigin();
		
		// Play sound
		m_AudioHandle = AudioSystem.PlayEvent(m_SoundProject, "SOUND_TOILET", mat, signalName, signalValue);
	}
		
	
	void GetScoreManager()
	{
		if(!m_scoreManager)
		{
			BaseWorld world = GetOwner().GetWorld();
			IEntity entity = world.FindEntityByName("KOTT");
			m_scoreManager =  SCR_ScoringSystemComponent.Cast(entity.FindComponent(SCR_ScoringSystemComponent));
		}
	}

	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#AR-UserAction_FlushToilet";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		GetScoreManager();
		
		if(!(lastActiveTimer<=GetGame().GetWorld().GetWorldTime()))
		{
			return false;
		}
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~RLC_KOTTAddScoreActionContext()
	{
		// Tesminate 
	}
};