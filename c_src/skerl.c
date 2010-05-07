
#include "erl_nif.h"
#include "skein_api.h"

static ErlNifResourceType* skein_RESOURCE;
static ErlNifResourceType* skein_hashstate;
static ErlNifResourceType* skein_hashval;

typedef struct
{
} skein_handle;

// Prototypes
ERL_NIF_TERM skein_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM skein_update(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM skein_final(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM skein_hash(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

static ErlNifFunc nif_funcs[] =
{
    {"init", 1, skein_init},
    {"update", 2, skein_update},
    {"final", 1, skein_final},
    {"hash", 2, skein_hash}
};
	
static char *hash_return_strings[] = {"success", "fail", "bad hashlen"};

ERL_NIF_TERM skein_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{   
    int bits = 0;
    enif_get_int(env, argv[0], &bits);
    
    hashState *state = enif_alloc_resource(env, skein_hashstate, sizeof(hashState));
    HashReturn r = Init(state, bits);
    if (r == SUCCESS) {
        enif_make_resource(env, state);
        enif_release_resource(env, state);
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), state);
    } else {
        enif_release_resource(env, state);
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, hash_return_strings[r]));
    }
}

ERL_NIF_TERM skein_update(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    hashState *state = NULL;
    enif_get_resource(env, argv[0], skein_hashstate, (void**)&state);

    ErlNifBinary bin;
    enif_inspect_binary(env, argv[1], &bin);
    
    HashReturn r = Update(state, (BitSequence *)(bin.data), bin.size * 8);
    if (r == SUCCESS) {
        enif_release_resource(env, state);
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), state);
    } else {
        enif_release_resource(env, state);
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, hash_return_strings[r]));
    }
}

ERL_NIF_TERM skein_final(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    hashState *state = NULL;
    enif_get_resource(env, argv[0], skein_hashstate, (void**)&state);
    
    BitSequence *hashval = enif_alloc_resource(env, skein_hashval, sizeof(state->statebits / 8));
    HashReturn r = Final(state, hashval);
    if (r == SUCCESS) {
        enif_make_resource(env, hashval);
        enif_release_resource(env, hashval);
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), hashval);
    } else {
        enif_release_resource(env, hashval);
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, hash_return_strings[r]));
    }
}

ERL_NIF_TERM skein_hash(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int bits = 0;
    enif_get_int(env, argv[0], &bits);
    
    ErlNifBinary bin;
    enif_inspect_binary(env, argv[1], &bin);
    
    BitSequence *hashval = enif_alloc_resource(env, skein_hashval, sizeof(bits / 8));
    HashReturn r = Hash(bits, (BitSequence *)(bin.data), bin.size * 8, hashval);
    if (r == SUCCESS) {
        enif_make_resource(env, hashval);
        enif_release_resource(env, hashval);
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), hashval);
    } else {
        enif_release_resource(env, hashval);
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, hash_return_strings[r]));
    }   
}

static void skein_resource_cleanup(ErlNifEnv* env, void* arg)
{
    // Delete any dynamically allocated memory stored in skein_handle
    // skein_handle* handle = (skein_handle*)arg;
}

static int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
{
    skein_RESOURCE = enif_open_resource_type(env, "skein_resource",
                                                  &skein_resource_cleanup,
                                                  ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER,
                                                  0);
    return 0;
}

ERL_NIF_INIT(skein, nif_funcs, &on_load, NULL, NULL, NULL);
