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

#include "DFA.h"
#include "triple.h"
#include <vector>
#include <cstdio>

template<typename T, typename InputType, typename StackTopType>
class PA : public FSA<T, InputType,
Triple<InputType, State<T>*, StackTopType>,
//std::pair<std::pair<InputType, StackTopType>, State<T>*>,
std::pair<State<T>*, StackTopType> > {


//	typedef typename std::pair<InputType, StackTopType> PAInputType;
	typedef class Triple<InputType, State<T>*, StackTopType> PATransitionDomain;
//	typedef typename std::pair<PAInputType, State<T>*> PATransitionDomain;
	typedef typename std::pair<State<T>*, StackTopType> PATransitionCodomain;
private:
	StackTopType empty_stack_symbol_;
	State<T>* cur_state_;
	std::vector<StackTopType> stack_;
public:

	PA(StackTopType empty_stack_symbol) : empty_stack_symbol_(empty_stack_symbol) {
		init();
	}

	const State<T>* getCurrentState() const {
		return cur_state_;
	}

	StackTopType stackTop() const {
		return stack_.empty() ? this->empty_stack_symbol() : stack_.back();
	}

	const std::vector<StackTopType>& stackContents() const {
	}

	virtual bool process(const InputType& input) {
		if(this->current_state() == NULL) {
			fprintf(stderr, "Upozorenje: prijelaz iz nepostojeceg stanja!\n");
			return false;
		}
		StackTopType top = stackTop();
		PATransitionDomain domain = PATransitionDomain(input,
				this->current_state(), top);
		if(!stack_.empty()) {
			stack_.pop_back();
		}
		else {
			fprintf(stderr, "Upozorenje: skidanje s praznog stoga njega!\n");
		}

		PATransitionCodomain next = get_next(domain);
		set_current_state(getCodomainState(next));
		top = stackTop();
		if(top != this->empty_stack_symbol()) {
			stack_.push_back(top);
		}

		transition_occured(domain, next, input);
		return next != noCodomain();
	}

	virtual void reset() {
		//TODO
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
		return PATransitionCodomain(NULL, empty_stack_symbol());
	}

	virtual StackTopType empty_stack_symbol() const {
		return empty_stack_symbol_;
	}

protected:

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

private:
	void init() {
		stack_.push_back(empty_stack_symbol());
	}

};


#endif /* PUSHDOWN_AUTOMATON_H_ */
