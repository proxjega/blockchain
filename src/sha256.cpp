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
