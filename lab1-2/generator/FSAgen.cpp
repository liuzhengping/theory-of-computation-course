/*
 * FSAgen.cpp
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

#include "../include/FSAgen.h"

Generator::Generator() {
	cur_hash_ = 0;
}

void Generator::add_state(const std::string& label, bool accept) {
	if(!ids_.count(label)) {
		ids_[label] = cur_hash_++;
		if(accept)
			accept_states_.insert(ids_[label]);
	}
}

void Generator::add_transition(const std::string& from_label,
		const std::string& to_label,
		const std::string& transition_input) {
	int from_id = (ids_.count(from_label) ? ids_[from_label] : ERROR_HASH);
	int to_id = (ids_.count(to_label) ? ids_[to_label] : ERROR_HASH);
	transitions_.push_back(TransitionInfo(from_id, to_id,
			transition_input));
}

void Generator::output(const std::string& filename) {
	FILE *f = fopen(filename.c_str(), "w");
	FORC(it, ids_) {
		fprintf(f, "%c%d ", STATE_DEFINITION_TAG, ids_[it->first]);
		if(is_accept_state(it->first))
			fprintf(f, "%c", STATE_ACCEPT_PREFIX);
		fprintf(f, "%s\n", it->first.c_str());
	}
	for(std::vector<TransitionInfo>::iterator it = transitions_.begin();
			it != transitions_.end(); ++it)
		fprintf(f, "%d %d %s\n", it->from_id, it->to_id,
				it->transition_input.c_str());
	fclose(f);
}

bool Generator::is_accept_state(const std::string& label) {
	return ids_.count(label)
					&& accept_states_.find(ids_[label]) != accept_states_.end();
}
