#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WServer>

#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "Session.h"

class RDAAuthModel : public Wt::Auth::AuthModel
{
public:
  RDAAuthModel (const Wt::Auth::AuthService &baseAuth,
		Wt::Auth::AbstractUserDatabase &users,
		Wt::WObject* parent=0)
    : Wt::Auth::AuthModel(baseAuth, users, parent)
  { }

  virtual void reset()
  {
    Wt::Auth::AuthModel::reset();
    this->setValue(RememberMeField, true);
  }
};

class TestApplication : public Wt::WApplication
{
public:
  TestApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
  {
    session_.login().changed().connect(this, &TestApplication::authEvent);

    messageResourceBundle()
      .use("/Users/koen/project/customers/openrda/templates");

    Wt::Auth::AuthWidget *authWidget
      = new Wt::Auth::AuthWidget(session_.login());

    authWidget->setModel(new RDAAuthModel(Session::auth(), session_.users(),
					  this));

    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    authWidget->processEnvironment();

    root()->addWidget(authWidget);
  }

  void authEvent() {
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();

      root()->clear();

      // David: create UI here
      Wt::WPushButton *b = new Wt::WPushButton("Test event", root());
      b->clicked().connect(this, &TestApplication::test);
    } else {
      Wt::log("notice") << "User logged out.";
    }
  }

  void test() {

  }

protected:
  virtual void notify(const Wt::WEvent& event) {
    if (event.eventType() == Wt::UserEvent) {
      if (session_.login().loggedIn()) {
	session_.users().extendAuthTokenValidity(session_.login().user());
      }
    }

    WApplication::notify(event);
  }

private:
  Session session_;
};

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
  return new TestApplication(env);
}

int main(int argc, char **argv)
{
  Session::configureAuth();

  return WRun(argc, argv, &createApplication);
}
