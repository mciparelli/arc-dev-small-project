type state = {
  email: string,
  password: string,
  result: option(Belt.Result.t(string, string)),
};

type action =
  | SetEmail(string)
  | SetPassword(string)
  | SetResult(option(Belt.Result.t(string, string)));

let reducer = (state, action) =>
  switch (action) {
  | SetEmail(email) => {...state, email}
  | SetPassword(password) => {...state, password}
  | SetResult(result) => {...state, result}
  };

let initialState = {email: "", password: "", result: None};

[@react.component]
let make = () => {
  let store = React.useContext(Store.context);
  let ({email, password, result}, send) =
    React.useReducer(reducer, initialState);
  React.useEffect2(
    () => {
      switch (result) {
      | Some(result) =>
        switch (result) {
        | Ok(jwt) => store->Types.Store.setJwt(_oldJwt => Some(jwt))
        | _ => ()
        }

      | _ => ()
      };
      None;
    },
    (result, store),
  );

  <form
    id="login"
    className="d-flex flex-column align-self-center my-auto"
    onSubmit={event => {
      event->ReactEvent.Form.preventDefault;
      None->SetResult->send;
      Api.login(~email, ~password)
      ->Promise.then_(result => {result->Some->SetResult->send})
      ->ignore;
    }}>
    <h1 className="text-center"> "Log In"->React.string </h1>
    <input
      type_="email"
      className="mt-3"
      placeholder="Email"
      autoFocus=true
      value=email
      onChange={event => {event->Utils.getInputValue->SetEmail->send}}
    />
    <input
      type_="password"
      className="mt-2"
      placeholder="Password"
      value=password
      onChange={event => {event->Utils.getInputValue->SetPassword->send}}
    />
    {switch (result) {
     | None => React.null
     | Some(result) =>
       switch (result) {
       | Error(error) =>
         <div className="mt-3 text-red"> error->React.string </div>
       | _ => React.null
       }
     }}
    <div className="mt-3 d-flex align-center">
      <button type_="submit" className="bg-green text-white">
        "log in"->React.string
      </button>
      <div className="ml-auto">
        <span> {js|Don’t have an account? |js}->React.string </span>
        <button
          type_="button"
          className="link text-green"
          onClick={_event => ReasonReactRouter.push("/signup")}>
          "Create an account"->React.string
        </button>
      </div>
    </div>
  </form>;
};