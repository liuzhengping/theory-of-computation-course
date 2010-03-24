/*
 * FSAgen.h
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

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "util.h"
#include "delimiters.h"

class Generator {
protected:
	struct TransitionInfo;
	static const int START_HASH = 0;
	static const int ERROR_HASH = -1;
private:
	std::map<std::string, int> ids_;
	std::set<int> accept_states_;
	int cur_hash_;
	std::vector<TransitionInfo> transitions_;

protected:
	void add_state(const std::string& label, bool accept);
	void add_transition(const std::string& from_label,
			const std::string& to_label,
			const std::string& transition_input);
	bool is_accept_state(const std::string& label);
public:
	Generator();
	virtual void output(const std::string& filename);

protected:
	struct TransitionInfo {
		int from_id;
		int to_id;
		std::string transition_input;
		TransitionInfo(int from_label, int to_label,
				const std::string& transition_input)
		: from_id(from_label), to_id(to_label),
		  transition_input(transition_input) { }

		friend bool operator<(const TransitionInfo& A, const TransitionInfo& B)
		{
			if(A.from_id != B.from_id)
				return A.from_id < B.from_id;
			if(A.to_id != B.to_id)
				return A.to_id < B.to_id;
			return A.transition_input < B.transition_input;
		}
	};
};

#endif /* FSAGEN_H_ */
