// regex_match example
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <regex>

class regex_callback {
    int sum_;
public:
    regex_callback() :
            sum_ { 0 } {
    }

    template<typename T> void operator()(const T& what) {
        sum_ += atoi(what[1].str().c_str());
    }

    int sum() const {
        return sum_;
    }

};

bool is_email_valid(const std::string& email) {
    //	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    //	return std::regex_match(email, pattern);
    return std::regex_match(email, std::regex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"));
}

void show_ip_parts(const std::string& ip) {
    // regular expression with 4 capture groups defined with
    // parenthesis (...)
    const std::regex pattern("(\\d{1,3}):(\\d{1,3}):(\\d{1,3}):(\\d{1,3})");

    // object that will contain the sequence of sub-matches
    std::match_results<std::string::const_iterator> result { };

    // match the IP address with the regular expression
    bool valid = std::regex_match(ip, result, pattern);
    std::cout << ip << " \t: " << (valid ? "valid" : "invalid") << std::endl;

    // if the IP address matched the regex, then print the parts
    if (valid) {
        std::cout << "IP: " << result[1] << ":" << result[2] << ":" << result[3] << ":" << result[4] << std::endl;
    }
}

void replace(const std::string& text, const std::regex &pattern, const std::string &replace) {
    std::string newtext = std::regex_replace(text, pattern, replace);
    std::cout << newtext << std::endl;
    std::cout << std::endl;
}

void extract(const std::string& s, const std::regex &pattern) {
    const std::sregex_token_iterator end { };  //需要注意一下这里
    for (std::sregex_token_iterator i(s.begin(), s.end(), pattern); i != end; ++i) {
        std::cout << *i << std::endl;
    }
    std::cout << std::endl;
}

void counter(const std::string& s, const std::regex &pattern) {
    int new_counter { 0 };
    int delete_counter { 0 };
    std::smatch m { };
    std::string::const_iterator it = s.begin();
    std::string::const_iterator end = s.end();
    while (std::regex_search(it, end, m, pattern)) {
        m[1].matched ? ++new_counter : ++delete_counter;
        it = m[0].second;
    }

    if (new_counter != delete_counter)
        std::cout << "Leak detected!\n";
    else
        std::cout << "Seems ok...\n";
    std::cout << std::endl;
}

void sum(const std::string& s, const std::regex &pattern) {
    std::sregex_iterator it(s.begin(), s.end(), pattern);
    std::sregex_iterator end { };

    //	regex_callback c;
    //	int sum = std::for_each(it, end, c).sum();  //for_each返回的是这个函数对象，因此可以调用sum

    int sum { 0 };
    std::for_each(it, end, [&sum](std::smatch match)
    {
        sum += atoi(match[1].str().c_str());
    });
    std::cout << sum << std::endl;
}

void match(const std::string &s, const std::regex &pattern) {
    std::vector<std::string> vec { };
    std::sregex_token_iterator it(s.begin(), s.end(), pattern, -1); //// -1逆向匹配,就是匹配除了'/'之外的
    std::sregex_token_iterator end { };
    while (it != end)
        vec.push_back(*it++);
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

std::set<std::string> get_addresses(std::istream& is) {
    std::set<std::string> addr;
    std::regex pat {
            R"((\w+([.-]\w+)*)@(\w+([.-]\w+)*))"}; // email address pattern
    std::smatch m;
    for (std::string s; getline(is,s); )                    // read a line
        if (regex_search(s, m, pat))                     // look for the pattern
            addr.insert(m[0]);                             // save address in set
    return addr;
}

std::istringstream test {  // a stream initialized to a sting containing some addresses
    "asasasa\n"
    "bs@foo.com\n"
    "ms@foo.bar.com$aaa\n"
    "ms@foo.bar.com aaa\n"
    "asdf bs.ms@x\n"
    "<span class=\"MathJax_Preview\">(bs.ms@x)</span><script type=\"math/tex\">bs.ms@x</script>goo\n"
    "cft foo-bar.ff@ss-tt.vv@yy asas"
    "qwert\n"
};

void test_regex() {

    auto addr = get_addresses(test);  // get the email addresses
    for (auto& s : addr)              // write out the addresses
        std::cout << s << '\n';

    if (std::regex_match("subject", std::regex("(sub)(.*)")))
    {
        std::cout << "string literal matched\n";
    }

    std::string s("subject");
    std::regex e("(sub)(.*)");
    if (std::regex_match(s, e))
        std::cout << "string object matched\n";

    if (std::regex_match(s.begin(), s.end(), e))
        std::cout << "range matched\n";

    std::cmatch cm;    // same as std::match_results<const char*> cm;
    std::regex_match("subject", cm, e);
    std::cout << "string literal with " << cm.size() << " matches\n";

    std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
    std::regex_match(s, sm, e);
    std::cout << "string object with " << sm.size() << " matches\n";

    std::regex_match(s.cbegin(), s.cend(), sm, e);
    std::cout << "range with " << sm.size() << " matches\n";

    // using explicit flags:
    std::regex_match("subject", cm, e, std::regex_constants::match_default);

    std::cout << "the matches were: ";
    for (unsigned i = 0; i < sm.size(); ++i) {
        std::cout << "[" << sm[i] << "] ";
    }

    std::cout << std::endl;

    /***************************************************************/
    std::string email1 = "marius.bancila@domain.com";
    std::string email2 = "mariusbancila@domain.com";
    std::string email3 = "marius_b@domain.co.uk";
    std::string email4 = "marius@domain";
    std::cout << email1 << " : " << (is_email_valid(email1) ? "valid" : "invalid") << std::endl;
    std::cout << email2 << " : " << (is_email_valid(email2) ? "valid" : "invalid") << std::endl;
    std::cout << email3 << " : " << (is_email_valid(email3) ? "valid" : "invalid") << std::endl;
    std::cout << email4 << " : " << (is_email_valid(email4) ? "valid" : "invalid") << std::endl;

    /***************************************************************/
    show_ip_parts("1:22:33:444");
    show_ip_parts("1:22:33:4444");
    show_ip_parts("100:200");

    /***************************************************************/
    // the source text
    std::string weekend = "Saturday and Sunday, but some Fridays also.";
    // regular expression
    extract(weekend, std::regex("\\w+day"));

    /***************************************************************/
    std::string text = "This is a element and this a unique ID.";
    replace(text, std::regex("(\\ba (a|e|i|u|o))+"), std::string("an $2"));

    /***************************************************************/
    std::string date1 = "1/2/2008";
    std::string date2 = "12.08.2008";
    const std::regex pattern("(\\d{1,2})(\\.|-|/)(\\d{1,2})(\\.|-|/)(\\d{4})");
    std::string replace = "$5$4$3$2$1";
    std::cout << date1 << " -> " << std::regex_replace(date1, pattern, replace) << std::endl;
    std::cout << date2 << " -> " << std::regex_replace(date2, pattern, replace) << std::endl;
    std::cout << std::endl;

    /***************************************************************/
    std::string s1 = "Calls to new must be followed by delete. Calling simply new results in a leak!";
    counter(s1, std::regex("(new)|(delete)"));

    /***************************************************************/
    std::string s2 = "1,1,2,3,5,8,13,21";
    sum(s2, std::regex("(\\d+),?"));

    /***************************************************************/
    std::string s3 = "Split/Vulue/Teather/Neusoft/Write/By/Lanwei";
    match(s3, std::regex("/"));
}

