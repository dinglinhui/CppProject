#include "application/crtp.hpp"
#include "application/smart_pointer.hpp"
#include "application/system.hpp"
#include "application/linked_list.hpp"
#include "multithreading/threadpool.hpp"
#include "multithreading/thread.hpp"
#include "basic/template.hpp"
#include "utils/utils.hpp"

int main(int argc, char *argv[]) {
	
	application::test_link_loop();

#if 0
	basic::test_template();

	application::test_crtp();
	application::test_employeesystem();
	application::test_smartpointer();
	multithreading::test_threadpool();	
	multithreading::test_thread();
	multithreading::test_future();
	multithreading::test_promise();
	multithreading::manager();
#endif

	return 0;
}
