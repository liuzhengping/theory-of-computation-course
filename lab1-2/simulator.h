/*
 * simulator.h
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
 * simulator.h
 *
 *  Created on: May 3, 2010
 *      Author: Leo Osvald
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_


#include "utr_labos2_types.h"
#include "ioconstants.h"

#include "include/FSAbuilder.h"
#include "include/util.h"
#include "include/tuple.h"
#include "include/pushdown_automaton.h"

#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class MyPA : public PA<void*, std::string, std::string> {
private:
	bool print_on_;
	mutable vector< Tuple<string, string, string, string> > print_stack_;
public:
	MyPA() : PA<void*, std::string, std::string>(ioconstants::STACK_EMPTY_SYMBOL,
			ioconstants::STACK_POP_TAG, ioconstants::STACK_EMPTY_SYMBOL) {
		print_on_ = 1;
	}
	bool build_from_file(const std::string& definition_filename);
	bool process(const std::string& input);
	void sequencePartProcessed(const std::string& sequence_part) const;
	void transition_occured(const PATransitionDomain& domain,
	    		const PATransitionCodomain& codomain,
	    		const std::string& input) const;
};

class MySimulator {
private:
	static const int MAX_BUFF = 1<<16;
	MyPA* pa_;

public:

	MySimulator(MyPA* pa) {
		pa_ = pa;
	}

	bool simulate(const std::vector<std::string>& input_sequence) {
		pa_->reset();
		return pa_->process_sequence(input_sequence);
	}

	bool simulate_from_file(const std::string& input_filename) {

		static const string DELIMITERS = " ";
		static char buff[MAX_BUFF];
		FILE *f = fopen(input_filename.c_str(), "r");
		if(f == NULL) {
			fprintf(stderr, "Ne postoji datoteka \"%s\".",
					input_filename.c_str());
			return false;
		}
		printf("Simulacija zapocela\n");
		while(fgets(buff, sizeof(buff), f)) {
			buff[strlen(buff)-1] = '\0';
			if(strlen(buff) && buff[0] == ioconstants::COMMENT_TAG)
				continue;
			printf("\nSimuliram za ulazni niz: %s\n", buff);
			const string& s = buff;
			const vector<string>& tokens = StringUtils::split(s, DELIMITERS);

			if(simulate(tokens)) {
				printf("Niz \"%s\" se prihvaca\n", buff);
			}
			else {
				printf("Niz \"%s\" se ne prihvaca\n", buff);
			}
			printf("-------------------\n");

//			printf("%d", simulate(tokens));
		}
		fclose(f);
		printf("Simulation zavrsena\n");
		return true;
	}
protected:
	void init() {
	}
};

#endif /* SIMULATOR_H_ */
