#include "Session.h"

#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"

#include "RDAUserDatabase.h"

namespace {
  Wt::Auth::AuthService myAuthService;
  Wt::Auth::PasswordService myPasswordService(myAuthService);
}

void Session::configureAuth()
{
  myAuthService.setAuthTokensEnabled(true, "rdalogin");
  myAuthService.setAuthTokenValidity(30);

  Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
  verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
  myPasswordService.setVerifier(verifier);

  // myPasswordService.setAttemptThrottlingEnabled(true);
}

Session::Session()
{ }

Session::~Session()
{ }

RDAUserDatabase& Session::users()
{
  return users_;
}

const Wt::Auth::AuthService& Session::auth()
{
  return myAuthService;
}

const Wt::Auth::PasswordService& Session::passwordAuth()
{
  return myPasswordService;
}
