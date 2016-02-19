#include "lib_import.h"

class cls_http_client
{
private:
	map<string,string> cookies;
	boost::asio::io_service io_core; 

	static map<string,string> analyse_url(string url)
	{

	}

public:
	
	string get_page(string url)
	{
		tcp::resolver resolver_page(io_core);
		tcp::resolver::query query_page(url,"http");
		
		tcp::resolver::iterator itor = resolver_page.resolve(query_page);
		tcp::resolver::iterator itor_end;

		tcp::socket sock_page(io_core);
		boost::system::error_code ec = boost::asio::error::host_not_found;
		while(ec && itor != itor_end)
		{
			sock_page.close();
			sock_page.connect(*itor,ec);
			itor++;
		}

		if (ec)
		{
			return "";
		}

		boost::asio::streambuf request;
		std::ostream request_stream(request);
		request_stream<<"GET"<<" "<<url<<endl;

	}
};