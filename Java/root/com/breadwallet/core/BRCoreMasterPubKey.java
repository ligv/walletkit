/*
 * BreadWallet
 *
 * Created by Ed Gamble <ed@breadwallet.com> on 1/22/18.
 * Copyright (c) 2018 breadwallet LLC
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
package com.breadwallet.core;

/**
 *
 */
public class BRCoreMasterPubKey extends BRCoreJniReference {

    public native byte[] getPubKey ();

    public BRCoreKey getPubKeyAsCoreKey () {
        return new BRCoreKey (createPubKey ());
    }

    protected native long createPubKey ();

    public BRCoreMasterPubKey (byte[] bytes, boolean isPaperKey) {
        this (isPaperKey
                ? createJniCoreMasterPubKeyFromPhrase(bytes)
                : createJniCoreMasterPubKeyFromPubKey(bytes));
    }

    private BRCoreMasterPubKey (long jniReferenceAddress) {
        super (jniReferenceAddress);
    }

    private static native long createJniCoreMasterPubKeyFromPhrase (byte[] phrase);

    private static native long createJniCoreMasterPubKeyFromPubKey (byte[] phrase);

    public static native byte[] bip32BitIDKey(byte[] seed, int index, String uri);

    /**
     * Validates `words` as 'phrase` for this BRCoreMasterPubKey.
     *
     * @param words
     * @return
     */
    //public native boolean validateRecoveryPhrase (String[] words)

    public static native boolean validateRecoveryPhrase(String[] words, String phrase);

    public static native byte[] encodeSeed (byte[] seed, String[] words);

    @Override
    public String toString() {
        return "BRCoreMasterPubKey{" +
                "jniReferenceAddress=" + jniReferenceAddress +
                '}';
    }
}
