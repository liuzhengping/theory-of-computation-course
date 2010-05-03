/*
 * simulator.cpp
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

#include "simulator.h"

bool MyPA::process(const string& input) {


	State<void*>* last = current_state();


	bool ret = PA<void*, string, string>::process(input);
	if(ret) {
		//printf("Presao u stanje:\n%s\n", current_state().get_name().c_str());
	}
	else {
		//printf("Usao u nepostojece stanje\n");
	}

	State<void*>* cur = current_state();

	stringstream ss_from, ss_to;
	if(last == NULL)
		ss_from << "null";
	else
		ss_from << *last;

	if(cur == NULL)
		ss_to << "null";
	else
		ss_to << *cur;

	if(print_stack_.empty()) {
		vector<string> sc = stackContents();
		print_stack_.push_back(Tuple<string, string, string, string>(
				input, ss_from.str(), ss_to.str(),
				StringUtils::to_string(sc.begin(), sc.end(),
						" | ", "[", "]")));
	}

	return ret;
}

void MyPA::sequencePartProcessed(const std::string& sequence_par) const {
	if(print_on_) {
		reverse(print_stack_.begin(), print_stack_.end());
		FORC(it, print_stack_) {
			cout << ">" << it->first << ": ";
			cout << it->second << "\t->\t" << it->third << endl;
			cout << "Stack: " << it->fourth << endl;
		}
	}
	print_stack_.clear();
}

void MyPA::transition_occured(const PATransitionDomain& domain,
		const PATransitionCodomain& codomain,
		const string& input) const {
//	printf("Bla");
}

#include <iostream>

bool MyPA::build_from_file(const string& definition_filename) {
	static const int MAX_BUFF = 4096;
	static std::map<int, std::string> label_map;
	static std::map<int, std::string> stack_alphabet_label;

	label_map.clear();
	stack_alphabet_label.clear();
	char buff[MAX_BUFF], label[MAX_BUFF];
	FILE *f = fopen(definition_filename.c_str(), "r");
	if(f == NULL) {
		fprintf(stderr, "Ne postoji datoteka \"%s\".", definition_filename.c_str());
		return false;
	}
	printf("Gradim PA...");
	while(fgets(buff, sizeof(buff), f)) {
		int len;
		buff[len = strlen(buff)-1] = '\0';
		if(len <= 1)
			continue;

		if(buff[0] == ioconstants::STATE_DEFINITION_TAG) {
			int id;
			sscanf(buff, "#%d %[^\n]", &id, label);
//			printf("[%d %s]\n", id, label);
			bool accept = false;
			bool start = false;
			if(label[0] == ioconstants::STATE_ACCEPT_PREFIX) {
				accept = true;
			}
			if(label[accept] == ioconstants::START_PAGE_TAG) {
				start = true;
			}
			State<void*> state(id, NULL, accept);
			state.set_name(label+accept+start);
			add_state(state, start);
			label_map[state.get_id()] = state.get_name();
		}
		else if(buff[0] == ioconstants::STACK_ALPHABET_DEFINITION_TAG) {
			int id;
			sscanf(buff, "@%d %[^\n]", &id, label);
			stack_alphabet_label[id] = label;
			printf("STACK LABEL: %d %s\n", id, stack_alphabet_label[id]
			                                                     .c_str());
		}
		else {
			int from, to, stack_before_id, stack_after_id;
			sscanf(buff, "%d%d%d%d %[^\n]", &from, &to, &stack_before_id,
					&stack_after_id, label);

			bool was_push = false;
			vector<string> tokens = StringUtils::split(buff, " ");
			if(tokens.size() >= 4 && tokens[3][0] == ioconstants::STACK_PUSH_TAG) {
				was_push = true;
			}

			string stack_before = stack_alphabet_label[stack_before_id];
			string stack_after = (stack_after_id >= 0 ?
					stack_alphabet_label[stack_after_id]
					                     : ioconstants::STACK_POP_TAG);

			PATransitionDomain domain(label, get_state(from), stack_before);
			PATransitionCodomain codomain(get_state(to),
					make_pair(stack_after, was_push));
			add_transition(domain, codomain);

			printf("TRANSITION: %d %d %d %d %s\n", from, to, stack_before_id,
									stack_after_id, label);

			cout << "add_transition: (" << domain.second->get_id() << ", "
					<< codomain.first->get_id()
							<< ", " << domain.third << ") ";
					cout << "->" << domain.first << "-> "
							<< "(" << codomain.first << ", "
							<< codomain.second << ")" << endl;

//			printf("GET(%d): %s\n", from, get_state(from)->get_name().c_str());
		}
	}
	fclose(f);
//	FORC(it, label_map) {
//		int id = it->first;
//		State<void*> *s = get_state(id);
//		cout << id << ":" << s << endl;
//	}

	printf("DONE\n");
	return true;
}






