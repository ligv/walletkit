//
//  BRCryptoWalletManager.h
//  BRCore
//
//  Created by Ed Gamble on 3/19/19.
//  Copyright © 2019 breadwallet. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BRCryptoWalletManager_h
#define BRCryptoWalletManager_h

#include "BRCryptoNetwork.h"
#include "BRCryptoAccount.h"
#include "BRCryptoTransfer.h"
#include "BRCryptoWallet.h"

#include "ethereum/BREthereum.h"
#include "bitcoin/BRWalletManager.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct BRCryptoWalletManagerRecord *BRCryptoWalletManager;

    typedef enum {
        CRYPTO_WALLET_MANAGER_STATE_CREATED,
        CRYPTO_WALLET_MANAGER_STATE_DISCONNECTED,
        CRYPTO_WALLET_MANAGER_STATE_CONNECTED,
        CRYPTO_WALLET_MANAGER_STATE_SYNCING,
        CRYPTO_WALLET_MANAGER_STATE_DELETED
    } BRCryptoWalletManagerState;


    typedef enum {
        CRYPTO_WALLET_MANAGER_EVENT_CREATED,
        CRYPTO_WALLET_MANAGER_EVENT_CHANGED,
        CRYPTO_WALLET_MANAGER_EVENT_DELETED,

        CRYPTO_WALLET_MANAGER_EVENT_WALLET_ADDED,
        CRYPTO_WALLET_MANAGER_EVENT_WALLET_CHANGED,
        CRYPTO_WALLET_MANAGER_EVENT_WALLET_DELETED,

        // wallet: added, ...
        CRYPTO_WALLET_MANAGER_EVENT_SYNC_STARTED,
        CRYPTO_WALLET_MANAGER_EVENT_SYNC_CONTINUES,
        CRYPTO_WALLET_MANAGER_EVENT_SYNC_STOPPED,

        CRYPTO_WALLET_MANAGER_EVENT_BLOCK_HEIGHT_UPDATED,
    } BRCryptoWalletManagerEventType;

    typedef struct {
        BRCryptoWalletManagerEventType type;
        union {
            struct {
                BRCryptoWalletManagerState oldValue;
                BRCryptoWalletManagerState newValue;
            } state;

            struct {
                /// Handler must 'give'
                BRCryptoWallet value;
            } wallet;

            struct {
                unsigned int percentComplete;
            } sync;

            struct {
                uint64_t value;
            } blockHeight;
        } u;
    } BRCryptoWalletManagerEvent;

    /// MARK: Listener

    typedef void *BRCryptoCWMListenerContext;

    /// Handler must 'give': manager, event.wallet.value
    typedef void (*BRCryptoCWMListenerWalletManagerEvent) (BRCryptoCWMListenerContext context,
                                                           BRCryptoWalletManager manager,
                                                           BRCryptoWalletManagerEvent event);

    /// Handler must 'give': manager, wallet, event.*
    typedef void (*BRCryptoCWMListenerWalletEvent) (BRCryptoCWMListenerContext context,
                                                    BRCryptoWalletManager manager,
                                                    BRCryptoWallet wallet,
                                                    BRCryptoWalletEvent event);

    /// Handler must 'give': manager, wallet, transfer
    typedef void (*BRCryptoCWMListenerTransferEvent) (BRCryptoCWMListenerContext context,
                                                      BRCryptoWalletManager manager,
                                                      BRCryptoWallet wallet,
                                                      BRCryptoTransfer transfer,
                                                      BRCryptoTransferEvent event);

    typedef struct {
        BRCryptoCWMListenerContext context;
        BRCryptoCWMListenerWalletManagerEvent walletManagerEventCallback;
        BRCryptoCWMListenerWalletEvent walletEventCallback;
        BRCryptoCWMListenerTransferEvent transferEventCallback;
    } BRCryptoCWMListener;

    // MARK: Client

    typedef void *BRCryptoCWMClientContext;

    typedef struct BRCryptoCWMCompletionStateRecord *BRCryptoCWMCompletionState;

    typedef void
        (*BRCryptoCWMEthGetEtherBalanceCallback) (BRCryptoCWMClientContext context,
                                                  BRCryptoWalletManager manager,
                                                  BRCryptoCWMCompletionState completetionState,
                                                  const char *network,
                                                  const char *address);

    typedef void
        (*BRCryptoCWMEthGetTokenBalanceCallback) (BRCryptoCWMClientContext context,
                                                  BRCryptoWalletManager manager,
                                                  BRCryptoCWMCompletionState completetionState,
                                                  const char *network,
                                                  const char *address,
                                                  const char *tokenAddress);

    typedef void
        (*BRCryptoCWMEthGetGasPriceCallback) (BRCryptoCWMClientContext context,
                                              BRCryptoWalletManager manager,
                                              BRCryptoCWMCompletionState completetionState,
                                              const char *network);

    typedef void
        (*BRCryptoCWMEthEstimateGasCallback) (BRCryptoCWMClientContext context,
                                              BRCryptoWalletManager manager,
                                              BRCryptoCWMCompletionState completetionState,
                                              const char *network,
                                              const char *from,
                                              const char *to,
                                              const char *amount,
                                              const char *data);

    typedef void
        (*BRCryptoCWMEthSubmitTransactionCallback) (BRCryptoCWMClientContext context,
                                                    BRCryptoWalletManager manager,
                                                    BRCryptoCWMCompletionState completetionState,
                                                    const char *network,
                                                    const char *transaction);

    typedef void
        (*BRCryptoCWMEthGetTransactionsCallback) (BRCryptoCWMClientContext context,
                                                  BRCryptoWalletManager manager,
                                                  BRCryptoCWMCompletionState completetionState,
                                                  const char *network,
                                                  const char *address,
                                                  uint64_t begBlockNumber,
                                                  uint64_t endBlockNumber);

    typedef void
        (*BRCryptoCWMEthGetLogsCallback) (BRCryptoCWMClientContext context,
                                          BRCryptoWalletManager manager,
                                          BRCryptoCWMCompletionState completetionState,
                                          const char *network,
                                          const char *contract,
                                          const char *address,
                                          const char *event,
                                          uint64_t begBlockNumber,
                                          uint64_t endBlockNumber);

    typedef void
        (*BRCryptoCWMEthGetBlocksCallback) (BRCryptoCWMClientContext context,
                                            BRCryptoWalletManager manager,
                                            BRCryptoCWMCompletionState completetionState,
                                            const char *network,
                                            const char *address, // disappears immediately
                                            BREthereumSyncInterestSet interests,
                                            uint64_t blockNumberStart,
                                            uint64_t blockNumberStop);

    typedef void
        (*BRCryptoCWMEthGetTokensCallback) (BRCryptoCWMClientContext context,
                                            BRCryptoWalletManager manager,
                                            BRCryptoCWMCompletionState completetionState);

    typedef void
        (*BRCryptoCWMEthGetBlockNumberCallback) (BRCryptoCWMClientContext context,
                                                 BRCryptoWalletManager manager,
                                                 BRCryptoCWMCompletionState completetionState,
                                                 const char *network);

    typedef void
        (*BRCryptoCWMEthGetNonceCallback) (BRCryptoCWMClientContext context,
                                           BRCryptoWalletManager manager,
                                           BRCryptoCWMCompletionState completetionState,
                                           const char *network,
                                           const char *address);

    typedef struct {
        BRCryptoCWMEthGetEtherBalanceCallback funcGetEtherBalance;
        BRCryptoCWMEthGetTokenBalanceCallback funcGetTokenBalance;
        BRCryptoCWMEthGetGasPriceCallback funcGetGasPrice;
        BRCryptoCWMEthEstimateGasCallback funcEstimateGas;
        BRCryptoCWMEthSubmitTransactionCallback funcSubmitTransaction;
        BRCryptoCWMEthGetTransactionsCallback funcGetTransactions; // announce one-by-one
        BRCryptoCWMEthGetLogsCallback funcGetLogs; // announce one-by-one
        BRCryptoCWMEthGetBlocksCallback funcGetBlocks;
        BRCryptoCWMEthGetTokensCallback funcGetTokens; // announce one-by-one
        BRCryptoCWMEthGetBlockNumberCallback funcGetBlockNumber;
        BRCryptoCWMEthGetNonceCallback funcGetNonce;
    } BRCryptoCWMClientETH;

    typedef void
    (*BRCryptoCWMBtcGetBlockNumberCallback) (BRCryptoCWMClientContext context,
                                             BRCryptoWalletManager manager,
                                             BRCryptoCWMCompletionState completetionState);

    typedef void
    (*BRCryptoCWMBtcGetTransactionsCallback) (BRCryptoCWMClientContext context,
                                              BRCryptoWalletManager manager,
                                              BRCryptoCWMCompletionState completetionState,
                                              char **addresses,
                                              size_t addressCount,
                                              uint64_t begBlockNumber,
                                              uint64_t endBlockNumber);

    typedef void
    (*BRCryptoCWMBtcSubmitTransactionCallback) (BRCryptoCWMClientContext context,
                                                BRCryptoWalletManager manager,
                                                BRCryptoCWMCompletionState completetionState,
                                                uint8_t *transaction,
                                                size_t transactionLength);

    typedef struct {
        BRCryptoCWMBtcGetBlockNumberCallback  funcGetBlockNumber;
        BRCryptoCWMBtcGetTransactionsCallback funcGetTransactions;
        BRCryptoCWMBtcSubmitTransactionCallback funcSubmitTransaction;
    } BRCryptoCWMClientBTC;

    typedef struct {
    } BRCryptoCWMClientGEN;

    typedef struct {
        BRCryptoCWMClientContext context;
        BRCryptoCWMClientBTC btc;
        BRCryptoCWMClientETH eth;
        BRCryptoCWMClientGEN gen;
    } BRCryptoCWMClient;


    extern BRCryptoWalletManager
    cryptoWalletManagerCreate (BRCryptoCWMListener listener,
                               BRCryptoCWMClient client,
                               BRCryptoAccount account,
                               BRCryptoNetwork network,
                               BRSyncMode mode,
                               const char *path);

    extern BRCryptoNetwork
    cryptoWalletManagerGetNetwork (BRCryptoWalletManager cwm);

    extern BRCryptoAccount
    cryptoWalletManagerGetAccount (BRCryptoWalletManager cwm);

    extern BRSyncMode
    cryptoWalletManagerGetMode (BRCryptoWalletManager cwm);

    extern BRCryptoWalletManagerState
    cryptoWalletManagerGetState (BRCryptoWalletManager cwm);

    extern const char *
    cryptoWalletManagerGetPath (BRCryptoWalletManager cwm);

    extern BRCryptoBoolean
    cryptoWalletManagerHasWallet (BRCryptoWalletManager cwm,
                                  BRCryptoWallet wallet);

    extern BRCryptoWallet
    cryptoWalletManagerGetWallet (BRCryptoWalletManager cwm);

    extern size_t
    cryptoWalletManagerGetWalletsCount (BRCryptoWalletManager cwm);

    extern BRCryptoWallet
    cryptoWalletManagerGetWalletAtIndex (BRCryptoWalletManager cwm,
                                                size_t index);

    extern BRCryptoWallet
    cryptoWalletManagerGetWalletForCurrency (BRCryptoWalletManager cwm,
                                               BRCryptoCurrency currency);

    extern void
    cryptoWalletManagerConnect (BRCryptoWalletManager cwm);

    extern void
    cryptoWalletManagerDisconnect (BRCryptoWalletManager cwm);

    extern void
    cryptoWalletManagerSync (BRCryptoWalletManager cwm);

    extern void
    cryptoWalletManagerSubmit (BRCryptoWalletManager cwm,
                               BRCryptoWallet wid,
                               BRCryptoTransfer tid,
                               const char *paperKey);

    extern void
    cwmAnnounceGetBlockNumberSuccessAsInteger (BRCryptoWalletManager cwm,
                                               BRCryptoCWMCompletionState completetionState,
                                               uint64_t blockNumber);

    extern void
    cwmAnnounceGetBlockNumberSuccessAsString (BRCryptoWalletManager cwm,
                                              BRCryptoCWMCompletionState completetionState,
                                              const char *blockNumber);

    extern void
    cwmAnnounceGetBlockNumberFailure (BRCryptoWalletManager cwm,
                                      BRCryptoCWMCompletionState completetionState);

    extern void
    cwmAnnounceGetTransactionsItemBTC (BRCryptoWalletManager cwm,
                                       BRCryptoCWMCompletionState completetionState,
                                       uint8_t *transaction,
                                       size_t transactionLength,
                                       uint64_t timestamp,
                                       uint64_t blockHeight);

    extern void
    cwmAnnounceGetTransactionsItemETH (BRCryptoWalletManager cwm,
                                       BRCryptoCWMCompletionState completetionState,
                                       const char *hash,
                                       const char *from,
                                       const char *to,
                                       const char *contract,
                                       const char *amount, // value
                                       const char *gasLimit,
                                       const char *gasPrice,
                                       const char *data,
                                       const char *nonce,
                                       const char *gasUsed,
                                       const char *blockNumber,
                                       const char *blockHash,
                                       const char *blockConfirmations,
                                       const char *blockTransactionIndex,
                                       const char *blockTimestamp,
                                       // cumulative gas used,
                                       // confirmations
                                       // txreceipt_status
                                       const char *isError);

    extern void
    cwmAnnounceGetTransactionsComplete (BRCryptoWalletManager cwm,
                                        BRCryptoCWMCompletionState completetionState,
                                        BRCryptoBoolean success);

    extern void
    cwmAnnounceSubmitTransferSuccess (BRCryptoWalletManager cwm,
                                      BRCryptoCWMCompletionState completetionState);

    extern void
    cwmAnnounceSubmitTransferSuccessForHash (BRCryptoWalletManager cwm,
                                             BRCryptoCWMCompletionState completetionState,
                                             const char *hash);

    extern void
    cwmAnnounceSubmitTransferFailure (BRCryptoWalletManager cwm,
                                      BRCryptoCWMCompletionState completetionState);

    extern void
    cwmAnnounceGetBalanceSuccess (BRCryptoWalletManager cwm,
                                  BRCryptoCWMCompletionState completetionState,
                                  const char *balance);

    extern void
    cwmAnnounceGetBalanceFailure (BRCryptoWalletManager cwm,
                                  BRCryptoCWMCompletionState completetionState);

    extern void
    cwmAnnounceGetBlocksSuccess (BRCryptoWalletManager cwm,
                                 BRCryptoCWMCompletionState completetionState,
                                 int blockNumbersCount,
                                 uint64_t *blockNumbers);

    extern void
    cwmAnnounceGetBlocksFailure (BRCryptoWalletManager cwm,
                                 BRCryptoCWMCompletionState completetionState);

    extern void
    cwmAnnounceGetGasPriceSuccess (BRCryptoWalletManager cwm,
                                   BRCryptoCWMCompletionState completetionState,
                                   const char *gasPrice);

    extern void
    cwmAnnounceGetGasPriceFailure (BRCryptoWalletManager cwm,
                                   BRCryptoCWMCompletionState completetionState);

    extern void
    cwmAnnounceGetGasEstimateSuccess (BRCryptoWalletManager cwm,
                                      BRCryptoCWMCompletionState completetionState,
                                      const char *gasEstimate);

    extern void
    cwmAnnounceGasEstimateFailure (BRCryptoWalletManager cwm,
                                   BRCryptoCWMCompletionState completetionState);

    extern void
    cwmAnnounceGetLogsItem(BRCryptoWalletManager cwm,
                           BRCryptoCWMCompletionState completetionState,
                           const char *strHash,
                           const char *strContract,
                           int topicCount,
                           const char **arrayTopics,
                           const char *strData,
                           const char *strGasPrice,
                           const char *strGasUsed,
                           const char *strLogIndex,
                           const char *strBlockNumber,
                           const char *strBlockTransactionIndex,
                           const char *strBlockTimestamp);

    extern void
    cwmAnnounceGetLogsComplete(BRCryptoWalletManager cwm,
                               BRCryptoCWMCompletionState completetionState,
                               BRCryptoBoolean success);

    extern void
    cwmAnnounceGetTokensItem (BRCryptoWalletManager cwm,
                              BRCryptoCWMCompletionState completetionState,
                              const char *address,
                              const char *symbol,
                              const char *name,
                              const char *description,
                              unsigned int decimals,
                              const char *strDefaultGasLimit,
                              const char *strDefaultGasPrice);

    extern void
    cwmAnnounceGetTokensComplete (BRCryptoWalletManager cwm,
                                  BRCryptoCWMCompletionState completetionState,
                                  BRCryptoBoolean success);

    extern void
    cwmAnnounceGetNonceSuccess (BRCryptoWalletManager cwm,
                                BRCryptoCWMCompletionState completetionState,
                                const char *address,
                                const char *nonce);

    extern void
    cwmAnnounceGetNonceFailure (BRCryptoWalletManager cwm,
                                BRCryptoCWMCompletionState completetionState);

    DECLARE_CRYPTO_GIVE_TAKE (BRCryptoWalletManager, cryptoWalletManager);

#ifdef __cplusplus
}
#endif

#endif /* BRCryptoWalletManager_h */
