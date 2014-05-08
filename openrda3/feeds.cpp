#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>

class FeedResource : public Wt::WResource 
{
public:
	FeedResource(int iterations, Wt::WObject *parent = 0)
		: Wt::WResource(parent), iterations_(iterations)
	{
		suggestFileName("feeds.html");
	}
	~FeedResource() {
		beingDeleted();
	}
	void handleRequest(const Wt::Http::Request& request,
		Wt::Http::Response& response) {

		Wt::Http::ResponseContinuation *continuation = 
			request.continuation();
	
		int iteration = continuation ? boost::any_cast<int>(continuation->data()) : 0;
		if(iteration == 0) response.setMimeType("text/html");

		int last = std::min(iterations_, iteration + 100);
		for(int i = iteration; i<last; ++i)
			response.out() << i << std::endl;

		if(last < iterations_) {
			continuation = response.createContinuation();
			continuation->setData(last);
		}
	}
private:
	int iterations_;
};
