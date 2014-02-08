// Copyright (c) 2013-2014 The FedoraCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "userdb.h"
#include "main.h"
#include "hash.h"
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
    return Read(string("ROOT"), owner) && !owner.empty();
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

bool CUserDB::UserAdd(string username, string password)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);
    if(this->UserExists(user)) return false;
    uint256 pass_hash = Hash(password.begin(), password.end());
    srand(time(NULL));
    string acct = user;
    return Write("U:" + user, pass_hash) && this->UserAccountAdd(user, acct, acct) && Write("UD:" + user, acct) && (!this->RootAccountExists() ? this->RootAccountSet(user) : true);
}

bool CUserDB::UserAuth(string username, string password)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);

    if(user == "root" || user == "false") return false;
    uint256 pass_hash("0x0");
    if(!Read("U:" + user, pass_hash) || pass_hash == uint256("0x0")) return false;
    uint256 auth_hash = Hash(password.begin(), password.end());
    return pass_hash == auth_hash;
}

bool CUserDB::UserAccountDefault(string username, string &account)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);
    if(!Read("UD:" + user, account))
    {
        string acct = user;
        if(!this->UserAccountAdd(user, acct, acct)) return false;
        account = acct;
    }
    return true;
}

bool CUserDB::UserAccountExists(string account)
{
    std::string owner;
    return Read("A:" + account, owner) && !owner.empty();
}

bool CUserDB::UserAccountAdd(string username, string account, string &accountInternal)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);

    bool ret = true;
    accountInternal = account;
    srand(time(NULL));
    while(ret)
    {
        accountInternal = account + "-" + boost::lexical_cast<std::string>(rand() % 1000000);
        ret = this->UserAccountExists(accountInternal);
    }

    string prev_accts = "0";
    Read("UA:" + user, prev_accts);
    if(this->UserAccountExists(accountInternal)) return false;
    return Write("A:" + accountInternal, user) && Write("UA:" + user, prev_accts + ":-:-:" + accountInternal);
}
bool CUserDB::UserOwnsAccount(string username, string account)
{
    string user = username;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);

    string owner;
    if(!Read("A:" + account, owner) || owner.empty()) return false;
    return owner == user;
}

bool CUserDB::UserAccountList(string username, list<string> &accounts)
{
    string user = username;
    string accts;
    list<string> list;
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);
    if(!Read("UA:" + user, accts)) return false;
    size_t s = accts.find(":-:-:");
    while(s != std::string::npos)
    {
        string acct = accts.substr(0, s);
        if(acct != "0")
            list.push_back(acct);
        accts = accts.substr(s + 5, accts.size() - (s+5));
        s = accts.find(":-:-:");
    }
    if(!accts.empty() && accts != "0")
        list.push_back(accts);
    accounts = list;
    return true;
}

