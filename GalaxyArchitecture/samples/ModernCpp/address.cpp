/*
 * address.cpp
 *
 *  Created on: Mar 21, 2015
 *      Author: dinglinhui
 */
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <regex>
using namespace std;

set<string> get_addresses(istream& is) {
	set<string> addr;
	regex pat {
			R"((\w+([.-]\w+)*)@(\w+([.-]\w+)*))"}; // email address pattern
	smatch m;
	for (string s; getline(is,s); )                    // read a line
		if (regex_search(s, m, pat))                     // look for the pattern
			addr.insert(m[0]);                             // save address in set
	return addr;
}


istringstream test {  // a stream initialized to a sting containing some addresses
	"asasasa\n"
	"bs@foo.com\n"
	"ms@foo.bar.com$aaa\n"
	"ms@foo.bar.com aaa\n"
	"asdf bs.ms@x\n"
	"<span class=\"MathJax_Preview\">(bs.ms@x)</span><script type=\"math/tex\">bs.ms@x</script>goo\n"
	"cft foo-bar.ff@ss-tt.vv@yy asas"
	"qwert\n"
};

#if 0
int main()
{
	auto addr = get_addresses(test);  // get the email addresses
	for (auto& s : addr)              // write out the addresses
		cout << s << '\n';
}

#endif
