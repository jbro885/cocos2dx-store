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

#include "CCSoomlaStore.h"
#include "CCStoreInfo.h"
#include "CCSoomlaUtils.h"
#include "CCNativeSoomlaStore.h"
#include "CCStoreEventDispatcher.h"

namespace soomla {
    #define TAG "SOOMLA SoomlaStore"

    USING_NS_CC;

    static CCSoomlaStore *s_SharedSoomlaStore = NULL;
    
    bool CCSoomlaStore::initialized = false;

    CCSoomlaStore *CCSoomlaStore::getInstance() {
        if (!s_SharedSoomlaStore)
        {
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            s_SharedSoomlaStore = new CCSoomlaStore();
            #else
            s_SharedSoomlaStore = new CCNativeSoomlaStore();
            #endif
            s_SharedSoomlaStore->retain();
        }
        return s_SharedSoomlaStore;
    }
    
    void CCSoomlaStore::initialize(CCStoreAssets *storeAssets) {
        
        if (initialized) {
            const char *err = "SoomlaStore is already initialized. You can't initialize it twice!";
            CCStoreEventDispatcher::getInstance()->onUnexpectedErrorInStore(__String::create(err), true);
            CCSoomlaUtils::logError(TAG, err);
            return;
        }
        
        CCSoomlaUtils::logDebug(TAG, "CCSoomlaStore Initializing...");
        
        getInstance()->loadBillingService();
        
        CCStoreInfo::createShared(storeAssets);
        
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // On iOS we only refresh market items
        CCError *error = NULL;
        getInstance()->refreshMarketItemsDetails(&error);
        #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        // On Android we refresh market items and restore transactions
        getInstance()->refreshInventory();
        #endif
        
        initialized = true;
        CCStoreEventDispatcher::getInstance()->onSoomlaStoreInitialized(true);
    }
    
    void CCSoomlaStore::buyMarketItem(const char *productId, const char *payload, CCError **error) {
        CCPurchasableVirtualItem *item = CCStoreInfo::sharedStoreInfo()->getPurchasableItemWithProductId(productId, error);
        if (item == NULL) {
            return;
        }
        
        item->give(1);
    }
}