-module(skerl).

-export([init/2,
         update/3,
         final/2,
         hash/3]).

-on_load(init/0).

-ifdef(TEST).
-include_lib("eunit/include/eunit.hrl").
-endif.

init() ->
    case code:priv_dir(skerl) of
        {error, bad_name} ->
            SoName = filename:join("../priv", skerl);
        Dir ->
            SoName = filename:join(Dir, skerl)
    end,
    erlang:load_nif(SoName, 0).

init(Ref, Bits) ->
    "NIF library not loaded".

update(Ref, State, Data) ->
    "NIF library not loaded".

final(Ref, State) ->
    "NIF library not loaded".
    
hash(Ref, Bits, Data) ->
    "NIF library not loaded".

%% ===================================================================
%% EUnit tests
%% ===================================================================
-ifdef(TEST).

basic_test() ->
    {ok, Ref} = new(),
    ok = init(Ref).

-endif.
