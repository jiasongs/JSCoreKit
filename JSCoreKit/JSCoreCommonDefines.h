//
//  JSCommonDefines.h
//  JSCoreKit
//
//  Created by jiasong on 2020/8/24.
//  Copyright © 2020 jiasong. All rights reserved.
//

#ifndef JSCommonDefines_h
#define JSCommonDefines_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <os/lock.h>
#import <objc/runtime.h>
#import "JSCoreWeakProxy.h"
#import "NSNumber+JSCore.h"

#pragma mark - Clang

#define JSArgumentToString(macro) #macro
#define JSClangWarningConcat(warning_name) JSArgumentToString(clang diagnostic ignored warning_name)
#define JSBeginIgnoreClangWarning(warningName) _Pragma("clang diagnostic push") _Pragma(JSClangWarningConcat(#warningName))
#define JSEndIgnoreClangWarning _Pragma("clang diagnostic pop")
#define JSBeginIgnorePerformSelectorLeaksWarning JSBeginIgnoreClangWarning(-Warc-performSelector-leaks)
#define JSEndIgnorePerformSelectorLeaksWarning JSEndIgnoreClangWarning
#define JSBeginIgnoreDeprecatedWarning JSBeginIgnoreClangWarning(-Wdeprecated-declarations)
#define JSEndIgnoreDeprecatedWarning JSEndIgnoreClangWarning

#pragma mark - Synthesize

#define _JSSynthesizeId(_getterName, _setterName, _policy) \
_Pragma("clang diagnostic push") _Pragma(JSClangWarningConcat("-Wmismatched-parameter-types")) _Pragma(JSClangWarningConcat("-Wmismatched-return-types"))\
static char kAssociatedObjectKey_##_getterName;\
- (void)_setterName:(id)_getterName {\
objc_setAssociatedObject(self, &kAssociatedObjectKey_##_getterName, _getterName, OBJC_ASSOCIATION_##_policy##_NONATOMIC);\
}\
\
- (id)_getterName {\
return objc_getAssociatedObject(self, &kAssociatedObjectKey_##_getterName);\
}\
_Pragma("clang diagnostic pop")

#define _JSSynthesizeWeakId(_getterName, _setterName) \
_Pragma("clang diagnostic push") _Pragma(JSClangWarningConcat("-Wmismatched-parameter-types")) _Pragma(JSClangWarningConcat("-Wmismatched-return-types"))\
static char kAssociatedObjectKey_##_getterName;\
- (void)_setterName:(id)_getterName {\
objc_setAssociatedObject(self, &kAssociatedObjectKey_##_getterName, [[JSCoreWeakProxy alloc] initWithTarget:_getterName], OBJC_ASSOCIATION_RETAIN_NONATOMIC);\
}\
\
- (id)_getterName {\
return ((JSCoreWeakProxy *)objc_getAssociatedObject(self, &kAssociatedObjectKey_##_getterName)).target;\
}\
_Pragma("clang diagnostic pop")

#define _JSSynthesizeNonObject(_getterName, _setterName, _type, valueInitializer, valueGetter) \
_Pragma("clang diagnostic push") _Pragma(JSClangWarningConcat("-Wmismatched-parameter-types")) _Pragma(JSClangWarningConcat("-Wmismatched-return-types"))\
static char kAssociatedObjectKey_##_getterName;\
- (void)_setterName:(_type)_getterName {\
objc_setAssociatedObject(self, &kAssociatedObjectKey_##_getterName, [NSNumber valueInitializer:_getterName], OBJC_ASSOCIATION_RETAIN_NONATOMIC);\
}\
\
- (_type)_getterName {\
return [((NSNumber *)objc_getAssociatedObject(self, &kAssociatedObjectKey_##_getterName)) valueGetter];\
}\
_Pragma("clang diagnostic pop")

/// @property (nonatomic, strong) id xxx
#define JSSynthesizeIdStrongProperty(_getterName, _setterName) _JSSynthesizeId(_getterName, _setterName, RETAIN)

/// @property (nonatomic, weak) id xxx
#define JSSynthesizeIdWeakProperty(_getterName, _setterName) _JSSynthesizeWeakId(_getterName, _setterName)

/// @property (nonatomic, copy) id xxx
#define JSSynthesizeIdCopyProperty(_getterName, _setterName) _JSSynthesizeId(_getterName, _setterName, COPY)

#pragma mark - NonObject Marcos

/// @property (nonatomic, assign) Int xxx
#define JSSynthesizeIntProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, int, numberWithInt, intValue)

/// @property (nonatomic, assign) unsigned int xxx
#define JSSynthesizeUnsignedIntProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, unsigned int, numberWithUnsignedInt, unsignedIntValue)

/// @property (nonatomic, assign) float xxx
#define JSSynthesizeFloatProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, float, numberWithFloat, floatValue)

/// @property (nonatomic, assign) double xxx
#define JSSynthesizeDoubleProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, double, numberWithDouble, doubleValue)

/// @property (nonatomic, assign) BOOL xxx
#define JSSynthesizeBOOLProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, BOOL, numberWithBool, boolValue)

/// @property (nonatomic, assign) NSInteger xxx
#define JSSynthesizeNSIntegerProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, NSInteger, numberWithInteger, integerValue)

/// @property (nonatomic, assign) NSUInteger xxx
#define JSSynthesizeNSUIntegerProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, NSUInteger, numberWithUnsignedInteger, unsignedIntegerValue)

/// @property (nonatomic, assign) CGFloat xxx
#define JSSynthesizeCGFloatProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, CGFloat, numberWithDouble, js_CGFloatValue)

/// @property (nonatomic, assign) CGPoint xxx
#define JSSynthesizeCGPointProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, CGPoint, valueWithCGPoint, CGPointValue)

/// @property (nonatomic, assign) CGSize xxx
#define JSSynthesizeCGSizeProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, CGSize, valueWithCGSize, CGSizeValue)

/// @property (nonatomic, assign) CGRect xxx
#define JSSynthesizeCGRectProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, CGRect, valueWithCGRect, CGRectValue)

/// @property (nonatomic, assign) UIEdgeInsets xxx
#define JSSynthesizeUIEdgeInsetsProperty(_getterName, _setterName) _JSSynthesizeNonObject(_getterName, _setterName, UIEdgeInsets, valueWithUIEdgeInsets, UIEdgeInsetsValue)

#pragma mark - Lock

#ifndef JSLockDeclare
#define JSLockDeclare(lock) os_unfair_lock lock;
#endif

#ifndef JSLockInit
#define JSLockInit(lock) lock = OS_UNFAIR_LOCK_INIT;
#endif

#ifndef JSLockAdd
#define JSLockAdd(lock) os_unfair_lock_lock(&lock);
#endif

#ifndef JSLockRemove
#define JSLockRemove(lock) os_unfair_lock_unlock(&lock);
#endif

#pragma mark - CGFloat

CG_INLINE CGFloat
JSCGRemoveFloatMin(CGFloat floatValue) {
    return floatValue == CGFLOAT_MIN ? 0 : floatValue;
}

CG_INLINE CGFloat
JSCGFlatSpecificScale(CGFloat floatValue, CGFloat scale) {
    floatValue = JSCGRemoveFloatMin(floatValue);
    scale = scale ? : UIScreen.mainScreen.scale;
    CGFloat JSFlattedValue = ceil(floatValue * scale) / scale;
    return JSFlattedValue;
}

CG_INLINE CGFloat
JSCGFlat(CGFloat floatValue) {
    return JSCGFlatSpecificScale(floatValue, 0);
}

#pragma mark - Runtime

CG_INLINE BOOL
JSRuntimeHasOverrideSuperclassMethod(Class targetClass, SEL targetSelector) {
    Method method = class_getInstanceMethod(targetClass, targetSelector);
    if (!method) return NO;
    
    Method methodOfSuperclass = class_getInstanceMethod(class_getSuperclass(targetClass), targetSelector);
    if (!methodOfSuperclass) return YES;
    
    return method != methodOfSuperclass;
}

CG_INLINE BOOL
JSRuntimeOverrideImplementation(Class targetClass, SEL targetSelector, id (^implementationBlock)(__unsafe_unretained Class originClass, SEL originCMD, IMP (^originalIMPProvider)(void))) {
    Method originMethod = class_getInstanceMethod(targetClass, targetSelector);
    IMP imp = method_getImplementation(originMethod);
    BOOL hasOverride = JSRuntimeHasOverrideSuperclassMethod(targetClass, targetSelector);
    
    // 以 block 的方式达到实时获取初始方法的 IMP 的目的，从而避免先 swizzle 了 subclass 的方法，再 swizzle superclass 的方法，会发现前者调用时不会触发后者 swizzle 后的版本的 bug。
    IMP (^originalIMPProvider)(void) = ^IMP(void) {
        IMP result = NULL;
        if (hasOverride) {
            result = imp;
        } else {
            // 如果 superclass 里依然没有实现，则会返回一个 objc_msgForward 从而触发消息转发的流程
            // https://github.com/Tencent/JS_iOS/issues/776
            Class superclass = class_getSuperclass(targetClass);
            result = class_getMethodImplementation(superclass, targetSelector);
        }
        
        // 这只是一个保底，这里要返回一个空 block 保证非 nil，才能避免用小括号语法调用 block 时 crash
        // 空 block 虽然没有参数列表，但在业务那边被转换成 IMP 后就算传多个参数进来也不会 crash
        if (!result) {
            result = imp_implementationWithBlock(^(id selfObject){
                
            });
        }
        
        return result;
    };
    
    if (hasOverride) {
        method_setImplementation(originMethod, imp_implementationWithBlock(implementationBlock(targetClass, targetSelector, originalIMPProvider)));
    } else {
        NSMethodSignature *signature = [targetClass instanceMethodSignatureForSelector:targetSelector];
        JSBeginIgnorePerformSelectorLeaksWarning
        NSString *typeString = [signature performSelector:NSSelectorFromString([NSString stringWithFormat:@"_%@String", @"type"])];
        JSEndIgnorePerformSelectorLeaksWarning
        const char *typeEncoding = method_getTypeEncoding(originMethod) ?: typeString.UTF8String;
        class_addMethod(targetClass, targetSelector, imp_implementationWithBlock(implementationBlock(targetClass, targetSelector, originalIMPProvider)), typeEncoding);
    }
    
    return YES;
}

#endif /* JSCommonDefines_h */