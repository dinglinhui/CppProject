#ifndef EVENTMANAGER_HPP_
#define EVENTMANAGER_HPP_

#include <unordered_map>
#include <deque>
#include <memory>
#include <iostream>
#include <functional>
#include <type_traits>

#include "cmn.hpp"

namespace application {

// Type of events
enum EventTypes {
	EVENT_TYPE_MSG,
};

struct EventArgs {
	// EventID
	const int EventID;
	// Type of the event
	const EventTypes EventType;
	// For Windows message, wparam and lparam are typically passed on without any modification from the wndProc
	// For some custom message, wparam and lparam can mean different things, it is up to the client to interpret them
//    const WPARAM WParam;
//    const LPARAM LParam;

	// Default contructor - This eventArgs should only be constructed by Event Manager
	//eventID_          - ID of th Event
	//eventType_        - type of the event
	//wparam_, lparam_  - For Windows message, wparam and lparam are typically passed on without any modification from the wndProc
	//                    For some custom message, wparam and lparam can mean different things, it is up to the client to interpret them
	//clientPtr         - clientPtr
	EventArgs(int eventId_,
			EventTypes eventType_/*, WPARAM wparam_, LPARAM lparam_*/) :
			EventID(eventId_), EventType(eventType_)/*, WParam(wparam_), LParam(lparam_)*/
	{
	}
};

template<typename Func>
class EventManager;
class EventManagerClient {
	template<typename Func>
	friend class EventManager;
public:
	EventManagerClient() {
		// create a simple shared_ptr to keep track when this client is deleted
		// if the client is deleted, this shared_ptr will decrement ref count to 0 and become expired
		// event manager will keep a weak_ptr reference to this shared_ptr
		// this will allow us to debug when clients are deleted without unregister its events
		IsValid_ = std::shared_ptr<int>(new int());
	}

	virtual ~EventManagerClient() {
	}

	virtual int OnEvent(const EventArgs *args_) = 0;
private:
	std::shared_ptr<int> IsValid_;
};

// Event IDs
static enum {
	WM_SHOW = 0, WM_HIDE, WM_HIDEALL, WM_EXIT, WM_PAUSE, WM_RESUME
};

template<typename Func>
class EventManager: NonCopyable {
public:
private:
	// A linked list for event handlers
	// When insert or delete, Event Manager will ensure sorting by priority
	class EventHandler {
	public:
		EventHandler(int componentID_, int priority_,
				EventManagerClient *clientPtr_) :
				ComponentID_(componentID_), Priority_(priority_), ClientPtr_(
						clientPtr_) {
			IsValid_ = clientPtr_->IsValid_;
		}

		int ComponentID_;
		int Priority_;
		EventManagerClient *ClientPtr_;
		std::weak_ptr<int> IsValid_;

		bool operator<(const EventHandler &rhs) const {
			return (Priority_ > rhs.Priority_);
		}
	};
	using EventHandlerQueue = std::deque<EventHandler>;
	using EventHandlerQueueMap = std::unordered_map<int, EventHandlerQueue>;

private:
	int m_nextKey;
	EventHandlerQueueMap HandlerQueueMap_;

//	std::unordered_map<int, Func> m_connections;

public:
	EventManager() :
			m_nextKey(0) {
	}

	~EventManager() {
	}

	bool DispatchMessage(int message) {
		auto it = HandlerQueueMap_.find(message);
		if (it != HandlerQueueMap_.end()) {
			EventArgs arg(message, EVENT_TYPE_MSG/*, wParam, lParam*/);
			EventHandlerQueue queue = it->second;

			std::for_each(std::begin(queue), std::end(queue), [&arg](EventHandler &handler) {
				int handlerRet = handler.ClientPtr_->OnEvent(&arg);
			});
		}

		return true;
	}

/*	int operator +=(Func&& f) {
		return Connect(std::forward<Func>(f));
	}

	int operator +=(Func& f) {
		return Connect(f);
	}

	template<typename ... Args>
	void operator()(Args&&... args) {
		Notify(std::forward<Args>(args)...);
	}

	EventManager& operator -=(int key) {
		Disconnect(key);
		return *this;
	}

	void Clear() {
		m_connections.clear();
	}*/

private:
	/*int Connect(Func&& f) {
		return Assgin(std::forward<Func>(f));
	}

	int Connect(Func& f) {
		return Assgin(f);
	}

	void Disconnect(int key) {
		m_connections.erase(key);
	}

	template<typename ... Args>
	void Notify(Args&&... args) {
		//m_connections.find(m_nextKey)(std::forward<Args>(args)...);
		for (auto& it : m_connections) {
			it.second(std::forward<Args>(args)...);
		}
	}

	template<typename F>
	int Assgin(F&& f) {
		int index = m_nextKey++;
		Push(index, std::forward<Func>(f));
		return index;
	}

	template<typename F>
	void Push(int index, F&& f) {
		m_connections.emplace(index, std::move(f));
	}

	template<typename F>
	void Push(int index, F& f) {
		m_connections.emplace(index, f);
	}*/
}
;

class SystemServices: public EventManagerClient {
public:
	SystemServices() {
	}

	virtual ~SystemServices() {
	}

	int OnEvent(const EventArgs* eventArgs_) {
		switch (eventArgs_->EventID) {
		case WM_SHOW: {
			break;
		}
		default:
			break;
		}

		return 0;
	}
};

struct ST {
	int a;
	int b;
	void fun(int x, int y) {
		a = x;
		b = y;
		std::cout << "a = " << a << " b= " << b << std::endl;
	}
};

void print(int a, int b) {
	std::cout << a << ", " << b << std::endl;
}

void test_eventmanager() {
	std::cout << "testEventManager:" << std::endl;

	using Delegate = std::function<void(int, int)>;
	using Event = EventManager<Delegate>;
	Event event;

	/*	ST t;
	 auto key1 = event += &print;
	 auto key2 = event += [&t](int a, int b) {
	 t.a = a;
	 t.b = b;
	 std::cout << "t.a = " << t.a << " t.b = " << t.b << std::endl;
	 };
	 auto key3 = event += std::bind(&ST::fun, &t, std::placeholders::_1,
	 std::placeholders::_2);

	 event(2, 3);

	 event -= key1;
	 event -= key2;
	 event -= key3;

	 event(4, 5);
	 event.Clear();
	 event(1, 2);*/

	std::cout << std::endl;
}

/*
 struct stA {
 int a;
 int b;
 void fun(int x, int y) {
 a = x;
 b = y;
 std::cout << "a = " << a << " b= " << b << std::endl;
 }
 };

 void print(int a, int b) {
 std::cout << a << ", " << b << std::endl;
 }

 void testEventManager() {
 std::cout << "testEventManager:" << std::endl;

 using Delegate1 = std::function<void(int, int)>;
 using Event1 = EventManager<Delegate1>;
 Event1 event1;

 //添加委托
 stA t;
 auto key1 = event1 += &print;
 auto key2 = event1 += [&t](int a, int b) {
 t.a = a;
 t.b = b;
 std::cout << "t.a = " << t.a << " t.b = " << t.b << std::endl;
 };
 auto key3 = event1 += std::bind(&stA::fun, &t, std::placeholders::_1,
 std::placeholders::_2);

 //广播
 event1(2, 3);

 //移除委托
 event1 -= key1;
 event1 -= key2;
 event1 -= key3;

 event1(4, 5);
 //清空事件
 event1.Clear();
 event1(1, 2); //清空什么都不会输出

 std::cout << std::endl;
 }

 */
} //namespace cplusplus
#endif /* EVENTMANAGER_HPP_ */
