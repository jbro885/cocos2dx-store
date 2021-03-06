/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "CCVirtualCurrencyStorage.h"
#include "CCStoreEventDispatcher.h"
#include "CCSoomlaUtils.h"
#include "CCNativeVirtualCurrencyStorage.h"

namespace soomla {
#define TAG "SOOMLA VirtualCurrencyStorage"
    
    USING_NS_CC;
    
    static CCVirtualCurrencyStorage *s_SharedVirtualCurrencyStorage = NULL;
    
    CCVirtualCurrencyStorage *CCVirtualCurrencyStorage::getInstance() {
        if (!s_SharedVirtualCurrencyStorage)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            s_SharedVirtualCurrencyStorage = new CCNativeVirtualCurrencyStorage();
#else
            s_SharedVirtualCurrencyStorage = new CCVirtualCurrencyStorage();
#endif
//            s_SharedVirtualCurrencyStorage->retain();
        }
        return s_SharedVirtualCurrencyStorage;
    }
    
    CCVirtualCurrencyStorage::CCVirtualCurrencyStorage() {
        
    }
    
    CCVirtualCurrencyStorage::~CCVirtualCurrencyStorage() {
        
    }
    
    const char* CCVirtualCurrencyStorage::keyBalance(const char *itemId) const {
        return keyCurrencyBalance(itemId);
    }
    
    void CCVirtualCurrencyStorage::postBalanceChangeEvent(CCVirtualItem *item, int balance, int amountAdded) {
        CCVirtualCurrency *virtualCurrency = dynamic_cast<CCVirtualCurrency *>(item);
        if (virtualCurrency == NULL) {
            CCSoomlaUtils::logError(TAG, __String::createWithFormat("Trying to post currency balance changed with a non VirtualCurrency item %s", item->getId()->getCString())->getCString());
            return;
        }
        
        CCStoreEventDispatcher::getInstance()->onCurrencyBalanceChanged(virtualCurrency, balance, amountAdded);
    }

    const char *CCVirtualCurrencyStorage::keyCurrencyBalance(const char *itemId) {
        return __String::createWithFormat("currency.%s.balance", itemId)->getCString();
    }
}