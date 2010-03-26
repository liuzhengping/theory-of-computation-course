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
 *
 */
template<typename T>
class DFA : public FSA<T> {
private:
    State<T>* cur_state_;

public:
    const State<T>& current_state() const {
        return *cur_state_;
    }

    virtual bool process(const std::string& input) {
    	cur_state_ = cur_state_->get_next(input);
        return cur_state_ != NULL;
    }
protected:
    void set_current_state(State<T>& state) {
    	cur_state_ = &state;
    }

};

#endif // DFA_H_INCLUDED
