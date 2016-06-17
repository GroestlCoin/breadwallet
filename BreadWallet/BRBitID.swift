//
//  BRBitID.swift
//  BreadWallet
//
//  Created by Samuel Sutch on 6/17/16.
//  Copyright © 2016 breadwallet LLC. All rights reserved.
//

import Foundation
import Security

@objc public class BRBitID : NSObject {
    static let SCHEME = "bitid"
    static let PARAM_NONCE = "x"
    static let PARAM_UNSECURE = "u"
    static let USER_DEFAULTS_NONCE_KEY = "brbitid_nonces"
    static let DEFAULT_INDEX: UInt32 = 42
    
    class public func isBitIDURL(url: NSURL!) -> Bool {
        return url.scheme == SCHEME
    }
    
    public static let BITCOIN_SIGNED_MESSAGE_HEADER = "Bitcoin Signed Message:\n".dataUsingEncoding(NSUTF8StringEncoding)!
    
    public class func formatMessageForBitcoinSigning(message: String) -> NSData {
        let data = NSMutableData()
        data.appendUInt8(UInt8(BITCOIN_SIGNED_MESSAGE_HEADER.length))
        data.appendData(BITCOIN_SIGNED_MESSAGE_HEADER)
        let msgBytes = message.dataUsingEncoding(NSUTF8StringEncoding)!
        data.appendVarInt(UInt64(msgBytes.length))
        data.appendData(msgBytes)
        return data
    }
    
    // sign a message with a key and return a base64 representation
    public class func signMessage(message: String, usingKey key: BRKey) -> String {
        let signingData = formatMessageForBitcoinSigning(message)
        let signature = key.compactSign(signingData.SHA256_2())!
        return NSString(data: signature.base64EncodedDataWithOptions([]), encoding: NSUTF8StringEncoding)! as String
    }
    
    public let url: NSURL
    
    public var siteName: String {
        return "\(url.host!)\(url.path!)"
    }
    
    public init(url: NSURL!) {
        self.url = url
    }
    
    public func newNonce() -> String {
        let defs = NSUserDefaults.standardUserDefaults()
        let nonceKey = "\(url.host!)/\(url.path!)"
        var allNonces = [String: [String]]()
        var specificNonces = [String]()
        
        // load previous nonces. we save all nonces generated for each service
        // so they are not used twice from the same device
        if let existingNonces = defs.objectForKey(BRBitID.USER_DEFAULTS_NONCE_KEY) {
            allNonces = existingNonces as! [String: [String]]
        }
        if let existingSpecificNonces = allNonces[nonceKey] {
            specificNonces = existingSpecificNonces
        }
        
        // generate a completely new nonce
        var nonce: String
        repeat {
            nonce = "\(Int(NSDate().timeIntervalSince1970))"
        } while (specificNonces.contains(nonce))
        
        // save out the nonce list
        specificNonces.append(nonce)
        allNonces[nonceKey] = specificNonces
        defs.setObject(allNonces, forKey: BRBitID.USER_DEFAULTS_NONCE_KEY)
        
        return nonce
    }
    
    public func runCallback(completionHandler: (NSData?, NSURLResponse?, NSError?) -> Void) {
        guard let manager = BRWalletManager.sharedInstance() else {
            dispatch_async(dispatch_get_main_queue()) {
                completionHandler(nil, nil, NSError(domain: "", code: -1001, userInfo:
                    [NSLocalizedDescriptionKey: NSLocalizedString("No wallet", comment: "")]))
            }
            return
        }
        autoreleasepool {
            guard let phrase = manager.seedPhrase else {
                dispatch_async(dispatch_get_main_queue()) {
                    completionHandler(nil, nil, NSError(domain: "", code: -1001, userInfo:
                        [NSLocalizedDescriptionKey: NSLocalizedString("Could not unlock", comment: "")]))
                }
                return
            }
            let seq = BRBIP32Sequence()
            let seed = BRBIP39Mnemonic().deriveKeyFromPhrase(phrase, withPassphrase: nil)
            var scheme = "https"
            var nonce: String
            guard let query = url.query?.parseQueryString() else {
                dispatch_async(dispatch_get_main_queue()) {
                    completionHandler(nil, nil, NSError(domain: "", code: -1001, userInfo:
                        [NSLocalizedDescriptionKey: NSLocalizedString("Malformed URI", comment: "")]))
                }
                return
            }
            if let u = query[BRBitID.PARAM_UNSECURE] where u.count == 1 && u[0] == "1" {
                scheme = "http"
            }
            if let x = query[BRBitID.PARAM_NONCE] where x.count == 1 {
                nonce = x[0] // service is providing a nonce
            } else {
                nonce = newNonce() // we are generating our own nonce
            }
            let uri = "\(scheme)://\(url.host!)\(url.path!)"
            
            // build a payload consisting of the signature, address and signed uri
            let priv = BRKey(privateKey: seq.bitIdPrivateKey(BRBitID.DEFAULT_INDEX, forURI: uri, fromSeed: seed))!
            let uriWithNonce = "bitid://\(url.host!)\(url.path!)?x=\(nonce)"
            let signature = BRBitID.signMessage(uriWithNonce, usingKey: priv)
            let payload: [String: String] = [
                "address": priv.address!,
                "signature": signature,
                "uri": uriWithNonce
            ]
            let json = try! NSJSONSerialization.dataWithJSONObject(payload, options: [])
            
            // send off said payload
            let req = NSMutableURLRequest(URL: NSURL(string: "\(uri)?x=\(nonce)")!)
            req.setValue("application/json", forHTTPHeaderField: "Content-Type")
            req.HTTPMethod = "POST"
            req.HTTPBody = json
            NSURLSession.sharedSession().dataTaskWithRequest(req, completionHandler: completionHandler).resume()
        }
    }
}