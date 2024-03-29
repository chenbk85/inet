# inet
  
  c++ network framework built on boost::asio and tbb [using c++11]

	/*proactor*/
	inet::asio::proactor io;
	io.listen(port_number,[&] (inet::session_ptr session) {
		session->on_data = [&] (const inet::shared_buffer& buffer) {
			session->send(buffer);
		};
	});
	io.wait_end();
	
	/*reactor*/
	inet::asio::reactor io;
	io.listen(port_number,[&] (inet::session_ptr session) {
		session->on_data = [&] (const inet::shared_buffer& buffer) {
			session->send(buffer);
		};
	});
	io.run();
	/* or
	while(!io.stopped()) {
		io.poll();
		boost::thread::yield();
	}*/


## Installation

  working directory setting
  
	+3rdParty
		+include
			+boost
			+gtest
			+gmock
			+tbb
		+lib
			+bebug
			+release
		+dll
			+debug
			+release
	+inet (this project)

	
## License 

(The MIT License)

Copyright (c) 2011-2012 iwaltgen &lt;iwaltgen@gmail.com&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
