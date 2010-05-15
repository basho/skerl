{application, skerl,
 [
  {description, "Skein hash function NIF"},
  {vsn, "0.1"},
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