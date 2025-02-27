#define BASE62_ALPHABET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define BASE62_LENGTH 62

void encode_base62(unsigned long num, char *short_url) {
    char temp[12];
    int i = 0;

    do {
        temp[i++] = BASE62_ALPHABET[num % BASE62_LENGTH];
        num /= BASE62_LENGTH;
    } while (num > 0);

    int j = 0;
    while (i > 0) {
        short_url[j++] = temp[--i];
    }
    short_url[j] = '\0';
}
