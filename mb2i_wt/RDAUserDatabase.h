// This may look like C code, but it's really -*- C++ -*-
#ifndef RDA_USER_DATABASE_H_
#define RDA_USER_DATABASE_H_

#include <Wt/Auth/AbstractUserDatabase>

class RDAUserDatabase : public Wt::Auth::AbstractUserDatabase
{
public:
  RDAUserDatabase();

  virtual Wt::Auth::User findWithId (const std::string &id) const;
  virtual Wt::Auth::User findWithIdentity (const std::string &provider,
					   const Wt::WString &identity) const;
 
  virtual void addIdentity (const Wt::Auth::User &user,
			    const std::string &provider,
			    const Wt::WString &id);
 
  virtual void setIdentity (const Wt::Auth::User &user,
			    const std::string &provider,
			    const Wt::WString &id);
 
  virtual Wt::WString identity (const Wt::Auth::User &user,
				const std::string &provider) const;
 
  virtual void removeIdentity (const Wt::Auth::User &user,
			       const std::string &provider);
 
  virtual Wt::Auth::PasswordHash password (const Wt::Auth::User &user) const;
 
  virtual void addAuthToken (const Wt::Auth::User &user,
			     const Wt::Auth::Token &token); 

  virtual void removeAuthToken (const Wt::Auth::User &user,
				const std::string &hash);
 
  virtual Wt::Auth::User findWithAuthToken (const std::string &hash) const;

  virtual int updateAuthToken (const Wt::Auth::User &user,
			       const std::string &oldhash,
			       const std::string &newhash);

  virtual void extendAuthTokenValidity (const Wt::Auth::User& user);

  /*
    to implement attempt throttling:

    virtual void setFailedLoginAttempts (const User &user, int count);
    virtual int failedLoginAttempts (const User &user) const
    virtual void setLastLoginAttempt (const User &user, const WDateTime &t)
    virtual WDateTime lastLoginAttempt (const User &user) const
  */

private:
  std::string loginTokenHash_;
};

#endif // RDA_USER_DATABASE_H_
