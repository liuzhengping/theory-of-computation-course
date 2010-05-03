/*
 * PAgen.h
 * 
 * Copyright (C) 2010 Leo Osvald <leo.osvald@gmail.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.
 *  
 * You may obtain a copy of the License at 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 *  
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License. 
 */

#ifndef FSAGEN_H_
#define FSAGEN_H_

#include "include/util.h"

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>


class Generator {
protected:
	struct TransitionInfo;
	static const int START_HASH = 0;
	static const int ERROR_HASH = -1;
//private:
	std::map<std::string, int> state_ids_;
	std::map<std::string, int> stack_symbol_ids_;
	std::set<int> accept_states_;
	int cur_state_id_;
	int cur_stack_symbol_id_;
	int start_state_id_;
	std::vector<TransitionInfo> transitions_;

protected:
	int add_state(const std::string& label, bool accept, bool start = false);
	int add_stack_symbol(const std::string& label);
	std::vector<std::string> getStateLabels() const;
	std::vector<std::string> getStackSymbolLabels() const;
	void add_transition(const std::string& from_label,
			const std::string& to_label,
			const std::string& transition_input,
			const std::string& from_stack_top,
			const std::string& to_stack_top,
			bool push = false);
	bool is_accept_state(const std::string& label);

public:
	Generator();
	virtual void output(const std::string& filename);

protected:
	struct TransitionInfo {
		int from_id;
		int to_id;
		std::string transition_input;
		std::string from_stack_top;
		std::string to_stack_top;
		bool push;
		TransitionInfo(int from_label, int to_label,
				const std::string& transition_input,
				const std::string& from_stack_top,
				const std::string& to_stack_top,
				bool push)
		: from_id(from_label), to_id(to_label),
		  transition_input(transition_input),
		  from_stack_top(from_stack_top),
		  to_stack_top(to_stack_top),
		  push(push) { }

		friend bool operator<(const TransitionInfo& A, const TransitionInfo& B)
		{
			if(A.from_id != B.from_id)
				return A.from_id < B.from_id;
			if(A.to_id != B.to_id)
				return A.to_id < B.to_id;
			if(A.from_stack_top != B.from_stack_top)
				return A.from_stack_top < B.from_stack_top;
			if(A.to_stack_top != B.to_stack_top)
				return A.to_stack_top < B.to_stack_top;
			return A.transition_input < B.transition_input;
		}

		friend bool operator==(const TransitionInfo& A, const TransitionInfo& B)
		{
			return A.from_id != B.from_id
			&& A.to_id < B.to_id
			&& A.from_stack_top < B.from_stack_top
			&& A.to_stack_top < B.to_stack_top
			&& A.transition_input < B.transition_input;
		}
	};
};

#endif /* FSAGEN_H_ */
