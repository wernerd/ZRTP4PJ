/*
  Copyright (C) 2010 Werner Dittmann

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <crypto/macSkein.h>
#include <stdlib.h>

void macSkein(uint8_t* key, int32_t key_length,
               const uint8_t* data, uint32_t data_length,
               uint8_t* mac, int32_t mac_length, SkeinSize_t skeinSize)
{
    SkeinMacCtx_t ctx;
    
    skeinMacCtxPrepare(&ctx, skeinSize);
    
    skeinMacInit(&ctx, key, key_length, mac_length);
    skeinMacUpdate(&ctx, data, data_length);
    skeinMacFinal(&ctx, mac);
}

void macSkein(uint8_t* key, int32_t key_length,
               const uint8_t* data[], uint32_t data_length[],
               uint8_t* mac, int32_t mac_length, SkeinSize_t skeinSize)
{
    SkeinMacCtx_t ctx;
    
    skeinMacCtxPrepare(&ctx, skeinSize);

    skeinMacInit(&ctx, key, key_length, mac_length);
    while (*data) {
        skeinMacUpdate(&ctx, *data, *data_length);
        data++;
        data_length ++;
    }
    skeinMacFinal(&ctx, mac);
}

void* createSkeinMacContext(uint8_t* key, int32_t key_length, 
                            int32_t mac_length, SkeinSize_t skeinSize)
{
    SkeinMacCtx_t* ctx = (SkeinMacCtx_t*)malloc(sizeof(SkeinMacCtx_t));

    skeinMacCtxPrepare(ctx, skeinSize);
    skeinMacInit(ctx, key, key_length, mac_length);
    return ctx;
}

void macSkeinCtx(void* ctx, const uint8_t* data, uint32_t data_length,
                uint8_t* mac)
{
    SkeinMacCtx_t* pctx = (SkeinMacCtx_t*)ctx;
    
    skeinMacInit(pctx, NULL, 0, 0);
    skeinMacUpdate(pctx, data, data_length);
    skeinMacFinal(pctx, mac);
}

void macSkeinCtx(void* ctx, const uint8_t* data[], uint32_t data_length[],
                uint8_t* mac)
{
    SkeinMacCtx_t* pctx = (SkeinMacCtx_t*)ctx;
    
    skeinMacInit(pctx, NULL, 0, 0);
    while (*data) {
        skeinMacUpdate(pctx, *data, *data_length);
        data++;
        data_length++;
    }
    skeinMacFinal(pctx, mac);
}

void freeSkeinMacContext(void* ctx)
{
    if (ctx)
        free(ctx);
}
