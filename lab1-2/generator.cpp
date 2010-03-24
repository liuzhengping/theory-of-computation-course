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

#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>

#include "include/FSAgen.h"
#include "include/labos1_types.h"
#include "include/util.h"

using namespace std;

class MyGenerator : public Generator {
private:
	WebPageManager web_page_manager_;
	string input_filename_;
	static const int LINE_BUFFER_LENGTH = 1024;
public:
	MyGenerator(const string& input_filename);
	string hash(const WebPage* web_page, int load_res_mask, int n);
	void output(const string& output_filename);
private:
	void input(const string& input_file);
};

MyGenerator::MyGenerator(const std::string& input_filename)
: input_filename_(input_filename) {
	input(input_filename);
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
	while ( fgets(buff, sizeof(buff), f) ) {
		buff[strlen(buff)-1] = '\0';
		web_page_manager_.create_page_from_string(buff);
	}
	fclose(f);

	const vector<const WebPage*> pages = web_page_manager_.get_pages();
	FORC(wp, pages) {
		int n = (*wp)->get_resources().size();
		for(int mask = 0; mask < 1<<n; ++mask) {
			add_state(hash(*wp, mask, n), mask == (1<<n)-1);
			printf("%s\n", hash(*wp, mask, n).c_str());
		}
	}
	FORC(wp, pages) {
		int n = (*wp)->get_resources().size();
		for(int mask = 0; mask < 1<<n; ++mask) {
			for(int i = 0; i < n; ++i)
				if(((1<<i) & mask) == 0) {
					int next_mask = mask | (1<<i);
					add_transition(hash(*wp, mask, n), hash(*wp, next_mask, n),
							(*wp)->get_resource(i));
					printf("%s %s %s\n", hash(*wp, mask, n).c_str(),
							hash(*wp, next_mask, n).c_str(),
							(*wp)->get_resource(i).c_str());
				}
			if(mask == (1<<n)-1) {
				FORC(link, (*wp)->get_links()) {
					const WebPage *link_page = web_page_manager_.get_page(*link);
					int n = link_page->get_resources().size();
					add_transition(hash(*wp, mask, (*wp)->get_resources().size()),
							hash(link_page, 0, n),
							web_page_manager_.get_page(*link)->get_url());
				}
			}
		}
	}

	string s = "0:pocetna stranica|";
	for(int i = 0; i < (int)pages.size(); ++i) {
		if(i > 0) s += ", ";
		stringstream ss; ss << pages[i]->get_id();
		s += ss.str();
	}
	s += "|";
//	printf("[%s]\n", s.c_str());
	WebPage start_page(s);
	add_state(start_page.get_url(), true);
	FORC(link, start_page.get_links()) {
		const WebPage *link_page = web_page_manager_.get_page(*link);
		int n = link_page->get_resources().size();
		add_transition(start_page.get_url(), hash(link_page, 0, n),
				web_page_manager_.get_page(*link)->get_url());
	}
}

void MyGenerator::output(const string& output_filename) {

	Generator::output(output_filename);
};


const std::vector<const WebPage*> WebPageManager::get_pages() const {
	vector<const WebPage*> ret;
	FORC(it, pages_)
		ret.push_back(it->second);
	return ret;
}

void WebPageManager::create_page_from_string(std:: string s) {
	const WebPage* wp = new WebPage(s);
	pages_[wp->get_id()] = wp;
}

const WebPage* WebPageManager::get_page(int id) {
	if(pages_.count(id))
		return pages_[id];
	return NULL;
}

/*int main(int argc, char **argv) {
	if(argc <= 2) {
		printf("Usage: (input file) (output file)\n");
		return 1;
	}
	static MyGenerator gen(argv[1]);
	gen.output(argv[2]);
	return 0;
}*/
