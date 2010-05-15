-module(skerl_tests).

-include_lib("eunit/include/eunit.hrl").

eight_bit_input_test() ->
  ?assertEqual(
    {ok, hex:hexstr_to_bin("42AA6BD9CA92E90EA28DF6F6F" ++
      "2D0D9B85A2D1907EE4DC1B171ACE7EB1159BE3BD1" ++
      "BC56586D92492B6EFF9BE03306994C65A332C4C24" ++
      "160F46655040E558E8329")}, 
    skerl:hash(512, <<255>>)).

fivetwelve_bit_input_test() ->
  ?assertEqual(
    {ok, hex:hexstr_to_bin("04F96C6F61B3E237A4FA7755EE" ++
      "4ACF34494222968954F495AD147A1A715F7A73EBECFA1EF" ++
      "275BED87DC60BD1A0BC602106FA98F8E7237BD1AC0958E7" ++
      "6D306678")},
    skerl:hash(512, hex:hexstr_to_bin("FFFEFDFCFBFAF9F" ++
      "8F7F6F5F4F3F2F1F0EFEEEDECEBEAE9E8E7E6E5E4E3E2E1" ++
      "E0DFDEDDDCDBDAD9D8D7D6D5D4D3D2D1D0CFCECDCCCBCAC" ++
      "9C8C7C6C5C4C3C2C1C0"))).
      
