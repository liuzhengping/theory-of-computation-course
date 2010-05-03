/*
 * generator.cpp
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

#include "generator.h"
#include "ioconstants.h"

#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>

MyGenerator::MyGenerator(const std::string& input_filename)
: input_filename_(input_filename) {
	input(input_filename);
}

std::string MyGenerator::createStartPageString(const vector<const WebPage*>& pages) {
	string s = "0:pocetna stranica|";
	for(int i = 0; i < (int)pages.size(); ++i) {
		if(i > 0) s += ", ";
		stringstream ss; ss << pages[i]->get_id();
		s += ss.str();
	}
	s += "|";
	return s;
}

string MyGenerator::hash(const WebPage* web_page, int load_res_mask, int n) {
	stringstream ss;
	ss << web_page->get_url();
	if(load_res_mask != (1<<n)-1) {
		vector<string> v;
		for(int i = 0; i < n; ++i)
			if(((1<<i) & load_res_mask) == 0)
				v.push_back(web_page->get_resource(i));
		ss << " ceka da se ucitaju resursi: " << StringUtils::to_string(v.begin(),
				v.end(), ", ", "", "");
	}
	else
		ss << " ucitana";
	return ss.str();
}

void MyGenerator::input(const string& input_filename) {
	FILE *f = fopen(input_filename_.c_str(), "r");
	char buff[LINE_BUFFER_LENGTH];
	int back_n = generator_constants::NO_BACK;
	for(int line = 0; fgets(buff, sizeof(buff), f); ++line) {
		buff[strlen(buff)-1] = '\0';
		string s = buff;
		static char back[16];
		if(sscanf(buff, "%[^:]%*c%d", back, &back_n) == 2 && strcmp(back, "BACK") == 0) {
			printf("Maksimalan broj povratka: %d\n", back_n);
		}
		else
			web_page_manager_.create_page_from_string(buff);
	}
	fclose(f);


	createStates(back_n);

	const vector<const WebPage*> pages = web_page_manager_.get_pages();

	createStackSymbols();

	createBackTransitions(pages, back_n);


	std::vector<std::string> labels = getStateLabels();
	FORC(stack_top_label, labels) {
		printf("state labels: %s %d\n", stack_top_label->c_str(),
				state_ids_[*stack_top_label]);
	}
	FORC(wp, pages) {
		printf("%d %s %d\n", (*wp)->get_id(), (*wp)->get_url().c_str(),
				(*wp)->get_resources().size());
	}

	printf("ALL OK\n");

	createRegularWebPageTransitions(pages, back_n);


}

void MyGenerator::output(const string& output_filename) {

	Generator::output(output_filename);
};

std::string MyGenerator::toCompoundLabel(const string& label1, const string& label2) {
	return "("+label1+", "+label2+")";
}

const WebPage* MyGenerator::createStartState(const vector<const WebPage*>& pages) {
	start_page_ = web_page_manager_.create_page_from_string(
			createStartPageString(pages));
	int n = start_page_->get_resources().size();
	for(int mask = 0; mask < 1<<n; ++mask) {
		string state_label = hash(start_page_, mask, n);
		add_state(state_label, mask == (1<<n)-1, true);
		printf("%s\n", hash(start_page_, mask, n).c_str());
	}
	return start_page_;
}

void MyGenerator::createStartPageTransitions() {
	string start_state_label = start_page_->get_url();
	FORC(link, start_page_->get_links()) {
		const WebPage *link_page = web_page_manager_.get_page(*link);
		int n = link_page->get_resources().size();
		add_transition(
				start_state_label,
				hash(link_page, 0, n),
				web_page_manager_.get_page(*link)->get_url(),
				ioconstants::STACK_EMPTY_SYMBOL,
				ioconstants::STACK_EMPTY_SYMBOL
		);
	}
}


void MyGenerator::createRegularWebPageTransitions(const vector<const WebPage*>& pages,
		int back_n) {

	std::vector<std::string> stack_symbol_labels
	= getStackSymbolLabels();

	FORC(wp, pages) {
		int n = (*wp)->get_resources().size();
		for(int mask = 0; mask < 1<<n; ++mask) {
			for(int i = 0; i < n; ++i)
				if(((1<<i) & mask) == 0) {
					int next_mask = mask | (1<<i);
					std::vector<std::string> stack_symbol_labels
					= getStackSymbolLabels();
					FORC(stack_top_label, stack_symbol_labels) {
						add_transition(
								hash(*wp, mask, n),
								hash(*wp, next_mask, n),
								(*wp)->get_resource(i),
								*stack_top_label,
								*stack_top_label
						);
					}

					printf("%s %s %s\n", hash(*wp, mask, n).c_str(),
							hash(*wp, next_mask, n).c_str(),
							(*wp)->get_resource(i).c_str());
				}
			if(mask == (1<<n)-1) {
				FORC(link, (*wp)->get_links()) {
					const WebPage *link_page = web_page_manager_.get_page(*link);
					int n = link_page->get_resources().size();
					string from_page_label = hash(*wp, mask,
							(*wp)->get_resources().size());
					string to_page_label = hash(link_page, 0, n);
					string to_loaded_page_label = hash(link_page, (1<<n)-1, n);

					FORC(stack_top_before, stack_symbol_labels) {
						add_transition(
								from_page_label,
								to_page_label,
								web_page_manager_.get_page(*link)->get_url(),
								*stack_top_before,
								from_page_label,
								true
						);
					}
//					for(int back = 1; back <= back_n; ++back) {
//						add_transition(
//								to_loaded_page_label,
//								generator_constants::to_back_n_string(back)
//								web_page_manager_.get_page(*link)->get_url(),
//								from_page_label,
//								ioconstants::STACK_POP_TAG
//						);
//					}
					printf("[%s \t %s]\n",
							to_loaded_page_label.c_str(),
							from_page_label.c_str());
				}
			}
		}
	}


	FORC(wp, pages) {
		int n = (*wp)->get_resources().size();
		for(int mask = 0; mask < 1<<n; ++mask) {
			string page_label = hash(*wp, mask, n);
			FORC(stack_top, stack_symbol_labels) {
				for(int back = 1; back <= back_n; ++back) {
					add_transition(
							page_label,
							generator_constants::to_back_n_string(back),
							generator_constants::to_back_n_string(back),
							*stack_top,
							*stack_top
					);
				}
			}
		}
	}

}

void MyGenerator::createBackTransitions(const vector<const WebPage*>& pages,
		int back_n) {
	FORC(stack_top, pages) {
		int n = (*stack_top)->get_resources().size();
		string stack_top_label = hash(*stack_top, (1<<n)-1, n);
		printf("Back transition from page label: %s\n",
				stack_top_label.c_str());
		for(int back = back_n; back >= 1; --back) {
			if(back == 1 || (*stack_top)->get_id() == start_page_->get_id()) {
				add_transition(
						generator_constants::to_back_n_string(back),
						stack_top_label,
						ioconstants::EMPTY_SYMBOL,
						stack_top_label,
						ioconstants::STACK_POP_TAG
				);
			} else {
				add_transition(
						generator_constants::to_back_n_string(back),
						generator_constants::to_back_n_string(back-1),
						ioconstants::EMPTY_SYMBOL,
						stack_top_label,
						ioconstants::STACK_POP_TAG
				);
			}

		}
	}


}

void MyGenerator::createStates(int back_n) {
	const vector<const WebPage*> pages = web_page_manager_.get_pages();
	FORC(wp, pages) {
		int n = (*wp)->get_resources().size();
		for(int mask = 0; mask < 1<<n; ++mask) {
			string state_label = hash(*wp, mask, n);
			add_state(state_label, mask == (1<<n)-1);
			printf("%s\n", hash(*wp, mask, n).c_str());
		}
	}

	createStartState(pages);

	for(int back = 1; back <= back_n; ++back) {
		add_state(generator_constants::to_back_n_string(back), false);
	}
}

void MyGenerator::createStackSymbols() {
	const vector<const WebPage*> pages = web_page_manager_.get_pages();
	add_stack_symbol(ioconstants::STACK_EMPTY_SYMBOL);
	FORC(wp, pages) {
		int n = (*wp)->get_resources().size();
		add_stack_symbol(hash(*wp, (1<<n)-1, n));
	}
}


namespace generator_constants {
	std::string to_back_n_string(int n) {
		stringstream ss;
		ss << BACK_N << n;
		return ss.str();
	}
}

