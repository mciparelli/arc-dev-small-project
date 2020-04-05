let storeContext = Store.context;
open Types;

let useStore = () => {
  let (jwt, setJwt) = React.useState(_ => None);
  Types.Store.t(~jwt, ~setJwt);
};

let useCurrentUserStatus = () => {
  let (status, setStatus) = React.useState(_ => User.NotCalled);
  let store = React.useContext(storeContext);
  let jwt = store->Types.Store.jwt;
  let setJwt = store->Types.Store.setJwt;
  let updateJwt =
    React.useCallback2(
      () => {
        let%Promise jwt = Api.getJwt();
        switch (jwt) {
        | None => setStatus(_oldStatus => User.Loaded(None))
        | jwt => setJwt(_oldJwt => jwt)
        };
      },
      (setStatus, setJwt),
    );
  let findUser =
    React.useCallback2(
      jwt => {
        setStatus(_ => User.Loading);
        let%Promise user = Api.getUser(jwt);
        switch (user->User.reason) {
        | None => setStatus(_oldUser => User.Loaded(Some(user)))
        | _someReason => updateJwt()->ignore
        };
      },
      (setStatus, updateJwt),
    );
  React.useEffect3(
    () => {
      (
        switch (jwt) {
        | None => updateJwt()
        | Some(jwt) => findUser(jwt)
        }
      )
      ->ignore;
      None;
    },
    (jwt, updateJwt, findUser),
  );
  status;
};