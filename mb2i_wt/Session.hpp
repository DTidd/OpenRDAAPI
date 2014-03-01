// This may look like C code, but it's really -*- C++ -*-
#ifndef SESSION_H_
#define SESSION_H_

#include <Wt/Auth/Login>

#include <RDAUserDatabase.hpp>

class Session
{
public:
  Session();
  ~Session();

  RDAUserDatabase& users();
  Wt::Auth::Login& login() { return login_; }

  static void configureAuth();
  static const Wt::Auth::AuthService& auth();
  static const Wt::Auth::PasswordService& passwordAuth();

private:
  RDAUserDatabase users_;
  Wt::Auth::Login login_;
};

#endif // SESSION_H_
