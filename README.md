# My blockchain from scratch
## Project overview

This project is a simplified blockchain implementation written in C++. It simulates the core mechanisms used in real blockchain systems: transaction validation, block creation, mining using a proof-of-work algorithm, and block verification before inclusion in the chain.

The blockchain uses an account-based balance model, maintains a mempool of pending transactions, and allows miners to create blocks containing the most valuable transactions. Blocks are mined by searching for a hash that satisfies the current difficulty target. The difficulty automatically adjusts based on the time required to mine previous blocks.

The implementation also includes Merkle tree construction for transaction integrity, transaction validation to prevent invalid transfers, and parallel block mining using OpenMP, where multiple miners compete to mine and append a block. A successful miner receives a 50 BTC block reward.

## Build and run

### Dependencies
This project uses:
- C++20 compiler (g++ 11+ or clang++ 14+)
- GNU Make
- OpenMP runtime and headers
- OpenSSL development libraries (for SHA-256 via EVP)

Linux (Debian/Ubuntu) example:
```bash
sudo apt update
sudo apt install -y build-essential libssl-dev libomp-dev
```

### Compile
From the project root:
```bash
make
```

This builds the executable:
- `./blockchain`

### Run
```bash
./blockchain
```

Or use the Makefile convenience target:
```bash
make run
```

### Clean build artifacts
```bash
make clean
```
## Implementation details:
### Blockchain class:
```c++
class Blockchain {
    private:
        list<Block> blockList;
        unordered_map<string, User> users;
        unordered_map<string, Transaction> memPool;
        vector<string> sortedTransactionHashes;

        int difficulty;
        // private methods (are called only by blockchain itself)
        void ExecuteTransactions(const vector<Transaction> &transactions);
        void GenerateUsers();
        void GenerateMemPool();
    public:
        // constructors
        Blockchain();
        ~Blockchain();

        // getters and setters
        const Block& getLastBlock() const { return blockList.back();}
        Block getBlock(int n) const;
        int getDifficulty() const {return difficulty; }

        // protected getters
        const list<Block>& getBlockChain() const {return blockList;}
        const unordered_map<string, Transaction>& getMemPool() const {return memPool;}
        const vector<string>& getSortedHashVector() const {return sortedTransactionHashes;}
        const unordered_map<string, User>& getUsers() const {return users;}

        // user methods
        bool addTransactionToMempool(const Transaction &transactionToAdd);
        void addUser(const User& user);
        void validateAndAddBlock(Block &BlockToAdd);
};
```
#### The main blockchain class has the following private members:
- list<Block> blockList – list of blocks  
- users – user hash set  
- memPool – transaction hash set  
- sortedTransactionHashes – sorted transaction vector (sorted by the amount of transferred bitcoins)  
- difficulty – mining difficulty  

#### Private methods:
- ExecuteTransactions – only the blockchain can execute transactions  
- GenerateUsers – user generation  
- GenerateMemPool – transaction generation  

#### User methods:
- addTransaction – add a transaction (the transaction is validated before being added)  
- addUser – add a user  
- validateAndAddBlock – attempt to add a block to the blockchain (the block is validated before being added)  

#### The class also has a constructor (described below), getters, and setters  

### Constructor:
```c++
Blockchain::Blockchain() : difficulty(3) {
    // generate users and transactions
    getLogger().Log("Initializing blockchain...");
    GenerateUsers();
    getLogger().Log("Generated " + to_string(this->users.size()) + " users.");
    GenerateMemPool();
    getLogger().Log("Generated " + to_string(this->memPool.size()) + " txes and added to mempool.");

    //satoshi
    User Satoshi("Satoshi Nakamoto", "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX", 0);
    this->users.insert({Satoshi.getKey(), Satoshi});

    // add genesis block
    Block genesisBlock(Satoshi);
    this->ExecuteTransactions(genesisBlock.getTransactions());
    blockList.push_back(genesisBlock);
}
```
- Selects difficulty (3 by default)  
- Creates users and transactions  
- Creates the Satoshi user  
- Creates the genesis block  
- Executes the genesis block transactions (one transaction from block reward -> Satoshi)  
- Adds the genesis block to the blockchain  

### Block class:
```c++
struct BlockHeader {
    // calculated by mining
    string hash;
    string timestamp;
    long long int nonce;
    std::chrono::duration<long, std::ratio<1, 1000>> miningTime;

    // calculated after block creation
    long int height;
    string prevBlockHash;
    string version;
    string merkleRootHash;
    int difficultyTarget;  
}
class Block {
    private:
        BlockHeader mHeader;
        vector<Transaction> mData;
    public:
        //default constructor, destructor and rule of five
        Block() = default;
        Block(const Block &BlockToCopy) = default;
        Block& operator=(const Block &BlockToCopy) = default;
        Block(Block &&BlockToMove) = default;
        Block& operator=(Block &&BlockToMove) = default;
        ~Block();

        //genesis block
        Block(const User &satoshi);

        //new block with 100 random transactions
        Block(const Blockchain &blockchain, User &miner);

        //getters and setters
        long long int getNonce() const {return mHeader.nonce;}
        BlockHeader getHeader() const {return mHeader;}
        string getHash() const {return mHeader.hash;}
        long int getHeight() const {return mHeader.height;}
        const vector<Transaction> &getTransactions() const  {return mData;}
        void setHeader(BlockHeader header);
        void setTransactions(vector<Transaction> transactions);
        auto getMiningTime() const {return mHeader.miningTime;}

        void addTransaction(const Transaction &tx);

        void CoutBlock() const;
        bool Mine();
        bool Mine5secs();
};
```
- A block consists of a header and a vector of transactions  
- A block is created through the constructor `Block(const Blockchain &blockchain, User &miner)`, where the 100 most valuable transactions are added, along with a mining transaction (Block Reward -> Miner, 50 BTC)  
- The block can be mined  
- An unmined block cannot be added to the blockchain and cannot receive the reward (50 BTC)  

### User class:
```c++
class User {
    private:
        string mName;
        string mPublicKey;
        int mBalance;
        int mPendingBalance;
    public:
        User() = default;
        User(const User &other) = default;
        User& operator=(const User &other) = default;
        User(const string &name);
        User(const string &name, const string &key, int balance);
        ~User();

        string getName() const {return mName; }
        string getKey() const { return mPublicKey; }
        int getBalance() const { return mBalance; }
        int getPendingBalance() const { return mPendingBalance; }
        void setBalance(int newBalance) { mBalance = newBalance;}
        void setPendingBalance(int newBalance) {mPendingBalance = newBalance;}
};
```
- Contains name, public key, balance, and pending balance  
- Balance – the real balance currently in the blockchain  
- Pending balance – balance including transactions that were added to the mempool but have not yet been mined  
### Block mining:
```c++
bool Block::Mine(){
    if(!mHeader.hash.empty()) return true;
    mHeader.nonce = 0;
    string blockNumber = to_string(this->mHeader.height);
    getLogger().Log("Starting to mine block #" + blockNumber + "...");
    auto miningStart = std::chrono::high_resolution_clock::now(); 
    while (true) {
        string arg = mHeader.ToString() + std::to_string(mHeader.nonce);
        string hash = HashFunction(arg);
        bool mined = true;
        for (int i = 0; i < mHeader.difficultyTarget; i++) {
            if (hash[i]!='0') mined = false;
        }
        if (mined == true) {
            mHeader.hash = hash;
            mHeader.timestamp = GetCurrentTimeStamp();
            auto miningEnd = std::chrono::high_resolution_clock::now(); 
            mHeader.miningTime = duration_cast<std::chrono::milliseconds>(miningEnd - miningStart);
            break;
        }
        mHeader.nonce++;
        if (mHeader.nonce % 1000000 == 0) getLogger().Log("(Block #" + blockNumber + "): " + to_string(mHeader.nonce) + " hashes checked...");
    }
    
    getLogger().Log("Block #" + blockNumber + " mined with nonce: " + to_string(mHeader.nonce) + ", Hash: " + mHeader.hash);
    return true;
}
```
- Hashes the block header and nonce  
- When the hash satisfies the mining difficulty (starts with the required number of zeros), mining finishes  
### Block addition and validation:
```c++
void Blockchain::validateAndAddBlock(Block &BlockToAdd){
    string blockHeight = to_string(BlockToAdd.getHeight());

    // check if transactions are in mempool
    for (auto tx : BlockToAdd.getTransactions()) {
        if(tx.getSender() == "Block Reward") continue;
        if (memPool.find(tx.getHash()) == memPool.end()) {
            getLogger().Log("Block #" + blockHeight + " denied, reason: Txs are not present in mempool");
            return;
        }
    }

    //check merkle root
    if(BlockToAdd.getHeader().merkleRootHash != MerkleRootHash(BlockToAdd.getTransactions())) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: MerkleRoot differs");
        return;
    }

    // check if the block has correct previous block hash
    if (BlockToAdd.getHeader().prevBlockHash != this->getLastBlock().getHash()) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Previous block hash does not match");
        return;
    }

    //check block hash
    if (BlockToAdd.getHash().empty()) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Hash is empty");
        return;
    }

    // check if hash is correct (according to proof of work)
    bool valid = true;
    for (int i = 0; i < difficulty; i++) {
        if (BlockToAdd.getHash()[i]!='0') valid = false;
    }
    if (!valid) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Hash does not match difficulty");
        return;
    }

    //check if hash is correct
    if (BlockToAdd.getHash() != HashFunction(BlockToAdd.getHeader().ToString() + std::to_string(BlockToAdd.getHeader().nonce))) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Hash is not correct");
        return;
    };


    this->ExecuteTransactions(BlockToAdd.getTransactions());
    blockList.push_back(BlockToAdd);
    getLogger().Log("Block #" + to_string(BlockToAdd.getHeight()) + " added to blockchain!");
    getLogger().Log("User \"" + users.at(BlockToAdd.getTransactions().at(0).getReceiver()).getName() +
    "\" got the reward!");

    if (BlockToAdd.getMiningTime().count() > 10000) {
        this->difficulty-=1;
        getLogger().Log("Difficulty decreased by 1");
    }
    if (BlockToAdd.getMiningTime().count() < 1000){
        getLogger().Log("Difficulty increased by 1");
        this->difficulty+=1;
    }
}
```
- Checks whether the block’s transactions exist in the mempool  
- Checks the Merkle root hash of all transactions  
- Checks the previous block hash  
- Checks whether the block hash exists  
- Checks whether the block hash satisfies the difficulty  
- Checks whether the hash is correct  
- Then executes transactions, outputs information, and adjusts difficulty (if needed)  
### Adding a transaction to the mempool:
```c++
bool Blockchain::addTransactionToMempool(const Transaction &transactionToAdd){

    // check if users exists
    if (users.find(transactionToAdd.getSender()) == users.end() ||
        users.find(transactionToAdd.getReceiver()) == users.end()) {
        getLogger().Log("Tx " + transactionToAdd.getHash() + " denied. Reason: users not found");
        return false;
    }

    if (transactionToAdd.getAmount() <=0) {
        return false;
    }

    // check if user has enough to send
    if(transactionToAdd.getAmount() > users.at(transactionToAdd.getSender()).getPendingBalance()) {
        getLogger().Log("Tx " + transactionToAdd.getHash() + " denied. Reason: not enough balance");
        return false;
    }

    // check hash
    if (transactionToAdd.getHash()!=
        HashFunction(transactionToAdd.getTimeStamp() + transactionToAdd.getSender() + transactionToAdd.getReceiver() + std::to_string(transactionToAdd.getAmount()) )) {
        getLogger().Log("Tx " + transactionToAdd.getHash() + " denied. Reason: hashes doesnt match");
        return false;
    }

    //insert
    memPool.insert({transactionToAdd.getHash(), transactionToAdd});
    users.at(transactionToAdd.getSender()).setPendingBalance(users.at(transactionToAdd.getSender()).getPendingBalance() - transactionToAdd.getAmount());
    users.at(transactionToAdd.getReceiver()).setPendingBalance(users.at(transactionToAdd.getReceiver()).getPendingBalance() + transactionToAdd.getAmount());
    // insert into sorted vector
    auto amountToInsert = transactionToAdd.getAmount();
    auto pos = std::lower_bound(
        sortedTransactionHashes.begin(),
        sortedTransactionHashes.end(),
        amountToInsert,
        [this](const string& existingHash, int amount) -> bool{
        return memPool.at(existingHash).getAmount() < amount;
    });
    sortedTransactionHashes.insert(pos, transactionToAdd.getHash());

    getLogger().Log("Tx " + transactionToAdd.getHash() + " successfully added to mempool.");
    return true;
}
```
- Checks whether users exist  
- Checks the transferred bitcoin amount  
- Checks the sender’s balance  
- Checks the transaction hash  
- Then inserts the transaction into the hash set and vector  
- Updates users’ pending balances (temporary balances)  
### MerkleRootHash:
```c++
string MerkleRootHash(const vector<Transaction> &transactions) {
    // return empty if no transactions
    if (transactions.size() == 0) return "";

    //put transaction hashes into vector
    vector<string> hashes;
    hashes.reserve(transactions.size());
    for (const auto &transaction : transactions) {
        hashes.push_back(transaction.getHash());
    }
    
    while (hashes.size() > 1) {
        if (hashes.size() % 2 != 0) hashes.push_back(hashes.back());
        vector<string> newHashes;
        newHashes.reserve((hashes.size()+1)/2);
        for (size_t i = 0; i < hashes.size() - 1; i+=2) {
            string newHash = HashFunction(hashes.at(i) + hashes.at(i+1));
            newHashes.push_back(newHash);
        }
        hashes = newHashes;
    }
    return hashes.at(0);
}
```
- Hashes the transaction hash array in pairs and places them into a new transaction array  
- Replaces the old array with the new one  
- Repeats until the array contains only one transaction  
- If the number of transactions is odd, the last transaction is hashed together with its copy  
### Parallel mining (implemented using OpenMP):
```c++
void Case2(Blockchain &Btc) {
    int counter = 0;
    vector<User> miners;
    for (auto user : Btc.getUsers()) {
        miners.push_back(user.second);
        getLogger().Log(user.second);
        counter++;
        if (counter == 5 ) break;
    }

#pragma omp parallel for
    for (int i = 0; i < 5; i++) {
        Block block(Btc, miners.at(i));
        block.Mine5secs();
        getLogger().Log(block);
        Btc.validateAndAddBlock(block);
    }
}
```
- Five miners are selected  
- They simultaneously create blocks and mine them for 5 seconds, then attempt to attach them to the blockchain  
- If the blocks are not mined, they will not be allowed to attach them  
- If someone mines a block and adds it, others may also mine theirs, but they will not be able to add them or receive the reward (because their block will fail validation)  

