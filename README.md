## Pseudo-kodas:
```c++
string HashFunction(string input) {
    const int HASH_LENGTH = 64;
    list<int> values;
    long int uniqueStringNumber = 1;
    int counter = 0;

    // Convert input into integer values and update uniqueNumber
    for each character c in input {
        int val = integer value of c (absolute);
        add val to values;

        update uniqueStringNumber based on counter and val;
        counter = counter + 1;
    }

    // Initialize random generator with uniqueStringNumber
    initialize random generator using uniqueStringNumber;

    int hash[HASH_LENGTH] = {0};

    // Distribute input values into hash
    for each val in values {
        int idx = random index between 0 and HASH_LENGTH-1;
        hash[idx] = hash[idx] + val;
    }

    // Add randomness and reduce values modulo 16
    for i from 0 to HASH_LENGTH-1 {
        int offset = random number between 0 and 15;
        hash[i] = (hash[i] + offset) mod 16;
    }

    // Convert result to hex string
    string hexstr = "";
    for i from 0 to HASH_LENGTH-1 {
        append hex representation of hash[i] to hexstr;
    }

    return hexstr;
}

```
### Kaip veikia:
1. Kiekvienas input'o simbolis konvertuojamas į skaičių ir saugomas. Taip pat pagal algoritmą generuojamas uniqueStringNumber
2. Inicializuojamas random generatorius naudojant uniqueStringNumber
3. Kiekvienas išsaugotas simbolio skaičius pridedamas i atsitiktinią vietą hash'e
4. Prie kiekvieno hash'o skaičiaus pridedama atsitiktinė reikšmė
5. Randamas likutis dalijant iš 16 (hex'ui)
6. Viskas konvertuojama i string'ą ir grąžinama

## Eksperimentinis tyrimas:
### 1. Testiniai failai <br /> 
#### Buvo sukurti testiniai failai: <br /> <img width="1800" height="1138" alt="image" src="https://github.com/user-attachments/assets/d02aed22-e689-46c2-9c5f-f707c7184a97" />
### 2. Išvedimo dydis <br />
#### Failai suhash'inti (matosi, kad hash'ai vienodo ilgio): <br /> <img width="1326" height="659" alt="image" src="https://github.com/user-attachments/assets/daaf8964-bf64-4531-8eb5-ce3293c9751d" />
### 3. Deterministiškumas matomas: <br /> <img width="929" height="234" alt="image" src="https://github.com/user-attachments/assets/0079a7ea-a2ab-4b7c-b51d-aee0cead0875" />
### 4. Sparta <br />
#### Konstitucija.txt failo skirtingi eilučių kiekiai buvo suhashinti: <img width="795" height="384" alt="image" src="https://github.com/user-attachments/assets/40ef1382-f28d-4c34-808b-19e20a994a0e" />
(čia yra pateiktas vidurkis iš 5 hash'ų)<br />
<img width="727" height="451" alt="image" src="https://github.com/user-attachments/assets/8bbd3ded-ec0c-4d55-9686-dd5359d32587" /><br />
### 5. Kolizijos <br />
#### Buvo sukurti po 100 000 random string poru:<br />
<img width="1469" height="665" alt="image" src="https://github.com/user-attachments/assets/5f12e85b-9777-46f5-aa3b-d3a2bcccd08e" /><br />
(eina pora po 10 simboliu, po to pora po 100 simboliu, po to pora po 500 ir pora po 1000, po to vel po 10 simboliu ir t. t.)<br />
### Koliziju rasta:<br />
<img width="822" height="92" alt="image" src="https://github.com/user-attachments/assets/6251a70b-f67a-4ae9-832c-b1d7e8450a4d" /><br />
### 6. Lavinos efektas: <br />
#### Buvo sukurti po 100 000 skirtingiu ilgiu stringu poros, kur skiriasi pirmi simboliai:<br />
<img width="1583" height="655" alt="image" src="https://github.com/user-attachments/assets/5440b5d7-c6fd-4b35-ad6e-daf2eec6a262" /><br />
### Panašumai (procentais):<br />
<img width="706" height="377" alt="image" src="https://github.com/user-attachments/assets/fcc72ca5-ee17-4adc-b91c-a69f6cdb3738" /><br />
### 7. Negrižtamumo demonstacija:<br />
<img width="840" height="180" alt="image" src="https://github.com/user-attachments/assets/eab67981-1658-4a67-b455-f1f5136282e1" /> <br />
## Išvada: 
### Stiprumai:
- Hashas veikia
- Yra deterministiškumas
- Labai mažai kolizijų
- Veikia greitai (suhashinti 512 eilutes reikėjo 2ms)
- Lavinos effektas: Hashai panašūs vidutiniškai 16%
### Silpnumai:
- Yra maža tikimybė gauti koliziją (stringai turi skirtis vienu simbolių ant 16 vienetų (pagal ascii lentelę), ir dar turi būti toks random generatoriaus seed'as, kuris nepridės prie to simbolio nieko.
## Palyginimas su SHA256:
### Buvo panaudota openssl biblioteka SHA256 funkcijai: <br />
```c++
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

std::string sha256(const std::string &input) {
    // Create context
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        // handle error
        return "";
    }

    // Initialize digest: SHA-256
    if (1 != EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
        // handle error
        EVP_MD_CTX_free(ctx);
        return "";
    }

    // Update with data
    if (1 != EVP_DigestUpdate(ctx, input.data(), input.size())) {
        // handle error
        EVP_MD_CTX_free(ctx);
        return "";
    }

    // Finalize digest
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    if (1 != EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
        // handle error
        EVP_MD_CTX_free(ctx);
        return "";
    }

    // Clean up
    EVP_MD_CTX_free(ctx);

    // Convert hash bytes to hexadecimal string
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < hash_len; ++i) {
        oss << std::setw(2) << static_cast<int>(hash[i]);
    }
    return oss.str();
}
```
### Su SHA256 buvo atlikti tie pat tyrimai (spartos su konstitucija.txt, kolizijų paieška, lavinos efekto patikrinimas):
### 1. Sparta: <br />
<img width="808" height="407" alt="image" src="https://github.com/user-attachments/assets/91b01db4-ea65-463d-8d6d-2579bcf4281b" /> <br />
<img width="726" height="453" alt="image" src="https://github.com/user-attachments/assets/f8f009d5-f946-448f-bed8-2d9308d7455f" /> <br />
Matosi, kad SHA256 veikia žymiai greičiau. <br />
### 2. Kolizijos: <br />
<img width="834" height="95" alt="image" src="https://github.com/user-attachments/assets/3818f1b9-2eca-43a5-b708-4e56ac754f27" /> <br />
Kolizijų nėra (kaip ir pas mane) <br />
### 3. Lavinos efektas: <br />
<img width="703" height="363" alt="image" src="https://github.com/user-attachments/assets/f62d78e7-1309-48ba-98f7-2919521ba264" /> <br />
Lavinos efektas geriau matomas SHA256. Hexo lygmeniu : vidutinis panašumas 9% mažiau negu pas mane, maksimalus panašumas: 25% (pas mane 100%). Bito lygmeniu min ir avg reikšmės panašios, skiriasi tik maksimalus panašumas.<br />
### Išvada:
SHA256 veikia geriau ir greičiau.







