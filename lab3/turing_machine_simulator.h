/*
 * turing_machine_simulator.h
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
 * turing_machine_simulator.h
 *
 *  Created on: Jun 4, 2010
 *      Author: Leo Osvald
 */

#ifndef TURING_MACHINE_SIMULATOR_H_
#define TURING_MACHINE_SIMULATOR_H_

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "turing_machine_base.h"
#include "turing_machine_state.h"
#include "turing_machine_tape.h"
#include "turing_machine_transition.h"
#include "utils.h"
#include "string_utils.h"

namespace turing_machine {

template<typename SymbolType>
class Simulator {
public:
	struct NoSuchStateException : public Exception {
		NoSuchStateException(const std::string& name)
		: Exception("No element with name\""+name+"\"") {
		}
	};
	struct NonDeterminismDetected : public Exception {
		template<typename T>
		NonDeterminismDetected(const Transition<T>& trans)
		: Exception(StringUtils::toString(trans)) {
		}
	};
protected:
	typedef typename Transition<SymbolType>::HeadMoveDirection HeadMoveDirection;
	typedef std::pair<State, SymbolType> TransitionDomain;
	typedef std::pair<State, HeadMoveDirection> PairStateHeadDir;
	struct TransitionCodomain {
		State state;
		SymbolType write_symbol;
		HeadMoveDirection head_move_dir;
		TransitionCodomain(const State& state, const SymbolType& write_symbol,
				const HeadMoveDirection head_move_dir);
	};
private:
	Alphabet<SymbolType> input_alphabet_;
	Alphabet<SymbolType> tape_alphabet_;
	std::map<std::string, State> states_;
	std::map<TransitionDomain, TransitionCodomain> next_;
	State start_state_;
public:
	Simulator();
	const Alphabet<SymbolType>& tape_alphabet() const;
	void set_blank_symbol(SymbolType symbol);
	void addState(const State& state);
	const State& start_state() const;
	const State& state(const std::string& name) const;
	void addTransition(const Transition<SymbolType>& transition);
	bool simulate(Tape<SymbolType>& tape) const;

	std::string transitionsToString() const;
};

template<typename SymbolType>
Simulator<SymbolType>::Simulator() {
	// TODO Auto-generated constructor stub
}

template<typename SymbolType>
void Simulator<SymbolType>::addTransition(const Transition<SymbolType>& transition) {
	bool b = next_.insert(make_pair(TransitionDomain(transition.from, transition.read_symbol),
			TransitionCodomain(transition.to, transition.write_symbol,
					transition.head_move_dir))).second;
	if(!b) {
		throw NonDeterminismDetected(transition);
	}
	tape_alphabet_.add(transition.read_symbol);
	input_alphabet_.add(transition.write_symbol);
}

template<typename SymbolType>
void Simulator<SymbolType>::addState(const State& state) {
	bool was_empty = states_.empty();
	states_[state.name()] = state;
	if(states_.empty() != was_empty)
		start_state_ = state;
}

template<typename SymbolType>
const State& Simulator<SymbolType>::state(const std::string& name) const {
	if(!states_.count(name))
		throw NoSuchStateException(name);
	return states_.at(name);
}

template<typename SymbolType>
bool Simulator<SymbolType>::simulate(Tape<SymbolType>& tape) const {
	using namespace std;
	State state = start_state();
	cout << "Simulacija zapoceta." << endl;
	while(true) {
		cout << "Stanje: " << state << endl;
		cout << tape << endl;
		SymbolType curSymbol = tape.read();

		if(state.isAccept())
			return true;

		TransitionDomain domain(state, curSymbol);
		if(!next_.count(domain)) {
			cout << "Nepoostojeci prijelaz." << endl;
			return false;
		}
		TransitionCodomain codomain = next_.at(domain);
		state = codomain.state;
		tape.write(codomain.write_symbol);
		if(codomain.head_move_dir == Transition<SymbolType>::LEFT) {
			tape.moveLeft();
		}
		else if(codomain.head_move_dir == Transition<SymbolType>::RIGHT) {
			tape.moveRight();
		}

		Transition<SymbolType> t(domain.first, domain.second,
				codomain.state, codomain.write_symbol, codomain.head_move_dir);
		cout << "Primjenjujem prijelaz: " << t << endl;
	}
	return false;
}

template<typename SymbolType>
std::string Simulator<SymbolType>::transitionsToString() const {
	//TODO
	return "";
}

template<typename SymbolType>
Simulator<SymbolType>::TransitionCodomain::TransitionCodomain(const State& state,
		const SymbolType& write_symbol,
		const HeadMoveDirection head_move_dir)
		: state(state), write_symbol(write_symbol), head_move_dir(head_move_dir) {
}

template<typename SymbolType>
const Alphabet<SymbolType>& Simulator<SymbolType>::tape_alphabet() const {
	return tape_alphabet_;
}

template<typename SymbolType>
const State& Simulator<SymbolType>::start_state() const {
	if(states_.empty())
		throw NoSuchStateException("(nema pocetno stanja!)");
	return start_state_;
}

template<typename SymbolType>
void Simulator<SymbolType>::set_blank_symbol(SymbolType symbol) {
	tape_alphabet_.set_blank_symbol(symbol);
}


}

#endif /* TURING_MACHINE_SIMULATOR_H_ */
