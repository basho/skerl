{application, skerl,
 [
  {description, "Skein hash function NIF"},
  {vsn, "1.1.0"},
  {modules, [
             skerl,
             hex
            ]},
  {registered, []},
  {applications, [
                  kernel,
                  stdlib
                 ]},
  {env, []}
 ]}.
