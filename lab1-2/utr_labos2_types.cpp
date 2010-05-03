/*
 * utr_labos2_types.cpp
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
 * utr_labos2_types.cpp
 *
 *  Created on: May 3, 2010
 *      Author: Leo Osvald
 */

#include "utr_labos2_types.h"

const std::vector<const WebPage*> WebPageManager::get_pages() const {
	vector<const WebPage*> ret;
	FORC(it, pages_)
		ret.push_back(it->second);
	return ret;
}

const WebPage* WebPageManager::create_page_from_string(std:: string s) {
	const WebPage* wp = new WebPage(s);
	pages_[wp->get_id()] = wp;
	return pages_[wp->get_id()];
}

const WebPage* WebPageManager::get_page(int id) {
	if(pages_.count(id))
		return pages_[id];
	return NULL;
}
