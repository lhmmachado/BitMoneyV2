// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017 The savenode developers
// Copyright (c) 2018 The BitMoney(Phoenix) developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include <string.h>
#include "crypto/scrypt.h"
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
boost::assign::map_list_of
(0, uint256("0x000006889bd568a836dd1a31a3f93949eaa24368f6dd98dc31edd44375578c2f"))
(22, uint256("0x45b8362f68be8d0948b9f48ffbd2af8598740ec6e1a920e2885bd6453b136a05"))
(26, uint256("0x3f50d50d8885a7393460f539fa42a4e12268a6efaf399a268709df5d1b08e972"))
(120, uint256("0xe9aa0316df6bc4020ea4a7161a6d4f2eaac075104d77d2806075364ae8bbc847"))
(142, uint256("0x3c113e8f036d16afd5096cf2c7522c4cae648bb18887e20fa67b1baf1036b0f4"))
(198, uint256("0x3bc3c68f21a860c5e835840f2dc0d1b0555170ac9ba38e869c63b4ad3e3740eb"))
(256, uint256("0xf7b0dab209a9f000c7f25d11fc89f70b2eae368ae400b15c42aab19766d220c6"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
	1539182060, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
	1538783565,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
	1538783565,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xcd;
        pchMessageStart[2] = 0x3b;
        pchMessageStart[3] = 0x68;
        vAlertPubKey = ParseHex("04bef0e73664d22c8b0be13ecaab85b68553b55d5ce8d17df0da1d9381c12bd57f5fb0e6db32850822600413b50bc131a3459439957280342d5aa5f4d7a3816c1b");
        nDefaultPort = 49444;
        bnProofOfWorkLimit = ~uint256(0) >> 20; //20 BitMoney starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 30; // BitMoney: 0.5 minutes
        nTargetSpacingSlowLaunch = 10 * 60;  // BitMoney: 10 minutes (Slow launch - Block 288)
        nTargetSpacing = 1 * 60;  // BitMoney: 60 Seconds
        nMaturity = 19;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 70000000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 100;
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 601;
        nAccumulatorStartHeight = 1;
        nZerocoinStartTime = 1541030400; // 1 - nov - 2018
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
            genesis.nTime = 1527974340
            genesis.nNonce = 236551
            genesis.nVersion = 1
            genesis.GetHash = 000003f46e57c04e31374710022a6685abea12cb99960dfe96eaaa430edc670f
            genesis.hashMerkleRoot = 90d274dca16659997d8f5fc9644127d260087d4a39ca61ba9b3334482aa8c55d

         */
        const char* pszTimestamp = "BitMoney V2 Phoenix is Born - Jacob is Gone";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0408287b53c96f466269c6b9f759d1d108f86eabff18afc539e22a2a3b77b5ba90229662fe6cd0d4e79b29bb64b8b0fd7b0635e4df767551e990c4915f5cf7387a") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1538783565;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 4974290;

        hashGenesisBlock = uint256("0x000006889bd568a836dd1a31a3f93949eaa24368f6dd98dc31edd44375578c2f");
        if (false && genesis.GetHash() != hashGenesisBlock)
        {
            printf("recalculating params for mainnet.\n");
            printf("old mainnet genesis nonce: %s\n");
            std::string nNonce = "nNonce is " + std::to_string(genesis.nNonce);
            std::cout << nNonce << '\n';
            printf("old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ }
            printf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            printf("new mainnet genesis nonce: %s\n");
            std::string nNoncenew = "nNonce is " + std::to_string(genesis.nNonce);
            std::cout << nNoncenew << '\n';
            printf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000006889bd568a836dd1a31a3f93949eaa24368f6dd98dc31edd44375578c2f"));
        assert(genesis.hashMerkleRoot == uint256("0x69ce58136f49e4b752e4a9d932dca99dea3d0e6847f6e685bf595c7709623dca"));


        vSeeds.push_back(CDNSSeedData("95.179.193.119", "95.179.193.119"));
        vSeeds.push_back(CDNSSeedData("45.32.176.66", "45.32.176.66"));
        vSeeds.push_back(CDNSSeedData("95.179.197.142", "95.179.197.142"));
        vSeeds.push_back(CDNSSeedData("95.179.200.18", "95.179.200.18"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 55); // C
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 108); // x
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 117); // c
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x06)(0x3F)(0x44)(0x23).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x09)(0x61)(0x22)(0x2D).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 1135
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x20)(0x01)(0x1f)(0x6c).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        //strSporkKey = "0459eede7626441f7802af2736cb3a4aeb3e1f95070cde39d068a4f16525ee8fdd3c075f29f9e115aeb91952239194aa6ac19765574fed8a0d7f174f2b450e9630";
		  strSporkKey = "03c4f9d42586766f08a576c1c12bc4341243cd0d38540d88930e758d01bc614a7b";
        strObfuscationPoolDummyAddress = "PXH4xkfoACR8AVAx6hcY6rffKA5469sUfm";
        nStartMasternodePayments = 1532051032 + 6000; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        //zerocoinModulus = "0xc95577b6dce0049b0a20c779af38079355abadde1a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";
		zerocoinModulus = "0xa6cf179443c8fcd54c3cb653e15b95c61f451fc0e9846ee25f223c637fcf1db04beee20200b501944a3c688e9d5ff679a22695bd871812929b11ea7e92a68e5682b6a0abf367cf75cc865640c34926497fe03bd733d3f5ef757f865de4c7db78531c4598a7ddf6c676a98bbc9f5ccbab83772eb39915ef305f1e4964bb196bd4a9b307d3737253e947cf1e54f90a04dd3dad97c0ad2cc0d9d990893dc6a1545b929d4ff2154e1f5acda1f7513d4c81dc15bb2aa1d8fe65a73de7c7aaa4bf54b043ecace1e5d475b5eb05d7f83b59fdb9f1fa586af318765f0ac85c25f35b8e769eb8aa32538fc5fc21f388d87e0c5e2897eb0173465c148ea9e0f7814b97900237f18ecb48a2d21b974365c698fbaa53970e36f2269ad3dcab5700d7b6a5365a25e3ef6cb3a9c210b65fbacc8ff3c4efc56bed0f0b49b95f1c0307316645e2ba32b58f142d8dfae578dc549e8840d59b6bff267a632f99030e777263812f260259588efd75e6cf51afb479d46c66463e05e2981ac02c995a42313cbdaf4bc145";
		nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * ZCENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
		pchMessageStart[0] = 0xc7;
		pchMessageStart[1] = 0xff;
		pchMessageStart[2] = 0x2d;
		pchMessageStart[3] = 0x4f;
        vAlertPubKey = ParseHex("04a62416422cc32e2295112f0803ab1c594e0b787c9f21b432e44cf37bf1cbf77cb0248dabb3cea6917780e3d0ee4ffb7753517c55a94b98f779ee918449f0d5b3");
        nDefaultPort = 32545;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // BitMoney: 1 day
        nTargetSpacing = 1 * 60;  // BitMoney: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1524711188;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1515616140;
        genesis.nNonce = 79855;

	       hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x000007cff63ef602a51bf074e384b3516f0dd202f14d52f7c8c9b1af9423ab2e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // vSeeds.push_back(CDNSSeedData("45.76.61.28", "207.148.0.129"));         // Single node address
        // vSeeds.push_back(CDNSSeedData("209.250.240.94", "45.77.239.30"));       // Single node address
        // vSeeds.push_back(CDNSSeedData("45.77.176.204", "45.76.226.204"));       // Single node address


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet BitMoney addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet BitMoney script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet BitMoney BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet BitMoney BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet BitMoney BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04aa681dd00d9c55d4c28367757b461f78d452de73d63678c04f62ae7047707a58c0477311c6f047056a9a02e904513b9ee80a993ac97667cf27ad07549f2d42f7";
        strObfuscationPoolDummyAddress = "xp87cG8UEQgzs1Bk67Yk884C7pnQfAeo7q";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
		pchMessageStart[0] = 0xc2;
		pchMessageStart[1] = 0xfd;
		pchMessageStart[2] = 0x5c;
		pchMessageStart[3] = 0x1f;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // BitMoney: 1 day
        nTargetSpacing = 1 * 60;        // BitMoney: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1515524400;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 732084;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 53546;
        //assert(hashGenesisBlock == uint256("0x000008415bdca132b70cf161ecc548e5d0150fd6634a381ee2e99bb8bb77dbb3"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 53547;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
