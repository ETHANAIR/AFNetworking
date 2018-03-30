// AFHTTPSessionManager.h
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
#if !TARGET_OS_WATCH
#import <SystemConfiguration/SystemConfiguration.h>
#endif
#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_WATCH || TARGET_OS_TV
#import <MobileCoreServices/MobileCoreServices.h>
#else
#import <CoreServices/CoreServices.h>
#endif

#import "AFURLSessionManager.h"

/**
 `AFHTTPSessionManager` is a subclass of `AFURLSessionManager` with convenience methods for making HTTP requests. When a `baseURL` is provided, requests made with the `GET` / `POST` / et al. convenience methods can be made with relative paths.
 `AFHTTPSessionManager` 是 `AFURLSessionManager` 的一个子类，它具有便捷的方法来进行HTTP请求。
 当提供'baseURL'时，使用'GET'或者'POST' 等方法进行的请求可以用相对路径进行

 ## Subclassing Notes 子类备注

 Developers targeting iOS 7 or Mac OS X 10.9 or later that deal extensively with a web service are encouraged to subclass `AFHTTPSessionManager`, providing a class method that returns a shared singleton object on which authentication and other configuration can be shared across the application.
 当开发者的目标版本是 iOS 7 或 Mac OS X 10.9 或更新的版本时，网络层鼓励使用子类'AFHTTPSessionManager', 提供一个类方法返回一个单例可以
 来进行全局验证和配置。

 For developers targeting iOS 6 or Mac OS X 10.8 or earlier, `AFHTTPRequestOperationManager` may be used to similar effect.
 对于目标版本低于 iOS 6 或者 Mac OS X 10.8的开发者，可以使用`AFHTTPRequestOperationManager`达到类似的效果。

 ## Methods to Override 重写方法

 To change the behavior of all data task operation construction, which is also used in the `GET` / `POST` / et al. convenience methods, override `dataTaskWithRequest:uploadProgress:downloadProgress:completionHandler:`.
 如果你想改变所有数据任务的操作结构，也用于'GET'或者'POST'等便捷方法，可以重写`dataTaskWithRequest:uploadProgress:downloadProgress:completionHandler:`

 ## Serialization 序列化

 Requests created by an HTTP client will contain default headers and encode parameters according to the `requestSerializer` property, which is an object conforming to `<AFURLRequestSerialization>`.
 由HTTP客户端创建的请求将包含默认标头并根据requestSerializer属性对参数进行编码，该属性符合`<AFURLRequestSerialization>

 Responses received from the server are automatically validated and serialized by the `responseSerializers` property, which is an object conforming to `<AFURLResponseSerialization>`
 从服务器收到的响应会自动验证并通过`responseSerializers`属性进行序列化，该属性符合`<AFURLResponseSerialization>
 

 ## URL Construction Using Relative Paths 使用相对路径的URL结构

 For HTTP convenience methods, the request serializer constructs URLs from the path relative to the `-baseURL`, using `NSURL +URLWithString:relativeToURL:`, when provided. If `baseURL` is `nil`, `path` needs to resolve to a valid `NSURL` object using `NSURL +URLWithString:`.
 对于HTTP便捷方法，请求序列化程序在提供时使用“NSURL + URLWithString：relativeToURL：”从相对于`-baseURL`的路径构造URL。 如果`baseURL`为`nil`，`path`需要使用`NSURL + URLWithString：'来解析为一个有效的NSURL对象。

 Below are a few examples of how `baseURL` and relative paths interact:
 下面是如何使用'baseURL'和相对路径的几个例子:

    NSURL *baseURL = [NSURL URLWithString:@"http://example.com/v1/"];
    [NSURL URLWithString:@"foo" relativeToURL:baseURL];                  // http://example.com/v1/foo
    [NSURL URLWithString:@"foo?bar=baz" relativeToURL:baseURL];          // http://example.com/v1/foo?bar=baz
    [NSURL URLWithString:@"/foo" relativeToURL:baseURL];                 // http://example.com/foo
    [NSURL URLWithString:@"foo/" relativeToURL:baseURL];                 // http://example.com/v1/foo
    [NSURL URLWithString:@"/foo/" relativeToURL:baseURL];                // http://example.com/foo/
    [NSURL URLWithString:@"http://example2.com/" relativeToURL:baseURL]; // http://example2.com/

 Also important to note is that a trailing slash will be added to any `baseURL` without one. This would otherwise cause unexpected behavior when constructing URLs using paths without a leading slash.
 另外需要注意的是，一个尾部的斜线将被添加到任何`baseURL`中，而没有一个。 当使用不带前导斜杠的路径构造URL时，这会导致抛出异常。

 @warning Managers for background sessions must be owned for the duration of their use. This can be accomplished by creating an application-wide or shared singleton instance.
 @警告 在使用期间后台会话必须由管理类持有，可以通过全局配置或单例完成。
 */

NS_ASSUME_NONNULL_BEGIN

@interface AFHTTPSessionManager : AFURLSessionManager <NSSecureCoding, NSCopying>

/**
 关于NSSecureCoding，它是NSCoding的一个子类，相对NSCoding更安全
 可以扩展阅读下面的文章
 中文：http://blog.jobbole.com/67655/
 英文：http://iosdevelopertips.com/general/object-encoding-and-decoding-with-nssecurecoding.html
 */

/**
 The URL used to construct requests from relative paths in methods like `requestWithMethod:URLString:parameters:`, and the `GET` / `POST` / et al. convenience methods.
 在‘requestWithMethod：URLString：parameters：’和 GET 或 POST 等便利方法中用于构造相对路径请求的URL。
 */
@property (readonly, nonatomic, strong, nullable) NSURL *baseURL;

/**
 Requests created with `requestWithMethod:URLString:parameters:` & `multipartFormRequestWithMethod:URLString:parameters:constructingBodyWithBlock:` are constructed with a set of default headers using a parameter serialization specified by this property. By default, this is set to an instance of `AFHTTPRequestSerializer`, which serializes query string parameters for `GET`, `HEAD`, and `DELETE` requests, or otherwise URL-form-encodes HTTP message bodies.
 
 使用`requestWithMethod：URLString：parameters：`＆`multipartFormRequestWithMethod：URLString：parameters：constructBodyWithBlock：`创建的请求由一组使用此属性指定的参数序列化的默认标头构造而成。 默认情况下，它被设置为“AFHTTPRequestSerializer”的实例，该实例序列化“GET”，“HEAD”和“DELETE”请求的字符串参数，否则HTTP消息体会以URL形式编码。
 
 @warning `requestSerializer` must not be `nil`.
 @警告: `requestSerializer` 不能为空
 */
@property (nonatomic, strong) AFHTTPRequestSerializer <AFURLRequestSerialization> * requestSerializer;

/**
 Responses sent from the server in data tasks created with `dataTaskWithRequest:success:failure:` and run using the `GET` / `POST` / et al. convenience methods are automatically validated and serialized by the response serializer.
 By default, this property is set to an instance of `AFJSONResponseSerializer`.

 通过创建数据任务 `dataTaskWithRequest:success:failure:` 来获取从服务器返回的数据，并使用`GET`/`POST`/等运行时， 便捷方法会被响应序列化器自动验证并序列化。
 默认情况下，该属性设置为“AFJSONResponseSerializer”的实例。
 
 @warning `responseSerializer` must not be `nil`.
 @警告: `requestSerializer` 不能为空
 */
@property (nonatomic, strong) AFHTTPResponseSerializer <AFURLResponseSerialization> * responseSerializer;

///-------------------------------
/// @name Managing Security Policy 管理安全策略
///-------------------------------

/**
 The security policy used by created session to evaluate server trust for secure connections. `AFURLSessionManager` uses the `defaultPolicy` unless otherwise specified. A security policy configured with `AFSSLPinningModePublicKey` or `AFSSLPinningModeCertificate` can only be applied on a session manager initialized with a secure base URL (i.e. https). Applying a security policy with pinning enabled on an insecure session manager throws an `Invalid Security Policy` exception.
 
 安全策略是指通过创建会话评估服务器信任以确保连接安全。
 `AFURLSessionManager`使用 `defaultPolicy`，除非进行特别配置。
 使用“AFSSLPinningModePublicKey”或“AFSSLPinningModeCertificate”配置的安全策略只能应用于使用安全基础URL（即https）初始化的会话管理器。
 在不安全的会话管理器上启用锁定的安全策略会抛出“Invalid Security Policy”异常。

 */
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

///---------------------
/// @name Initialization 初始化
///---------------------

/**
 Creates and returns an `AFHTTPSessionManager` object.
 创建并返回一个`AFHTTPSessionManager`对象
 */
+ (instancetype)manager;

/**
 Initializes an `AFHTTPSessionManager` object with the specified base URL.
 通过传入基础URL初始化一个`AFHTTPSessionManager`对象

 @param url The base URL for the HTTP client. HTTP客户端的基础URL

 @return The newly-initialized HTTP client 初始化个新的HTTP客户端
 */
- (instancetype)initWithBaseURL:(nullable NSURL *)url;

/**
 Initializes an `AFHTTPSessionManager` object with the specified base URL.
 通过传入基础URL初始化一个`AFHTTPSessionManager`对象

 This is the designated initializer.指定初始化方法

 @param url The base URL for the HTTP client. HTTP客户端的基础URL
 @param configuration The configuration used to create the managed session. 用来创建托管会话的配置

 @return The newly-initialized HTTP client 初始化个新的HTTP客户端
 */
- (instancetype)initWithBaseURL:(nullable NSURL *)url
           sessionConfiguration:(nullable NSURLSessionConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

///---------------------------
/// @name Making HTTP Requests 发起 HTTP 请求
///---------------------------

/**
 Creates and runs an `NSURLSessionDataTask` with a `GET` request.
 创建并运行一个配置为‘GET’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码 这些参数会通过客户端请求序列化器进行编码
 
 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 
 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。
 
 @see -dataTaskWithRequest:completionHandler:
 */
- (nullable NSURLSessionDataTask *)GET:(NSString *)URLString
                   parameters:(nullable id)parameters
                      success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                      failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure DEPRECATED_ATTRIBUTE;


/**
 Creates and runs an `NSURLSessionDataTask` with a `GET` request.
 创建并运行一个配置为‘GET’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码 这些参数会通过客户端请求序列化器进行编码

 @param downloadProgress A block object to be executed when the download progress is updated. Note this block is called on the session queue, not the main queue.
 						 当下载进度更新时会执行这个闭包对象。 注意这个闭包在会话队列， 不是主队列。

 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。

 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。

 @see -dataTaskWithRequest:uploadProgress:downloadProgress:completionHandler:
 */
- (nullable NSURLSessionDataTask *)GET:(NSString *)URLString
                            parameters:(nullable id)parameters
                              progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgress
                               success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                               failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 Creates and runs an `NSURLSessionDataTask` with a `HEAD` request.
 创建并运行一个配置为‘HEAD’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求 字符串URL用于创建请求

 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码 这些参数会通过客户端请求序列化器进行编码

 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes a single arguments: the data task.
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。

 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。

 @see -dataTaskWithRequest:completionHandler:
 */
- (nullable NSURLSessionDataTask *)HEAD:(NSString *)URLString
                    parameters:(nullable id)parameters
                       success:(nullable void (^)(NSURLSessionDataTask *task))success
                       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 Creates and runs an `NSURLSessionDataTask` with a `POST` request.
 创建并运行一个配置为‘POST’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求

 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码

 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 				
 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。

 @see -dataTaskWithRequest:completionHandler:
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                    parameters:(nullable id)parameters
                       success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure DEPRECATED_ATTRIBUTE;

/**
 Creates and runs an `NSURLSessionDataTask` with a `POST` request.
 创建并运行一个配置为‘POST’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码
 
 @param uploadProgress A block object to be executed when the upload progress is updated. Note this block is called on the session queue, not the main queue.
 						当下载进度更新时会执行这个闭包对象。 注意这个闭包在会话队列， 不是主队列。

 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。 

 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。

 @see -dataTaskWithRequest:uploadProgress:downloadProgress:completionHandler:
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                             parameters:(nullable id)parameters
                               progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgress
                                success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                                failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 Creates and runs an `NSURLSessionDataTask` with a multipart `POST` request.
 创建并运行一个配置为多部分‘POST’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码
 
 @param block A block that takes a single argument and appends data to the HTTP body. The block argument is an object adopting the `AFMultipartFormData` protocol.
 			  带有一个参数的闭包对象和HTTP体拼接的数据。这个闭包的参数是一个遵循`AFMultipartFormData`协议的对象

 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。

 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。

 @see -dataTaskWithRequest:completionHandler:
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                    parameters:(nullable id)parameters
     constructingBodyWithBlock:(nullable void (^)(id <AFMultipartFormData> formData))block
                       success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                       failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure DEPRECATED_ATTRIBUTE;

/**
 Creates and runs an `NSURLSessionDataTask` with a multipart `POST` request.
 创建并运行一个配置为多部分‘POST’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码
 
 @param block A block that takes a single argument and appends data to the HTTP body. The block argument is an object adopting the `AFMultipartFormData` protocol.
 				带有一个参数的闭包对象和HTTP体拼接的数据。这个闭包的参数是一个遵循`AFMultipartFormData`协议的对象

 @param uploadProgress A block object to be executed when the upload progress is updated. Note this block is called on the session queue, not the main queue.
 					   当下载进度更新时会执行这个闭包对象。 注意这个闭包在会话队列， 不是主队列。

 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 
 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。

 @see -dataTaskWithRequest:uploadProgress:downloadProgress:completionHandler:
 */
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                             parameters:(nullable id)parameters
              constructingBodyWithBlock:(nullable void (^)(id <AFMultipartFormData> formData))block
                               progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgress
                                success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                                failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 Creates and runs an `NSURLSessionDataTask` with a `PUT` request.
 创建并运行一个配置为‘PUT’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码
 
 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 
 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。
 
 @see -dataTaskWithRequest:completionHandler:
 */
- (nullable NSURLSessionDataTask *)PUT:(NSString *)URLString
                   parameters:(nullable id)parameters
                      success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                      failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 Creates and runs an `NSURLSessionDataTask` with a `PATCH` request.
 创建并运行一个配置为‘PATCH’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码
 
 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 
 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。
 
 @see -dataTaskWithRequest:completionHandler:
 */
- (nullable NSURLSessionDataTask *)PATCH:(NSString *)URLString
                     parameters:(nullable id)parameters
                        success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                        failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

/**
 Creates and runs an `NSURLSessionDataTask` with a `DELETE` request.
 创建并运行一个配置为‘DELETE’请求的‘NSURLSessionDataTask'

 @param URLString The URL string used to create the request URL. 字符串URL用于创建请求
 
 @param parameters The parameters to be encoded according to the client request serializer. 这些参数会通过客户端请求序列化器进行编码
 
 @param success A block object to be executed when the task finishes successfully. This block has no return value and takes two arguments: the data task, and the response object created by the client response serializer. 
 				当一个任务成功完成时将会执行这个闭包对象。这个闭包没有返回值并且返回两个参数: 数据任务，由客户端响应串行器返回的对象。
 
 @param failure A block object to be executed when the task finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the response data. This block has no return value and takes a two arguments: the data task and the error describing the network or parsing error that occurred.
				当一个任务失败时将会执行这个闭包对象。或者任务成功，但是当解析返回数据发生错误时。这个闭包没有返回值并且返回两个参数：错误描述或解析时发生的错误。
 
 @see -dataTaskWithRequest:completionHandler:
 */
- (nullable NSURLSessionDataTask *)DELETE:(NSString *)URLString
                      parameters:(nullable id)parameters
                         success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                         failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;

@end

NS_ASSUME_NONNULL_END
