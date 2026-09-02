// ============================================================================
// RHD / PROJECT33 - OVERTHROW COMPANION GAMEPLAY LAYER
// ============================================================================
// Data/service layer for the requested companion feature set.
// Third-party APIs are intentionally not guessed; authoritative runtime hooks
// belong in RHD_VirtualPlayerMissionAdapter.c.
// ============================================================================
class RHD_CompanionReputation
{
	string m_sFactionId;
	int m_iValue;
	void RHD_CompanionReputation(string factionId) { m_sFactionId = factionId; m_iValue = RHD_OverthrowCompanionConfig.STARTING_REPUTATION; }
};

class RHD_CompanionContract
{
	string m_sContractId;
	string m_sTitle;
	string m_sDescription;
	string m_sType;
	string m_sSourceId;
	string m_sDestinationId;
	string m_sItemId;
	int m_iReward;
	int m_iReputationDelta;
	int m_iStage;
	bool m_bCompleted;
	bool m_bActive;
};

enum RHD_CompanionContractStage { AVAILABLE, ACCEPTED, SOURCE, DESTINATION, COMPLETE };

class RHD_CompanionMarket
{
	string m_sTownId;
	string m_sDisplayName;
	map<string, int> m_mDemand = new map<string, int>();
	map<string, int> m_mMultiplier = new map<string, int>();
	bool m_bFriendly;
	bool m_bContested;
};

class RHD_TradeOffer
{
	string m_sTradeId;
	string m_sSellerPlayerId;
	string m_sBuyerPlayerId;
	string m_sItemId;
	int m_iQuantity;
	int m_iPrice;
	bool m_bAccepted;
	bool m_bOpen;
};

class RHD_OwnedVehicle
{
	string m_sVehicleId;
	string m_sDisplayName;
	string m_sOwnerPlayerId;
	string m_sStorageId;
	int m_iConditionPercent;
	bool m_bStored;
	bool m_bInsured;
	bool m_bStolen;
};

class RHD_LicenseEntry
{
	string m_sLicenseId;
	string m_sDisplayName;
	bool m_bOwned;
};

class RHD_InsuranceEntry
{
	string m_sVehicleId;
	int m_iPremium;
	int m_iClaimFee;
	bool m_bActive;
};

class RHD_LoanEntry
{
	string m_sLoanId;
	int m_iPrincipal;
	int m_iInterest;
	int m_iTotalDue;
	int m_iAmountPaid;
	bool m_bOpen;
};

class RHD_BlackMarketDealer
{
	string m_sDealerId;
	string m_sDisplayName;
	string m_sTownId;
	ref array<string> m_aItems = {};
	int m_iPricePremiumPercent;
	int m_iMinimumCriminalReputation;
	bool m_bAvailable;
};

class RHD_TerritoryIntel
{
	string m_sTownId;
	string m_sDisplayName;
	string m_sController;
	int m_iThreatPercent;
	int m_iSupplyPercent;
	bool m_bFriendly;
	bool m_bContested;
};

class RHD_CompanionNotification
{
	string m_sTitle;
	string m_sMessage;
	string m_sCategory;
	int m_iPriority;
	bool m_bUnread;
};

class RHD_CompanionStats
{
	int m_iContractsCompleted;
	int m_iLegalContractsCompleted;
	int m_iIllegalContractsCompleted;
	int m_iDeliveriesCompleted;
	int m_iTradesCompleted;
	int m_iVehiclesOwned;
	int m_iVehiclesSold;
	int m_iLoansPaid;
	int m_iInsuranceClaims;
	int m_iMoneyEarned;
	int m_iMoneySpent;
	int m_iDistanceMeters;
	void RecordMoneyEarned(int amount) { if (amount > 0) m_iMoneyEarned += amount; }
	void RecordMoneySpent(int amount) { if (amount > 0) m_iMoneySpent += amount; }
};

class RHD_CompanionProfile
{
	string m_sPlayerId;
	ref array<RHD_CompanionReputation> m_aReputation = {};
	ref array<RHD_CompanionContract> m_aContracts = {};
	ref array<RHD_TradeOffer> m_aTrades = {};
	ref array<RHD_OwnedVehicle> m_aVehicles = {};
	ref array<RHD_LicenseEntry> m_aLicenses = {};
	ref array<RHD_InsuranceEntry> m_aInsurance = {};
	ref array<RHD_LoanEntry> m_aLoans = {};
	ref array<RHD_CompanionNotification> m_aNotifications = {};
	ref RHD_CompanionStats m_Stats = new RHD_CompanionStats();
	void RHD_CompanionProfile(string playerId) { m_sPlayerId = playerId; }

	int GetReputation(string factionId)
	{
		foreach (RHD_CompanionReputation entry : m_aReputation)
			if (entry && entry.m_sFactionId == factionId) return entry.m_iValue;
		return RHD_OverthrowCompanionConfig.STARTING_REPUTATION;
	}
	void AddReputation(string factionId, int delta)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_REPUTATION || factionId.IsEmpty()) return;
		foreach (RHD_CompanionReputation entry : m_aReputation)
		{
			if (!entry || entry.m_sFactionId != factionId) continue;
			entry.m_iValue = Math.Max(RHD_OverthrowCompanionConfig.MIN_REPUTATION, Math.Min(RHD_OverthrowCompanionConfig.MAX_REPUTATION, entry.m_iValue + delta));
			return;
		}
		RHD_CompanionReputation created = new RHD_CompanionReputation(factionId);
		created.m_iValue = Math.Max(RHD_OverthrowCompanionConfig.MIN_REPUTATION, Math.Min(RHD_OverthrowCompanionConfig.MAX_REPUTATION, delta));
		m_aReputation.Insert(created);
	}
	bool HasLicense(string licenseId)
	{
		foreach (RHD_LicenseEntry entry : m_aLicenses)
			if (entry && entry.m_sLicenseId == licenseId && entry.m_bOwned) return true;
		return false;
	}
	bool AddLicense(string licenseId, string displayName)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_LICENSES || licenseId.IsEmpty() || m_aLicenses.Count() >= RHD_OverthrowCompanionConfig.MAX_LICENSES) return false;
		if (HasLicense(licenseId)) return true;
		RHD_LicenseEntry entry = new RHD_LicenseEntry(); entry.m_sLicenseId = licenseId; entry.m_sDisplayName = displayName; entry.m_bOwned = true; m_aLicenses.Insert(entry); return true;
	}
};

class RHD_OverthrowCompanionService
{
	protected static ref RHD_OverthrowCompanionService s_Instance;
	protected ref array<RHD_CompanionProfile> m_aProfiles = {};
	protected ref array<RHD_CompanionContract> m_aContractBoard = {};
	protected ref array<RHD_CompanionMarket> m_aMarkets = {};
	protected ref array<RHD_TerritoryIntel> m_aTerritories = {};
	protected ref array<RHD_BlackMarketDealer> m_aDealers = {};

	static RHD_OverthrowCompanionService GetInstance() { if (!s_Instance) s_Instance = new RHD_OverthrowCompanionService(); return s_Instance; }
	RHD_CompanionProfile GetProfile(string playerId)
	{
		if (playerId.IsEmpty()) return null;
		foreach (RHD_CompanionProfile profile : m_aProfiles) if (profile && profile.m_sPlayerId == playerId) return profile;
		RHD_CompanionProfile created = new RHD_CompanionProfile(playerId); m_aProfiles.Insert(created); return created;
	}

	bool PublishContract(string contractId, string title, string description, string type, string sourceId, string destinationId, string itemId, int reward, int repDelta)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_CONTRACT_BOARD || contractId.IsEmpty() || title.IsEmpty() || reward < 0 || m_aContractBoard.Count() >= RHD_OverthrowCompanionConfig.MAX_ACTIVE_CONTRACTS) return false;
		RHD_CompanionContract contract = new RHD_CompanionContract(); contract.m_sContractId = contractId; contract.m_sTitle = title; contract.m_sDescription = description; contract.m_sType = type; contract.m_sSourceId = sourceId; contract.m_sDestinationId = destinationId; contract.m_sItemId = itemId; contract.m_iReward = reward; contract.m_iReputationDelta = repDelta; contract.m_iStage = RHD_CompanionContractStage.AVAILABLE; contract.m_bActive = true; m_aContractBoard.Insert(contract); return true;
	}
	bool AcceptContract(string playerId, string contractId)
	{
		RHD_CompanionProfile profile = GetProfile(playerId);
		if (!profile || contractId.IsEmpty()) return false;
		foreach (RHD_CompanionContract board : m_aContractBoard)
		{
			if (!board || board.m_sContractId != contractId || !board.m_bActive || board.m_iStage != RHD_CompanionContractStage.AVAILABLE) continue;
			RHD_CompanionContract copy = new RHD_CompanionContract(); copy.m_sContractId = board.m_sContractId; copy.m_sTitle = board.m_sTitle; copy.m_sDescription = board.m_sDescription; copy.m_sType = board.m_sType; copy.m_sSourceId = board.m_sSourceId; copy.m_sDestinationId = board.m_sDestinationId; copy.m_sItemId = board.m_sItemId; copy.m_iReward = board.m_iReward; copy.m_iReputationDelta = board.m_iReputationDelta; copy.m_iStage = RHD_CompanionContractStage.ACCEPTED; copy.m_bActive = true; profile.m_aContracts.Insert(copy); board.m_bActive = false; return true;
		}
		return false;
	}
	bool CompleteContract(string playerId, string contractId, bool illegalContract)
	{
		RHD_CompanionProfile profile = GetProfile(playerId); if (!profile) return false;
		foreach (RHD_CompanionContract contract : profile.m_aContracts)
		{
			if (!contract || contract.m_sContractId != contractId || contract.m_bCompleted) continue;
			contract.m_iStage = RHD_CompanionContractStage.COMPLETE; contract.m_bCompleted = true; contract.m_bActive = false; profile.m_Stats.m_iContractsCompleted++;
			if (illegalContract) profile.m_Stats.m_iIllegalContractsCompleted++; else profile.m_Stats.m_iLegalContractsCompleted++;
			profile.m_Stats.RecordMoneyEarned(contract.m_iReward); profile.AddReputation(illegalContract ? "CRIMINAL" : "CIVILIAN", contract.m_iReputationDelta); PushNotification(playerId, "Contract complete", contract.m_sTitle, "CONTRACT", 1); return true;
		}
		return false;
	}

	bool RegisterTownMarket(string townId, string displayName, bool friendly, bool contested)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_DYNAMIC_MARKETS || townId.IsEmpty()) return false;
		if (GetMarket(townId)) return true;
		RHD_CompanionMarket market = new RHD_CompanionMarket(); market.m_sTownId = townId; market.m_sDisplayName = displayName; market.m_bFriendly = friendly; market.m_bContested = contested; m_aMarkets.Insert(market); return true;
	}
	RHD_CompanionMarket GetMarket(string townId) { foreach (RHD_CompanionMarket market : m_aMarkets) if (market && market.m_sTownId == townId) return market; return null; }
	void SetTownDemand(string townId, string itemId, int demand)
	{
		RHD_CompanionMarket market = GetMarket(townId); if (!market || itemId.IsEmpty()) return; market.m_mDemand.Set(itemId, Math.Max(-100, Math.Min(100, demand))); RecalculateMarketPrice(market, itemId);
	}
	void ChangeTownDemand(string townId, string itemId, int delta)
	{
		RHD_CompanionMarket market = GetMarket(townId); if (!market || itemId.IsEmpty()) return; int current = 0; if (market.m_mDemand.Contains(itemId)) current = market.m_mDemand.Get(itemId); SetTownDemand(townId, itemId, current + delta);
	}
	protected void RecalculateMarketPrice(RHD_CompanionMarket market, string itemId)
	{
		int demand = 0; if (market.m_mDemand.Contains(itemId)) demand = market.m_mDemand.Get(itemId); int multiplier = RHD_OverthrowCompanionConfig.MARKET_BASE_MULTIPLIER + Math.Round(demand * 0.5) * RHD_OverthrowCompanionConfig.MARKET_STEP; multiplier = Math.Max(RHD_OverthrowCompanionConfig.MARKET_MIN_MULTIPLIER, Math.Min(RHD_OverthrowCompanionConfig.MARKET_MAX_MULTIPLIER, multiplier)); market.m_mMultiplier.Set(itemId, multiplier);
	}
	int GetMarketMultiplier(string townId, string itemId)
	{
		RHD_CompanionMarket market = GetMarket(townId); if (!market || !market.m_mMultiplier.Contains(itemId)) return RHD_OverthrowCompanionConfig.MARKET_BASE_MULTIPLIER; return market.m_mMultiplier.Get(itemId);
	}
	bool IsIllegalItem(string itemId)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_LEGAL_ILLEGAL_MARKET || itemId.IsEmpty()) return false;
		return itemId == "CANNABIS_PLANT" || itemId == "CANNABIS_FLOWER" || itemId == "COCA_LEAF" || itemId == "COCAINE";
	}
	int CalculateMarketPrice(string townId, string itemId, int basePrice)
	{
		if (basePrice <= 0) return 0; int multiplier = GetMarketMultiplier(townId, itemId); if (IsIllegalItem(itemId)) multiplier += RHD_OverthrowCompanionConfig.BLACK_MARKET_PRICE_PREMIUM_PERCENT; return Math.Max(1, Math.Round(basePrice * multiplier / 100.0));
	}

	bool CreateTradeOffer(string sellerPlayerId, string buyerPlayerId, string itemId, int quantity, int price)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_PLAYER_TRADING || sellerPlayerId.IsEmpty() || itemId.IsEmpty() || quantity <= 0 || price < 0) return false;
		RHD_CompanionProfile seller = GetProfile(sellerPlayerId); if (!seller || seller.m_aTrades.Count() >= RHD_OverthrowCompanionConfig.MAX_OPEN_TRADE_OFFERS) return false;
		RHD_TradeOffer offer = new RHD_TradeOffer(); offer.m_sTradeId = sellerPlayerId + "_TRADE_" + seller.m_aTrades.Count().ToString(); offer.m_sSellerPlayerId = sellerPlayerId; offer.m_sBuyerPlayerId = buyerPlayerId; offer.m_sItemId = itemId; offer.m_iQuantity = quantity; offer.m_iPrice = price; offer.m_bOpen = true; seller.m_aTrades.Insert(offer); return true;
	}
	bool CompleteTrade(string sellerPlayerId, string tradeId)
	{
		RHD_CompanionProfile seller = GetProfile(sellerPlayerId); if (!seller) return false;
		foreach (RHD_TradeOffer offer : seller.m_aTrades)
		{
			if (!offer || offer.m_sTradeId != tradeId || !offer.m_bOpen) continue;
			offer.m_bAccepted = true; offer.m_bOpen = false; seller.m_Stats.m_iTradesCompleted++;
			RHD_CompanionProfile buyer = GetProfile(offer.m_sBuyerPlayerId); if (buyer) buyer.m_Stats.m_iTradesCompleted++;
			PushNotification(offer.m_sSellerPlayerId, "Trade complete", offer.m_sItemId, "TRADE", 1); if (buyer) PushNotification(offer.m_sBuyerPlayerId, "Trade received", offer.m_sItemId, "TRADE", 1); return true;
		}
		return false;
	}

	bool RegisterOwnedVehicle(string playerId, string vehicleId, string displayName, string storageId)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_VEHICLE_OWNERSHIP || vehicleId.IsEmpty()) return false; RHD_CompanionProfile profile = GetProfile(playerId); if (!profile || profile.m_aVehicles.Count() >= RHD_OverthrowCompanionConfig.MAX_OWNED_VEHICLES) return false;
		RHD_OwnedVehicle vehicle = new RHD_OwnedVehicle(); vehicle.m_sVehicleId = vehicleId; vehicle.m_sDisplayName = displayName; vehicle.m_sOwnerPlayerId = playerId; vehicle.m_sStorageId = storageId; vehicle.m_iConditionPercent = 100; vehicle.m_bStored = true; profile.m_aVehicles.Insert(vehicle); profile.m_Stats.m_iVehiclesOwned++; return true;
	}
	RHD_OwnedVehicle FindOwnedVehicle(string playerId, string vehicleId)
	{
		RHD_CompanionProfile profile = GetProfile(playerId); if (!profile) return null; foreach (RHD_OwnedVehicle vehicle : profile.m_aVehicles) if (vehicle && vehicle.m_sVehicleId == vehicleId) return vehicle; return null;
	}
	bool SetVehicleStored(string playerId, string vehicleId, bool stored) { RHD_OwnedVehicle vehicle = FindOwnedVehicle(playerId, vehicleId); if (!vehicle) return false; vehicle.m_bStored = stored; return true; }

	void ChangeReputation(string playerId, string factionId, int delta) { RHD_CompanionProfile profile = GetProfile(playerId); if (profile) profile.AddReputation(factionId, delta); }

	bool RegisterTerritory(string townId, string displayName, string controller, int threatPercent, int supplyPercent, bool friendly, bool contested)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_TERRITORY_INTELLIGENCE || townId.IsEmpty()) return false;
		RHD_TerritoryIntel intel = GetTerritory(townId); if (!intel) { intel = new RHD_TerritoryIntel(); intel.m_sTownId = townId; m_aTerritories.Insert(intel); }
		intel.m_sDisplayName = displayName; intel.m_sController = controller; intel.m_iThreatPercent = Math.Max(0, Math.Min(100, threatPercent)); intel.m_iSupplyPercent = Math.Max(0, Math.Min(100, supplyPercent)); intel.m_bFriendly = friendly; intel.m_bContested = contested; return true;
	}
	RHD_TerritoryIntel GetTerritory(string townId) { foreach (RHD_TerritoryIntel intel : m_aTerritories) if (intel && intel.m_sTownId == townId) return intel; return null; }

	int GetPropertyBenefitStorage(int propertyCount, int storageObjects)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_PROPERTY_BENEFITS) return 0; return Math.Max(0, propertyCount + storageObjects) * RHD_VirtualPlayerConfig.STORAGE_PER_OBJECT;
	}
	bool PublishLogisticsContract(string contractId, string sourceTownId, string destinationTownId, string itemId, int reward)
	{
		return RHD_OverthrowCompanionConfig.ENABLE_LOGISTICS_CONTRACTS && PublishContract(contractId, "Logistics Run", "Move supplies between two towns.", "LOGISTICS", sourceTownId, destinationTownId, itemId, reward + RHD_OverthrowCompanionConfig.LOGISTICS_CONTRACT_BONUS, 2);
	}

	bool PurchaseLicense(string playerId, string licenseId, string displayName, int price)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_LICENSES || price < 0) return false; RHD_CompanionProfile profile = GetProfile(playerId); return profile && profile.AddLicense(licenseId, displayName);
	}
	bool InsureVehicle(string playerId, string vehicleId, int premium = -1)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_INSURANCE) return false; RHD_CompanionProfile profile = GetProfile(playerId); if (!profile || profile.m_aInsurance.Count() >= RHD_OverthrowCompanionConfig.MAX_INSURED_VEHICLES || !FindOwnedVehicle(playerId, vehicleId)) return false;
		if (premium < 0) premium = RHD_OverthrowCompanionConfig.INSURANCE_BASE_PREMIUM; RHD_InsuranceEntry entry = new RHD_InsuranceEntry(); entry.m_sVehicleId = vehicleId; entry.m_iPremium = premium; entry.m_iClaimFee = RHD_OverthrowCompanionConfig.INSURANCE_CLAIM_FEE; entry.m_bActive = true; profile.m_aInsurance.Insert(entry); RHD_OwnedVehicle vehicle = FindOwnedVehicle(playerId, vehicleId); if (vehicle) vehicle.m_bInsured = true; return true;
	}
	bool CreateLoan(string playerId, string loanId, int principal)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_LOANS || principal < RHD_OverthrowCompanionConfig.LOAN_MIN_PRINCIPAL || principal > RHD_OverthrowCompanionConfig.LOAN_MAX_PRINCIPAL) return false; RHD_CompanionProfile profile = GetProfile(playerId); if (!profile || profile.m_aLoans.Count() >= RHD_OverthrowCompanionConfig.MAX_ACTIVE_LOANS) return false;
		RHD_LoanEntry loan = new RHD_LoanEntry(); loan.m_sLoanId = loanId; loan.m_iPrincipal = principal; loan.m_iInterest = Math.Round(principal * RHD_OverthrowCompanionConfig.LOAN_INTEREST_PERCENT / 100.0); loan.m_iTotalDue = loan.m_iPrincipal + loan.m_iInterest; loan.m_bOpen = true; profile.m_aLoans.Insert(loan); return true;
	}
	bool PayLoan(string playerId, string loanId, int amount)
	{
		if (amount <= 0) return false; RHD_CompanionProfile profile = GetProfile(playerId); if (!profile) return false;
		foreach (RHD_LoanEntry loan : profile.m_aLoans) { if (!loan || loan.m_sLoanId != loanId || !loan.m_bOpen) continue; loan.m_iAmountPaid = Math.Min(loan.m_iTotalDue, loan.m_iAmountPaid + amount); if (loan.m_iAmountPaid >= loan.m_iTotalDue) { loan.m_bOpen = false; profile.m_Stats.m_iLoansPaid++; } return true; }
		return false;
	}

	bool RegisterBlackMarketDealer(string dealerId, string displayName, string townId, int minimumCriminalReputation)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_BLACK_MARKET_DEALERS || dealerId.IsEmpty()) return false; RHD_BlackMarketDealer dealer = new RHD_BlackMarketDealer(); dealer.m_sDealerId = dealerId; dealer.m_sDisplayName = displayName; dealer.m_sTownId = townId; dealer.m_iPricePremiumPercent = RHD_OverthrowCompanionConfig.BLACK_MARKET_PRICE_PREMIUM_PERCENT; dealer.m_iMinimumCriminalReputation = minimumCriminalReputation; dealer.m_bAvailable = true; m_aDealers.Insert(dealer); return true;
	}
	bool CanUseDealer(string playerId, string dealerId)
	{
		RHD_CompanionProfile profile = GetProfile(playerId); if (!profile) return false; foreach (RHD_BlackMarketDealer dealer : m_aDealers) if (dealer && dealer.m_sDealerId == dealerId && dealer.m_bAvailable) return profile.GetReputation("CRIMINAL") >= dealer.m_iMinimumCriminalReputation; return false;
	}

	RHD_CompanionStats GetStats(string playerId) { RHD_CompanionProfile profile = GetProfile(playerId); return profile ? profile.m_Stats : null; }
	void PushNotification(string playerId, string title, string message, string category, int priority)
	{
		if (!RHD_OverthrowCompanionConfig.ENABLE_ATAK_NOTIFICATIONS) return; RHD_CompanionProfile profile = GetProfile(playerId); if (!profile) return;
		RHD_CompanionNotification note = new RHD_CompanionNotification(); note.m_sTitle = title; note.m_sMessage = message; note.m_sCategory = category; note.m_iPriority = priority; note.m_bUnread = true; profile.m_aNotifications.Insert(note);
	}
};
