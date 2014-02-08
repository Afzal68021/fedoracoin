// Copyright (c) 2013-2014 The FedoraCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "init.h" // for pwalletMain
#include "bitcoinrpc.h"
#include "ui_interface.h"
#include "base58.h"
#include "userdb.h"
#include <boost/lexical_cast.hpp>

#define printf OutputDebugStringF

using namespace json_spirit;
using namespace std;

Value adduser(const Array& params, std::string username, bool fHelp)
{
    if (fHelp || params.size() != 2)
        throw runtime_error(
            "adduser <username> <password>\n"
            "Allows the login combination to access the server via RPC.");

    if(username != "root") throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Method not found (unauthorized)");

    string strUser = params[0].get_str();
    string strPass = params[1].get_str();
    if(!pusers->UserAdd(strUser, strPass))
        throw JSONRPCError(RPC_INVALID_PARAMS, "Add user failed");
    return true;
}
Value passwd(const Array& params, std::string username, bool fHelp)
{
    if (fHelp || (username == "root" && params.size() != 2) || (username != "root" && params.size() != 1))
    {
        string start = (username == "root" ? "passwd <username> <password>\n" : "passwd <password>\n");
        throw runtime_error(
            start +
            "Updates user password.");
    }

    if(params.size() == 2 && username != "root") throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Method not found (unauthorized)");

    string strUser = params[0].get_str();
    string strNewPass;
    if(params.size() > 1)
        strNewPass = params[1].get_str();
    else
    {
        strNewPass = strUser;
        strUser = username;
    }
    if(!pusers->UserExists(strUser))
        throw JSONRPCError(RPC_INVALID_PARAMS, "User not found");
    if(!pusers->UserUpdate(strUser, strNewPass))
        throw JSONRPCError(RPC_INVALID_PARAMS, "User update failed");

    return true;
}
Value authuser(const Array& params, std::string username, bool fHelp)
{
    if (fHelp || params.size() != 2)
        throw runtime_error(
            "authuser <username> <password>\n"
            "Tests the username and password to see if the user can login.");

    if(username != "root") throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Method not found (unauthorized)");

    string strUser = params[0].get_str();
    string strPass = params[1].get_str();

    return pusers->UserAuth(strUser, strPass);
}
Value whoami(const Array& params, std::string username, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "whoami\n"
            "Prints the currently logged in users name.");

    return username;
}

Value root(const Array& params, std::string username, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "root"
            "Prints the root username.");

    if(username != "root") throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Method not found (unauthorized)");

    string user;
    pusers->RootAccountGet(user);
    return user;
}
