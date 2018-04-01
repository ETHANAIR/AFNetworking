// AFSecurityPolicy.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
#import <Security/Security.h>

typedef NS_ENUM(NSUInteger, AFSSLPinningMode) {
    AFSSLPinningModeNone,
    AFSSLPinningModePublicKey,
    AFSSLPinningModeCertificate,
};

/**
 `AFSecurityPolicy` evaluates server trust against pinned X.509 certificates and public keys over secure connections.
 `AFSecurityPolicy` 通过安全连接鉴定服务器信任与设置X.509证书和公钥的关系。

 Adding pinned SSL certificates to your app helps prevent man-in-the-middle attacks and other vulnerabilities. Applications dealing with sensitive customer data or financial information are strongly encouraged to route all communication over an HTTPS connection with SSL pinning configured and enabled.
 */

NS_ASSUME_NONNULL_BEGIN

@interface AFSecurityPolicy : NSObject <NSSecureCoding, NSCopying>

/**
 The criteria by which server trust should be evaluated against the pinned SSL certificates. Defaults to `AFSSLPinningModeNone`.
 服务器信任应根据标准鉴定的SSL证书进行评估
 注意这里的这个属性是只读的
 */
@property (readonly, nonatomic, assign) AFSSLPinningMode SSLPinningMode;

/**
 The certificates used to evaluate server trust according to the SSL pinning mode.
 证书通常通过设置SSL模式来鉴定服务器信任

  By default, this property is set to any (`.cer`) certificates included in the target compiling AFNetworking. Note that if you are using AFNetworking as embedded framework, no certificates will be pinned by default. Use `certificatesInBundle` to load certificates from your target, and then create a new policy by calling `policyWithPinningMode:withPinnedCertificates`.
 
  默认情况下，这个属性会在AFNetworking目标编译时设置为任一证书(.cer)，注意，如果你把AFNetwrorking当做嵌入框架，默认模式下不会绑定任何证书。使用`certificatesInBundle`从目标文件里加载证书，然后通过`policyWithPinningMode:withPinnedCertificates`方法来创建一个新的策略。
 
 Note that if pinning is enabled, `evaluateServerTrust:forDomain:` will return true if any pinned certificate matches.
 注意，如果没有绑定，如果匹配到任何证书，`evaluateServerTrust:forDomain:`将会返回YES
 */
@property (nonatomic, strong, nullable) NSSet <NSData *> *pinnedCertificates;

/**
 Whether or not to trust servers with an invalid or expired SSL certificates. Defaults to `NO`.
 是否允许不信任的证书（证书无效、证书时间过期）通过验证 ，默认为NO.
 */
@property (nonatomic, assign) BOOL allowInvalidCertificates;

/**
 Whether or not to validate the domain name in the certificate's CN field. Defaults to `YES`.
 是否验证域名证书的CN(common name)字段。默认值为YES
 */
@property (nonatomic, assign) BOOL validatesDomainName;

///-----------------------------------------
/// @name Getting Certificates from the Bundle
///-----------------------------------------

/**
 Returns any certificates included in the bundle. If you are using AFNetworking as an embedded framework, you must use this method to find the certificates you have included in your app bundle, and use them when creating your security policy by calling `policyWithPinningMode:withPinnedCertificates`.
 返回包内的证书。如果你把AFNetwrorking当做嵌入框架，你必须是用这个方法查找你包内包含的证书，调用`policyWithPinningMode:withPinnedCertificates`这个方法创建你的安全策略。

 @return The certificates included in the given bundle.返回这个包内的所有证书
 */
+ (NSSet <NSData *> *)certificatesInBundle:(NSBundle *)bundle;

///-----------------------------------------
/// @name Getting Specific Security Policies 获取详细的安全策略
///-----------------------------------------

/**
 Returns the shared default security policy, which does not allow invalid certificates, validates domain name, and does not validate against pinned certificates or public keys.
 返回一个被共用的默认安全策略，即不允许无效的证书，校验域名，不校验鉴定证书和公钥

 @return The default security policy. 默认安全策略
 */
+ (instancetype)defaultPolicy;

///---------------------
/// @name Initialization 初始化
///---------------------

/**
 Creates and returns a security policy with the specified pinning mode.
 通过设置特定的鉴定模式创建并返回一个安全策略

 @param pinningMode The SSL pinning mode.

 @return A new security policy.
 */
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode;

/**
 Creates and returns a security policy with the specified pinning mode.
 通过设置特定的鉴定模式创建并返回一个安全策略

 @param pinningMode The SSL pinning mode. 模式
 @param pinnedCertificates The certificates to pin against. 证书

 @return A new security policy.
 */
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode withPinnedCertificates:(NSSet <NSData *> *)pinnedCertificates;

///------------------------------
/// @name Evaluating Server Trust 鉴定服务器信任
///------------------------------

/**
 Whether or not the specified server trust should be accepted, based on the security policy.
 在安全策略基础上，是否接受特定的服务器信任

 This method should be used when responding to an authentication challenge from a server.
 响应来自服务器的身份验证质询时应使用此方法。

 @param serverTrust The X.509 certificate trust of the server. 服务器信任的X.509证书
 @param domain The domain of serverTrust. If `nil`, the domain will not be validated. 服务器信任里的域名，如果为空，这个域名将不会被验证。

 @return Whether or not to trust the server.
 */
- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(nullable NSString *)domain;

@end

NS_ASSUME_NONNULL_END

///----------------
/// @name Constants
///----------------

/**
 ## SSL Pinning Modes

 The following constants are provided by `AFSSLPinningMode` as possible SSL pinning modes.
 下面是'AFSSLPinningMode'提供的常量用来配置鉴定SSL的模式

 enum {
 AFSSLPinningModeNone,
 AFSSLPinningModePublicKey,
 AFSSLPinningModeCertificate,
 }

 `AFSSLPinningModeNone`
 Do not used pinned certificates to validate servers.不使用鉴定证书对服务器进行验证
 这个模式表示不做SSL pinning，只跟浏览器一样在系统的信任机构列表里验证服务端返回的证书。若证书是信任机构签发的就会通过，若是自己服务器生成的证书，这里是不会通过的。

 `AFSSLPinningModePublicKey`
 Validate host certificates against public keys of pinned certificates.根据鉴定证书的公钥验证主机证书
 这个模式同样是用证书绑定方式验证，客户端要有服务端的证书拷贝，只是验证时只验证证书里的公钥，不验证证书的有效期等信息。只要公钥是正确的，就能保证通信不会被窃听，因为中间人没有私钥，无法解开通过公钥加密的数据

 `AFSSLPinningModeCertificate`
 Validate host certificates against pinned certificates.根据鉴定证书验证主机证书
 这个模式表示用证书绑定方式验证证书，需要客户端保存有服务端的证书拷贝，这里验证分两步，第一步验证证书的域名，有效期等信息，第二步是对比服务端返回的证书跟客户端返回的是否一致。
*/
