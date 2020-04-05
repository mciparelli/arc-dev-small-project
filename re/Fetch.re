open Js.Promise;

[@bs.deriving {abstract: light}]
type headers = {
  [@bs.as "Content-Type"]
  contentType: string,
  [@bs.optional] [@bs.as "X-Access-Token"]
  token: string,
};

module Options = {
  [@bs.deriving {abstract: light}]
  type t = {
    [@bs.optional] [@bs.as "method"]
    method_: string,
    [@bs.optional]
    body: string,
    [@bs.optional]
    headers,
  };
};

[@bs.val] external fetch: (string, Options.t) => Js.Promise.t('b) = "fetch";

let make =
    (
      ~url,
      ~body=?,
      ~method_=body->Belt.Option.isSome ? "post" : "get",
      ~token=?,
      (),
    ) => {
  let options =
    Options.t(
      ~method_,
      ~body=?{
        switch (body) {
        | None => None
        | Some(body) => body->Js.Json.stringify->Some
        };
      },
      ~headers=headers(~contentType="application/json", ~token?, ()),
      (),
    );

  fetch(Urls.api ++ url, options)->then_(res => res##json()->resolve, _);
};

let delete = (~url, ~token) => {
  let options =
    Options.t(
      ~method_="delete",
      ~headers=headers(~contentType="application/json", ~token, ()),
      (),
    );
  fetch(Urls.api ++ url, options);
};