/*
 * DFA.h
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

#ifndef DFA_H_INCLUDED
#define DFA_H_INCLUDED

#include "FSA.h"

/**
 * Deterministicki konacni automat (Deterministic Finite Automaton)
 */
template<typename T, typename InputType>
class DFA : public FSA<T, InputType, std::pair<InputType, State<T>*>, State<T>* > {
public:
	typedef std::pair<InputType, State<T>*> DFATransitionDomain;
	typedef State<T>* DFATransitionCodomain;
	typedef InputType DFAInputType;

private:
    State<T>* cur_state_;

public:
    const State<T>* getCurrentState() const {
        return cur_state_;
    }

    virtual bool process(const InputType& input) {
    	if(this->current_state() == NULL) {
    		fprintf(stderr, "Upozorenje: prijelaz iz nepostojeceg stanja!\n");
    		return false;
    	}
    	DFATransitionDomain from = DFATransitionDomain(input, current_state());
    	DFATransitionCodomain next = get_next(from);
    	set_current_state(getCodomainState(next));
    	transition_occured(from, next, input);
    	return next != noCodomain();
    }

    virtual void reset() = 0;

    virtual void transition_occured(const DFATransitionDomain& domain,
    		const DFATransitionCodomain& codomain,
    		const InputType& input) const = 0;

    State<T>* getDomainState(DFATransitionDomain transition_domain) const {
    	return transition_domain.second;
    }

    State<T>* getCodomainState(DFATransitionCodomain transition_codomain) const {
    	return transition_codomain;
    }

    virtual State<T>* noCodomain() const {
    	return NULL;
    }

protected:
    void set_current_state(State<T>* state) {
    	cur_state_ = state;
    }

    State<T>* current_state() const {
    	return cur_state_;
    }
};

#endif // DFA_H_INCLUDED
