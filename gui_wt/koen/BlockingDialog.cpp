#include <Wt/WDialog>
#include <Wt/WApplication>
#include <Wt/WPushButton>
#include <Wt/WJavaScript>

class BlockingDialog : public Wt::WDialog
{
public:
  BlockingDialog(const std::string& url)
    : closedSignal_(this, "closed")
  {
    setWindowTitle("Blocked in dialog");

    contents()->addWidget
      (new Wt::WText("<p>Please use the newly opened window.</p>"
		     "<p>Continue here when done.</p>"));

    closedSignal_.connect(this, &BlockingDialog::accept);

    /*
     * See also: https://developer.mozilla.org/en-US/docs/Web/API/Window.open
     * for more options on the new window.
     */

    std::string js = 
      "var win = window.open('" + url + "');"
      "if (win) {" // not blocked
      """var timer = setInterval(function() {"
      """""if (win.closed) {"
      ""    "clearInterval(timer);"
      +      closedSignal_.createCall() + ";"
      +   "}"
      """}, 500);"
      "}";

    doJavaScript(js);
  }

private:
  Wt::JSignal<> closedSignal_;
};

void openWindow(const std::string& url)
{
  BlockingDialog d(url);
  d.exec();
}

using namespace Wt;

class Test : public WApplication
{
public:
  Test(const WEnvironment& env) 
   : WApplication(env)
  {
    WPushButton *b = new WPushButton("Test", root());
    b->clicked().connect(this, &Test::doTest);
  }

  void doTest() {
    openWindow("http://www.emweb.be");
  }
};

WApplication *createApplication(const WEnvironment& env){
  return new Test(env);
}

int main(int argc, char *argv[]){
  return WRun(argc, argv, createApplication);
}

