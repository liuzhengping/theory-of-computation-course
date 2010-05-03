/*
 * ioconstants.h
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
 * ioconstants.h
 *
 *  Created on: May 2, 2010
 *      Author: Leo Osvald
 */

#ifndef IOCONSTANTS_H_
#define IOCONSTANTS_H_



namespace ioconstants {

static const char STACK_ALPHABET_DEFINITION_TAG = '@';
static const char *STACK_EMPTY_SYMBOL = "$";
static const char STACK_PUSH_TAG = '+';
static const char *STACK_POP_TAG = "-";
static const int STACK_POP_SYMBOL_ID = -1;

static const char STATE_DEFINITION_TAG = '#';
static const char STATE_ACCEPT_PREFIX = '$';

static const char START_PAGE_TAG = '!';

static const char *EMPTY_SYMBOL = "$";

static const char COMMENT_TAG = '%';

}

#endif /* IOCONSTANTS_H_ */
