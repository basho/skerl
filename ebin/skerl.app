{application, skerl,
 [
  {description, "Skein hash function NIF"},
  {vsn, "1.0.1"},
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
