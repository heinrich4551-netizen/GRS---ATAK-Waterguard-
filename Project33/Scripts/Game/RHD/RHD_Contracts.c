// ============================================================================
// RHD / PROJECT33 - CONTRACTS
// ============================================================================
// Contracts are gameplay data only. Target/source characters remain mission
// content; Project33 never spawns replacement job civilians.
// ============================================================================
class RHD_ContractDefinition
{
	string m_sId;
	string m_sTitle;
	string m_sDescription;
	string m_sType;
	string m_sFaction;
	string m_sVirtualItemId;
	int m_iReward;
	int m_iXP;
	int m_iReputation;
	int m_iRequiredLevel;
};

class RHD_ContractEntry
{
	string m_sInstanceId;
	string m_sDefinitionId;
	string m_sTitle;
	string m_sType;
	string m_sFaction;
	string m_sSourceCivilian;
	string m_sDestinationCivilian;
	string m_sVirtualItemId;
	int m_iReward;
	int m_iXP;
	int m_iReputation;
	int m_iStage;
	bool m_bAccepted;
	bool m_bComplete;
};

enum RHD_ContractStage { AVAILABLE, ACCEPTED, SOURCE_CONTACT, DESTINATION_CONTACT, DELIVERY, COMPLETED, FAILED };

class RHD_ContractConfig
{
	static const int MAX_ACTIVE_CONTRACTS = 8;
	static const int MAX_AVAILABLE_CONTRACTS = 12;
	static const int CONTRACT_REFRESH_COUNT = 5;
	static const int MIN_REWARD = 250;
	static const int MAX_REWARD = 25000;
	static const int MAX_XP_REWARD = 5000;
	static const int MAX_REPUTATION_REWARD = 100;
	static const float CONTRACT_INTERACTION_DISTANCE_M = 4.0;
};

class RHD_ContractService
{
	static void BuildDefaultDefinitions(out array<RHD_ContractDefinition> definitions)
	{
		definitions = {};
		Add(definitions, "COURIER_BASIC", "Civilian Courier", "Move a package between two existing civilian contacts.", "COURIER", "CIVILIAN", "APPLE", 500, 250, 10, 1);
		Add(definitions, "SUPPLY_RUN", "Supply Run", "Deliver useful material to a waiting civilian contact.", "SUPPLY", "CONTRACTOR", "IRON", 1200, 450, 15, 1);
		Add(definitions, "SECURE_MEDICAL", "Medical Replenishment", "Move medical-related cargo to a civilian contact.", "LOGISTICS", "CIVILIAN", "CANNED_CORN", 900, 350, 12, 1);
		Add(definitions, "MATERIALS_ADVANCED", "Industrial Materials", "Transport refined industrial material between contacts.", "LOGISTICS", "CONTRACTOR", "COPPER", 2500, 750, 20, 5);
		Add(definitions, "HIGH_VALUE", "High Value Courier", "Complete a premium courier movement between existing contacts.", "PREMIUM", "CONTRACTOR", "GOLD", 7500, 1500, 35, 15);
	}

	static void Add(array<RHD_ContractDefinition> definitions, string id, string title, string description, string type, string faction, string itemId, int reward, int xp, int reputation, int level)
	{
		RHD_ContractDefinition definition = new RHD_ContractDefinition();
		definition.m_sId = id; definition.m_sTitle = title; definition.m_sDescription = description;
		definition.m_sType = type; definition.m_sFaction = faction; definition.m_sVirtualItemId = itemId;
		definition.m_iReward = reward; definition.m_iXP = xp; definition.m_iReputation = reputation; definition.m_iRequiredLevel = level;
		definitions.Insert(definition);
	}

	static bool IsDefinitionUnlocked(RHD_ContractDefinition definition, RHD_PlayerProgression progression)
	{
		return definition && progression && progression.GetLevel() >= definition.m_iRequiredLevel;
	}
};
