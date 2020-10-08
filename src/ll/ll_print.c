#include <fp256/fp256_ll.h>
#include <stdio.h>

/* TODO */
int ll_print_hex(const u64 *rd, size_t rl)
{
    u8 *hex;
    size_t len;

    len = 1 + rl * sizeof(u64) * 2;
    hex = malloc(len);
    hex[len - 1] = '\0';
    ll_to_hex(hex, NULL, rd, rl);
    printf("%s\n", hex);

    free(hex);
    return FP256_OK;
}

void print_hex(const char *desp, const unsigned char *s, size_t slen)
{
    size_t i;

    for(i = 0; i < strlen(desp); i++)
        printf("%c", desp[i]);

    unsigned char *hex = (unsigned char*)malloc(2*slen);
    u8_to_hex(hex, s, slen);
    for(i = 0; i < 2*slen; i++)
        printf("%c", hex[i]);
    printf("\n");
    free(hex);
}