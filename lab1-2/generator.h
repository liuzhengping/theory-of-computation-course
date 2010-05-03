/*
 * generator.h
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
/*
 * generator.h
 *
 *  Created on: May 1, 2010
 *      Author: Leo Osvald
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "PAgen.h"
#include "include/util.h"

#include "utr_labos2_types.h"

using namespace std;

class MyGenerator : public Generator {
private:
	WebPageManager web_page_manager_;
	const WebPage* start_page_;
	string input_filename_;
	static const int LINE_BUFFER_LENGTH = 1024;
public:
	MyGenerator(const string& input_filename);
	string hash(const WebPage* web_page, int load_res_mask, int n);
	void output(const string& output_filename);
private:
	void input(const string& input_file);
	std::string toCompoundLabel(const string& label1, const string& label2);
	std::string createStartPageString(const vector<const WebPage*>& pages);
	const WebPage* createStartState(const vector<const WebPage*>& pages);
	void createStartPageTransitions();
	void createRegularWebPageTransitions(const std::vector<const WebPage*>& pages,
			int back_n);
	void createBackTransitions(const std::vector<const WebPage*>& pages, int back_n);
	void createStates(int back_n);
	void createStackSymbols();
};

namespace generator_constants {
	static const int NO_BACK = 0;
	static const char* BACK_N = "BACK_";

	std::string to_back_n_string(int n);
}


#endif /* GENERATOR_H_ */
