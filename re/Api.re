open Types;

let getJwt = () => {
  switch (Storage.get("refresh_token")) {
  | None => None->Promise.resolve
  | Some(refreshToken) =>
    let%Promise response =
      Fetch.make(
        ~url="/access-tokens/refresh",
        ~body=
          Js.Dict.fromList([("refresh_token", refreshToken->Js.Json.string)])
          ->Js.Json.object_,
        (),
      );
    response->RefreshJwtResponse.jwt->Some;
  };
};

let getUser = token => Fetch.make(~url="/me", ~token, ());

let signup = (~name, ~email, ~password) => {
  let%Promise res =
    Fetch.make(
      ~url="/users",
      ~body=
        Js.Dict.fromList([
          ("name", name->Js.Json.string),
          ("email", email->Js.Json.string),
          ("password", password->Js.Json.string),
        ])
        ->Js.Json.object_,
      (),
    );
  SignupResponse.(
    switch (res->reason) {
    | None =>
      Storage.set("refresh_token", res->refreshToken);
      res->jwt->Ok;
    | Some(reason) => reason->Error
    }
  );
};

let logout = jwt => {
  switch (jwt, Storage.get("refresh_token")) {
  | (Some(_jwt), Some(refreshToken)) =>
    let%Promise _response =
      Fetch.make(
        ~url="/access-tokens",
        ~method_="delete",
        ~body=
          Js.Dict.fromList([("refresh_token", refreshToken->Js.Json.string)])
          ->Js.Json.object_,
        (),
      );
    Storage.remove("refresh_token");
  | _ => Promise.resolve()
  };
};

let login = (~email, ~password) => {
  let%Promise response =
    Fetch.make(
      ~url="/access-tokens",
      ~body=
        Js.Dict.fromList([
          ("email", email->Js.Json.string),
          ("password", password->Js.Json.string),
        ])
        ->Js.Json.object_,
      (),
    );
  switch (response->SignupResponse.reason) {
  | Some(reason) => Error(reason)
  | None =>
    Js.log(response->SignupResponse.refreshToken);
    Storage.set("refresh_token", response->SignupResponse.refreshToken);
    response->SignupResponse.jwt->Ok;
  };
};

let getIdeas = (~jwt, ~page) =>
  Fetch.make(~url="/ideas?page=" ++ page->string_of_int, ~token=jwt, ());

let createIdea = (~jwt, ~idea) =>
  Fetch.make(
    ~url="/ideas",
    ~token=jwt,
    ~body=
      Js.Dict.fromList([
        ("content", idea.description->Js.Json.string),
        ("impact", idea.impact->float_of_int->Js.Json.number),
        ("ease", idea.ease->float_of_int->Js.Json.number),
        ("confidence", idea.confidence->float_of_int->Js.Json.number),
      ])
      ->Js.Json.object_,
    (),
  );

let updateIdea = (~jwt, ~id, ~idea) =>
  Fetch.make(
    ~url="/ideas/" ++ id,
    ~method_="put",
    ~token=jwt,
    ~body=
      Js.Dict.fromList([
        ("content", idea.description->Js.Json.string),
        ("impact", idea.impact->float_of_int->Js.Json.number),
        ("ease", idea.ease->float_of_int->Js.Json.number),
        ("confidence", idea.confidence->float_of_int->Js.Json.number),
      ])
      ->Js.Json.object_,
    (),
  );

let deleteIdea = (~jwt, ~id) =>
  Fetch.delete(~url="/ideas/" ++ id, ~token=jwt);