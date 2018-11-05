// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Youm e Takbeer Pakistan 2015";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1576800;
        consensus.BIP34Height = 895810;
        consensus.BIP34Hash = uint256S("54e02bf892a34362c4b064b957eb90c8e06c2923e4c18aa11b85eb16bf7d01df");
        consensus.BIP65Height = 1000000; // bab3041e8977e0dc3eeff63fe707b92bde1dd449d8efafb248c27c8264cc311a
        consensus.BIP66Height = 1000000; // 7aceee012833fa8952f8835d8b1b3ae233cd6ab08fdb27a771d2bd7bdc491894
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // 1 minute
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8064; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.nMidasstartheight = 950000;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1556366400; // April 27, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0; // disabled

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1556366400; // April 27,2019
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // disabled

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000064fadc2a1d301f");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x3dfef0605077f7a3360998177879d42e702f7aecbeae0a45d1f6b25a7ee21580"); //893670

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x70;
        pchMessageStart[1] = 0x61;
        pchMessageStart[2] = 0x6b;
        pchMessageStart[3] = 0x63;
        nDefaultPort = 7867;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1432771201, 387576598, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x3091f440b6dfcf5d00be71b75bceded11f8606decaa339fe2c9f59e8efcfded2"));
        assert(genesis.hashMerkleRoot == uint256S("0x72bb72ee57a0e14a51f80011feb167105bfa40412b4385f54b84e1573b8ede99"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("seed.pakcoin.org", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,55); // P
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5); //3
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50); // M
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176); // T Q
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {     0, uint256S("0x3091f440b6dfcf5d00be71b75bceded11f8606decaa339fe2c9f59e8efcfded2")},
                {  1000, uint256S("0xb2756458a1d8f18040af6c3dcf0685c637080884992664a2dcf03263cfaddd81")},
                {  4032, uint256S("0x25eec03ad9aa10c93af3152eb92d20c71954b15412fdc0249fd0636dfa207674")},
                {  8064, uint256S("0x1a6f860eb25abbc1072f02a8f91afd597c725428509eb6877be2187b6fc45551")},
                { 23420, uint256S("0x0630ec7b4376fb270d55f0e2e95465e21705149830833ce61b3f2f477a5b59fd")},
                { 80000, uint256S("0x111ae6eacc09e06fa910709644dd3e32b81513fcb50dcf69b9c00d6363b8104e")},
                {120000, uint256S("0x24ca1a7777057c5e148e6d3dd60d795da9ac9e5dd1eec6ee5be5b235c3cd94b1")},
                {240000, uint256S("0x563a4fb537cbbc41a237c8edc903b8ce052a764ec1d578f8db697f121c1d2ab3")},
                {425250, uint256S("0xc71852ce4dcffa302b15eee26e3ca92ad1fd53d76c2cd45a194fa1f9178896c6")},
                {893670, uint256S("0x3dfef0605077f7a3360998177879d42e702f7aecbeae0a45d1f6b25a7ee21580")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 53069d8623db4571e1fb2414701df74f4c6b65758e1fda6ddd85579cdb889dc6 (height 425250).
            1540062838, // * UNIX timestamp of last known number of transactions
            1031740,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.06     // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 1576800;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 0; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nMidasstartheight = 51;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0; // disabled

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // disabled

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        nDefaultPort = 17867;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1540021727, 72420, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x3d4b5ed6ce0357d8f58b27f81fa8f484e66fd72b1e1b89dbc597a78e5c37d2fc"));
        assert(genesis.hashMerkleRoot == uint256S("0x72bb72ee57a0e14a51f80011feb167105bfa40412b4385f54b84e1573b8ede99"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.pakcointools.com", false);
        vSeeds.emplace_back("seed-b.pakcoin.loshan.co.uk", false);
        vSeeds.emplace_back("dnsseed-testnet.thrasher.io", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,58);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                /*{2056, uint256S("17748a31ba97afdc9a4f86837a39d287e3e7c7290a08a1d816c5969c78a83289")},*/
            }
        };

        chainTxData = ChainTxData{
            // Data as of block a0afbded94d4be233e191525dc2d467af5c7eab3143c852c3cd549831022aad6 (height 343833)
            0,
            0,
            0
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 1; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 25; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 25; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60; // two weeks
        consensus.nPowTargetSpacing =  60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.nMidasstartheight = 51;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1541218770; // 3 Nov 2018 0921 H
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1541221205; // 3 Nov 2018 1000 H
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1541218770; // 3 Nov 2018 0921 H
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1541221205; // 3 Nov 2018 1000 H

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xf2;
        pchMessageStart[1] = 0xb6;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 27867;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1541142134, 270914, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x14d5b93c55f5c46c26fc07b65460e35c86e5ba214d64aefcba78be85d7144077"));
        assert(genesis.hashMerkleRoot == uint256S("0x72bb72ee57a0e14a51f80011feb167105bfa40412b4385f54b84e1573b8ede99"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData) {
            {
             //   {0, uint256S("530827f38f93b43ed12af0b3ad25a288dc02ed74d6d7857862df51fc56c416f9")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
