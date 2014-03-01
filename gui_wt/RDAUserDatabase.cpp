#include <sec.hpp>
#include <gui.hpp>
#include <nrd.hpp>
#include <misc.hpp>

#include <RDAUserDatabase.hpp>

RDAUserDatabase::RDAUserDatabase()
{ }

Wt::Auth::User RDAUserDatabase::findWithId (const std::string &id) const
{
/* not used */
  throw std::runtime_error("Not Implemented");
}

Wt::Auth::User RDAUserDatabase::findWithIdentity (const std::string &provider,
						  const Wt::WString &identity)
  const
{
	std::string i;
  /*
   * David: find the 'id' for a user with name _identity_, leave it
   * empty if no user with this name exists
   */

	if(SEC_USERS_FILENO==(-1))
	{
    		return Wt::Auth::User();
	}
	ZERNRD(SEC_USERS_FILENO);
	i=identity.toUTF8();
	USERLOGIN=stralloc(i.c_str());
	FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",USERLOGIN);
	if(EQLNRD(SEC_USERS_FILENO,1)) 
	{
		KEYNRD(SEC_USERS_FILENO,1);
    		return Wt::Auth::User();
	} else {
    		return Wt::Auth::User(i, *this);
	}
}
 
void RDAUserDatabase::addIdentity (const Wt::Auth::User &user,
				   const std::string &provider,
				   const Wt::WString &id)
{
/* Registratation purposes only */
  throw std::runtime_error("Not Implemented");
}

void RDAUserDatabase::setIdentity (const Wt::Auth::User &user,
				   const std::string &provider,
				   const Wt::WString &id)
{
/* Registration Purposes only */
  throw std::runtime_error("Not Implemented");
}
 
Wt::WString RDAUserDatabase::identity (const Wt::Auth::User &user,
				       const std::string &provider) const
{
  /*
   * David: return here the name of the user with id user.id()
   */
	return(user.id());
}
 
void RDAUserDatabase::removeIdentity (const Wt::Auth::User &user,
				      const std::string &provider)
{
/* self registration purposes only */
  throw std::runtime_error("Not Implemented");
}
 
Wt::Auth::PasswordHash RDAUserDatabase::password (const Wt::Auth::User &user) const
{
	char *pw=NULL,*pwm=NULL;
  /*
   * David: return here the password hash of the user with id user.id()
   * Ideally, you should store 'function' and 'value' (salt is usually empty
   * as it's part of the hash).
   */

/* Backload hashed password identification into the file */
/* use 7 methods / times bcrypt */
/*
	Wt::WString pw="somethingIcaneasilyforget";
	PasswordHash result;

	result=Session::passwordAuth().verifier()->hashPassword(pw); */

/* */

  // STUB START
  std::string function = "bcrypt",f2use="";
  std::string salt = "";

  // hash of 'koen' with 7 rounds of bcrypt
  // STUB END
	if(SEC_USERS_FILENO==(-1))
	{
    		return Wt::Auth::PasswordHash("", "", "");
	}
	ZERNRD(SEC_USERS_FILENO);
	FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",user.id().c_str());
	if(EQLNRD(SEC_USERS_FILENO,1)) 
	{
		KEYNRD(SEC_USERS_FILENO,1);
    		return Wt::Auth::PasswordHash("", "", "");
	} else {
/* write PasswordHash result from above into WT PASSWORD */
		FINDFLDGETSTRING(SEC_USERS_FILENO,"WT PASSWORD",&pw);
		FINDFLDGETSTRING(SEC_USERS_FILENO,"WT PASSWORD METHOD",&pwm);
		if(!isEMPTY(pwm)) f2use=pwm;
			else f2use=function;
		return Wt::Auth::PasswordHash(f2use, salt, pw);
	}
} 

void RDAUserDatabase::addAuthToken (const Wt::Auth::User &user,
				    const Wt::Auth::Token &token)
{
	char *d=NULL,*t=NULL;
	RDATData *prev=NULL;
  /*
   * David: add a token for the user to the database, with token.hash() and
   * token.expirationTime() WDatetime values
   */

	loginTokenHash_ = token.hash();
/*
	std::cerr << "SEC_TOKEN_FILENO [" << SEC_TOKEN_FILENO << "]" << std::endl;
*/
	if(SEC_TOKEN_FILENO==(-1))
	{
    		return;
	}

	ZERNRD(SEC_TOKEN_FILENO);
	FINDFLDSETSTRING(SEC_TOKEN_FILENO,"TOKEN",token.hash().c_str());
	FINDFLDSETCHAR(SEC_TOKEN_FILENO,"DELETEFLAG",FALSE);
	FINDFLDSETSTRING(SEC_TOKEN_FILENO,"USER IDENTIFICATION",user.id().c_str());
	d=GETCURRENTDATE10();
	t=GETCURRENTTIME();
	WRTTRANS(SEC_TOKEN_FILENO,0,NULL,prev);

	ZERNRD(SEC_USERS_FILENO);
	FINDFLDSETSTRING(SEC_USERS_FILENO,"USER IDENTIFICATION",user.id().c_str());
	if(EQLNRD(SEC_USERS_FILENO,1))
	{
		KEYNRD(SEC_USERS_FILENO,1);
	} else {
		prev=RDATDataNEW(SEC_USERS_FILENO);
		FINDFLDSETSTRING(SEC_USERS_FILENO,"LAST LOGIN ATTEMPT DATE",d);
		FINDFLDSETSTRING(SEC_USERS_FILENO,"LAST LOGIN ATTEMPT TIME",t);
		WRTTRANS(SEC_USERS_FILENO,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
	}
	if(d!=NULL) Rfree(d);
	if(t!=NULL) Rfree(t);
}

void RDAUserDatabase::removeAuthToken (const Wt::Auth::User &user,
				       const std::string &hash)
{
	RDATData *prev=NULL;
  /*
   * David: remove a token from the user database
   */

  // STUB START
  // std::cerr << "Storing token: " << hash << " for " << user.id() << std::endl;
  // STUB END

	loginTokenHash_ = std::string();
	if(SEC_TOKEN_FILENO==(-1))
	{
    		return;
	}
	ZERNRD(SEC_TOKEN_FILENO);
	FINDFLDSETSTRING(SEC_TOKEN_FILENO,"TOKEN",hash.c_str());
	if(!EQLNRD(SEC_TOKEN_FILENO,1))
	{
		prev=RDATDataNEW(SEC_TOKEN_FILENO);
		FINDFLDSETCHAR(SEC_TOKEN_FILENO,"DELETEFLAG",TRUE);
		FINDFLDSETSTRING(SEC_TOKEN_FILENO,"USER IDENTIFICATION",user.id().c_str());
		WRTTRANS(SEC_TOKEN_FILENO,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
		DELNRD(SEC_TOKEN_FILENO);
	}
}
 
Wt::Auth::User RDAUserDatabase::findWithAuthToken (const std::string &hash)
  const
{
	char *ud=NULL,delflag=FALSE;
  /*
   * David: find a user associated with a token with given hash value, taking
   * into account that it should not be expired
   */

	if(SEC_TOKEN_FILENO==(-1))
	{
    		return;
	}
	ZERNRD(SEC_TOKEN_FILENO);
	FINDFLDSETSTRING(SEC_TOKEN_FILENO,"TOKEN",hash.c_str());
	if(!EQLNRD(SEC_TOKEN_FILENO,1))
	{
		FINDFLDGETCHAR(SEC_TOKEN_FILENO,"DELETEFLAG",&delflag);
		if(!delflag)
		{
			FINDFLDGETSTRING(SEC_TOKEN_FILENO,"USER IDENTIFICATION",&ud);
			USERLOGIN=stralloc(ud);
			const std::string u(ud);
/*
			std::cerr << "User Identification [" << ud << "]" << std::endl;
*/
  			return Wt::Auth::User(u, *this);
		} else {
    			return Wt::Auth::User();
		}
	} else {
    		return Wt::Auth::User();
	}
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
	RDATData *prev=NULL;
	char *d=NULL,*t=NULL;
  /*
   * David: find the token with hash 'loginTokenHash_' (for this user)
   * and reset the expiration time to 30 minutes.
   */

	if(SEC_TOKEN_FILENO==(-1))
	{
    		return;
	}
	ZERNRD(SEC_TOKEN_FILENO);
	FINDFLDSETSTRING(SEC_TOKEN_FILENO,"TOKEN",loginTokenHash_.c_str());
	if(!EQLNRD(SEC_TOKEN_FILENO,1))
	{
		prev=RDATDataNEW(SEC_TOKEN_FILENO);
		FINDFLDSETCHAR(SEC_TOKEN_FILENO,"DELETEFLAG",FALSE);
		FINDFLDSETSTRING(SEC_TOKEN_FILENO,"USER IDENTIFICATION",user.id().c_str());
		d=GETCURRENTDATE10();
		t=GETCURRENTTIME();
		if(d!=NULL) Rfree(d);
		if(t!=NULL) Rfree(t);
		WRTTRANS(SEC_TOKEN_FILENO,0,NULL,prev);
		if(prev!=NULL) FreeRDATData(prev);
	}
}
