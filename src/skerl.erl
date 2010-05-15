-module(skerl).
-author('b@fastip.com').

-export([init/0,
         init/1,
         update/2,
         final/1,
         hash/2]).

-on_load(init/0).

-ifdef(TEST).
-include_lib("eunit/include/eunit.hrl").
-endif.

init() ->
    case code:priv_dir(skerl) of
        {error, bad_name} ->
            SoName = filename:join("../priv", skerl_nifs);
        Dir ->
            SoName = filename:join(Dir, skerl_nifs)
    end,
    erlang:load_nif(SoName, 0).

init(Bits) ->
    "NIF library not loaded".

update(State, Data) ->
    "NIF library not loaded".

final(State) ->
    "NIF library not loaded".
    
hash(Bits, Data) ->
    "NIF library not loaded".

