

/* Enable Assert checks for this code */
#define SKEIN_ERR_CHECK 1
#include <crypto/SkeinMACApi.h>

#include <string.h>

int skeinMacCtxPrepare(SkeinMacCtx_t* ctx, SkeinSize_t size)
{
    Skein_Assert(ctx && size, SKEIN_FAIL);

    memset(ctx ,0, sizeof(SkeinMacCtx_t));
    ctx->skeinSize = size;

    return SKEIN_SUCCESS;
}

int skeinMacInit(SkeinMacCtx_t* ctx, const uint8_t *key, size_t keyLen,
                 size_t hashBitLen)
{
    int ret = SKEIN_FAIL;
    u64b_t*  X = NULL;
    size_t Xlen = 0;

    Skein_Assert(ctx, SKEIN_FAIL);

    switch (ctx->skeinSize) {
    case Skein256:
        X = ctx->m.s256.X;
        Xlen = sizeof(ctx->m.s256.X);
        break;
    case Skein512:
        X = ctx->m.s512.X;
        Xlen = sizeof(ctx->m.s512.X);
        break;
    case Skein1024:
        X = ctx->m.s1024.X;
        Xlen = sizeof(ctx->m.s1024.X);
        break;
    default:
        return SKEIN_FAIL;
    }
    /* Check if we shall re-use the context (saved chaining variables) */
    if (!key && !keyLen && !hashBitLen) {
        /* Restore the chaing variable, reset byte counter */
        memcpy(X, ctx->XSave, Xlen);

        /* Setup context to process the message */
        Skein_Start_New_Type(&ctx->m, MSG);
        return SKEIN_SUCCESS;
    }
    Skein_Assert(key && keyLen, SKEIN_FAIL);
    Skein_Assert(hashBitLen, SKEIN_BAD_HASHLEN);

    switch (ctx->skeinSize) {
    case Skein256:
        ret = Skein_256_InitExt(&ctx->m.s256, hashBitLen,
                                SKEIN_CFG_TREE_INFO_SEQUENTIAL,
                                (const u08b_t*)key, keyLen);

        break;
    case Skein512:
        ret = Skein_512_InitExt(&ctx->m.s512, hashBitLen,
                                SKEIN_CFG_TREE_INFO_SEQUENTIAL,
                                (const u08b_t*)key, keyLen);
        break;
    case Skein1024:
        ret = Skein1024_InitExt(&ctx->m.s1024, hashBitLen,
                                SKEIN_CFG_TREE_INFO_SEQUENTIAL,
                                (const u08b_t*)key, keyLen);

        break;
    }
    if (ret == SKEIN_SUCCESS) {
        /* Save chaining variables for this combination of key, keyLen, hashBitLen */
        memcpy(ctx->XSave, X, Xlen);
    }
    return ret;
}

int skeinMacUpdate(SkeinMacCtx_t *ctx, const uint8_t *msg,
                   size_t msgByteCnt)
{
    Skein_Assert(ctx, SKEIN_FAIL);

    switch (ctx->skeinSize) {
    case Skein256:
        return Skein_256_Update(&ctx->m.s256, (const u08b_t*)msg, msgByteCnt);
    case Skein512:
        return Skein_512_Update(&ctx->m.s512, (const u08b_t*)msg, msgByteCnt);
    case Skein1024:
        return Skein1024_Update(&ctx->m.s1024, (const u08b_t*)msg, msgByteCnt);
    }
    return SKEIN_FAIL;
}

int skeinMacFinal(SkeinMacCtx_t* ctx, uint8_t* macVal)
{
    Skein_Assert(ctx, SKEIN_FAIL);

    switch (ctx->skeinSize) {
    case Skein256:
        return Skein_256_Final(&ctx->m.s256, (u08b_t*)macVal);
    case Skein512:
        return Skein_512_Final(&ctx->m.s512, (u08b_t*)macVal);
    case Skein1024:
        return Skein1024_Final(&ctx->m.s1024, (u08b_t*)macVal);
    }
    return SKEIN_FAIL;
}
