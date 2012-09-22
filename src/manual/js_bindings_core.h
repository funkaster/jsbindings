/*
 * JS Bindings: https://github.com/zynga/jsbindings
 *
 * Copyright (c) 2012 Zynga Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#import <objc/runtime.h>
#import "jsapi.h"

#import "cocos2d.h"
#import "chipmunk.h"
#import "SimpleAudioEngine.h"

// Globals
// one shared key for associations
extern char * JSB_association_proxy_key;

/**
 */
@interface JSBCore : NSObject
{
	JSRuntime	*_rt;
	JSContext	*_cx;
	JSObject	*_object;
}

/** return the global context */
@property (nonatomic, readonly) JSRuntime* runtime;

/** return the global context */
@property (nonatomic, readonly) JSContext* globalContext;

/** return the global context */
@property (nonatomic, readonly) JSObject* globalObject;


/** returns the shared instance */
+(JSBCore*) sharedInstance;

/**
 * @param cx
 * @param message
 * @param report
 */
+(void) reportErrorWithContext:(JSContext*)cx message:(NSString*)message report:(JSErrorReport*)report;

/**
 * Log something using CCLog
 * @param cx
 * @param argc
 * @param vp
 */
+(JSBool) logWithContext:(JSContext*)cx argc:(uint32_t)argc vp:(jsval*)vp;

/**
 * run a script from script :)
 */
+(JSBool) executeScriptWithContext:(JSContext*)cx argc:(uint32_t)argc vp:(jsval*)vp;

/**
 * Register an object as a member of the GC's root set, preventing
 * them from being GC'ed
 */
+(JSBool) addRootJSWithContext:(JSContext*)cx argc:(uint32_t)argc vp:(jsval*)vp;

/**
 * removes an object from the GC's root, allowing them to be GC'ed if no
 * longer referenced.
 */
+(JSBool) removeRootJSWithContext:(JSContext*)cx argc:(uint32_t)argc vp:(jsval*)vp;

/**
 * Force a cycle of GC
 * @param cx
 * @param argc
 * @param vp
 */
+(JSBool) forceGCWithContext:(JSContext*)cx argc:(uint32_t)argc vp:(jsval*)vp;

/**
 * will eval the specified string
 * @param string The string with the javascript code to be evaluated
 * @param outVal The jsval that will hold the return value of the evaluation.
 * Can be NULL.
 */
-(BOOL) evalString:(NSString*)string outVal:(jsval*)outVal;

/**
 * will run the specified string
 * @param string The path of the script to be run
 */
-(JSBool) runScript:(NSString*)filename;

@end

#ifdef __cplusplus
extern "C" {
#endif

	enum {
		JSB_C_FLAG_CALL_FREE = 0,
		JSB_C_FLAG_DO_NOT_CALL_FREE =1,
	};

	// structure used by "Object Oriented Functions".
	// handle is a pointer to the native object
	// flags: flags for the object
	struct jsb_c_proxy_s {
		unsigned long flags;	// Should it be removed at "destructor" time, or not ?
		void *handle;			// native object, like cpSpace, cpBody, etc.
		JSObject *jsobj;		// JS Object. Needed for rooting / unrooting
	};
	
	// Functions for setting / removing / getting the proxy used by the "C" Object Oriented API. Think of Chipmunk classes
	struct jsb_c_proxy_s* jsb_get_c_proxy_for_jsobject( JSObject *jsobj );
	void jsb_del_c_proxy_for_jsobject( JSObject *jsobj );
	void jsb_set_c_proxy_for_jsobject( JSObject *jsobj, void *handle, unsigned long flags);

	// JSObject -> proxy
	/** gets a proxy for a given JSObject */
	void* jsb_get_proxy_for_jsobject(JSObject *jsobj);
	/** sets a proxy for a given JSObject */
	void jsb_set_proxy_for_jsobject(void* proxy, JSObject *jsobj);
	/** dels a proxy for a given JSObject */
	void jsb_del_proxy_for_jsobject(JSObject *jsobj);

	// reverse: proxy -> JSObject
	/** gets a JSObject for a given proxy */
	JSObject* jsb_get_jsobject_for_proxy(void *proxy);
	/** sets a JSObject for a given proxy */
	void jsb_set_jsobject_for_proxy(JSObject *jsobj, void* proxy);
	/** delts a JSObject for a given proxy */
	void jsb_del_jsobject_for_proxy(void* proxy);

	JSBool jsb_set_reserved_slot(JSObject *obj, uint32_t idx, jsval value);
	
#ifdef __cplusplus
}
#endif

