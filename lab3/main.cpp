/*
 * main.cpp
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
 * main.cpp
 *
 *  Created on: Jun 4, 2010
 *      Author: Leo Osvald
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>

#include "utils.h"
#include "string_utils.h"
#include "turing_machine_tape.h"
#include "turing_machine_simulator.h"
#include "turing_machine_state.h"

using namespace std;

const char COMMENT_TAG = '#';

void exit_error(const char* filename, int line, const char* msg) {
	fprintf(stderr, "Greska: %s - linija #%d: %s\n", filename, line, msg);
	exit(EXIT_FAILURE);
}

turing_machine::Tape<char> loadTape(const char* filename,
		const turing_machine::Alphabet<char>& alphabet) {
	static char buff[4096];
	FILE *f = fopen(filename, "r");
	if(f == NULL) {
		fprintf(stderr, "Greska: Ne postoji ulazna datoteka: \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}
	if(!fgets(buff, sizeof(buff), f)) {
		fprintf(stderr, "Greska: Nevaljana datoteka s pocetnim stanjem trake\n");
		exit(EXIT_FAILURE);
	}
	buff[strlen(buff)-1] = '\0';
	std::string s(buff);
	return turing_machine::Tape<char>(alphabet, s.begin(), s.end());
}

void loadTuringMachine(turing_machine::Simulator<char>& sim, const char* filename) {
	static char buff[1024];
	static char STATE_DECLARATION_TAG = '$';
	static char STATE_ACCEPT_PREFIX = '!';
	static char TRANSITION_DECLARATION_TAG = '@';
	static char BLANK_DECLARATION_PREFIX = '?';

	using namespace turing_machine;

	FILE *f = fopen(filename, "r");
	if(f == NULL) {
		fprintf(stderr, "Greska: Ne postoji ulazna datoteka: \"%s\"\n", filename);
		exit(EXIT_FAILURE);
	}
	for(int line = 1; fgets(buff, sizeof(buff), f); ++line) {
		int len = strlen(buff);
		buff[--len] = '\0';
		if(len == 0 || buff[0] == COMMENT_TAG)
			continue;
		if(buff[0] == STATE_DECLARATION_TAG) {
			char *name = buff+1;
			State::StateType type = State::NON_FINAL;
			if(len > 1 && *name == STATE_ACCEPT_PREFIX) {
				++name;
				type = State::YES;
			}
			sim.addState(State(name, type));
			cerr << "Dodao stanje: " << sim.state(name);
			cerr << " (pocetno = " << (sim.state(name) == sim.start_state());
			cerr << "; prihvatljivo = " << sim.state(name).isAccept() << ")\n";
		}
		else if(buff[0] == TRANSITION_DECLARATION_TAG) {
			vector<string> tokens = StringUtils::split(buff+1, " \t");
			if(tokens.size() < 5) {
				exit_error(filename, line, "Nevaljan prijelaz");
			}
//			cout << "Tokens: " << StringUtils::to_string(tokens.begin(), tokens.end()) << endl;
			try {
				const turing_machine::State& from = sim.state(tokens[0]);
				char read_symbol = tokens[1][0];
				const turing_machine::State& to = sim.state(tokens[2]);
				char write_symbol = tokens[3][0];
				char head_move_dir_char = tokens[4][0];
				turing_machine::Transition<char>::HeadMoveDirection head_move_dir;
				if(head_move_dir_char == 'L')
					head_move_dir = turing_machine::Transition<char>::LEFT;
				else if(head_move_dir_char == 'R')
					head_move_dir = turing_machine::Transition<char>::RIGHT;
				else if(head_move_dir_char == 'H')
					head_move_dir = turing_machine::Transition<char>::HOLD;
				else {
					exit_error(filename, line,
							"Pomak glave mora biti 'L', 'R' or 'H'");
				}

				turing_machine::Transition<char> tran(from, read_symbol,
						to, write_symbol, head_move_dir);
				try {
					sim.addTransition(tran);
					cerr << "Dodan prijelaz: " << tran << endl;
				} catch(const turing_machine::Simulator<char>::NonDeterminismDetected& ex) {
					exit_error(filename, line, "Nedeterministicki prijelaz");
				}
			}
			catch(turing_machine::Simulator<char>::NoSuchStateException) {
				exit_error(filename, line, "Nepostojece stanje\n");
			}
		}
		else if(buff[0] == BLANK_DECLARATION_PREFIX) {
			sim.set_blank_symbol(buff[1]);
			cerr << "Postavljen \"blank\" znak: " << buff[1] << endl;
		}
	}
	cerr << "Znakovi trake: " << endl;
	cerr << sim.tape_alphabet() << endl;
}

int main(int argc, char **argv) {
	if(argc <= 2) {
		fprintf(stderr, "Argumenti: (dat-def-turingovog-stroja) \
(dat-pocetno-stanje-trake)\n");
		return EXIT_FAILURE;
	}

	static turing_machine::Simulator<char> sim;
	loadTuringMachine(sim, argv[1]);

	try {
		static turing_machine::Tape<char> tape = loadTape(argv[2],
				sim.tape_alphabet());

		bool b = sim.simulate(tape);
		printf("%s\n", (b ? "DA" : "NE"));
	} catch(const std::exception& ex) {
		fprintf(stderr, "Greska u izvodjenju: %s\n",
				ex.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

