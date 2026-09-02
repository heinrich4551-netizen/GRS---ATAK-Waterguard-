// ============================================================================
// RHD / PROJECT33 - CONTRACT RUNTIME
// ============================================================================
// Uses existing civilian AI supplied by the mission. No contract NPCs are
// spawned by Project33.
// ============================================================================
class RHD_ContractRuntime
{
	static void RefreshAvailable(RHD_VirtualPlayerState state, RHD_VirtualPlayerMissionAdapter mission)
	{
		if (!state || !mission || state.m_aContracts.Count() >= RHD_ContractConfig.MAX_AVAILABLE_CONTRACTS) return;
		array<RHD_ContractDefinition> definitions;
		RHD_ContractService.BuildDefaultDefinitions(definitions);
		foreach (RHD_ContractDefinition definition : definitions)
		{
			if (!definition || !RHD_ContractService.IsDefinitionUnlocked(definition, state.m_Progression)) continue;
			if (HasDefinition(state, definition.m_sId)) continue;
			string source = mission.FindRandomCivilian("LOCAL_PLAYER");
			string destination = mission.FindRandomCivilian("LOCAL_PLAYER");
			if (source.IsEmpty() || destination.IsEmpty() || source == destination) continue;
			if (!mission.IsCivilianCharacter(source) || !mission.IsCivilianCharacter(destination)) continue;

			RHD_ContractEntry entry = new RHD_ContractEntry();
			entry.m_sInstanceId = definition.m_sId + "_" + state.m_aContracts.Count().ToString();
			entry.m_sDefinitionId = definition.m_sId;
			entry.m_sTitle = definition.m_sTitle;
			entry.m_sType = definition.m_sType;
			entry.m_sFaction = definition.m_sFaction;
			entry.m_sSourceCivilian = source;
			entry.m_sDestinationCivilian = destination;
			entry.m_sVirtualItemId = definition.m_sVirtualItemId;
			entry.m_iReward = Math.Max(RHD_ContractConfig.MIN_REWARD, Math.Min(RHD_ContractConfig.MAX_REWARD, definition.m_iReward));
			entry.m_iXP = Math.Max(0, Math.Min(RHD_ContractConfig.MAX_XP_REWARD, definition.m_iXP));
			entry.m_iReputation = Math.Max(0, Math.Min(RHD_ContractConfig.MAX_REPUTATION_REWARD, definition.m_iReputation));
			entry.m_iStage = RHD_ContractStage.AVAILABLE;
			state.m_aContracts.Insert(entry);
			if (state.m_aContracts.Count() >= RHD_ContractConfig.MAX_AVAILABLE_CONTRACTS) return;
		}
	}

	static bool Accept(RHD_VirtualPlayerState state, string instanceId)
	{
		if (!state || instanceId.IsEmpty() || CountAccepted(state) >= RHD_ContractConfig.MAX_ACTIVE_CONTRACTS) return false;
		RHD_ContractEntry entry = Find(state, instanceId);
		if (!entry || entry.m_bAccepted || entry.m_bComplete || entry.m_iStage != RHD_ContractStage.AVAILABLE) return false;
		entry.m_bAccepted = true;
		entry.m_iStage = RHD_ContractStage.ACCEPTED;
		return true;
	}

	static bool ContactSource(RHD_VirtualPlayerState state, RHD_VirtualPlayerMissionAdapter mission, string instanceId, string playerId)
	{
		RHD_ContractEntry entry = Find(state, instanceId);
		if (!entry || !entry.m_bAccepted || entry.m_iStage != RHD_ContractStage.ACCEPTED) return false;
		if (!mission || !mission.IsPlayerNearCivilian(playerId, entry.m_sSourceCivilian, RHD_ContractConfig.CONTRACT_INTERACTION_DISTANCE_M)) return false;
		entry.m_iStage = RHD_ContractStage.SOURCE_CONTACT;
		return true;
	}

	static bool ContactDestination(RHD_VirtualPlayerState state, RHD_VirtualPlayerMissionAdapter mission, string instanceId, string playerId)
	{
		RHD_ContractEntry entry = Find(state, instanceId);
		if (!entry || !entry.m_bAccepted || entry.m_iStage != RHD_ContractStage.SOURCE_CONTACT) return false;
		if (!mission || !mission.IsPlayerNearCivilian(playerId, entry.m_sDestinationCivilian, RHD_ContractConfig.CONTRACT_INTERACTION_DISTANCE_M)) return false;
		entry.m_iStage = RHD_ContractStage.DELIVERY;
		return true;
	}

	static bool Deliver(RHD_VirtualPlayerState state, RHD_VirtualPlayerMissionAdapter mission, RHD_ATAKPlayerState economy, string instanceId, string playerId)
	{
		RHD_ContractEntry entry = Find(state, instanceId);
		if (!entry || !entry.m_bAccepted || entry.m_iStage != RHD_ContractStage.DELIVERY || !mission || !economy) return false;
		if (!mission.IsPlayerNearCivilian(playerId, entry.m_sDestinationCivilian, RHD_ContractConfig.CONTRACT_INTERACTION_DISTANCE_M)) return false;
		if (state.GetVirtualItemQuantity(entry.m_sVirtualItemId) < 1) return false;
		if (!state.RemoveVirtualItem(entry.m_sVirtualItemId, 1)) return false;

		economy.AddMoney(entry.m_iReward);
		if (state.m_Progression)
		{
			state.m_Progression.AddXP(entry.m_iXP);
			state.m_Progression.AddReputation(entry.m_sFaction, entry.m_iReputation);
			state.m_Progression.RecordContractCompleted(entry.m_iReward);
		}
		entry.m_iStage = RHD_ContractStage.COMPLETED;
		entry.m_bComplete = true;
		return true;
	}

	static bool Fail(RHD_VirtualPlayerState state, string instanceId)
	{
		RHD_ContractEntry entry = Find(state, instanceId);
		if (!entry || !entry.m_bAccepted || entry.m_bComplete) return false;
		entry.m_iStage = RHD_ContractStage.FAILED;
		if (state.m_Progression)
		{
			state.m_Progression.RecordContractFailed();
			state.m_Progression.AddReputation(entry.m_sFaction, -RHD_ProgressionConfig.CONTRACT_FAILURE_REPUTATION_PENALTY);
		}
		return true;
	}

	static int CountAccepted(RHD_VirtualPlayerState state)
	{
		int count = 0;
		if (!state) return count;
		foreach (RHD_ContractEntry entry : state.m_aContracts)
			if (entry && entry.m_bAccepted && !entry.m_bComplete && entry.m_iStage != RHD_ContractStage.FAILED) count += 1;
		return count;
	}

	static RHD_ContractEntry Find(RHD_VirtualPlayerState state, string instanceId)
	{
		if (!state || instanceId.IsEmpty()) return null;
		foreach (RHD_ContractEntry entry : state.m_aContracts)
			if (entry && entry.m_sInstanceId == instanceId) return entry;
		return null;
	}

	static bool HasDefinition(RHD_VirtualPlayerState state, string definitionId)
	{
		if (!state || definitionId.IsEmpty()) return false;
		foreach (RHD_ContractEntry entry : state.m_aContracts)
			if (entry && entry.m_sDefinitionId == definitionId && entry.m_iStage != RHD_ContractStage.COMPLETED && entry.m_iStage != RHD_ContractStage.FAILED) return true;
		return false;
	}

	static string GetSummary(RHD_VirtualPlayerState state)
	{
		if (!state) return "CONTRACTS: UNAVAILABLE";
		string result = "CONTRACTS ACTIVE: " + CountAccepted(state).ToString() + "/" + RHD_ContractConfig.MAX_ACTIVE_CONTRACTS.ToString();
		foreach (RHD_ContractEntry entry : state.m_aContracts)
		{
			if (!entry || entry.m_iStage == RHD_ContractStage.COMPLETED || entry.m_iStage == RHD_ContractStage.FAILED) continue;
			result += "\n" + entry.m_sInstanceId + " | " + entry.m_sTitle + " | " + entry.m_iReward.ToString() + " | STAGE " + entry.m_iStage.ToString();
		}
		return result;
	}
};
