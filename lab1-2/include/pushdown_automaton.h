/*
 * pushdown_automaton.h
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
 * PA.h
 *
 *  Created on: May 1, 2010
 *      Author: Leo Osvald
 */

#ifndef PUSHDOWN_AUTOMATON_H_
#define PUSHDOWN_AUTOMATON_H_

#include "dfa.h"
#include "triple.h"
#include "util.h"

#include <vector>
#include <cstdio>


template<typename T, typename InputType, typename StackTopType>
class PA : public FSA<T, InputType,
Triple<InputType, State<T>*, StackTopType>,
std::pair<State<T>*, std::pair<StackTopType, bool> > > {
//Triple<State<T>*, StackTopType, bool> > {

public:
	typedef class Triple<InputType, State<T>*, StackTopType> PATransitionDomain;
	typedef typename std::pair<State<T>*,
	std::pair<StackTopType, bool> > PATransitionCodomain;
//	typedef class Triple<State<T>*, StackTopType, bool> PATransitionCodomain;
private:
	const StackTopType empty_stack_symbol_;
	const StackTopType stack_pop_symbol_;
	const InputType empty_simbol_;
	State<T>* cur_state_;
	std::vector<StackTopType> stack_;
public:

	PA(StackTopType empty_stack_symbol, StackTopType stack_pop_symbol,
			InputType empty_symbol) : empty_stack_symbol_(empty_stack_symbol),
			stack_pop_symbol_(stack_pop_symbol), empty_simbol_(empty_symbol) {
		init();
	}

	const State<T>* getCurrentState() const {
		return cur_state_;
	}

	StackTopType stackTop() const {
		return stack_.empty() ? this->empty_stack_symbol() : stack_.back();
	}

	const std::vector<StackTopType>& stackContents() const {
		return stack_;
	}

	virtual bool process(const InputType& input) {
		if(this->current_state() == NULL) {
			fprintf(stderr, "Upozorenje: prijelaz iz nepostojeceg stanja!\n");
			return false;
		}

		PATransitionDomain domain = PATransitionDomain(input,
				this->current_state(), stackTop());
		PATransitionCodomain next = get_next(domain);

		if(!next.second.second) {
			if(!stack_.empty()) {
				stack_.pop_back();
			}
			else {
				fprintf(stderr, "Upozorenje: skidanje s praznog stoga!\n");
			}
		}

		set_current_state(getCodomainState(next));
		if(next.second.first != stack_pop_symbol_)
			stack_.push_back(next.second.first);

		transition_occured(domain, next, input);

		if(next != noCodomain()) {
			domain = PATransitionDomain(empty_simbol_,
					this->current_state(), stackTop());
			if(get_next(domain) != noCodomain())
				return process(empty_simbol_);
		}

		return next != noCodomain();
	}

	virtual void sequencePartProcessed(const InputType& sequence_part) const {
	}

	virtual void reset() {
		this->set_current_state(this->start_state());
		this->initStack();
	}

	virtual State<T>* getDomainState(
			PATransitionDomain transition_domain) const {
		return transition_domain.second;
	}

	virtual State<T>* getCodomainState(
			PATransitionCodomain transition_codomain) const {
		return transition_codomain.first;
	}

	virtual PATransitionCodomain noCodomain() const {
		return PATransitionCodomain(NULL, make_pair(empty_stack_symbol(), false));
	}

	virtual StackTopType empty_stack_symbol() const {
		return empty_stack_symbol_;
	}

protected:

	void initStack() {
		clearStack();
		stack_.push_back(empty_stack_symbol());
	}

	void clearStack() {
		stack_.clear();
	}

	void set_current_state(State<T>* state) {
		cur_state_ = state;
	}

	State<T>* current_state() const {
		return cur_state_;
	}

	virtual void transition_occured(const PATransitionDomain& domain,
			const PATransitionCodomain& codomain,
			const InputType& input) const {

	}

	void transition_added(const PATransitionDomain& from,
			const PATransitionCodomain& to) const {
	}

private:
	void init() {
		initStack();
	}

};


#endif /* PUSHDOWN_AUTOMATON_H_ */
