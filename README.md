# Bloku grandine
Cia yra mano bloku grandine. 
- Realizuota naudojant account model
- Yra transakciju ir bloku pridejimo validavimas
- Bloku kasimo sunkumas keiciasi priklausomai nuo kasimo laiko
- Uz bloku kasima yra atlygis (50 btc) 
### Bloku grandine realizuote per kelias klases:
## Blockchain klase:
```c++
class Blockchain {
    private:
        list<Block> blockList;
        unordered_map<string, User> users;
        unordered_map<string, Transaction> memPool;
        vector<string> sortedTransactionHashes;

        int difficulty;
        // private methods (are called only by blockchain itself)
        void ExecuteTransactions(const vector<Transaction> &transactions); //TODO
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
        void validateAndAddBlock(Block &BlockToAdd); // TODO
};
```
### Pagrindine bloku grandines klase turi tokius private narius:
- list<Block> blockList - bloku sarasas
- users - naudotoju hashsetas 
- memPool - transakciju hashsetas
- sortedTransactionHashes - surusiuotas transakciju vektorius (surusiotas pagal perduotu bitkoinu kieki)
- difficulty - kasinimo sunkumas 
### Privatus metodai: 
- ExecuteTransactions - tik bloku grandine gali ivykdyti transakcijas
- GenerateUsers - naudotoju generavimas
- GenerateMemPool - transakciju generavimas
### Naudotojo metodai:
- addTransaction - prideti transakcija (transakcija yra validuojama pries pridedant)
- addUser - prideti naudotoja
- validateAndAddBlock - pabandyti prideti savo bloka i bloku grandine (blokas yra validuojamas pries pridedant)
### Taip pat klase turi konstruktoriu (apie ji zemiau), getterius ir setterius
### Konstruktorius:
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
- Pasirenka sunkuma (3)
- Sukuria naudotojus ir transakcijas 
- Sukuria Satoshi user
- Sukuria genesis bloka
- Ivykdo genesis bloko transakcijas (viena transakcija nuo block reward -> satoshi)
- Ideda genesis bloka i bloku grandine
## Block klase:
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
- Blokas sudarytas is headerio ir transakciju vektoriaus
- Blokas sudaromas per konstruktoriu Block(const Blockchain &blockchain, User &miner), kur i ji dedama 100 vertingiausiu transakciju, ir taip pat kasinimo transakcija (Block Reward -> Miner, 50 btc)
- Bloka galima kasti
- Neiskasta bloka nepavyks prideti prie bloku grandines ir gauti reward (50 btc)
## User klase:
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
- Turi varda, viesaji rakta, balansa ir pending balansa
- Balansas - realus balansas dabar bloku grandineje
- Pending balansas - balansas, iskaitant transakcijas, kurios buvo pridetos prie mempool'o, bet nebuvo iskastos
## Bloko kasimas:
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
- Hashuoja bloko headeri ir nonce
- Kai hashas atitinka kasimo sunkumui (prasideda reikalingu nuliu kiekiu) - baigia kasima
## Bloko pridejimas ir validavimas:
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
- Tikrina ar transakcijos bloke yra mempoole
- Tikrina visu transakciju merkleroot hash'a
- Tikrina ankstesnio bloko hasha
- Tikrina ar egzistuoja sito bloko hashas
- Tikrina ar atitinka bloko hashas sunkumui
- Tikrina ar hashas teisingas
- Tada ivykdo transakcijas, isveda informacija, ir keicia sunkuma (jei reikia)
## Transakcijos pridejimas prie mempoolo:
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
- Tikrina ar egzistuoja naudotojai
- Tikrina perduotu bitkoinu kieki
- Tikrina siuntejo balansa
- Tikrina transakcijos hasha
- Tada ideda transakcija i hashseta ir vektoriu
- Keicia naudotoju pending balansus (laikini balansai)
## MerkleRootHash:
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
- Hashuoja transakciju hashu masyva poromis ir ideda i nauja transakciju masyva
- Keicia sena masyva i nauja
- Kartoja kol masyve yra daugiau nei 1 transakcija
- Jeigu transakciju kiekis yra nelyginis, paskutine transakcija yra hashuojama poroje su juos kopija
## Lygiagretus kasimas (realizuotas naudojant OpenMP):
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
- Imami 5 miner'iai
- Jie kartu sukuria blokus ir minina po 5 sekundes ir po to bando prijungti prie bloku grandines
- Jei blokai neiskasti - jiems neleis prijungti juos
- Jei kazkas iskase bloka ir pridejo - kiti gali irgi ji iskasti, bet prideti ir gauti rewarda negales (nes ju blokas nepraeis validavimo)
## AI
### AI pagalba buvo naudota tokiems zingzniams:
- Architekturos klausimams (kas sukuria blokus, kas validuoja, kaip transakcijos validuojamos, kaip apsisaugoti nuo double spending attack)
- Viesu raktu generavimo funkcijai
- Hash funkcijos pagerinimas kasimui
- Pagalbai su std::chrono biblioteka
- OpenMp instaliavimui
- Protected reference getteriu sukurimui
