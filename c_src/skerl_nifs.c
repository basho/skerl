
#include "erl_nif.h"
#include "skein_api.h"

static ErlNifResourceType* skein_hashstate;

typedef struct
{
} skein_handle;

// Prototypes
ERL_NIF_TERM skein_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM skein_update(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM skein_final(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
ERL_NIF_TERM skein_hash(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
// lifecycle
int load(ErlNifEnv* env, void ** priv_data, ERL_NIF_TERM load_info);

static ErlNifFunc nif_funcs[] =
{
    {"init", 1, skein_init},
    {"update", 2, skein_update},
    {"final", 1, skein_final},
    {"hash", 2, skein_hash}
};
	
ERL_NIF_INIT(skerl, nif_funcs, load, NULL, NULL, NULL);
	
static char *hash_return_strings[] = {"success", "fail", "bad_hashlen"};

int load(ErlNifEnv* env, void ** priv_data, ERL_NIF_TERM load_info)
{
  skein_hashstate = enif_open_resource_type(env, "hashstate", enif_free, ERL_NIF_RT_CREATE, NULL);
  return 0;
}

ERL_NIF_TERM skein_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{   
  ERL_NIF_TERM hash_state_term;
    int bits = 0;
    if(!enif_get_int(env, argv[0], &bits))
		return enif_make_badarg(env);
    
    hashState *state = (hashState*) enif_alloc_resource(env, skein_hashstate, sizeof(hashState));
    HashReturn r = Init(state, bits);
    if (r == SUCCESS) {
        hash_state_term = enif_make_resource(env, state);
        enif_release_resource(env, state);
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), hash_state_term);
    } else {
        enif_release_resource(env, state);
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, "fail"));
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
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_resource(env, state));
    } else {
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, hash_return_strings[r]));
    }
}

ERL_NIF_TERM skein_final(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    hashState *state = NULL;
    enif_get_resource(env, argv[0], skein_hashstate, (void**)&state);
    
    ErlNifBinary out;
    enif_alloc_binary(env, (size_t)(state->statebits/8), &out);
    
    HashReturn r = Final(state, (BitSequence *)out.data);
    if (r == SUCCESS) {
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_binary(env, &out));
    } else {
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, hash_return_strings[r]));
    }
}

ERL_NIF_TERM skein_hash(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int bits = 0;
    enif_get_int(env, argv[0], &bits);
    
    ErlNifBinary bin, out;
    enif_inspect_binary(env, argv[1], &bin);
    enif_alloc_binary(env, (size_t)(bits/8), &out);
    
    HashReturn r = Hash(bits, (BitSequence *)(bin.data), bin.size * 8, (BitSequence *)out.data);
    if (r == SUCCESS) {
        return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_binary(env, &out));
    } else {
        return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_atom(env, hash_return_strings[r]));
    }   
}
