#include "RDAUserDatabase.h"

RDAUserDatabase::RDAUserDatabase()
{ }

Wt::Auth::User RDAUserDatabase::findWithId (const std::string &id) const
{
  throw std::runtime_error("Not Implemented");
}

Wt::Auth::User RDAUserDatabase::findWithIdentity (const std::string &provider,
						  const Wt::WString &identity)
  const
{
  /*
   * David: find the 'id' for a user with name _identity_, leave it
   * empty if no user with this name exists
   */

  // STUB START
  std::string id = "koen_id";
  // STUB END

  if (id.empty())
    return Wt::Auth::User();
  else
    return Wt::Auth::User(id, *this);
}
 
void RDAUserDatabase::addIdentity (const Wt::Auth::User &user,
				   const std::string &provider,
				   const Wt::WString &id)
{
  throw std::runtime_error("Not Implemented");
}

void RDAUserDatabase::setIdentity (const Wt::Auth::User &user,
				   const std::string &provider,
				   const Wt::WString &id)
{
  throw std::runtime_error("Not Implemented");
}
 
Wt::WString RDAUserDatabase::identity (const Wt::Auth::User &user,
				       const std::string &provider) const
{
  /*
   * David: return here the name of the user with id user.id()
   */

  // STUB START
  if (user.id() == "koen_id")
    return "Koen";
  else
    return "NYI";
  // STUB END
}
 
void RDAUserDatabase::removeIdentity (const Wt::Auth::User &user,
				      const std::string &provider)
{
  throw std::runtime_error("Not Implemented");
}
 
Wt::Auth::PasswordHash RDAUserDatabase::password (const Wt::Auth::User &user) const
{
  /*
   * David: return here the password hash of the user with id user.id()
   * Ideally, you should store 'function' and 'value' (salt is usually empty
   * as it's part of the hash).
   */

  // STUB START
  std::string function = "bcrypt";
  std::string salt = "";

  // hash of 'koen' with 7 rounds of bcrypt
  std::string value
    = "$2a$07$r3uytjIOX0RpTz1Zism1puqXgTuiJT18M28HUBSTGk/6SRA6MVFVC";
  // STUB END

  if (user.id() == "koen_id")
    return Wt::Auth::PasswordHash(function, salt, value);
  else
    return Wt::Auth::PasswordHash("", "", "");
} 

void RDAUserDatabase::addAuthToken (const Wt::Auth::User &user,
				    const Wt::Auth::Token &token)
{
  /*
   * David: add a token for the user to the database, with token.hash() and
   * token.expirationTime() values
   */

  // HERE

  loginTokenHash_ = token.hash();
}

void RDAUserDatabase::removeAuthToken (const Wt::Auth::User &user,
				       const std::string &hash)
{
  /*
   * David: remove a token from the user database
   */

  // STUB START
  std::cerr << "Storing token: " << hash << " for " << user.id() << std::endl;
  // STUB END

  loginTokenHash_ = std::string();
}
 
Wt::Auth::User RDAUserDatabase::findWithAuthToken (const std::string &hash)
  const
{
  /*
   * David: find a user associated with a token with given hash value, taking
   * into account that it should not be expired
   */

  // STUB START
  std::cerr << "Lookup for hash: " << hash << std::endl;

  return Wt::Auth::User("koen_id", *this);
  // STUB END
}

int RDAUserDatabase::updateAuthToken (const Wt::Auth::User &user,
				      const std::string &oldhash,
				      const std::string &newhash)
{
  loginTokenHash_ = oldhash;

  extendAuthTokenValidity(user);

  return 30 * 60;
}

void RDAUserDatabase::extendAuthTokenValidity(const Wt::Auth::User& user)
{
  /*
   * David: find the token with hash 'loginTokenHash_' (for this user)
   * and reset the expiration time to 30 minutes.
   */

  // STUB START
  std::cerr << "Extending auth token validity for hash: " << loginTokenHash_
	    << std::endl;
  // STUB END
}
