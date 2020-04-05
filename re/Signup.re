open Utils;

type state = {
  email: string,
  name: string,
  password: string,
  error: option(string),
};

type action =
  | SetEmail(string)
  | SetName(string)
  | SetPassword(string)
  | SetError(option(string));

let reducer = (state, action) =>
  switch (action) {
  | SetEmail(email) => {...state, email}
  | SetName(name) => {...state, name}
  | SetPassword(password) => {...state, password}
  | SetError(error) => {...state, error}
  };

let initialState = {email: "", name: "", password: "", error: None};

[@react.component]
let make = () => {
  let ({name, email, password, error}, send) =
    React.useReducer(reducer, initialState);
  let store = React.useContext(Store.context);
  <form
    id="login"
    className="d-flex flex-column align-self-center my-auto"
    onSubmit={event => {
      event->ReactEvent.Form.preventDefault;
      Api.signup(~name, ~email, ~password)
      ->Promise.then_(result => {
          switch (result) {
          | Error(error) => error->Some->SetError->send
          | Ok(jwt) => store->Types.Store.setJwt(_oldJwt => Some(jwt))
          }
        })
      ->ignore;
    }}>
    <h1 className="text-center"> "Sign Up"->React.string </h1>
    <input
      type_="text"
      className="mt-3"
      placeholder="Name"
      autoFocus=true
      value=name
      onChange={event => {event->getInputValue->SetName->send}}
    />
    <input
      type_="email"
      className="mt-2"
      placeholder="Email"
      value=email
      onChange={event => {event->getInputValue->SetEmail->send}}
    />
    <input
      type_="password"
      className="mt-2"
      placeholder="Password"
      value=password
      onChange={event => {event->getInputValue->SetPassword->send}}
    />
    {switch (error) {
     | None => React.null
     | Some(error) =>
       <div className="mt-3 text-red"> error->React.string </div>
     }}
    <div className="mt-3 d-flex align-center">
      <button type_="submit" className="bg-green text-white">
        "sign up"->React.string
      </button>
      <div className="ml-auto">
        <span> "Already have an account? "->React.string </span>
        <button
          type_="button"
          className="link text-green"
          onClick={_event => ReasonReactRouter.push("/login")}>
          "Log in"->React.string
        </button>
      </div>
    </div>
  </form>;
};