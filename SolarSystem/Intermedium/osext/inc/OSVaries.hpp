/*
 * OSVaries.hpp
 *
 *  Created on: Jun 19, 2015
 *      Author: dinglinhui
 */

#ifndef OSVARIES_HPP_
#define OSVARIES_HPP_

static const auto OS_THREAD_PAUSE = 100;
static const auto OS_MAX_TASK_COUNT = 20;

enum class OSRet {
	OK, ERROR, THREAD_ERROR
};

enum class OSMessage {
	MSG_NULL,
	MSG_ACT,	// 异步完成消息
	MSG_GETSERVICE,		// 获取系统服务
	MSG_CREATE,
	MSG_DESTROY,
	MSG_MOVE,
	MSG_SIZE,
	MSG_USER
};

#endif /* OSVARIES_HPP_ */
