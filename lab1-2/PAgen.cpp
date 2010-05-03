/*
 * PAgen.cpp
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

#include "PAgen.h"

#include "ioconstants.h"

#include <sstream>

Generator::Generator() : cur_state_id_(0), cur_stack_symbol_id_(0) {
}

int Generator::add_state(const std::string& label, bool accept, bool start) {
	if(!state_ids_.count(label)) {
		state_ids_[label] = cur_state_id_++;
		if(accept)
			accept_states_.insert(state_ids_[label]);
	}
	if(start)
		start_state_id_ = state_ids_[label];
	return state_ids_[label];
}

int Generator::add_stack_symbol(const std::string& label) {
	stack_symbol_ids_[label] = cur_stack_symbol_id_++;
	return stack_symbol_ids_[label];
}

void Generator::add_transition(const std::string& from_label,
		const std::string& to_label,
		const std::string& transition_input,
		const std::string& from_stack_top,
		const std::string& to_stack_top,
		bool push) {
	int from_id = (state_ids_.count(from_label) ? state_ids_[from_label] : ERROR_HASH);
	int to_id = (state_ids_.count(to_label) ? state_ids_[to_label] : ERROR_HASH);
	transitions_.push_back(TransitionInfo(from_id, to_id,
			transition_input, from_stack_top, to_stack_top, push));

}

std::vector<std::string> Generator::getStateLabels() const {
	std::vector<std::string> ret;
	FORC(it, state_ids_) {
		ret.push_back(it->first);
	}
	return ret;
}

std::vector<std::string> Generator::getStackSymbolLabels() const {
	std::vector<std::string> ret;
	FORC(it, stack_symbol_ids_) {
		ret.push_back(it->first);
	}
	return ret;
}

void Generator::output(const std::string& filename) {
	FILE *f = fopen(filename.c_str(), "w");
	FORC(it, state_ids_) {
		fprintf(f, "%c%d ", ioconstants::STATE_DEFINITION_TAG,
				state_ids_[it->first]);

		if(is_accept_state(it->first))
			fprintf(f, "%c", ioconstants::STATE_ACCEPT_PREFIX);
		if(start_state_id_ == it->second)
					fprintf(f, "%c", ioconstants::START_PAGE_TAG);
		fprintf(f, "%s\n", it->first.c_str());
	}
	FORC(it, stack_symbol_ids_) {
		fprintf(f, "%c%d %s\n", ioconstants::STACK_ALPHABET_DEFINITION_TAG,
				it->second, it->first.c_str());
	}
	for(std::vector<TransitionInfo>::iterator it = transitions_.begin();
			it != transitions_.end(); ++it) {
		int stack_top_before = stack_symbol_ids_[it->from_stack_top];
		int stack_top_after = stack_symbol_ids_[it->to_stack_top];
		fprintf(f, "%2d %2d %2d ", it->from_id, it->to_id,
				stack_top_before);
		if(it->push) {
			fprintf(f, "%c%-2d", ioconstants::STACK_PUSH_TAG,
					stack_top_after);
		}
		else if(it->to_stack_top == ioconstants::STACK_POP_TAG)
			fprintf(f, "%-3d", ioconstants::STACK_POP_SYMBOL_ID);
		else
			fprintf(f, "%-3d", stack_top_after);
		fprintf(f, " %s\n", it->transition_input.c_str());
	}
	fclose(f);
}

bool Generator::is_accept_state(const std::string& label) {
	return state_ids_.count(label)
					&& accept_states_.find(state_ids_[label]) != accept_states_.end();
}
