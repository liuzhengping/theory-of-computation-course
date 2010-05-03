/*
 * labos1_types.h
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

#ifndef LABOS1_TYPES_H_INCLUDED
#define LABOS1_TYPES_H_INCLUDED

#include "ioconstants.h"

#include "include/util.h"
#include "include/string_utils.h"

#include <cstdlib>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <iostream>

using namespace std;

class WebPage {
private:
	int id_;
	string url_;
	vector<int> links_;
	vector<string> res_;

public:
	WebPage(const string& unparsed_string) {
		parse(unparsed_string);
	}

	int get_id() const {
		return id_;
	}

	const string& get_url() const {
		return url_;
	}

	int get_link(int i) {
		return links_[i];
	}

	string get_resource(int i) const {
		return res_[i];
	}

	const vector<int>& get_links() const {
		return links_;
	}

	const vector<string>& get_resources() const {
		return res_;
	}

private:
	void parse(const string& s) {
		int colon_pos = -1, pipe_pos = -1;
		for ( int i = 0; i < (int)s.size(); ++i ) {
			if ( s[i] == ':' ) {
				id_ = atoi(s.substr(0, i).c_str());
				colon_pos = i;
			} else if ( s[i] == '|') {
				if ( pipe_pos == -1 ) {
					url_ = string(s.begin()+colon_pos+1, s.begin()+i);
					pipe_pos = i;
				} else {
					vector<string> l = StringUtils::split(string(s.begin()+pipe_pos+1, s.begin()+i), ", ");
					res_ = StringUtils::split(string(s.begin()+i+1, s.end()), ", ");
					for(int i = 0; i < (int)l.size(); ++i)
						links_.push_back(atoi(l[i].c_str()));
					break;
				}
			}
		}
	}



	friend bool operator<(const WebPage& A, const WebPage& B) {
		return A.id_ < B.id_;
	}

	friend bool operator==(const WebPage& A, const WebPage& B) {
		return A.id_ == B.id_;
	}

	friend ostream& operator<<(ostream& os, const WebPage& wp) {
		os << "id=" << wp.id_ << "; url=" << wp.url_
				<< "; links=" << StringUtils::to_string(wp.links_.begin(), wp.links_.end())
				<< "; res=" << StringUtils::to_string(wp.res_.begin(), wp.res_.end());
		return os;
	}

	friend ostream& operator<<(ostream& os, WebPage* wp) {
		return operator<<(os, *wp);
	}

};

class WebPageManager {
private:
	std::map<int, const WebPage*> pages_;
public:
	const WebPage* create_page_from_string(std::string s);
	const std::vector<const WebPage*> get_pages() const;
	const WebPage* get_page(int id);
};

class ResourceManager {
private:
	vector<string> res_;
	map<string, int> m_;
	int cnt_;
public:
	ResourceManager() : cnt_(0) { }
	void add_resource(const string& resource) {
		if (!m_.count(resource)) {
			res_.push_back(resource);
			m_.insert(make_pair(resource, cnt_++));
		}
	}
	int get_id(string resource) {
		return m_[resource];
	}
	string get_res(int id) {
		return res_[id];
	}
};




#endif // LABOS1_TYPES_H_INCLUDED
