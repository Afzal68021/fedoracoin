// Copyright (c) 2013-2014 The FedoraCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "userdb.h"
#include "hash.h"
#include "wallet.h"
#include "bitcoinrpc.h"
#include <boost/lexical_cast.hpp>
using namespace std;

CUserDB::CUserDB(size_t nCacheSize, bool fMemory, bool fWipe) : CLevelDB(GetDataDir() / "users", nCacheSize, fMemory, fWipe) {
}
bool CUserDB::WriteLastUserIndex(int bLastIdx)
{
    return Write('U', bLastIdx);
}

bool CUserDB::ReadLastUserIndex(int& bLastIdx)
{
    return Read('U', bLastIdx);
}

bool CUserDB::UserExists(string username)
{
    if(username == "root" || username == "false") return true;
    uint256 pass_hash("0x0");
    return Read("U:" + username, pass_hash) && pass_hash != uint256("0x0");
}

bool CUserDB::RootAccountExists()
{
    std::string owner;
    if(Read(string("ROOT"), owner) && !owner.empty())
    {
        this->root = owner;
        return true;
    }
    return false;
}

bool CUserDB::RootAccountSet(string username)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);
    bool w = Write(string("ROOT"), user);
    if(w) this->root = user;
    return w;
}

bool CUserDB::RootAccountGet(string &username)
{
    string rot;
    bool r = Read(string("ROOT"), rot);
    if(r) username = rot;
    return r;
}

bool CUserDB::UserAdd(string username, const SecureString& password)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);
    if(this->UserExists(user)) return false;
    uint256 pass_hash = Hash(password.begin(), password.end());
    CRPCContext ctx;
    ctx.username = username;
    CWallet* userWallet = CWallet::GetUserWallet(ctx, NULL);
    if(userWallet)
        userWallet->EncryptWallet(password);
    return userWallet && Write("U:" + user, pass_hash) && (!this->RootAccountExists() ? this->RootAccountSet(user) : true);
}

bool CUserDB::UserUpdate(string username, const SecureString& password)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);
    if(user == "root" || user == "false") return false;
    if(!this->UserExists(user)) return false;
    uint256 pass_hash = Hash(password.begin(), password.end());
    return Write("U:" + user, pass_hash);
}

bool CUserDB::UserAuth(string username, const SecureString& password)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);

    if(user == "root" || user == "false") return false;
    uint256 pass_hash("0x0");
    if(!Read("U:" + user, pass_hash) || pass_hash == uint256("0x0")) return false;
    uint256 auth_hash = Hash(password.begin(), password.end());
    return pass_hash == auth_hash;
}

